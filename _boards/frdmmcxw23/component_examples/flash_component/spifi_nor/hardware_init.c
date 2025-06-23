/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_nor_flash.h"
#include "fsl_spifi_nor_flash.h"

/*${header:end}*/

/*${variable:start}*/
void BOARD_SpifiClockInit(spifi_nor_clock_init_t param)
{
    /* Always use FRO 32MHz. */
}

spifi_mem_nor_config_t MX25R6435FM2IL0Config =
{
    .clockInit = BOARD_SpifiClockInit,
    .cmd_format = kSPIFI_CommandAllSerial,
    .quad_mode_setting = kSerialNorQuadMode_StatusReg1_Bit6,
};

nor_config_t norConfig = {
  .memControlConfig = &MX25R6435FM2IL0Config,
  .driverBaseAddr = SPIFI0,
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    POWER_Init();

    CLOCK_EnableClock(kCLOCK_Iocon);
    CLOCK_EnableClock(kCLOCK_Gpio0);
    GPIO_PortInit(GPIO, 0);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);
    CLOCK_EnableClock(kCLOCK_FlexComm0);
    CLOCK_AttachClk(kFRO_HF_DIV_to_FLEXCOMM1);
    CLOCK_EnableClock(kCLOCK_FlexComm1);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Set SPIFI clock source */
    CLOCK_AttachClk(kFRO_HF_to_SPIFI);
    /* Set the clock divider */
    CLOCK_SetClkDiv(kCLOCK_DivSpifiClk, 1U, false);
}
/*${function:end}*/
