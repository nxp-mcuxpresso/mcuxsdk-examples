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
/* clang-format off */
#if defined(USE_PSRAM)
#if defined(USE_PSRAM_W958D6) && (USE_PSRAM_W958D6 == 1)
#define BOARD_XMCD_PAYLOAD_INIT { 0xC0010008UL, 0xC0001A00UL }  /* XMCD for W958D6 */
#else
#define BOARD_XMCD_PAYLOAD_INIT { 0xC0010008UL, 0xC0100900UL }  /* XMCD for APS256XXN */
#endif
#else
#define BOARD_XMCD_PAYLOAD_INIT { 0xFFFFFFFFUL, 0xFFFFFFFFUL }  /* Invalid XMCD */
#endif /* USE_PSRAM */
/* clang-format on */

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
#if defined(USE_FLASH_W35T51NW) && (USE_FLASH_W35T51NW == 1)
const xspi_nor_config_t W35T51NW_config = {
    .memConfig =
        {
            .tag                  = XSPI_CFG_BLK_TAG,
            .version              = XSPI_CFG_BLK_VERSION,
            .readSampleClkSrc     = kxSpiReadSampleClk_ExternalInputFromDqsPad,
            .csHoldTime           = 3u,
            .csSetupTime          = 3u,
            .columnAddressWidth   = 0u,
            .deviceModeCfgEnable  = 1u,
            .deviceModeType       = kDeviceConfigCmdType_Spi2Xpi,
            .waitTimeCfgCommands  = 1u,
            .deviceModeSeq        = {.seqNum = 1u, .seqId = 6u},
            .deviceModeArg        = 0x000000E7UL,
            .configCmdEnable      = 0u,
            .controllerMiscOption =
                (1u << kxSpiMiscOffset_SafeConfigFreqEnable) | (1u << kxSpiMiscOffset_DdrModeEnable),
            .deviceType           = kxSpiDeviceType_SerialNOR,
            .sflashPadType        = kSerialFlash_8Pads,
            .serialClkFreq        = kxSpiSerialClk_100MHz,
            .lutCustomSeqEnable   = 0u,
            .sflashA1Size         = 64u * 1024u * 1024u,
            .commandInterval      = 10u,
            .lookupTable =
                {
                    /* Read (Octal DDR 0xFD, 32-bit addr, 6 dummy cycles) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_READ] =
                        XSPI_LUT_SEQ(CMD_DDR, XSPI_8PAD, 0xFD, CMD_DDR, XSPI_8PAD, 0xFD),
                    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
                        XSPI_LUT_SEQ(ADDR_DDR, XSPI_8PAD, 0x20, DUMMY, XSPI_8PAD, 0x06),
                    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 2] =
                        XSPI_LUT_SEQ(READ_DDR, XSPI_8PAD, 0x04, STOP, XSPI_1PAD, 0x00),

                    /* Read Status (legacy SPI) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0x05, READ, XSPI_1PAD, 0x04),

                    /* Read Status (Octal DDR) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_XPI] =
                        XSPI_LUT_SEQ(CMD_DDR, XSPI_8PAD, 0x05, CMD_DDR, XSPI_8PAD, 0x05),
                    [5 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_XPI + 1] =
                        XSPI_LUT_SEQ(DUMMY, XSPI_8PAD, 0x03, READ_DDR, XSPI_8PAD, 0x04),

                    /* Write Enable (legacy SPI) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0x06, STOP, XSPI_1PAD, 0x00),

                    /* Write Enable (Octal DDR) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_XPI] =
                        XSPI_LUT_SEQ(CMD_DDR, XSPI_8PAD, 0x06, CMD_DDR, XSPI_8PAD, 0x06),

                    /* Erase Sector (Octal DDR 0x21, 32-bit addr) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
                        XSPI_LUT_SEQ(CMD_DDR, XSPI_8PAD, 0x21, CMD_DDR, XSPI_8PAD, 0x21),
                    [5 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 1] =
                        XSPI_LUT_SEQ(ADDR_DDR, XSPI_8PAD, 0x20, STOP, XSPI_1PAD, 0x00),

                    /* Device mode / enter-octal configure sequence (referenced by deviceModeSeq, seqId 6) */
                    [5 * 6] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0x81, CMD, XSPI_1PAD, 0x00),
                    [5 * 6 + 1] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0x00, CMD, XSPI_1PAD, 0x00),
                    [5 * 6 + 2] =
                        XSPI_LUT_SEQ(WRITE, XSPI_1PAD, 0x01, STOP, XSPI_1PAD, 0x00),

                    /* Erase Block (Octal DDR 0xDC, 32-bit addr) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_ERASEBLOCK] =
                        XSPI_LUT_SEQ(CMD_DDR, XSPI_8PAD, 0xDC, CMD_DDR, XSPI_8PAD, 0xDC),
                    [5 * NOR_CMD_LUT_SEQ_IDX_ERASEBLOCK + 1] =
                        XSPI_LUT_SEQ(ADDR_DDR, XSPI_8PAD, 0x20, STOP, XSPI_1PAD, 0x00),

                    /* Page Program (Octal DDR 0x12, 32-bit addr) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
                        XSPI_LUT_SEQ(CMD_DDR, XSPI_8PAD, 0x12, CMD_DDR, XSPI_8PAD, 0x12),
                    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
                        XSPI_LUT_SEQ(ADDR_DDR, XSPI_8PAD, 0x20, WRITE_DDR, XSPI_8PAD, 0x04),

                    /* Chip Erase (Octal DDR 0x60) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE] =
                        XSPI_LUT_SEQ(CMD_DDR, XSPI_8PAD, 0x60, CMD_DDR, XSPI_8PAD, 0x60),
                },
        },
    .pageSize           = 256u,
    .sectorSize         = 4u * 1024u,
    .ipcmdSerialClkFreq = 1u,
    .isUniformBlockSize = false,
    .serialNorType      = 2u,
    .blockSize          = 64u * 1024u,
    .flashStateCtx      = 0x06008200UL,
};

#elif defined(USE_FLASH_W25H512NW) && (USE_FLASH_W25H512NW == 1)
const xspi_nor_config_t W25H512NW_config = {
    .memConfig =
        {
            .tag                  = XSPI_CFG_BLK_TAG,
            .version              = XSPI_CFG_BLK_VERSION,
            .readSampleClkSrc     = kxSpiReadSampleClk_LoopbackFromDqsPad,
            .csHoldTime           = 3u,
            .csSetupTime          = 3u,
            .columnAddressWidth   = 0u,
            .deviceModeCfgEnable  = 0u,
            .deviceModeType       = kDeviceConfigCmdType_Generic,
            .waitTimeCfgCommands  = 0u,
            .deviceModeSeq        = {0},
            .deviceModeArg        = 0u,
            .configCmdEnable      = 0u,
            .controllerMiscOption = (1u << kxSpiMiscOffset_SafeConfigFreqEnable),
            .deviceType           = kxSpiDeviceType_SerialNOR,
            .sflashPadType        = kSerialFlash_4Pads,
            .serialClkFreq        = kxSpiSerialClk_100MHz,
            .lutCustomSeqEnable   = 0u,
            .sflashA1Size         = 64u * 1024u * 1024u,
            .lookupTable =
                {
                    /* Read (Quad I/O 0xEC, 4-byte addr, 6 dummy cycles) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_READ] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0xEC, ADDR, XSPI_4PAD, 0x20),
                    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
                        XSPI_LUT_SEQ(DUMMY, XSPI_4PAD, 0x06, READ, XSPI_4PAD, 0x04),

                    /* Read Status Register */
                    [5 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0x05, READ, XSPI_1PAD, 0x04),

                    /* Write Enable */
                    [5 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0x06, STOP, XSPI_1PAD, 0x00),

                    /* Erase Sector (0x21, 4-byte addr) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0x21, ADDR, XSPI_1PAD, 0x20),

                    /* Erase Block 64KB (0xDC, 4-byte addr) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_ERASEBLOCK] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0xDC, ADDR, XSPI_1PAD, 0x20),

                    /* Page Program Quad Input (0x34, 4-byte addr) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0x34, ADDR, XSPI_1PAD, 0x20),
                    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
                        XSPI_LUT_SEQ(WRITE, XSPI_4PAD, 0x04, STOP, XSPI_1PAD, 0x00),

                    /* Chip Erase (0x60) */
                    [5 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE] =
                        XSPI_LUT_SEQ(CMD, XSPI_1PAD, 0x60, STOP, XSPI_1PAD, 0x00),
                },
        },
    .pageSize           = 256u,
    .sectorSize         = 4u * 1024u,
    .ipcmdSerialClkFreq = 1u,
    .isUniformBlockSize = false,
    .serialNorType      = 0u,
    .blockSize          = 64u * 1024u,
    .flashStateCtx      = 0x00000000UL,
};

#else

/*
 * Intentionally zero-initialised: tag == 0 is not a valid XSPI_CFG_BLK_TAG (0x42464346),
 * so the ROM discards this FCB and falls back to auto-probing the connected NOR flash
 * to determine its type and apply built-in default read parameters.
 * NOTE: auto-probe only works for standard SPI/QSPI NOR flash. Octal DDR flash
 * (e.g. W35T51NW) requires an explicit FCB and cannot be discovered by auto-probe.
 */
const xspi_nor_config_t xspi_nor_qspi_probe_config = { 0 };
#endif
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
