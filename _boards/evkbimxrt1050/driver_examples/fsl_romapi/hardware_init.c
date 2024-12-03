/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_common.h"
/*${header:end}*/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*${function:start}*/
/* Get FLEXSPI NOR Configuration Block */
void FLEXSPI_NorFlash_GetConfig(flexspi_nor_config_t *config)
{
    config->memConfig.tag              = FLEXSPI_CFG_BLK_TAG;
    config->memConfig.version          = FLEXSPI_CFG_BLK_VERSION;
    config->memConfig.readSampleClkSrc = kFLEXSPIReadSampleClk_ExternalInputFromDqsPad;
    config->memConfig.serialClkFreq =
        kFLEXSPISerialClk_30MHz; /* Serial Flash Frequencey.See System Boot Chapter for more details */
    config->memConfig.lutCustomSeqEnable = true;
    config->memConfig.sflashA1Size       = FLASH_SIZE;
    config->memConfig.csHoldTime         = 3U;                       /* Data hold time, default value: 3 */
    config->memConfig.csSetupTime        = 3U;                       /* Date setup time, default value: 3 */
    config->memConfig.deviceType     = kFLEXSPIDeviceType_SerialNOR; /* Flash device type default type: Serial NOR */
    config->memConfig.deviceModeType = kDeviceConfigCmdType_Generic;
    config->memConfig.columnAddressWidth  = 3U;
    config->memConfig.deviceModeCfgEnable = 0U;
    config->memConfig.waitTimeCfgCommands = 0U;
    config->memConfig.configCmdEnable     = 0U;
    config->memConfig.busyOffset          = 15U;
    config->memConfig.busyBitPolarity     = 1U;
    /* Always enable Safe configuration Frequency */
    config->memConfig.controllerMiscOption = FSL_ROM_FLEXSPI_BITMASK(kFLEXSPIMiscOffset_DiffClkEnable) |
                                             FSL_ROM_FLEXSPI_BITMASK(kFLEXSPIMiscOffset_WordAddressableEnable) |
                                             FSL_ROM_FLEXSPI_BITMASK(kFLEXSPIMiscOffset_SafeConfigFreqEnable) |
                                             FSL_ROM_FLEXSPI_BITMASK(kFLEXSPIMiscOffset_DdrModeEnable);
    config->memConfig.sflashPadType = kSerialFlash_8Pads; /* Pad Type: 1 - Single, 2 - Dual, 4 - Quad, 8 - Octal */
    config->pageSize                = FLASH_PAGE_SIZE;
    config->sectorSize              = FLASH_SECTOR_SIZE;
    config->blockSize               = FLASH_BLOCK_SIZE;
    config->isUniformBlockSize      = true;
    config->ipcmdSerialClkFreq      = kFLEXSPISerialClk_30MHz; /* Clock frequency for IP command */
    config->serialNorType           = kSerialNorType_HyperBus;

    // Read
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_READ + 0U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0xA0U, RADDR_DDR, FLEXSPI_8PAD, 0x18U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_READ + 1U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CADDR_DDR, FLEXSPI_8PAD, 0x10U, DUMMY_DDR, FLEXSPI_8PAD, 0x06U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_READ + 2U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(READ_DDR, FLEXSPI_8PAD, 0x04U, STOP, FLEXSPI_1PAD, 0x0U);

    // Read Status
    // 0
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x00U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_READSTATUS + 1U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0xAAU);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_READSTATUS + 2U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x05U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_READSTATUS + 3U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x70U);
    // 1
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_READSTATUS + 4U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0xA0U, RADDR_DDR, FLEXSPI_8PAD, 0x18U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_READSTATUS + 5U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CADDR_DDR, FLEXSPI_8PAD, 0x10U, DUMMY_RWDS_DDR, FLEXSPI_8PAD, 0x0BU);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_READSTATUS + 6U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(READ_DDR, FLEXSPI_8PAD, 0x04U, STOP, FLEXSPI_1PAD, 0x00U);

    // Write Enable
    // 0
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x00U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE + 1U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0xAAU);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE + 2U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x05U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE + 3U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0xAAU);
    // 1
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE + 4U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x00U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE + 5U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x55U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE + 6U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x02U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE + 7U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x55U);

    // Page Program
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x00U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0xAAU);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 2U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x05U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 3U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0xA0U);
    // 1
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 4U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, RADDR_DDR, FLEXSPI_8PAD, 0x18U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 5U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CADDR_DDR, FLEXSPI_8PAD, 0x10U, WRITE_DDR, FLEXSPI_8PAD, 0x80U);

    // Erase Sector
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x00U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 1U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0xAAU);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 2U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x05U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 3U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x80U);
    // 1
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 4U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x00U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 5U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0xAAU);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 6U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x05U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 7U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0xAAU);
    // 2
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 8U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x00U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 9U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x55U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 10U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x02U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 11U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, CMD_DDR, FLEXSPI_8PAD, 0x55U);
    // 3
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 12U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00U, RADDR_DDR, FLEXSPI_8PAD, 0x18U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 13U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CADDR_DDR, FLEXSPI_8PAD, 0x10U, CMD_DDR, FLEXSPI_8PAD, 0x00U);
    config->memConfig.lookupTable[4U * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 14U] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x30U, STOP, FLEXSPI_1PAD, 0x0U);

    // Erase Chip
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0x00);
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 1] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0xAA);
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 2] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0x05);
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 3] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0x80);
    // 1
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 4] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0x00);
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 5] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0xAA);
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 6] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0x05);
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 7] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0xAA);
    // 2
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 8] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0x00);
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 9] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0x55);
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 10] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0x02);
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 11] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0x55);
    // 3
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 12] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0x00);
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 13] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0xAA);
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 14] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0x05);
    config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE + 15] =
        FSL_ROM_FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x00, CMD_DDR, FLEXSPI_8PAD, 0x10);

    // LUT customized sequence
    config->memConfig.lutCustomSeq[NOR_CMD_INDEX_WRITEENABLE].seqNum = 2U;
    config->memConfig.lutCustomSeq[NOR_CMD_INDEX_WRITEENABLE].seqId  = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE;
    config->memConfig.lutCustomSeq[NOR_CMD_INDEX_READSTATUS].seqNum  = 2U;
    config->memConfig.lutCustomSeq[NOR_CMD_INDEX_READSTATUS].seqId   = NOR_CMD_LUT_SEQ_IDX_READSTATUS;
    config->memConfig.lutCustomSeq[NOR_CMD_INDEX_PAGEPROGRAM].seqNum = 2U;
    config->memConfig.lutCustomSeq[NOR_CMD_INDEX_PAGEPROGRAM].seqId  = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM;
    config->memConfig.lutCustomSeq[NOR_CMD_INDEX_ERASESECTOR].seqNum = 4U;
    config->memConfig.lutCustomSeq[NOR_CMD_INDEX_ERASESECTOR].seqId  = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR;
    config->memConfig.lutCustomSeq[NOR_CMD_INDEX_CHIPERASE].seqNum   = 4U;
    config->memConfig.lutCustomSeq[NOR_CMD_INDEX_CHIPERASE].seqId    = NOR_CMD_LUT_SEQ_IDX_CHIPERASE;
}
#if !(defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
/* Configure clock for FlexSPI peripheral */
void BOARD_SetupFlexSpiClock(void)
{
    /* Disable FlexSPI peripheral */
    FLEXSPI->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;
    CLOCK_DisableClock(kCLOCK_FlexSpi);

    /* Init Usb1 PLL3 to 480MHZ. */
    CLOCK_InitUsb1Pll(&usb1PllConfig_BOARD_BootClockRUN);
    /* Init Usb1 PLL3->pfd0 360MHZ. */
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 24);
    /* Enable Usb1 PLL output for USBPHY1. */
    CCM_ANALOG->PLL_USB1 |= CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK;

    /* Set FLEXSPI_PODF, FlexSPI out clock is 60MHZ. */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 5);
    /* Set flexspi clock source to PLL3->pfd0 */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 3);

    CLOCK_EnableClock(kCLOCK_FlexSpi);
    /* Enable FlexSPI peripheral */
    FLEXSPI->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
}
#endif

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
#if !(defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
    BOARD_SetupFlexSpiClock();
#endif
    BOARD_InitDebugConsole();
}
/*${function:end}*/
