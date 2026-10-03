#ifndef COURSE_LED_H
#define COURSE_LED_H

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

void course_led_set_blink_period_ms(const struct device *dev, int period);

#ifdef __cplusplus
}
#endif

#endif /* COURSE_LED_H */
