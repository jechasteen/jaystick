#ifndef JOYSTICK_HANDLER_H
#define JOYSTICK_HANDLER_H

#include <joystick.h>

class JoystickHandler
{
public:
    JoystickHandler();
    ~JoystickHandler();

    // This function will connect to ALL joysticks
    std::vector<struct js_info> connect();
    // poll all joysticks, potentially firing their callbacks
    std::vector<struct js_event> poll();
    // poll a single joystick, potentially firing its callback
    struct js_event poll(std::string id);

    // public access to the _joysticks vector
    std::vector<Joystick>* joysticks();
    // public access to a joystick by id
    Joystick* joystick(std::string id);
private:
    std::vector<Joystick> _joysticks;
};

#endif // JOYSTICK_HANDLER_H
