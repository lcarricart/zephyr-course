#include "zephyr/device.h"
#include "zephyr/toolchain.h"
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>

/* Handle the info subcomand every time it gets called */
static int cmd_info_handler(const struct shell* sh, int argc, char** argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(course_led));
    bool state = device_is_ready(dev);

    shell_print(sh, "Device: %s", dev->name);
    shell_print(sh, "Ready: %s", state ? "yes" : "no");
    
    return 0;
}

static int cmd_fetch_handler(const struct shell* sh, int argc, char** argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(course_led));

    /* No need to check its success in this small exercise */
    sensor_sample_fetch(dev);

    return 0;
}

static int cmd_read_handler(const struct shell* sh, int argc, char** argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(course_led));
    struct sensor_value value;

    int ret = sensor_channel_get(dev, SENSOR_CHAN_PRIV_START, &value /* Output */);
    if (ret != 0 /* SUCCESS */)
    {
        shell_error(sh, "Could not read LED state: %d", ret);
    }

    shell_print(sh, "LED State: %d", value.val1);

    return 0;
}

/* Define the sub-commands of sensor, their description and their handlers */
SHELL_STATIC_SUBCMD_SET_CREATE(sensor_subcmd,
    SHELL_CMD_ARG(fetch, NULL, "Toggle the LED", cmd_fetch_handler, 1, 0),
    SHELL_CMD_ARG(read, NULL, "Read LED state", cmd_read_handler, 1, 0),
    SHELL_CMD_ARG(info, NULL, "Show device name and ready state", cmd_info_handler, 1, 0),
    SHELL_SUBCMD_SET_END,
);

/* We don't assign sensor a handler because it is a container for subcommands. */
SHELL_CMD_REGISTER(sensor, &sensor_subcmd, "Course sensor commands", NULL);
