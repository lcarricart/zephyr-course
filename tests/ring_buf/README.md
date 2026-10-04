# Ring buffer tests on Windows

From `zephyr-course`, in PowerShell:

```powershell
$env:QEMU_BIN_PATH = 'C:\Program Files\qemu'
west twister -T tests/ring_buf -p qemu_x86 -v
```

Twister builds and runs the tests. Add `-n` for faster incremental builds on
subsequent runs. `-b` only builds; `built (not run)` does not mean tests passed.

Requires QEMU and the SDK's `x86_64-zephyr-elf` toolchain:
[Windows setup](../calculator/README.md#windows-setup).

See [HOMEWORK.md](HOMEWORK.md) for the exercises, coverage commands and expected
results, and [TEST_SPEC.md](TEST_SPEC.md) for the test cases to implement.
