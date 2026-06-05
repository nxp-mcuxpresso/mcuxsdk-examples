# bbnsm_rtc

## Overview
The BBNSM RTC project is a simple demonstration program of the SDK BBNSM driver's RTC (Real-Time
Counter) function. The RTC is implemented inside the Battery-Backed Non-Secure Module (BBNSM). The
example sets the current date and time to a predefined value, starts the RTC counter, and prints the
running time on the UART terminal. It then sets up the BBNSM hardware block to trigger an alarm after
a user-specified number of seconds. The alarm is set with reference to the current RTC time.

The example also demonstrates RTC persistence across power cycles. On each power-on, it reads a
magic value from the BBNSM General Purpose Register. If the magic is present — meaning
the BBNSM kept its state through the power-off — the example prints the current RTC time instead
of re-initializing the clock. This relies on the BBNSM being kept powered by its backup supply
while the main power is off.

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/bbnsm/rtc/example_board_readme.md)
- [FRDM-IMXRT1186](../../../_boards/frdmimxrt1186/driver_examples/bbnsm/rtc/example_board_readme.md)
