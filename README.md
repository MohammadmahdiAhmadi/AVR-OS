# AVR Microcontroller Operating System

This operating system is designed for AVR microcontrollers, offering fixed-priority pre-emptive scheduling, process aging to prevent starvation, a pre-emptive spin-lock mechanism, and a relatively small and simple-to-learn architecture.

## Key Features
- **Fixed-Priority Pre-emptive Scheduling**: Ensures equitable switching between tasks based on their priorities, enhancing overall system performance.
- **Process Aging**: Implemented to prevent process starvation and maintain system stability.
- **Pre-emptive Spin-Lock Mechanism**: Facilitates synchronization between processes, enhancing concurrency control.
- **Compatibility**: Currently supports ATMega32 microcontrollers.
- **Compiler Support**: Compatible with GCC in Atmel Studio.

## How to Use
Refer to the `main.c` file for examples on how to integrate and use the operating system in your AVR microcontroller projects.

## Fixes and Enhancements
### Commit 1dd2795b2b36b324f3c0443ce71b1fe36cd6419a (HEAD -> fix)
#### Fix: Enhance Context Switch Strategy for Fair Task Prioritization

- **Description:**
  Revamped the existing context switch strategy to address fairness issues observed in the previous implementation. The old strategy did not ensure equitable switching between tasks based on their priorities, leading to suboptimal performance. The new context switch algorithm prioritizes fairness by considering task priorities, resulting in improved overall system behavior.

- **Additional Changes:**
  Two video files have been added to the documents directory for visual comparison between the old and new algorithms.
  - [New Context Switch Algorithm Video](./documents/new_context_switch_algorithm.mp4)
  - [Old Context Switch Algorithm Video](./documents/old_context_switch_algorithm.mp4)

### Commit 9703b5bb40e2c97cabcfe3d2fc55b8af2d2a5f95
#### Fix: Resolve Issue in `osAsmYieldFromTick` and `SAVE_CONTEXT` in `asm.c`

- **Description:**
  Addressed a critical problem in the `osAsmYieldFromTick` function and `SAVE_CONTEXT` macro within the `asm.c` file. The issue stemmed from the improper handling of global interrupt – specifically, disabling it without re-enabling, leading to the unintended disablement of timer1 interrupt. Consequently, this was disrupting context switching functionality. The fix ensures proper global interrupt management, preventing interference with timer1 and restoring seamless context switching.

## Supported Devices
- **ATMega32**

## Supported Compilers
- **GCC (Atmel Studio)**

## Known Issues
- It is unsafe to create processes, mutexes, or call `malloc`/`free` when the system is running (fix: add system-side and pre-emptive save allocation mechanism).
