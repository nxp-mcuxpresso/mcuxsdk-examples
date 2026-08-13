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
/* deviceSize must be the column-stride span (page count x 2^columnAddrWidth), NOT the raw data
 * size; otherwise blocks >= 512 fall outside SFAR and their row commands are silently dropped. */
#define NAND_PAGE_SIZE           2048U
#define NAND_PAGES_PER_BLOCK     64U
#define NAND_BLOCK_COUNT         1024U
#define NAND_COLUMN_ADDR_WIDTH   12U
#define NAND_XSPI_DEVICE_SIZE_KB (NAND_BLOCK_COUNT * NAND_PAGES_PER_BLOCK * (1U << NAND_COLUMN_ADDR_WIDTH) / 1024U)

/* Winbond W25N01KW data pin output-driver-strength.
 *   25 ohm = 0x00
 *   33 ohm = 0x02
 *   50 ohm = 0x04 (default)
 *   75 ohm = 0x06
 */
#define W25N01KW_SR2_ADDR      0xB0U
#define W25N01KW_SR2_ODS_MASK  0x06U
#define W25N01KW_SR2_ODS_25OHM 0x00U
#define W25N01KW_SR2_ODS_33OHM 0x02U
#define W25N01KW_SR2_ODS_50OHM 0x04U
#define W25N01KW_SR2_ODS_75OHM 0x06U

static status_t BOARD_XspiNandDeviceInit(nand_handle_t *handle)
{
    uint8_t reg = 0U;
    status_t status;

    status = Nand_Flash_GetFeature(handle, W25N01KW_SR2_ADDR, &reg);
    if (status != kStatus_Success)
    {
        return status;
    }
    reg = (uint8_t)((reg & (uint8_t)~W25N01KW_SR2_ODS_MASK) | W25N01KW_SR2_ODS_25OHM);

    return Nand_Flash_SetFeature(handle, W25N01KW_SR2_ADDR, reg);
}

xspi_mem_nand_config_t xspiNandMemConfig = {
    .deviceConfig =
        {
            .xspiRootClk                                             = 199680000U,
            .enableCknPad                                            = false,
            .deviceInterface                                         = kXSPI_StrandardExtendedSPI,
            .interfaceSettings.strandardExtendedSPISettings.pageSize = NAND_PAGE_SIZE,
            .CSHoldTime                                              = 3U,
            .CSSetupTime                                             = 3U,
            .sampleClkConfig.sampleClkSource                = kXSPI_SampleClkFromInvertedFullySpeedDummyPadLoopback,
            .sampleClkConfig.dllConfig.dllMode              = kXSPI_AutoUpdateMode,
            .sampleClkConfig.dllConfig.useRefValue          = false,
            .sampleClkConfig.dllConfig.dllCustomDelayTapNum = 7U,
            .sampleClkConfig.dllConfig.enableCdl8           = false,
            .sampleClkConfig.dllConfig.dllCustomPara.autoUpdateModoPara.referenceCounter              = 2U,
            .sampleClkConfig.dllConfig.dllCustomPara.autoUpdateModoPara.resolution                    = 4U,
            .sampleClkConfig.dllConfig.dllCustomPara.autoUpdateModoPara.enableHighFreq                = true,
            .sampleClkConfig.dllConfig.dllCustomPara.autoUpdateModoPara.tDiv16OffsetDelayElementCount = 5U,
            .sampleClkConfig.dllConfig.dllCustomPara.autoUpdateModoPara.offsetDelayElementCount       = 7U,
            .ptrDeviceDdrConfig                                                                       = NULL,
            .addrMode              = kXSPI_DeviceByteAddressable,
            .columnAddrWidth       = NAND_COLUMN_ADDR_WIDTH,
            .enableCASInterleaving = false,
            .deviceSize[0]         = NAND_XSPI_DEVICE_SIZE_KB, /*!< column-stride span (256MB), not raw data size */
            .ptrDeviceRegInfo      = NULL,
        },
    .targetGroup    = EXAMPLE_XSPI_NAND_PORT,
    .ambaBase       = EXAMPLE_XSPI_AMBA_BASE,
    .deviceInitHook = BOARD_XspiNandDeviceInit,
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
    CLOCK_SetClkDiv(kCLOCK_DivXspi1Clk, 2U);
}
/*${function:end}*/
