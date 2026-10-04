# Ring Buffer Module Unit Test Homework — Lecture 08

## Overview

Write unit tests for the `ring_buf` module — a tiny circular FIFO buffer.

The exact test cases to implement are specified in `TEST_SPEC.md`. The
skeleton in `src/test_ring_buf.c` already provides one complete test
(`test_fresh_state`) as a worked example, and declares the remaining 7
stubs with `ztest_test_skip()` so the binary builds cleanly from the start.

**Reference**: lecture 08 slides for `ZTEST_SUITE`, `ZTEST`, `before` hook,
and the three assertion families (`zassert_*`, `zexpect_*`, `zassume_*`).

## Module Under Test

```text
app/modules/ring_buf/
├── include/ring_buf.h     # public API
└── src/ring_buf.c         # implementation (static circular buffer)
```

The `ring_buf` module provides a fixed-capacity circular buffer of `int`s with the following API:
```c
int rb_init(size_t capacity);
int rb_pop(int *value);
int rb_peek(int *value);
bool rb_is_empty(void);
bool rb_is_full(void);
size_t rb_count(void);
```

> Before starting homework, read the full API reference in `include/ring_buf.h`. The implementation 
> in `src/ring_buf.c` is under 100 lines of code, so you can read it in its entirety if you like.
> The tests will be black-box, so you don't need to understand the implementation to write them, 
> but reading the code may help you understand the expected behavior and edge cases.

## Provided Infrastructure

`src/test_ring_buf.c` already contains:

- All required `#include`s (`zephyr/ztest.h`, `errno.h`, `ring_buf.h`)
- A shared `before` hook that calls `rb_init(4)` before every test
- Three `ZTEST_SUITE` registrations (`ring_buf_init`, `ring_buf_push_pop`,
  `ring_buf_boundaries`)
- One complete test (`test_fresh_state`) as a worked example
- Seven `ZTEST` stubs, each with a `// TODO(l8-task1)` comment pointing at
  the matching entry in `TEST_SPEC.md`

Each stub currently calls `ztest_test_skip()`. Replace that line with the
actual test body.

## Running the Tests on Windows

Use PowerShell from the `zephyr-course` directory, or `cd tests/ring_buf` and use
`-T .`. Twister builds and runs the test application; no separate main-app build
or flashing is needed. Use `qemu_x86`, since `native_sim` requires Linux.

Install QEMU and the SDK's `x86_64-zephyr-elf` toolchain using the
[Windows setup instructions](../calculator/README.md#windows-setup).
These dependencies are already installed on this workspace's machine.

```powershell
# Make QEMU available to this terminal
$env:QEMU_BIN_PATH = 'C:\Program Files\qemu'

# Let Twister discover the SDK compiler and coverage tools permanently
$env:ZEPHYR_SDK_INSTALL_DIR = "$env:USERPROFILE\zephyr-sdk-0.17.2"
[Environment]::SetEnvironmentVariable('ZEPHYR_SDK_INSTALL_DIR', $env:ZEPHYR_SDK_INSTALL_DIR, 'User')

# Build and run
west twister -T tests/ring_buf -p qemu_x86 -v

# Faster subsequent runs: reuse the build for incremental compilation
west twister -T tests/ring_buf -p qemu_x86 -v -n

# Build only (does not execute tests)
west twister -T tests/ring_buf -p qemu_x86 -b
```

Without `-n`, Twister starts fresh and renames the previous `twister-out` directory.
Check the executed/passed counts: `built (not run)` is not a test pass. If nothing
runs, check `QEMU_BIN_PATH` and that you did not pass `-b`. A missing
`x86_64-zephyr-elf-gcc` error means the SDK needs the x86 toolchain.

---

## Task 1 — Implement the Tests  `git tag l8-task1`

Open `src/test_ring_buf.c` and fill in each stub `ZTEST` body according to
`TEST_SPEC.md`. Study `test_fresh_state` first — it shows the pattern.
Seven tests to write across three suites:

- `ring_buf_init`: 1 test (reinit clears state)
- `ring_buf_push_pop`: 3 tests (single push/pop, FIFO order, full buffer push)
- `ring_buf_boundaries`: 3 tests (peek non-consuming, `pop(NULL)`, `is_full` after fill)

For each test:
1. Read the matching row in `TEST_SPEC.md`.
2. Remove `ztest_test_skip()` from the stub.
3. Write the test body using `zassert_*` assertions to verify the expected behavior.
4. Re-run Twister and confirm the test passes.

**Acceptance:** all 8 tests pass.

```powershell
west twister -T tests/ring_buf -p qemu_x86 -v -n
```

### Tag `l8-task1`

After every test passes, tag the commit `l8-task1`.

---

## Task 2 — Coverage Analysis  `git tag l8-task2`

Run from `zephyr-course` in PowerShell, with QEMU configured as above:

```powershell
python -m pip install gcovr
.\tests\ring_buf\coverage.ps1
```

The script follows [Zephyr's documented QEMU coverage workflow](https://docs.zephyrproject.org/4.2.0/develop/test/coverage.html):
build with coverage, capture QEMU output, extract data using Zephyr's
`gen_gcov_files.py`, then generate HTML using `gcovr`. It reads the SDK location
from `ZEPHYR_SDK_INSTALL_DIR` or the saved Windows user setting. Run it without
Twister options. This avoids Zephyr 4.2's Windows Twister capture timeout and
preserves the course's test configuration, including shuffle repetitions.
It also uses Twister's standard logging/assertion branch exclusions so coverage
totals are comparable to the course reference below.
When the script says the coverage is captured, press **Ctrl+A**, release, then
**X** to exit QEMU and generate the report (Zephyr's documented QEMU exit).

Install `gcovr` in the Python environment used by Twister. Coverage percentages below
are the course's reference values; compiler and tool versions can affect them.

1. Run the command above to generate the coverage report.
2. Open `twister-out/coverage/index.html` in a browser and click into `ring_buf.c`.
3. After a correct Task 1 implementation, the expected numbers are:
```
Lines:     81.4%  (35/43)
Functions: 100.0%  (7/7)
Branches:  64.3%  (9/14)
```
   If your numbers are lower, a test body is likely still calling `ztest_test_skip()`,
   eventually tests where not implemented according to the spec. Go back and check each
   test body against the spec to find the missing one(s).
4. Look at the red lines. Think about **why** those paths are not exercised by the spec.
   Ask yourself: *what test case(s) would be needed to cover those lines?* (no need to 
   implement them, just think about it).

### Tag `l8-task2`

Include the `twister-out/coverage/` directory in the commit and tag it
`l8-task2`.
