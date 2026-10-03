#ifndef COURSE_LED_H
#define COURSE_LED_H

#include <zephyr/device.h>

void course_led_set_blink_period_ms(const struct device *dev, int period_ms);

#endif