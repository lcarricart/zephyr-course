# Ring buffer tests on Windows

From `zephyr-course`, in PowerShell:

```powershell
$env:QEMU_BIN_PATH = 'C:\Program Files\qemu'
$env:ZEPHYR_SDK_INSTALL_DIR = "$env:USERPROFILE\zephyr-sdk-0.17.2"
[Environment]::SetEnvironmentVariable('ZEPHYR_SDK_INSTALL_DIR', $env:ZEPHYR_SDK_INSTALL_DIR, 'User')
west twister -T tests/ring_buf -p qemu_x86 -v
```

Twister builds and runs the tests. Add `-n` for faster incremental builds on
subsequent runs. `-b` only builds; `built (not run)` does not mean tests passed.

Requires QEMU and the SDK's `x86_64-zephyr-elf` toolchain:
[Windows setup](../calculator/README.md#windows-setup).

For coverage, run `.\tests\ring_buf\coverage.ps1` without extra arguments.
It follows [Zephyr's documented QEMU coverage workflow](https://docs.zephyrproject.org/4.2.0/develop/test/coverage.html):
build, capture QEMU output, extract data with `gen_gcov_files.py`, then run `gcovr`.
When prompted, press **Ctrl+A**, release, then **X** to exit QEMU and generate HTML.
Open `twister-out/coverage/index.html`; include `twister-out/coverage/` in the commit.

See [HOMEWORK.md](HOMEWORK.md) for the exercises, coverage commands and expected
results, and [TEST_SPEC.md](TEST_SPEC.md) for the test cases to implement.
