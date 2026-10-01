/*
 * Objective: use an onboard LED as a pretend sensor.
 *
 * sample_fetch turns the LED on.
 * channel_get turns the LED off and returns its recorded state.
 */

#define DT_DRV_COMPAT zephyr_course_led

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <stdbool.h>
#include <errno.h>

/* Data structures */
typedef struct
{
	struct gpio_dt_spec led;
} course_led_config;

typedef struct
{
	bool is_on;
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
static DEVICE_API(sensor, course_led_api) =
{
	.sample_fetch = course_led_sample_fetch,
	.channel_get = course_led_channel_get,
};

/* Device instances */
#define COURSE_LED_DEFINE(inst) \
	static course_led_data data_##inst = {0}; \
	static const course_led_config config_##inst = \
	{ \
		.led = GPIO_DT_SPEC_INST_GET(inst, gpios), \
	}; \
	DEVICE_DT_INST_DEFINE(inst, course_led_init, NULL, &data_##inst, &config_##inst, POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY, &course_led_api);

DT_INST_FOREACH_STATUS_OKAY(COURSE_LED_DEFINE)

/* Functions */
static int course_led_init(const struct device *dev)
{
	const course_led_config *cfg = dev->config;
	course_led_data *state = dev->data;
	int ret = STATUS_SUCCESS;

	if (gpio_is_ready_dt(&cfg->led) == false)
	{
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
		return -ENOTSUP;
	}

	return course_led_set(cfg, state, true);
}

static int course_led_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
	const course_led_config *cfg = dev->config;
	course_led_data *state = dev->data;
	int ret = STATUS_SUCCESS;

	if (chan != SENSOR_CHAN_PRIV_START)
	{
		return -ENOTSUP;
	}

	ret = course_led_set(cfg, state, false);
	if (ret != STATUS_SUCCESS)
	{
		return ret;
	}

	val->val1 = state->is_on;
	val->val2 = 0;

	return STATUS_SUCCESS;
}