# dsc_flexcan_interrupt_transfer

## Overview
The flexcan_interrupt example shows how to use FlexCAN driver in none-blocking interrupt way:

In this example, 2 boards are connected through CAN bus. Endpoint A(board A) send a CAN Message to
Endpoint B(board B) when user press space key in terminal. Endpoint B receive the message, print
the message content to terminal and echo back the message. Endpoint A will increase the received
message and waiting for the next transmission of the user initiated.

For self wake up from STOP mode, since steps which MCU enters STOP mode differs on different MCUs,
take flexcan_interrupt_transfer of twrke18f for example, user should do like this:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
##include "fsl_smc.h"
...
...
flexcanConfig.enableSelfWakeup = true;
FLEXCAN_Init();
...
...
SMC_SetPowerModeStop(SMC, kSMC_PartialStop1);
if (wakenUp)
{
    PRINTF("B has been waken up!\r\n\r\n");
}
...
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [MC56F83000-EVK](../../../_boards/mc56f83000evk/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [TWR-MC56F8400](../../../_boards/twrmc56f8400/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
