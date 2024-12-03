/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_
/*${header:start}*/
#include "fsl_qspi.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_QSPI           QuadSPI0
#define QSPI_CLK_FREQ          CLOCK_GetIpFreq(kCLOCK_Qspi)
#define EXAMPLE_DMAMUX         DMA_CH_MUX0
#define EXAMPLE_CHANNEL        16U
#define EXAMPLE_DMA            DMA0
#define EXAMPLE_DMA_SOURCE     kDmaRequestMux0QSPITx
#define FLASH_PAGE_SIZE        256U
#define FLASH_SECTORE_SIZE     4096U
#define FLASH_SIZE             0x00800000U
#define FLASH_ENABLE_QUAD_CMD  0x40U
#define QSPI_ERASE_ADDR_OFFSET 0x40000U
/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
uint32_t lut[FSL_FEATURE_QSPI_LUT_DEPTH] =
    {/* Seq0 :Quad Read */
     /* CMD:        0xEB - Quad Read, Single pad */
     /* ADDR:       0x18 - 24bit address, Quad pads */
     /* DUMMY:      0x06 - 6 clock cyles, Quad pads */
     /* READ:       0x80 - Read 128 bytes, Quad pads */
     /* JUMP_ON_CS: 0 */
     [0] = 0x0A1804EB,
     [1] = 0x1E800E06,
     [2] = 0x2400,

     /* Seq1: Write Enable */
     /* CMD:      0x06 - Write Enable, Single pad */
     [4] = 0x406,

     /* Seq2: Erase All */
     /* CMD:    0x60 - Erase All chip, Single pad */
     [8] = 0x460,

     /* Seq3: Read Status */
     /* CMD:    0x05 - Read Status, single pad */
     /* READ:   0x01 - Read 1 byte */
     [12] = 0x1c010405,

     /* Seq4: Page Program */
     /* CMD:    0x02 - Page Program, Single pad */
     /* ADDR:   0x18 - 24bit address, Single pad */
     /* WRITE:  0x80 - Write 128 bytes at one pass, Single pad */
     [16] = 0x08180402,
     [17] = 0x2080,

     /* Seq5: Write Register */
     /* CMD:    0x01 - Write Status Register, single pad */
     /* WRITE:  0x01 - Write 1 byte of data, single pad */
     [20] = 0x20010401,

     /* Seq6: Read Config Register */
     /* CMD:  0x05 - Read Config register, single pad */
     /* READ: 0x01 - Read 1 byte */
     [24] = 0x1c010405,

     /* Seq7: Erase Sector */
     /* CMD:  0x20 - Sector Erase, single pad */
     /* ADDR: 0x18 - 24 bit address, single pad */
     [28] = 0x08180420,

     /* Seq8: Dummy */
     /* CMD:    0xFF - Dummy command, used to force SPI flash to exit continuous read mode */
     [32] = 0x4FF,

     /* Seq9: Fast Single read */
     /* CMD:        0x0B - Fast Read, Single Pad */
     /* ADDR:       0x18 - 24bit address, Single Pad */
     /* DUMMY:      0x08 - 8 clock cyles, Single Pad */
     /* READ:       0x80 - Read 128 bytes, Single Pad */
     /* JUMP_ON_CS: 0 */
     [36] = 0x0818040B,
     [37] = 0x1C800C08,
     [38] = 0x2400,

     /* Seq10: Fast Dual read */
     /* CMD:        0x3B - Dual Read, Single Pad */
     /* ADDR:       0x18 - 24bit address, Single Pad */
     /* DUMMY:      0x08 - 8 clock cyles, Single Pad */
     /* READ:       0x80 - Read 128 bytes, Dual pads */
     /* JUMP_ON_CS: 0 */
     [40] = 0x0818043B,
     [41] = 0x1D800C08,
     [42] = 0x2400,

     /* Match MISRA rule */
     [63] = 0};

qspi_flash_config_t single_config = {.flashA1Size = FLASH_SIZE, /* 4MB */
                                     .flashA2Size = 0,
#if defined(FSL_FEATURE_QSPI_SUPPORT_PARALLEL_MODE) && (FSL_FEATURE_QSPI_SUPPORT_PARALLEL_MODE)
                                     .flashB1Size = FLASH_SIZE,
                                     .flashB2Size = 0,
#endif
                                     .dataHoldTime      = 0,
                                     .CSHoldTime        = 0,
                                     .CSSetupTime       = 0,
                                     .cloumnspace       = 0,
                                     .dataLearnValue    = 0,
                                     .endian            = kQSPI_64LittleEndian,
                                     .enableWordAddress = false};
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
