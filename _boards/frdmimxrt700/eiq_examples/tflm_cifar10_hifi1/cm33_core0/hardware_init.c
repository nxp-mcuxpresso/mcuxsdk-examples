/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "core1_support.h"
#include "fsl_debug_console.h"
#include "pmic_support.h"
/*${header:end}*/

/*${function:start}*/
void APP_BootCore1(void)
{
    BOARD_ReleaseCore1Power();
    BOARD_BootCore1(CORE1_BOOT_ADDRESS, CORE1_BOOT_ADDRESS);
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitAHBSC();
#if defined(EIQ_EXAMPLE_HSRUN_CLOCK)
    /* eIQ example running with 325MHz */
    /* Using external PMIC supply for VDD1/VDD2 */
    // Disable LDO
    POWER_SetVddnSupplySrc(kVddSrc_PMIC);
    POWER_SetVdd1SupplySrc(kVddSrc_PMIC);
    POWER_SetVdd2SupplySrc(kVddSrc_PMIC);
    POWER_ApplyPD();
    BOARD_InitPmicPins();
    BOARD_InitPmic();
    BOARD_SetPmicVdd2Voltage(1100000U); /* 1.1v for 325MHz clock. */
    BOARD_SetPmicVdd1Voltage(1100000U); /* 1.1v for Sense domain 250MHz clock. */
    
    BOARD_BootClockHSRUN();
#endif

#ifdef CORE1_IMAGE_COPY_TO_RAM
    BOARD_CopyCore1Image(CORE1_BOOT_ADDRESS);
#endif

    APP_BootCore1();
}
/*${function:end}*/
