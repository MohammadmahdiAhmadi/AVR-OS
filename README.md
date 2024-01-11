# AVR Microcontroller Operating System

## Key Features
* Fixed-priority pre-emptive scheduling
* Process aging to avoid starvation
* Pre-emptive spin-lock mechanism
* Relatively small and simple to learn from

## How to Use
Refer to the `main.c` file for examples on how to integrate and use the operating system in your AVR microcontroller projects.

## Fixes and Enhancements
#### Fix: Enhance Context Switch Strategy for Fair Task Prioritization (Commit 9177caf)

- **Description:**
  Revamped the existing context switch strategy to address fairness issues observed in the previous implementation. The old strategy did not ensure equitable switching between tasks based on their priorities, leading to suboptimal performance. The new context switch algorithm prioritizes fairness by considering task priorities, resulting in improved overall system behavior.

  Two video files have been added to the documents directory for visual comparison between the old and new algorithms.
  - [New Context Switch Algorithm Video](./documents/new_context_switch_algorithm.mp4)
  - [Old Context Switch Algorithm Video](./documents/old_context_switch_algorithm.mp4)

#### Fix: Resolve Issue in `osAsmYieldFromTick` and `SAVE_CONTEXT` in `asm.c` (Commit 061fb67)

- **Description:**
  Addressed a critical problem in the `osAsmYieldFromTick` function and `SAVE_CONTEXT` macro within the `asm.c` file. The issue stemmed from the improper handling of global interrupt – specifically, disabling it without re-enabling, leading to the unintended disablement of timer1 interrupt. Consequently, this was disrupting context switching functionality. The fix ensures proper global interrupt management, preventing interference with timer1 and restoring seamless context switching.

## Supported Devices
- **ATMega32**

## Supported Compilers
- **GCC (Atmel Studio)**

## Known Issues
- It is unsafe to create processes, mutexes, or call `malloc`/`free` when the system is running (fix: add system-side and pre-emptive save allocation mechanism).
