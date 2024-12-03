# usart_interrupt_rx_timeout

## Overview
The uart_interrupt_rx_timeout example shows how to use uart driver functional
API to receive data with rx timeout interrupt method:

In this example, one uart instance connect to PC through uart, the board will
send back all characters that PC send to the board when receive timeout.

This example had added GPIO Toggle for test the rx timeout. When enter the uart 
interrupt handle, the interrupt handler will toggle led pin for logic analyzer.
You can measure the timeout.