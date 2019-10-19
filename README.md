# libjoystick
A simple linux c++ library for interpreting joystick input on linux.

## Usage example

See also [test.cpp](test.cpp)

```cpp
#include "joystick.h"

int main()
{
    Joystick js0;
    JoystickEvent event;
    js_info* info = js0.info();

    std::cout << "\nName: " << info->name
              << "\nButtons: " << info->nButtons
              << "\nAxes: " << info-nAxes << std::endl;
    
    while (true)
    {
        // limit the polling rate
        usleep(50000);
        js0.poll(&event);
        if (event.isButton)
        {
            std::cout << "BUTTON EVENT: #" << event.number
                      << " Value: " << event.value;
        }
        // Nullify the event, otherwise we get the same data next poll (nothing has replaced it)
        event = js_empty_event;
    }
    
    return 0;
}
```

## Future Features

1. Support callbacks that will execute when a specified event happens, per button and axis, both for pressed and released states.