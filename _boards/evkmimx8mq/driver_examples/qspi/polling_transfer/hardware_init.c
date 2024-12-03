/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
    {/* Seq0 :Quad Read */
     /* CMD:        0x6B - Quad Read, Single pad */
     /* ADDR:       0x18 - 24bit address, Single pads */
     /* DUMMY:      0x06 - 8 clock cyles, Quad pads */
     /* READ:       0x80 - Read 128 bytes, Quad pads */
     /* JUMP_ON_CS: 0 */
     [0] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x6B, QSPI_ADDR, QSPI_PAD_1, 0x18),
     [1] = QSPI_LUT_SEQ(QSPI_DUMMY, QSPI_PAD_4, 0x08, QSPI_READ, QSPI_PAD_4, 0x80),
     [2] = QSPI_LUT_SEQ(QSPI_JMP_ON_CS, QSPI_PAD_1, 0x0, 0, 0, 0),

     /* Seq1: Write Enable */
     /* CMD:      0x06 - Write Enable, Single pad */
     [4] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x06, 0, 0, 0),

     /* Seq2: Erase All */
     /* CMD:    0x60 - Erase All chip, Single pad */
     [8] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x60, 0, 0, 0),

     /* Seq3: Read Status */
     /* CMD:    0x05 - Read Status, single pad */
     /* READ:   0x01 - Read 1 byte */
     [12] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x05, QSPI_READ, QSPI_PAD_1, 0x1),

     /* Seq4: Page Program */
     /* CMD:    0x02 - Page Program, Single pad */
     /* ADDR:   0x18 - 24bit address, Quad pad */
     /* WRITE:  0x80 - Write 128 bytes at one pass, Quad pad */
     [16] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x38, QSPI_ADDR, QSPI_PAD_4, 0x18),
     [17] = QSPI_LUT_SEQ(QSPI_WRITE, QSPI_PAD_4, 0x80, 0, 0, 0),

     /* Seq5: Write Register */
     /* CMD:    0x01 - Write Status Register, single pad */
     /* WRITE:  0x01 - Write 1 byte of data, single pad */
     [20] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x61, QSPI_WRITE, QSPI_PAD_1, 0x1),

     /* Seq6: Read Config Register */
     /* CMD:  0x15 - Read Config register, single pad */
     /* READ: 0x01 - Read 1 byte */
     [24] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x15, QSPI_READ, QSPI_PAD_1, 0x1),

     /* Seq7: Erase Sector */
     /* CMD:  0x20 - Sector Erase, single pad */
     /* ADDR: 0x18 - 24 bit address, single pad */
     [28] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0xD8, QSPI_ADDR, QSPI_PAD_1, 0x18),

     /* Match MISRA rule */
     [63] = 0};

qspi_flash_config_t single_config = {.flashA1Size = FLASH_SIZE, /* 4MB */
                                     .flashA2Size = 0,
#if defined(FSL_FEATURE_QSPI_SUPPORT_PARALLEL_MODE) && (FSL_FEATURE_QSPI_SUPPORT_PARALLEL_MODE)
                                     .flashB1Size = FLASH_SIZE,
                                     .flashB2Size = 0,
#endif
#if !defined(FSL_FEATURE_QSPI_HAS_NO_TDH) || (!FSL_FEATURE_QSPI_HAS_NO_TDH)
                                     .dataHoldTime = 0,
#endif
                                     .CSHoldTime        = 0,
                                     .CSSetupTime       = 0,
                                     .cloumnspace       = 0,
                                     .dataLearnValue    = 0,
                                     .endian            = kQSPI_64LittleEndian,
                                     .enableWordAddress = false};
void BOARD_InitHardware(void)
{
    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMemory();

    CLOCK_SetRootMux(kCLOCK_RootQspi, kCLOCK_QspiRootmuxSysPll1Div8); /* Set QSPI source to SYSTEM PLL1 DIV8 100MHZ */
    CLOCK_SetRootDivider(kCLOCK_RootQspi, 1U, 2U);                    /* Set root clock to 100MHZ / 2 = 50MHZ */
}
/*${function:end}*/
