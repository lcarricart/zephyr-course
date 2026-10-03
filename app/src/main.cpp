/*
 * Objective: use an onboard LED as a pretend sensor.
 *
 * The application requests operations through the sensor API.
 * The driver owns the hardware configuration and runtime state.
 */

#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#include "course_led.h"

/* Data structures */
typedef enum
{
	STATUS_SUCCESS = 0,
} app_status_t;

/* Variables */
static const struct device *const dev = DEVICE_DT_GET(DT_NODELABEL(course_led));

/* Application */
int main(void)
{
	struct sensor_value value = {0};
	int ret = STATUS_SUCCESS;

	if (device_is_ready(dev) == false)
	{
		printk("ERROR: Course LED device is not ready\n");
		return 0;
	}

	/* Use the API extension but do not change anything in the physical LED blink, keeping the exercise as a mere driver development training, only changing a struct field value */
	course_led_set_blink_period_ms(dev, 100);

	while (1)
	{
		/* Fetch: the driver toggles the LED. */
		ret = sensor_sample_fetch(dev);
		if (ret != STATUS_SUCCESS)
		{
			printk("ERROR: Sample fetch failed (%d)\n", ret);
			return 0;
		}

		/* Get the current LED state without changing it. */
		ret = sensor_channel_get(dev, SENSOR_CHAN_PRIV_START, &value);
		if (ret != STATUS_SUCCESS)
		{
			printk("ERROR: Channel get failed (%d)\n", ret);
			return 0;
		}

		printk("Recorded LED state: %d\n", value.val1);

		k_sleep(K_SECONDS(1));
	}
}
