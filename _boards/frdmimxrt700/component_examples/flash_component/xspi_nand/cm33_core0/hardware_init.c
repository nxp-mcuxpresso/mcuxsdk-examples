/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_power.h"
#include "fsl_nand_flash.h"
#include "fsl_xspi_nand_flash.h"
/*${header:end}*/

/*${variable:start}*/
/* The deviceSize must be the column-stride span (page count x 2^columnAddrWidth). NOT the raw data
 * size, otherwise blocks >= 512 fall outside SFAR and their row commands are silently dropped. */
#define NAND_PAGE_SIZE           2048U
#define NAND_PAGES_PER_BLOCK     64U
#define NAND_BLOCK_COUNT         1024U
#define NAND_COLUMN_ADDR_WIDTH   12U
#define NAND_XSPI_DEVICE_SIZE_KB (NAND_BLOCK_COUNT * NAND_PAGES_PER_BLOCK * (1U << NAND_COLUMN_ADDR_WIDTH) / 1024U)

xspi_mem_nand_config_t xspiNandMemConfig = {
    .deviceConfig =
        {
            .xspiRootClk                                             = 80000000U,
            .enableCknPad                                            = false,
            .deviceInterface                                         = kXSPI_StrandardExtendedSPI,
            .interfaceSettings.strandardExtendedSPISettings.pageSize = NAND_PAGE_SIZE,
            .CSHoldTime                                              = 3U,
            .CSSetupTime                                             = 3U,
            .sampleClkConfig.sampleClkSource       = kXSPI_SampleClkFromInvertedFullySpeedDummyPadLoopback,
            .sampleClkConfig.dllConfig.dllMode     = kXSPI_AutoUpdateMode,
            .sampleClkConfig.dllConfig.useRefValue = true,
            .ptrDeviceDdrConfig                    = NULL,
            .addrMode                              = kXSPI_DeviceByteAddressable,
            .columnAddrWidth                       = NAND_COLUMN_ADDR_WIDTH,
            .enableCASInterleaving                 = false,
            .deviceSize[0]    = NAND_XSPI_DEVICE_SIZE_KB, /* column-stride span (256MB), not raw data size */
            .ptrDeviceRegInfo = NULL,
        },
    .targetGroup = EXAMPLE_XSPI_NAND_PORT,
    .ambaBase    = EXAMPLE_XSPI_AMBA_BASE,
};

nand_config_t nandConfig = {
    .memControlConfig = &xspiNandMemConfig,
    .driverBaseAddr   = EXAMPLE_XSPI,
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitAHBSC();
    BOARD_InitXSPI1Pins();

    POWER_DisablePD(kPDRUNCFG_APD_XSPI1);
    POWER_DisablePD(kPDRUNCFG_PPD_XSPI1);
    POWER_ApplyPD();

    CLOCK_AttachClk(kAUDIO_PLL_PFD1_to_XSPI1);
    CLOCK_SetClkDiv(kCLOCK_DivXspi1Clk, 5U);
}
/*${function:end}*/
