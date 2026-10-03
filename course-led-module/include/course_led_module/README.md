Question: "Why include/course_led_module/drivers and not directly include/drivers, if we are already inside the course-led-module in the end?"

Because the compiler searches **all exported include directories together**. The outer `course-led-module/` folder is part of the search path, but it isn’t part of the name you write in `#include`.

With `include/drivers/course_led.h`, you write:

```c
#include <drivers/course_led.h>
```

That works. But if another module exports the same `drivers/course_led.h` path, the compiler picks whichever appears first in its search paths.

With `include/course_led_module/drivers/course_led.h`, you write:

```c
#include <course_led_module/drivers/course_led.h>
```

The extra directory makes the header’s name unique across modules. That’s the reason for the convention—not a Zephyr requirement. **Your proposed `include/drivers/` layout is valid**, and I should have explained the choice before applying it.