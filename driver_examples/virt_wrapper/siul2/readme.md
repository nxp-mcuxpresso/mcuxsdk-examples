# virt_wrapper_suil2

## Overview
This exapmle shows how to use VIRT WRAPPER driver to map the SIUL2 peripheral to a
different virtual address.

In this example, SIUL2 is mapped to a virtual address, software controls the pin's
level and interrupt through this virtual address. The pin's input and output
are both enabled, so when toggle the output voltage level, this pin can detect
level change interrupt.

## Running the demo
The log is shown in the terminal, if the demo runs to any issue, the log will show errors.

## Supported Boards
- [FRDM-MCXE31B](../../../_boards/frdmmcxe31b/driver_examples/virt_wrapper/siul2/example_board_readme.md)
