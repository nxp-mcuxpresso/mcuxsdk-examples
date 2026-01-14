/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_SPI_FILTER_BASE         SPI_FILTER
#define EXAMPLE_SPI_FILTER_MANUFACTURER kSPI_FilterWinbond
#define EXAMPLE_FILTER_PERMITTED_ADDR   0x10000
#define EXAMPLE_FILTER_BLOCKED_ADDR     0x20000

#define EXAMPLE_FLEXSPI                 FLEXSPI0
#define FLASH_SIZE                      0x2000 /* 64Mb/KByte */
#define EXAMPLE_FLEXSPI_AMBA_BASE       FlexSPI0_AMBA_BASE
#define FLASH_PAGE_SIZE                 256
#define EXAMPLE_FLEXSPI_CLOCK           kCLOCK_Flexspi1
#define FLASH_PORT                      kFLEXSPI_PortB1
#define EXAMPLE_FLEXSPI_RX_SAMPLE_CLOCK kFLEXSPI_ReadSampleClkLoopbackInternally

#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL        10
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST          11
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD     0
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE        1
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR        2
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE 3
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD   4
#define NOR_CMD_LUT_SEQ_IDX_READID             5
#define NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG     6
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG      7
#define NOR_CMD_LUT_SEQ_IDX_ERASECHIP          8
#define NOR_CMD_LUT_SEQ_IDX_TEST_0x95          9

#define CUSTOM_LUT_LENGTH        60
#define FLASH_QUAD_ENABLE        0x40
#define FLASH_BUSY_STATUS_POL    1
#define FLASH_BUSY_STATUS_OFFSET 0

/*${macro:end}*/

/*******************************************************************************
 * Functions
 ******************************************************************************/
/*${function:start}*/

/*${function:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
static inline void flexspi_clock_init(void)
{
    CLOCK_SetClockDiv(kCLOCK_DivFLEXSPI0, 2);
    CLOCK_AttachClk(kFRO_HF_to_FLEXSPI);
}
/*${prototype:end}*/

#endif /* _APP_H_ */
