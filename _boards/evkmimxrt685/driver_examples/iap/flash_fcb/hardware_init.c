/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${macro:start}*/
#define FLASH_CONFIG_BLOCK_TAG     (0x42464346)
#define FLASH_CONFIG_BLOCK_VERSION (0x56010400)

#define CMD_SDR        0x01
#define CMD_DDR        0x21
#define RADDR_SDR      0x02
#define RADDR_DDR      0x22
#define WRITE_SDR      0x08
#define WRITE_DDR      0x28
#define READ_SDR       0x09
#define READ_DDR       0x29
#define DUMMY_SDR      0x0C
#define DUMMY_DDR      0x2C
#define DUMMY_RWDS_SDR 0x0D
#define DUMMY_RWDS_DDR 0x2D
#define STOP_EXE       0

#define FLEXSPI_1PAD 0
#define FLEXSPI_2PAD 1
#define FLEXSPI_4PAD 2
#define FLEXSPI_8PAD 3

#define FLEXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1)                                                              \
    (FLEXSPI_LUT_OPERAND0(op0) | FLEXSPI_LUT_NUM_PADS0(pad0) | FLEXSPI_LUT_OPCODE0(cmd0) | FLEXSPI_LUT_OPERAND1(op1) | \
     FLEXSPI_LUT_NUM_PADS1(pad1) | FLEXSPI_LUT_OPCODE1(cmd1))
/*${macro:end}*/

/*${variable:start}*/
flexspi_nor_config_t
    config =
        {
            .memConfig =
                {
                    .tag                 = FLASH_CONFIG_BLOCK_TAG,
                    .version             = FLASH_CONFIG_BLOCK_VERSION,
                    .csHoldTime          = 3,
                    .csSetupTime         = 3,
                    .deviceModeCfgEnable = 1,
                    .deviceModeType      = kDeviceConfigCmdType_Generic,
                    .waitTimeCfgCommands = 1,
                    .deviceModeSeq =
                        {
                            .seqNum   = 1,
                            .seqId    = 6, /* See Lookup table for more details */
                            .reserved = 0,
                        },
                    .deviceModeArg   = 0,
                    .configCmdEnable = 1,
                    .configModeType  = {kDeviceConfigCmdType_Generic, kDeviceConfigCmdType_Spi2Xpi,
                                        kDeviceConfigCmdType_Generic},
                    .configCmdSeqs   = {{
                                            .seqNum   = 1,
                                            .seqId    = 7,
                                            .reserved = 0,
                                      },
                                        {
                                            .seqNum   = 1,
                                            .seqId    = 10,
                                            .reserved = 0,
                                      }},
                    .configCmdArgs   = {0x2, 0x1},
                    .controllerMiscOption =
                        0x50, /* Safe Configuration Frequency enable & DDR clock confiuration indication */
                    .deviceType    = 0x1,
                    .sflashPadType = kSerialFlash_8Pads,
                    .serialClkFreq = 1,
                    .sflashA1Size  = 0,
                    .sflashA2Size  = 0,
                    .sflashB1Size  = BOARD_FLASH_SIZE,
                    .sflashB2Size  = 0,
                    .lookupTable =
                        {
                            /* Read */
                            [0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0xEC, CMD_SDR, FLEXSPI_8PAD, 0x13),
                            [1] = FLEXSPI_LUT_SEQ(RADDR_SDR, FLEXSPI_8PAD, 0x20, DUMMY_SDR, FLEXSPI_8PAD, 0x14),
                            [2] = FLEXSPI_LUT_SEQ(READ_SDR, FLEXSPI_8PAD, 0x04, STOP_EXE, FLEXSPI_1PAD, 0x00),

                            /* Read status SPI */
                            [4 * 1 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x05, READ_SDR, FLEXSPI_1PAD, 0x04),

                            /* Read Status OPI */
                            [4 * 2 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x05, CMD_SDR, FLEXSPI_8PAD, 0xFA),
                            [4 * 2 + 1] = FLEXSPI_LUT_SEQ(RADDR_SDR, FLEXSPI_8PAD, 0x20, DUMMY_SDR, FLEXSPI_8PAD, 0x14),
                            [4 * 2 + 2] = FLEXSPI_LUT_SEQ(READ_SDR, FLEXSPI_8PAD, 0x04, STOP_EXE, FLEXSPI_1PAD, 0x00),

                            /* Write Enable */
                            [4 * 3 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x06, STOP_EXE, FLEXSPI_1PAD, 0x00),
                            /* Write Enable - OPI */
                            [4 * 4 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x06, CMD_SDR, FLEXSPI_8PAD, 0xF9),

                            /* Sector Erase */
                            [4 * 5 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x21, CMD_SDR, FLEXSPI_8PAD, 0xDE),
                            [4 * 5 + 1] = FLEXSPI_LUT_SEQ(RADDR_SDR, FLEXSPI_8PAD, 0x20, STOP_EXE, FLEXSPI_1PAD, 0x00),

                            /* Configure dummy cycles */
                            [4 * 6 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x72, CMD_SDR, FLEXSPI_1PAD, 0x00),
                            [4 * 6 + 1] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x00, CMD_SDR, FLEXSPI_1PAD, 0x03),
                            [4 * 6 + 2] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x00, WRITE_SDR, FLEXSPI_1PAD, 0x01),

                            /* Configure Register */
                            [4 * 7 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x72, CMD_SDR, FLEXSPI_1PAD, 0x00),
                            [4 * 7 + 1] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x00, CMD_SDR, FLEXSPI_1PAD, 0x02),
                            [4 * 7 + 2] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x00, WRITE_SDR, FLEXSPI_1PAD, 0x01),

                            /* Block erase (64KB) */
                            [4 * 8 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0xDC, CMD_SDR, FLEXSPI_8PAD, 0x23),
                            [4 * 8 + 1] = FLEXSPI_LUT_SEQ(RADDR_SDR, FLEXSPI_8PAD, 0x20, STOP_EXE, FLEXSPI_1PAD, 0x00),

                            /* Page program */
                            [4 * 9 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x12, CMD_SDR, FLEXSPI_8PAD, 0xED),
                            [4 * 9 + 1] = FLEXSPI_LUT_SEQ(RADDR_SDR, FLEXSPI_8PAD, 0x20, WRITE_SDR, FLEXSPI_8PAD, 0x04),

                            /* Enable OPI STR mode */
                            [4 * 10 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x72, CMD_SDR, FLEXSPI_1PAD, 0x00),
                            [4 * 10 + 1] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x00, CMD_SDR, FLEXSPI_1PAD, 0x00),
                            [4 * 10 + 2] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x00, WRITE_SDR, FLEXSPI_1PAD, 0x01),

                            /* Read identification */
                            [4 * 11 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_8PAD, 0x60, CMD_SDR, FLEXSPI_8PAD, 0xf0),
                        },
                },
            .pageSize           = 0x100,
            .sectorSize         = 0x1000,
            .ipcmdSerialClkFreq = 1, /* Serial Clock Frequency for IP command, the same definition as serialClkFreq. */
            .blockSize          = 0x10000,
            .serialNorType      = 2, /* Serial Nor Type XPI. */
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    RESET_PeripheralReset(kHSGPIO2_RST_SHIFT_RSTn);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Switch flexspi clock to FFRO*/
    IAP_FlexspiSetClockSource(3);
}
AT_QUICKACCESS_SECTION_CODE(status_t Demo_FlexspiNorInit(uint32_t instance, flexspi_nor_config_t *config))
{
    /* Reset external flash */
    GPIO->CLR[2] = 1 << 12;
    for (uint32_t i = 0; i < 6000; i++)
        __NOP();
    GPIO->SET[2] = 1 << 12;
    /* Clear FLEXSPI NOR flash configure context */
    SYSCTL0->FLEXSPI_BOOTROM_SCRATCH0 = 0;

    /* Wait until the FLEXSPI is idle */
    register uint32_t delaycnt = 10000u;

    while ((delaycnt--) != 0U)
    {
    }
    status_t status = IAP_FlexspiNorInit(instance, config);

    if ((CACHE64->CCR & CACHE64_CTRL_CCR_ENCACHE_MASK) != 0)
    {
        /* set command to invalidate all ways and write GO bit to initiate command */
        CACHE64->CCR = CACHE64_CTRL_CCR_INVW1_MASK | CACHE64_CTRL_CCR_INVW0_MASK;
        CACHE64->CCR |= CACHE64_CTRL_CCR_GO_MASK;
        /* Wait until the command completes */
        while (CACHE64->CCR & CACHE64_CTRL_CCR_GO_MASK)
        {
        }
        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        CACHE64->CCR &= ~(CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK);
    }
    return status;
}
/*${function:end}*/
