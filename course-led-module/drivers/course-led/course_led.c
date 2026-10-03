/*
 * Objective: use an onboard LED as a pretend sensor.
 *
 * sample_fetch toggles the LED.
 * channel_get returns its current recorded state without changing it.
 */

#define DT_DRV_COMPAT zephyr_course_led

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <stdbool.h>
#include <errno.h>

#include "course_led.h"

/* Data structures */
typedef struct
{
	struct gpio_dt_spec led;
} course_led_config;

typedef struct
{
	bool is_on;
    int blink_period_ms;
} course_led_data;

typedef enum
{
	STATUS_SUCCESS = 0,
} app_status_t;

/* Prototypes */
static int course_led_init(const struct device *dev);
static int course_led_set(const course_led_config *cfg, course_led_data *state, bool on);
static int course_led_sample_fetch(const struct device *dev, enum sensor_channel chan);
static int course_led_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val);

/* Sensor API */
/* The attributes are not created on the fly, they're fixed .sample_fetch and .channel_get, and defined this way because I'm implementing the
 * sensor API. There are other possible attributes available in this API abstraction 
 */
static DEVICE_API(sensor, course_led_api) =
{
    /* Pass the pointers to each function */
	.sample_fetch = course_led_sample_fetch,
	.channel_get = course_led_channel_get,
};

/* Device instances */
/* Template for creating one device instance. This is very smart! A single function-like macro with an "inst" parameters passed to it, defines for you at compile time, the specified C code. In this case, it is creating two structs and calling another function-like macro
 * The ## is a C pre-processor trick to create unique names per instance. The ## is a glueing thing, so it glue config_ with instance, resulting in variables such as course_led_data_0, course_led_data_1 and so on 
 * The macro continues as long as the lines finish with backslashes 
 * 
 * These instances are not created in my .c files as I would expect. These are created in my app.overlay, and Zephyr creates as many instances as I defined there */
#define COURSE_LED_DEFINE(inst) \
	static course_led_data data_##inst = {0}; \
	static const course_led_config config_##inst = \
	{ \
		.led = GPIO_DT_SPEC_INST_GET(inst, gpios), \
	}; \
	DEVICE_DT_INST_DEFINE(inst, course_led_init, NULL, &data_##inst, &config_##inst, POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY, &course_led_api);

/* This macro looks for all DTS nodes with the compatible = "zephyr,course-led" and status = "okay"
 * For all DTS nodes satisfying those two conditions, it runs COURSE_LED_DEFINE(inst), creating the actual instances with their structs and all needed parameters */
DT_INST_FOREACH_STATUS_OKAY(COURSE_LED_DEFINE)

/* Functions */
/* In order to know what functions are relevant calls when using a driver, I need to check the Zephyr's official documentation for the involved subsystems, and ideally existing drivers and samples
 * For example, the Zephyr GPIO API docs explains this: 
 * - https://docs.zephyrproject.org/latest/hardware/peripherals/gpio.html
 * - https://github.com/zephyrproject-rtos/zephyr/blob/main/samples/basic/blinky/src/main.c
 */
static int course_led_init(const struct device *dev)
{
	const course_led_config *cfg = dev->config;
	course_led_data *state = dev->data;
	int ret = STATUS_SUCCESS;

	if (gpio_is_ready_dt(&cfg->led) == false)
	{
        /* -ENODEV means "No such GPIO device" 
         * The Zephyr API uses negative numbers for returned error values */
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
	if (ret != STATUS_SUCCESS)
	{
		return ret;
	}

	state->is_on = false;

	return STATUS_SUCCESS;
}

static int course_led_set(const course_led_config *cfg, course_led_data *state, bool on)
{
	int ret = gpio_pin_set_dt(&cfg->led, on);

	if (ret != STATUS_SUCCESS)
	{
		return ret;
	}

	state->is_on = on;

	return STATUS_SUCCESS;
}

static int course_led_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
	const course_led_config *cfg = dev->config;
	course_led_data *state = dev->data;

	if ((chan != SENSOR_CHAN_ALL) && (chan != SENSOR_CHAN_PRIV_START))
	{
        /* -ENOTSUP means "Operation not supported"
         * The Zephyr API uses negative numbers for returned error values */
		return -ENOTSUP;
	}

	return course_led_set(cfg, state, !state->is_on);
}

static int course_led_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
	course_led_data *state = dev->data;

	if (chan != SENSOR_CHAN_PRIV_START)
	{
        /* -ENOTSUP means "Operation not supported"
         * The Zephyr API uses negative numbers for returned error values */
		return -ENOTSUP;
	}

	val->val1 = state->is_on;
	val->val2 = 0;

	return STATUS_SUCCESS;
}

void course_led_set_blink_period_ms(const struct device *dev, int period)
{
    course_led_data *state = dev->data;
    state->blink_period_ms = period;
}
