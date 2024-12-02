/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#include "fsl_nand_flash.h"
#include "fsl_flexspi_nand_flash.h"
/*${header:end}*/

/*${function:start}*/

flexspi_mem_config_t memConfig = {
    .deviceConfig =
        {
            .flexspiRootClk       = 60000000U, /* 60MHZ SPI serial clock */
            .flashSize            = FLASH_SIZE,
            .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
            .CSInterval           = 1000,
            .CSHoldTime           = 3,
            .CSSetupTime          = 3,
            .dataValidTime        = 1,
            .columnspace          = 12,
            .enableWordAddress    = 0,
            .AWRSeqIndex          = 0,
            .AWRSeqNumber         = 0,
            .ARDSeqIndex          = NAND_CMD_LUT_SEQ_IDX_READCACHE,
            .ARDSeqNumber         = 1,
            .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
            .AHBWriteWaitInterval = 0,
        },
    .devicePort           = kFLEXSPI_PortA1,
    .dataBytesPerPage     = 2048,
    .bytesInPageSpareArea = 128,
    .pagesPerBlock        = 64,
    .busyOffset           = 0,
    .busyBitPolarity      = 0,
    .eccStatusMask        = 0x30U,
    .eccFailureMask       = 0x20U,
};

nand_config_t nandConfig = {
    .memControlConfig = &memConfig,
    .driverBaseAddr   = EXAMPLE_FLEXSPI,
};

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};

    CLOCK_InitUsb1Pll(&g_ccmConfigUsbPll);
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 24);   /* Set PLL3 PFD0 clock 360MHZ. */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 5);   /* flexspi clock 360/6 = 30M. */

    /* Initialize FLEXSPI */
    flexspi_config_t config;
    /* Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);
    /* Init FLEXSPI. */
    FLEXSPI_Init(EXAMPLE_FLEXSPI, &config);
}

/*${function:end}*/
