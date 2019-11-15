#include <string>
#include <sstream>
#include <functional>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

// Event Constants
#define JS_EVENT_BUTTON     0x01    // button pressed/released
#define JS_EVENT_AXIS       0x02    // joystick moved
#define JS_EVENT_INIT       0x80    // initial state
#define JS_EVENT_INIT_BUTTON    JS_EVENT_BUTTON & JS_EVENT_INIT
#define JS_EVENT_INIT_AXIS      JS_EVENT_AXIS & JS_EVENT_INIT

// Event Data
struct js_event
{
    unsigned int time;       // event timestamp in milliseconds
    short value;            // value
    unsigned char type;     // event type (see above)
    unsigned char number;   // axis/button number
};

// IOCTL commands
#define JSIOCGVERSION		_IOR('j', 0x01, __u32)	        // get driver version
#define JSIOCGAXES	        _IOR('j', 0x11, __u8)		    // get number of axes
#define JSIOCGBUTTONS		_IOR('j', 0x12, __u8)			// get number of buttons
#define JSIOCGNAME(len)		_IOC(_IOC_READ, 'j', 0x13, len) // get identifier string
// NOT USED INTERNALLY from /usr/include/linux/joystick.h
#define JSIOCSAXMAP		    _IOW('j', 0x31, __u8[ABS_CNT])			        // set axis mapping
#define JSIOCGAXMAP		    _IOR('j', 0x32, __u8[ABS_CNT])			        // get axis mapping
#define JSIOCSBTNMAP		_IOW('j', 0x33, __u16[KEY_MAX - BTN_MISC + 1])	// set button mapping
#define JSIOCGBTNMAP		_IOR('j', 0x34, __u16[KEY_MAX - BTN_MISC + 1])	// get button mapping

// Joystick Metadata
struct js_info
{
    std::stringstream path;
    std::string name;
    unsigned int nButtons, nAxes;
    unsigned char *bstate;
    int *astate;
};

class JoystickEvent
{
public:
    unsigned int time;
    short value;
    unsigned char type;
    unsigned char number;
    JoystickEvent() {}
    bool isButton() { return (type & JS_EVENT_BUTTON) != 0; }
    bool isAxis() { return (type & JS_EVENT_AXIS) != 0; }
    bool isInit() { return (type & JS_EVENT_INIT) != 0; }
};

// Use this empty class instance to clear an event
static const JoystickEvent js_empty_event;

class Joystick
{
private:
    int _fd;
    js_info _info = {};
public:
    Joystick(unsigned int index = 0)
    {
        char name[100];
        unsigned int init_counter = 0;
        JoystickEvent init_event;

        _info.path << "/dev/input/js" << index;
        _fd = open(_info.path.str().c_str(), O_RDONLY | O_NONBLOCK);
        ioctl(_fd, JSIOCGNAME(100), &name);
        ioctl(_fd, JSIOCGBUTTONS, &_info.nButtons);
        ioctl(_fd, JSIOCGAXES, &_info.nAxes);
        _info.name = name;

        // Now that we have joystick metadata, we can set the size of these
        unsigned char buttonState[_info.nButtons];
        int axisState[_info.nAxes];

        // Set the initial state
        read(_fd, &init_event, sizeof(JoystickEvent*));
        while (init_event.isInit() && init_counter < ((_info.nButtons + _info.nAxes) - 2))
        {
            if (init_event.isButton())
                buttonState[init_event.number] = init_event.value;
            else if (init_event.isAxis())
                axisState[init_event.number] = init_event.value;
            read(_fd, &init_event, sizeof(JoystickEvent*));
            init_counter++;
        }
        _info.bstate = buttonState;
        _info.astate = axisState;
    }

    js_info info() { return _info; }
    bool connected() { return _fd >= 0; }

    void poll(JoystickEvent* event)
    {
        read(_fd, event, sizeof(*event));
    }

    void poll(JoystickEvent* event, void (*cb)(JoystickEvent* e)) {
        read(_fd, event, sizeof(*event));
        cb(event);
    }
};