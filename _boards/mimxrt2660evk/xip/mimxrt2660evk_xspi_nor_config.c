/*
 * Copyright 2024-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mimxrt2660evk_xspi_nor_config.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.xip_board"
#endif

#if defined(XIP_BOOT_HEADER_ENABLE) && (XIP_BOOT_HEADER_ENABLE == 1)

#if defined(__ARMCC_VERSION) || defined(__GNUC__)
__attribute__((section(".boot_hdr.xmcd_data"), used))
#elif defined(__ICCARM__)
#pragma location = ".boot_hdr.xmcd_data"
#endif

#if defined(USE_PSRAM)

#if defined(USE_PSRAM_W958D6) && (USE_PSRAM_W958D6 == 1)
/* XMCD for W958D6 */
const uint32_t xmcd_data[] = {
    0xC0010008,
    0xC0001A00
};
#else
/* XMCD for APS256XXN */
const uint32_t xmcd_data[] = {
    0xC0010008,
    0xC0100900
};
#endif

#else
const uint32_t xmcd_data[] = {
    0xFFFFFFFF,
    0xFFFFFFFF
};
#endif /* USE_PSRAM_W958D6 == 1 */

#if defined(__ARMCC_VERSION) || defined(__GNUC__)
__attribute__((section(".boot_hdr.fcb"), used))
#elif defined(__ICCARM__)
#pragma location = ".boot_hdr.fcb"
#endif

/* clang-format off */
const xspi_nor_config_t qspiflash_config = {
#if 1
    { 0 }
#else
    .memConfig =
        {
            .tag                  = XSPI_CFG_BLK_TAG,
            .version              = XSPI_CFG_BLK_VERSION,
            .readSampleClkSrc     = kxSpiReadSampleClk_LoopbackFromDqsPad,
            .csHoldTime           = 3u,
            .csSetupTime          = 3u,
            .controllerMiscOption = (1u << kxSpiMiscOffset_SafeConfigFreqEnable),
            .deviceType           = kxSpiDeviceType_SerialNOR,
            .sflashPadType        = kSerialFlash_4Pads,
            .serialClkFreq        = kxSpiSerialClk_120MHz,
            .sflashA1Size         = 64u * 1024u * 1024u,
            .lookupTable =
                {
                    /* Read (Quad I/O 0xEB, 24-bit addr, 6 dummy cycles) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_READ] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0xEB, ADDR, XSPI_4PAD, 0x18),
                    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
                        XSPI_LUT_SEQ(DUMMY, XSPI_4PAD, 0x06, READ, XSPI_4PAD, 0x08),

                    /* Read Status */
                    [5 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0x05, READ, XSPI_1PAD, 0x08),

                    /* Write Enable */
                    [5 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0x06, STOP, XSPI_1PAD, 0x0),

                    /* Erase Sector */
                    [5 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0x20, ADDR, XSPI_1PAD, 0x18),

                    /* Erase Block */
                    [5 * NOR_CMD_LUT_SEQ_IDX_ERASEBLOCK] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0xD8, ADDR, XSPI_1PAD, 0x18),

                    /* Page Program */
                    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0x02, ADDR, XSPI_1PAD, 0x18),
                    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
                        XSPI_LUT_SEQ(WRITE, XSPI_1PAD, 0x08, STOP, XSPI_1PAD, 0x0),

                    /* Chip Erase */
                    [5 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0x60, STOP, XSPI_1PAD, 0x0),
                },
        },
    .pageSize           = 256u,
    .sectorSize         = 4u * 1024u,
    .ipcmdSerialClkFreq = 1u,
    .blockSize          = 64u * 1024u,
    .isUniformBlockSize = false,
#endif
};
/* clang-format on */

#endif
