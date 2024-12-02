# dsc_qtmr_timer

## Overview

The QTMR project is a simple demonstration program of the SDK QTMR driver to use QTMR as a timer.
The quad-timer module provides four timer channels with a variety of controls affecting both individual
and multi-channel features. 
It sets up the QTMR hardware block to trigger an interrupt every 50 millisecond.
When chaining the first channel and the second channel, Timer interrupt occur every 10 second.
When the QTMR interrupt is triggered a message a printed on the UART terminal.
In lots of RT10x devices, overflow flag and related interrupt cannot be generated successfully in upward 
count mode (ERRATA050194), the workaround is to use compare interrupt instead of overflow interrupt 
by setting compare value to 0xFFFF, the compare interrupt has the same timing effect as overflow interrupt 
in this way. So we add a new demo scene like that, when setting the compare value to 0xFFFF, Timer compare 
interrupt occurs every 65.5 seconds.

## Supported Boards
