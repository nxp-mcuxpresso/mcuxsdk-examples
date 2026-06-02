/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "fwk_platform_ble.h"
/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    POWER_Init();

    CLOCK_EnableClock(kCLOCK_Gpio0);
    GPIO_PortInit(GPIO, 0);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM1);
    CLOCK_EnableClock(kCLOCK_FlexComm1);
    /* Enable clock that is used by OSTimer and FLASH_CheckECC */
    CLOCK_EnableClock(kCLOCK_Sysctl);
    /* Enable watchdog clock */
    CLOCK_Enable1MFRO(true);
    CLOCK_AttachClk(kOSC32K_to_CTIMER0);

    CLOCK_SetClkDiv(kCLOCK_DivWdtClk, 10U, true);

    BOARD_Configure32kOsc();

    BOARD_InitPins();
    BOARD_BootClockXTAL32M();

#if defined(gDebugConsoleEnable_d) && (gDebugConsoleEnable_d == 1)
    BOARD_InitDebugConsole();
#endif
    PLATFORM_InitBle();

    BOARD_InitI2c();
    /* Configure the sensors that share the sensor interrupt line to open drain */
    uint8_t registerValue;
    registerValue = FXLS8964AF_CONFIG4_PP_OD_OPEN_DRAIN << FXLS8964AF_CONFIG4_PP_OD_POS;
    BOARD_I2cWriteFunc(FXLS8964AF_I2C_ADDR, FXLS8964AF_CONFIG4_REG_ADDR, &registerValue, sizeof(registerValue));
}
/*${function:end}*/
