# intmux

## Overview
This example shows how to use INTMUX to route the internel interrupt to the system.

In this example, one of the INTMUX input source(such as LPIT, TPM) is used to generate internel interrupt.
The internel interrupt is outputted to system through INTMUX. Then the interrupt is handled by IRQSTEER, which
located outside of the CM4 subsystem.
