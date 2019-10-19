#include "joystick.h"

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
    
    while (1)
    {
        usleep(50000);
        js0.poll(&event);
        if (event.isButton())
        {
            std::string btnStatus = (event.value & 0x01) != 0  ? "Pressed" : "Released";
            std::cout << "BUTTON#" << event.number
                      << (int)event.number << " "
                      << btnStatus << std::endl;
        }
        // Do this otherwise we get the same event each time we poll if nothing has changed
        event = js_empty_event;
    }

    return 0;
}