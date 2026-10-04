# Zephyr course

My code for each laboratory is saved as a Git tag. On GitHub, use the branch selector → Tags to browse the code. Locally, run `git fetch --tags`, then `git checkout TAG` using a tag name.

From the `zephyr-course` directory, build the application with `west build -p always -b YOUR_BOARD app`.

- `l2-task1`: Initial blinky application and testing exercises.
- `l3-task1`: Use Kconfig to configure the LED blinking frequency.
- `l4-task1`: Define the `app-led` alias through an application overlay.
- `l5-task1`: Create a custom board through copy-rename (`our_board`).
- `l5-task2`: Create a custom board definition from scratch (`my_board`).
- `l6-task1`: Create a driver for the onboard LED. Append `-- -DEXTRA_ZEPHYR_MODULES="/absolute/path/to/zephyr-course/course-led-driver"` to the build command.
- `l6-task2`: Extend the driver API. From this tag onward, append `-- -DEXTRA_ZEPHYR_MODULES="/absolute/path/to/zephyr-course/course-led-module"` to the build instead.
- `l7-task1`: Operate the application solely through the Zephyr shell.
- `l7-task2`: Expand the shell to support the extended driver API.
- `l8-task1`: Create ring buffer unit tests with Ztest and run `west twister -T tests/ring_buf -p qemu_x86` (specific to Windows; for Linux replace QEMU with native_sim)
- `l8-task2`: Analyze test coverage. On Windows, run `.\tests\ring_buf\coverage.ps1` and open `twister-out/coverage/index.html` to see the results.

