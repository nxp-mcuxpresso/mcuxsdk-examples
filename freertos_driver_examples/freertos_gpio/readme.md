# freertos_gpio

## Overview
This document explains the freertos_gpio example. It shows the CPU enter at sleep mode and then 
it is waked up by expired time delay that using GPT Timer module or occur any a hardware interrupt.

The example application creates two identical instances task,gpio_task and print_task. Each task will lock the mutex
before executing task and unlock it after the task finishes.


## Running the demo
After the board is flashed the Tera Term will start periodically printing strings synchronized by
mutex.

Freertos Gpio example

The LED is blinking.

The gpio task is running:1times.

The LED is blinking.

The gpio task is running:2times.

The LED is blinking.

The gpio task is running:3times.
......
......

## Explanation of the example
The example application will get tick count number after every period time delay vTaskDelay(time)
When this function called the CPU enter to sleep mode a period of time that called by vTaskDelay 
function.

## Supported Boards
- [KW47-EVK](../../_boards/kw47evk/freertos_driver_examples/freertos_gpio/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/freertos_driver_examples/freertos_gpio/example_board_readme.md)
