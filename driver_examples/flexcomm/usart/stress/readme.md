# usart_stress

## Overview
The usart_stress example demonstrates performance and stability of the USART driver 
when under load from multiple contexts. It can be used as an integration stress 
test of the USART peripheral.

The demo USART sends the string "M_" when Tx is invoked from main thread, and "S_" when invoked from systick handler.
Both main and SysTick handler also perform USART receive of 1 byte. Each context logs successful receive in their rx_catch_count variable.
Terminal output will look similar to this:

M_M_S_M_M_M_M_S_M_M_M_M_S_M_M_M_M_M_M_S_M_M_M_M_S_ ...

You can tweak the DEMO_RNG_MIN_DELAY and DEMO_RNG_MAX_DELAY macros in usart_stress.c to control the random wait between API calls in the main function.

SysTick settings and IRQ frequency can be changed in hardware_init.c (board specific file)
