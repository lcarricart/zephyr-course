#include <zephyr/shell/shell.h>

static int cmd_channel_get_handler(const struct shell* sh, int argc, char** argv)
{
    shell_info(sh, "Hello from channel_get");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(course_led_subcmd,
    SHELL_CMD_ARG(channel_get, NULL, "Get channel of my driver", cmd_channel_get_handler, 1, 0),
    SHELL_SUBCMD_SET_END,
);

SHELL_CMD_REGISTER(course_led, &course_led_subcmd, "Course LED driver set of commands", NULL);