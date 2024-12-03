/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_gpio.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* M7 has its local cache and enabled by default,
     * need to set smart subsystems (0x28000000 ~ 0x3FFFFFFF)
     * non-cacheable before accessing this address region */
    BOARD_InitMemory();

    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootMux(kCLOCK_RootFlexCan1, kCLOCK_FlexCanRootmuxSysPll1); /* Set FLEXCAN1 source to SYSTEM PLL1 800MHZ */
    CLOCK_SetRootDivider(kCLOCK_RootFlexCan1, 2U, 5U);                   /* Set root clock to 800MHZ / 10 = 80MHZ */

    /* GPIO5_IO05 is used to control CAN1_STBY which is ebaled active high */
    gpio_pin_config_t config = {kGPIO_DigitalOutput, 1, kGPIO_NoIntmode};
    GPIO_PinInit(GPIO5, 5U, &config);
}
/*${function:end}*/
