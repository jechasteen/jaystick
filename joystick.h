#include <iostream>
#include <string>
#include <sstream>
#include <functional>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

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
    bool isButton() { return (type & JS_EVENT_BUTTON) != 0; }
    bool isAxis() { return (type & JS_EVENT_AXIS) != 0; }
    bool isInit() { return (type & JS_EVENT_INIT) != 0; }
    friend std::ostream& operator<<(std::ostream& os, const JoystickEvent& e);
};

// Use this empty class instance to clear an event
static const JoystickEvent js_empty_event;

class Joystick
{
public:
    Joystick(unsigned int index = 0)
    {
        char name[100];
        unsigned int nButtons, nAxes, init_counter = 0;
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

        if (_fd > 0) std::cout << "Opened js" << index << std::endl;

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
        std::cout << "Connected.";
    }

    js_info* info() { return &_info; }
    bool connected() { return _fd >= 0; }

    void poll(JoystickEvent* event) {
        read(_fd, event, sizeof(*event));
    }
private:
    int _fd;
    js_info _info;
};