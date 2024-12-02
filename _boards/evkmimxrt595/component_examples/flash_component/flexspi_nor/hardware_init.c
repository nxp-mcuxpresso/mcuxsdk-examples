/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "app.h"
#include "fsl_flexspi.h"
#include "fsl_flexspi_nor_flash.h"
#include "fsl_nor_flash.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
flexspi_mem_config_t mem_Config = {
    .deviceConfig =
        {
            .flexspiRootClk       = 99000000,
            .flashSize            = FLASH_SIZE,
            .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
            .CSInterval           = 2,
            .CSHoldTime           = 3,
            .CSSetupTime          = 3,
            .dataValidTime        = 0,
            .columnspace          = 0,
            .enableWordAddress    = 0,
            .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
            .AHBWriteWaitInterval = 0,
        },
    .devicePort      = kFLEXSPI_PortA1,
    .deviceType      = kSerialNorCfgOption_DeviceType_ReadSFDP_SDR,
    .quadMode        = kSerialNorQuadMode_NotConfig,
    .enhanceMode     = kSerialNorEnhanceMode_Disabled,
    .commandPads     = kFLEXSPI_1PAD,
    .queryPads       = kFLEXSPI_1PAD,
    .statusOverride  = 0,
    .busyOffset      = 0,
    .busyBitPolarity = 0,

};

nor_config_t norConfig = {
    .memControlConfig = &mem_Config,
    .driverBaseAddr   = EXAMPLE_FLEXSPI,
};

void BOARD_InitHardware(void)
{
    flexspi_config_t config;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    POWER_DisablePD(kPDRUNCFG_APD_FLEXSPI0_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_FLEXSPI0_SRAM);
    POWER_ApplyPD();

    CLOCK_AttachClk(kAUX0_PLL_to_FLEXSPI0_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivFlexspi0Clk, 4);

    /* Initialize FLEXSPI */
    /* Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus. */
    config.ahbConfig.enableAHBPrefetch    = true;
    config.ahbConfig.enableAHBBufferable  = true;
    config.ahbConfig.enableReadAddressOpt = true;
    config.ahbConfig.enableAHBCachable    = true;
    config.rxSampleClock                  = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
    FLEXSPI_Init(EXAMPLE_FLEXSPI, &config);
}

/*${function:end}*/
