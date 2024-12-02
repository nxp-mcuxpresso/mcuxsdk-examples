# usart_interrupt_ring_buffer_transfer

## Overview
The uart_interrupt ring buffer example shows how to use uart driver in interrupt way with ring buffer:

In this example, one uart instance connect to PC through uart, the board will
send back all characters that PC send to the board.

Note: The example echo every 8 characters, so input 8 characters every time.
