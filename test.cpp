#include <iostream>
#include "joystick.h"

void handleEvent(JoystickEvent* e)
{
    if (e->isButton())
        std::cout << "BUTTON #" << (int)e->number << " : " << e->value << std::endl;
    if (e->isAxis())
        std::cout << "AXIS #" << (int)e->number << " : " << e->value << std:: endl;
    // clear the event so we don't think it's a new one next time around
    *e = js_empty_event;
}

int main ()
{
    Joystick js0;
    JoystickEvent event;
    std::cout << "\nName: " << js0.info().name
              << "\nButtons: " << js0.info().nButtons
              << "\nAxes: " << js0.info().nAxes << std::endl;
    
    while (1)
    {
        // set the polling rate
        usleep(50000);

        // poll the device, populating 'event'
        js0.poll(&event);
        handleEvent(&event);

        // or with a callback
        // js0.poll(&event, handleEvent);
    }

    return 0;
}