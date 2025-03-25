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
{    /* Seq0 :Quad Read */
     /* CMD:        0x6B - Quad Read, Single pad */
     /* ADDR:       0x18 - 24bit address, Single pads */
     /* DUMMY:      0x06 - 8 clock cyles, Quad pads */
     /* READ:       0x80 - Read 128 bytes, Quad pads */
//     [0] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x03, QSPI_ADDR, QSPI_PAD_1, 0x18),
//     [1] = QSPI_LUT_SEQ(QSPI_READ, QSPI_PAD_1, 0x80, 0, 0, 0),
     [0] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x6B, QSPI_ADDR, QSPI_PAD_1, 0x18),
     [1] = QSPI_LUT_SEQ(QSPI_DUMMY, QSPI_PAD_4, 0x08, QSPI_READ, QSPI_PAD_4, 0x80),

     /* Seq1: Write enable + Erase Sector */
     [5] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x06, QSPI_CMD, QSPI_PAD_1, 0xD8),
     [6] = QSPI_LUT_SEQ(QSPI_ADDR, QSPI_PAD_1, 0x18, 0, 0, 0),

     /* Seq2: Read Status */
     /* CMD:    0x05 - Read Status, single pad */
     /* READ:   0x01 - Read 1 byte */
     [10] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x05, QSPI_READ, QSPI_PAD_1, 0x1),

     /* Seq3: Write enable + Page Program */
     /* CMD:    0x02 - Page Program, Single pad */
     /* ADDR:   0x18 - 24bit address, Quad pad */
     /* WRITE:  0x80 - Write 128 bytes at one pass, Quad pad */
     //[15] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0xD8, QSPI_ADDR, QSPI_PAD_1, 0x18),
     //[16] = QSPI_LUT_SEQ(QSPI_WRITE, QSPI_PAD_1, 0x80, 0, 0, 0),
     [15] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x06, QSPI_CMD, QSPI_PAD_1, 0x32),
     [16] = QSPI_LUT_SEQ(QSPI_ADDR, QSPI_PAD_1, 0x18, QSPI_WRITE, QSPI_PAD_4, 0x80),

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

qspi_flash_config_t single_config = {.flashA1Size = FLASH_SIZE, /* 4MB */
                                     .flashA2Size = 0,
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
    CLOCK_SetClkDiv(kCLOCK_DivQspiSfckClk, 4U);
    CLOCK_AttachClk(kPLL_PHI1_CLK_to_QSPI_SFCK);
}
/*${function:end}*/
