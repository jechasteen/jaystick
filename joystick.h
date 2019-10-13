#include <linux/types.h>
#include <linux/joystick.h>
#include <string>
#include <functional>
#include <vector>

#ifndef JOYSTICK_H
#define JOYSTICK_H

// We get the joystick info by polling the driver via macros in linux/joystick.h
struct js_info
{
    __u32 version = 1;
    std::string id;
    std::string name;
    std::string path;
    __u8 nButtons;
    __u8 nAxes;
    //struct js_corr corr;
};

enum event { button, axis, init };

class Joystick
{
public:
    // Setting up, we need to open the /dev/input/jsX device
    // then populate the _info struct. When the device is first connected to, it will send
    // the current state of all of the buttons/axes. Store this state in _state;
    Joystick();
    // On teardown, we should release the file handle.
    // TODO: Find out if we need to handle releasing other data
    ~Joystick();

    // event: event.{button,axis,init}
    // index: button or axis number
    // callback: function to run on change, passed the value as a parameter
    void addEventListener(std::string event, __u8 index, std::function<void(__u8)> callback);

    // Allow the user to rename a joystick
    void setJoystickName(std::string name) { _info->name = name; }

    // Public access to the _info struct
    struct js_info* info() { return _info; }

    // TODO: Figure out how these are supposed to work so I look like I know what I'm doing
    // struct js_corr getCorrectionValues();
    // bool setCorrectionValues(struct js_corr val);
    // __u8 getAxisMapping();
    // bool setAxisMapping(__u8* mapping);
    // __u16 getButtonMapping();
    // bool setButtonMapping(__u16 mapping);
private:
    struct js_info* _info;
    std::vector<std::function<void(__u8)>>* _callbacks;
    __u8* _state;
};

#endif // JOYSTICK_H
