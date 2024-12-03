# freertos_fpu

## Overview

The freertos_fpu example shows an application using RTOS tasks with FPU count to test FPU context
save/restore support in FreeRTOS.

Example could be devided in two parts. First part is FPU count. It contain two tasks:
sin_task()....Do sin() count and judge the result. It will print the error number if result error.
cos_task()....Do cos() count and judge the result. It will print the error number if result error.
This two task have equal priority and the RTOS scheduler will switch between these two tasks 
on every RTOS tick interrupt.

Second part is a periodic interrupt handle of gtp timer. This interrupt will do a single FPU count and
will make sin_task/cos_task appear error if without FPU context save/restore.


## Running the demo
After the board is flashed the Tera Term will show debug console output.

Example output:
sin_task FPU test succeed!
cos_task FPU test succeed!
sin_task FPU test succeed!
cos_task FPU test succeed!
sin_task FPU test succeed!
cos_task FPU test succeed!
sin_task FPU test succeed!
cos_task FPU test succeed!
sin_task FPU test succeed!
cos_task FPU test succeed!

## Supported Boards
