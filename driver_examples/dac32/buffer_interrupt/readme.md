# dac32_buffer_interrupt

## Overview

The dac32_buffer_interrupt example shows how to use DAC32 buffer with interrupts.

When the DAC32's buffer feature is enabled, user can benefit from the automation of updating DAC output by hardware/software trigger.
As we know, the DAC converter outputs the value of item pointed by current read pointer. Once the buffer is triggered by software
or hardware, the buffer's read pointer would move automatically as the work mode is set, like normal (cycle) mode, swing mode,
one-time-scan mode or FIFO mode.

In this example, it captures the user's type-in operation from terminal and does the software trigger to the buffer.
The terminal would also display the log that shows the current buffer pointer's position with buffer events.
