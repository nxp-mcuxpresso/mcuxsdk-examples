/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_gpio.h"
#include "app.h"
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

    /* Init GPIO used for peripheral access demonstration. */
    gpio_pin_config_t pinConfig = {
        kGPIO_DigitalOutput,
        0,
        kGPIO_IntRisingEdge,
    };

    GPIO_PinInit(GPIO1, 0, &pinConfig);

    /* Set the IOMUXC_GPR10[2:3], thus the memory violation triggers the hardfault. */
    *(volatile uint32_t *)0x30340028 |= (0x0C);
}

void APP_TouchPeriph(void)
{
    GPIO_PinRead(GPIO1, 0);
}

void APP_TouchMem(void)
{
    /* Touch the memory. */
    (*(volatile uint32_t *)APP_RDC_MEM_BASE_ADDR)++;
}
/*${function:end}*/
