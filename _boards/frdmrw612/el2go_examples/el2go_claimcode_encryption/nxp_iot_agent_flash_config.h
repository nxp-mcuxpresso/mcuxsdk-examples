/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */
#ifndef _NXP_IOT_AGENT_FLASH_CONFIG_H_
#define _NXP_IOT_AGENT_FLASH_CONFIG_H_

#include "flash_config.h"
#include <fsl_romapi_iap.h>

const flexspi_nor_config_t flexspi_config_agent = {
    .memConfig =
        {
            .tag                 = FC_BLOCK_TAG,
            .version             = FC_BLOCK_VERSION,
            .readSampleClkSrc    = 1,
            .csHoldTime          = 3,
            .csSetupTime         = 3,
            .deviceModeCfgEnable = 1,
            .deviceModeSeq       = {.seqNum = 1, .seqId = 2},
            .deviceModeArg       = 0x02,
            .configCmdEnable     = 0,
            .deviceType          = 0x1,
            .sflashPadType       = kSerialFlash_4Pads,
            .serialClkFreq       = 5,
            .sflashA1Size        = 0x4000000U,
            .sflashA2Size        = 0,
            .sflashB1Size        = 0,
            .sflashB2Size        = 0,
            .lookupTable =
                {
                    /* Read */
                    [0] = FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0xEC, FC_RADDR_SDR, FC_FLEXSPI_4PAD, 0x20),
                    [1] = FC_FLEXSPI_LUT_SEQ(FC_MODE8_SDR, FC_FLEXSPI_4PAD, 0xF0, FC_DUMMY_SDR, FC_FLEXSPI_4PAD, 0x04),
                    [2] = FC_FLEXSPI_LUT_SEQ(FC_READ_SDR, FC_FLEXSPI_4PAD, 0x04, FC_STOP_EXE, FC_FLEXSPI_1PAD, 0x00),

                    /* Read Status */
                    [4 * 1 + 0] =
                        FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0x05, FC_READ_SDR, FC_FLEXSPI_1PAD, 0x04),

                    /* Write Status */
                    [4 * 2 + 0] =
                        FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0x31, FC_WRITE_SDR, FC_FLEXSPI_1PAD, 0x01),

                    /* Write Enable */
                    [4 * 3 + 0] =
                        FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0x06, FC_STOP_EXE, FC_FLEXSPI_1PAD, 0x00),

                    /* Sector erase */
                    [4 * 5 + 0] =
                        FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0x21, FC_RADDR_SDR, FC_FLEXSPI_1PAD, 0x20),

                    /* Block erase */
                    [4 * 8 + 0] =
                        FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0xDC, FC_RADDR_SDR, FC_FLEXSPI_1PAD, 0x20),

                    /* Page program */
                    [4 * 9 + 0] =
                        FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0x34, FC_RADDR_SDR, FC_FLEXSPI_1PAD, 0x20),
                    [4 * 9 + 1] =
                        FC_FLEXSPI_LUT_SEQ(FC_WRITE_SDR, FC_FLEXSPI_4PAD, 0x00, FC_STOP_EXE, FC_FLEXSPI_1PAD, 0x00),

                    /* chip erase */
                    [4 * 11 + 0] =
                        FC_FLEXSPI_LUT_SEQ(FC_CMD_SDR, FC_FLEXSPI_1PAD, 0xC7, FC_STOP_EXE, FC_FLEXSPI_1PAD, 0x00),
                },
        },
    .pageSize           = 0x100,
    .sectorSize         = 0x1000,
    .ipcmdSerialClkFreq = 0,
    .blockSize          = 0x10000,
    .flashStateCtx      = 0xFFFFFFFF,
};

#endif // #ifndef _NXP_IOT_AGENT_FLASH_CONFIG_H_
