/*
 * Copyright 2024-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.xip_board"
#endif

/*
 * XMCD (external memory configuration data) payload.
 *
 * When no PSRAM is used, the payload is set to all 0xFFFFFFFF (an invalid XMCD).
 * This is intentional: it overwrites the XMCD region so that any stale, valid
 * XMCD left in flash from a previous image is wiped out and does not take effect.
 */
#if defined(USE_PSRAM)
#if defined(USE_PSRAM_W958D6) && (USE_PSRAM_W958D6 == 1)
#define BOARD_XMCD_PAYLOAD_INIT { 0xC0010008UL, 0xC0001A00UL }  /* XMCD for W958D6 */
#else
#define BOARD_XMCD_PAYLOAD_INIT { 0xC0010008UL, 0xC0100900UL }  /* XMCD for APS256XXN */
#endif
#else
#define BOARD_XMCD_PAYLOAD_INIT { 0xFFFFFFFFUL, 0xFFFFFFFFUL }  /* Invalid XMCD */
#endif /* USE_PSRAM */

#if defined(XIP_BOOT_HEADER_ENABLE) && (XIP_BOOT_HEADER_ENABLE == 1)

#include "mimxrt2660evk_xspi_nor_config.h"

#if defined(__ARMCC_VERSION) || defined(__GNUC__)
__attribute__((section(".boot_hdr.xmcd_data"), used))
#elif defined(__ICCARM__)
#pragma location = ".boot_hdr.xmcd_data"
#endif
const uint32_t xmcd_data[] = BOARD_XMCD_PAYLOAD_INIT;

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

#endif /* XIP_BOOT_HEADER_ENABLE */

#if defined(USDHC_BOOT_HEADER_ENABLE) && (USDHC_BOOT_HEADER_ENABLE == 1)

#include "mimxrt2660evk_usdhc_config.h"

#if defined(__ARMCC_VERSION) || defined(__GNUC__)
__attribute__((section(".boot_hdr.xmcd_data"), used))
#elif defined(__ICCARM__)
#pragma location = ".boot_hdr.xmcd_data"
#endif
const usdhc_boot_mcb_t g_usdhc_boot_mcb = {
    .pad0                = { 0 },
    .secondary_img_table = { 0 },
    .pad1                = { 0 },
    .xmcd_data           = BOARD_XMCD_PAYLOAD_INIT,
    .pad2                = { 0 },
};

#endif /* USDHC_BOOT_HEADER_ENABLE */
