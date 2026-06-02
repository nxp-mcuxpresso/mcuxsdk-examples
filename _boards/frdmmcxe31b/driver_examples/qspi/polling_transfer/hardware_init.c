/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
uint32_t lut[FSL_FEATURE_QSPI_LUT_DEPTH] =
{
    /* Seq0 :Quad Read */
    /* CMD:        0x6B - Quad Read, Single pad */
    /* ADDR:       0x18 - 24bit address, Single pads */
    /* DUMMY:      0x06 - 8 clock cyles, Quad pads */
    /* READ:       0x80 - Read 128 bytes, Quad pads */
    //[0] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x03, QSPI_ADDR, QSPI_PAD_1, 0x18),
    //[1] = QSPI_LUT_SEQ(QSPI_READ, QSPI_PAD_1, 0x80, 0, 0, 0),
    [0] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x6B, QSPI_ADDR, QSPI_PAD_1, 0x18),
    [1] = QSPI_LUT_SEQ(QSPI_DUMMY, QSPI_PAD_4, 0x08, QSPI_READ, QSPI_PAD_4, 0x80),

    /* Seq1: Write enable or Erase Sector */
    [QSPI_CMD_SEQ_WRITE_ENABLE] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x06, 0, 0, 0),
    //[QSPI_CMD_SEQ_ERASE_SECTOR] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x20, QSPI_ADDR, QSPI_PAD_1, 0x18),

    /* Seq2: Read Status */
    /* CMD:    0x05 - Read Status, single pad */
    /* READ:   0x01 - Read 1 byte */
    [QSPI_CMD_SEQ_READ_STATUS_REG] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x05, QSPI_READ, QSPI_PAD_1, 0x1),

    /* Seq3: Page Program */
    /* CMD:    0x32 - Quad Page Program, Single pad */
    /* ADDR:   0x18 - 24bit address, Quad pad */
    /* WRITE:  0x80 - Write 128 bytes at one pass, Quad pad */
    //[15] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x02, QSPI_ADDR, QSPI_PAD_1, 0x18),
    //[16] = QSPI_LUT_SEQ(QSPI_WRITE, QSPI_PAD_1, 0x80, 0, 0, 0),
    [QSPI_CMD_SEQ_PROGRAM_PAGE] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x32, QSPI_ADDR, QSPI_PAD_1, 0x18),
    [QSPI_CMD_SEQ_PROGRAM_PAGE + 1U] = QSPI_LUT_SEQ(QSPI_WRITE, QSPI_PAD_4, 0x80, 0, 0, 0),

    /* Erase All */
    /* CMD:    0x60 - Erase All chip, Single pad */
    //[x] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x60, 0, 0, 0),

    /* Write Register */
    /* CMD:    0x01 - Write Status Register, single pad */
    /* WRITE:  0x01 - Write 1 byte of data, single pad */
    //[x] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x01, QSPI_WRITE, QSPI_PAD_1, 0x1),

    /* Read Config Register */
    /* CMD:  0x15 - Read Config register, single pad */
    /* READ: 0x01 - Read 1 byte */
    //[x] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x15, QSPI_READ, QSPI_PAD_1, 0x1),

    /* Read ID */
    //[x] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x90, QSPI_ADDR, QSPI_PAD_1, 0x18),
    //[x] = QSPI_LUT_SEQ(QSPI_READ, QSPI_PAD_1, 0x2, 0, 0, 0),
};

qspi_flash_config_t single_config = {.flashA1Size       = FLASH_SIZE,
                                     .flashA2Size       = 0,
                                     .CSHoldTime        = 3,
                                     .CSSetupTime       = 3,
                                     .cloumnspace       = 0,
                                     .enableWordAddress = false};
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitFLASHPins();

    /* Set QuadSPI SFCK from PLL_PHI1_CLK. */
    CLOCK_AttachClk(kPLL_PHI1_CLK_to_QSPI_SFCK);
    CLOCK_SetClkDiv(kCLOCK_DivQspiSfckClk, 4U);
}

void BOARD_QspiSocConfigure(QuadSPI_Type *base)
{
    qspi_delay_chain_config_t delayConfig = {.dqsDelayEnable = true};

    QSPI_SetDelayChainConfig(base, &delayConfig);
}

#if defined(QSPI_CMD_REUSE_LUT) && QSPI_CMD_REUSE_LUT
void BOARD_QspiUpdateLUT(uint8_t seqID, uint8_t cmdType)
{
    uint32_t cmd[FSL_FEATURE_QSPI_LUT_SEQ_UNIT] = {0};

    switch(cmdType)
    {
        case QSPI_CMD_TYPE_WRITE_ENABLE:
            cmd[0] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x06, 0, 0, 0);
            break;
        case QSPI_CMD_TYPE_ERASE_SECTOR:
            cmd[0] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x20, QSPI_ADDR, QSPI_PAD_1, 0x18);
            break;
        default:
            assert(false);
            break;
    }

    QSPI_UpdateLUT(EXAMPLE_QSPI, seqID, &cmd[0]);
}
#endif
/*${function:end}*/
