#include "j.h"

void p(JoystickEvent* e)
{
    std::cout << "BUTTON PRESSED";
}

int main ()
{
    Joystick js0;
    JoystickEvent event;

    js_info* info = js0.info();
    std::cout << "\nName: " << info->name
              << "\nButtons: " << info->nButtons
              << "\nAxes: " << info->nAxes << std::endl;
    
    // while (1)
    // {
    //     usleep(50000);
    //     js0.poll(&event);
    //     if (false && event.isButton())
    //     {
    //         std::string btnStatus = (event.value & 0x01) != 0  ? "Pressed" : "Released";
    //         std::cout << "BUTTON#" << event.number
    //                   << (int)event.number << " "
    //                   << btnStatus << std::endl;
    //     }
    //     event = empty_event;
    // }
    
    for (int i = 0; i < info->nButtons; i++)
    {
        js0.addEventListener(JS_EVENT_BUTTON, i, p);
    }

    while (true)
    {
        usleep(50000);
        js0.poll(&event);
    }

    return 0;
}