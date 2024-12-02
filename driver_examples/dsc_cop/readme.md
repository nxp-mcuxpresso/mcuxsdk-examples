# dsc_cop

## Overview
The COP Example project is to demonstrate usage of the cop driver.In
this example, after 5 times of refreshing in interrupt routine, interrupt will
be disabled, thus the COP is not serviced, then a timeout reset is generated.
Please notice that Write Protect bit in CTRL register are write-once only, it can be cleared only by resetting COP module.
All COP registers are read-only when Write Protect bit set.

## Supported Boards
