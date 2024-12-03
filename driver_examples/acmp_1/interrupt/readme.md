# acmp_1_interrupt

## Overview
The ACMP Interrupt project is a simple demonstration program that uses the SDK software. It
compares the selected analog input with ACMP internal DAC output continuously and toggle the LED
when the final comparison result changed. The purpose of this demo is to show how to use the
ACMP driver in SDK software by interrupt way. The ACMP can be configured based on default
configuration returned by the API ACMP_GetDefaultConfig().

## Supported Boards
- [FRDM-KE02Z40M](../../../_boards/frdmke02z40m/driver_examples/acmp/interrupt/example_board_readme.md)
