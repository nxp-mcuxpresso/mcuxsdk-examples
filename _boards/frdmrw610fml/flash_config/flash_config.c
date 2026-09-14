/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Flash configuration for XM25QH64D (Wuhan Xinxin / XMC)
 *
 * Device:      XM25QH64D
 * Density:     64 Mbit (8 MByte)
 * Interface:   Standard / Dual / Quad SPI
 * VCC:         2.7 V - 3.6 V
 * Page size:   256 bytes
 * Sector size: 4 KB  (erase command 20h)
 * Block size:  64 KB (erase command D8h)
 * Address:     24-bit (3-byte)
 * Max clock:   166 MHz (Quad SDR), 80 MHz (DTR)
 * JEDEC ID:    20h / 40h / 17h  (MFR=20h, Type=40h, Capacity=17h)
 *
 * Read sequence used: Fast Read Quad I/O (EBh), 1-4-4 SDR
 *   - Instruction : EBh on 1 pad
 *   - Address     : 24-bit on 4 pads
 *   - Mode byte   : 8 bits on 4 pads (2 mode clocks, set non-continuous = F0h)
 *   - Dummy       : 4 clocks on 4 pads (DC[1:0]=00 default)
 *   - Data        : on 4 pads
 *
 * Quad Enable: QE = bit 1 of Status Register-2; set by writing 02h via
 *   Write Status Register-2 instruction (31h).
 */

#include "flash_config.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.flash_config"
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(BOOT_HEADER_ENABLE) && (BOOT_HEADER_ENABLE == 1)
#if defined(__ARMCC_VERSION) || defined(__GNUC__)
__attribute__((section(".flash_conf"), used))
#elif defined(__ICCARM__)
#pragma location = ".flash_conf"
#endif
const fc_flexspi_nor_config_t flexspi_config = {
    .memConfig =
        {
            .tag     = FC_BLOCK_TAG,
            .version = FC_BLOCK_VERSION,
            /* Sample on loopback clock from SCK pad - safe for Quad reads up to ~80 MHz */
            .readSampleClkSrc = 1,
            .csHoldTime       = 3,
            .csSetupTime      = 3,
            /* Enable device mode configuration to set QE bit in Status Register-2 */
            .deviceModeCfgEnable = 1,
            /* seqId 2 = Write Status Register-2 (31h) sequence defined below */
            .deviceModeSeq = {.seqNum = 1, .seqId = 2},
            /* Value written to Status Register-2: QE = bit 1 -> 0x02 */
            .deviceModeArg   = 0x02,
            .configCmdEnable = 0,
            .deviceType      = 0x1,
            .sflashPadType   = kSerialFlash_4Pads,
            /* Clock frequency index 5 (~80 MHz) - within EBh Quad I/O max with 4 dummies */
            .serialClkFreq = 5,
            /* 64 Mbit = 8 MByte on port A1 */
            .sflashA1Size = 0x800000U,
            .sflashA2Size = 0,
            .sflashB1Size = 0,
            .sflashB2Size = 0,
            .lookupTable =
                {
                    /* ---------- Seq 0: Fast Read Quad I/O (EBh), 1-4-4 SDR ----------
                     * EBh | A23-A0 (4 pads) | M7-M0 (4 pads, 2 clocks) |
                     * 4 dummy clocks (4 pads) | Data (4 pads)
                     * M7-M0 = F0h -> bits M5-M4 != 10 -> normal (non-continuous) mode */
                    [0] = FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0xEB, FC_RADDR_SDR, FC_FLEXSPI_4PAD, 0x18),
                    [1] = FC_FLEXSPI_LUT_SEQ(FC_MODE8_SDR, FC_FLEXSPI_4PAD, 0xF0, FC_DUMMY_SDR, FC_FLEXSPI_4PAD, 0x04),
                    [2] = FC_FLEXSPI_LUT_SEQ(FC_READ_SDR, FC_FLEXSPI_4PAD, 0x04, FC_STOP_EXE, FC_FLEXSPI_1PAD, 0x00),

                    /* ---------- Seq 1: Read Status Register-1 (05h) ---------- */
                    [4 * 1 + 0] =
                        FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0x05, FC_READ_SDR, FC_FLEXSPI_1PAD, 0x04),

                    /* ---------- Seq 2: Write Status Register-2 (31h) ----------
                     * Used at boot to set QE = 1 (deviceModeArg = 0x02) */
                    [4 * 2 + 0] =
                        FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0x31, FC_WRITE_SDR, FC_FLEXSPI_1PAD, 0x01),

                    /* ---------- Seq 3: Write Enable (06h) ---------- */
                    [4 * 3 + 0] =
                        FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0x06, FC_STOP_EXE, FC_FLEXSPI_1PAD, 0x00),

                    /* ---------- Seq 5: Sector Erase 4 KB (20h), 24-bit addr ---------- */
                    [4 * 5 + 0] =
                        FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0x20, FC_RADDR_SDR, FC_FLEXSPI_1PAD, 0x18),

                    /* ---------- Seq 8: Block Erase 64 KB (D8h), 24-bit addr ---------- */
                    [4 * 8 + 0] =
                        FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0xD8, FC_RADDR_SDR, FC_FLEXSPI_1PAD, 0x18),

                    /* ---------- Seq 9: Quad Input Page Program (32h), 1-1-4 ----------
                     * 32h | A23-A0 (1 pad) | Data (4 pads) */
                    [4 * 9 + 0] =
                        FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0x32, FC_RADDR_SDR, FC_FLEXSPI_1PAD, 0x18),
                    [4 * 9 + 1] =
                        FC_FLEXSPI_LUT_SEQ(FC_WRITE_SDR, FC_FLEXSPI_4PAD, 0x00, FC_STOP_EXE, FC_FLEXSPI_1PAD, 0x00),

                    /* ---------- Seq 11: Chip Erase (C7h) ---------- */
                    [4 * 11 + 0] =
                        FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0xC7, FC_STOP_EXE, FC_FLEXSPI_1PAD, 0x00),
                },
        },
    .pageSize           = 0x100,   /* 256 bytes per page */
    .sectorSize         = 0x1000,  /* 4 KB sector erase granularity */
    .ipcmdSerialClkFreq = 0,
    .blockSize          = 0x10000, /* 64 KB block erase granularity */
    .fcb_fill[0]        = 0xFFFFFFFF,
};
#endif /* BOOT_HEADER_ENABLE */
