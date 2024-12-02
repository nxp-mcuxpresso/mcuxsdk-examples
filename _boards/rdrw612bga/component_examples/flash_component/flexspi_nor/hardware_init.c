/*
 * Copyright 2019-2023 NXP
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
            .flexspiRootClk       = 65000000U,
            .flashSize            = FLASH_SIZE,
            .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
            .CSInterval           = 2,
            .CSHoldTime           = 3,
            .CSSetupTime          = 3,
            .dataValidTime        = 2,
            .columnspace          = 0,
            .enableWordAddress    = 0,
            .AWRSeqIndex          = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM,
            .AWRSeqNumber         = 1,
            .ARDSeqIndex          = NOR_CMD_LUT_SEQ_IDX_READ,
            .ARDSeqNumber         = 1,
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
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Flexspi);
    RESET_ClearPeripheralReset(kFLEXSPI_RST_SHIFT_RSTn);
}

/*${function:end}*/
