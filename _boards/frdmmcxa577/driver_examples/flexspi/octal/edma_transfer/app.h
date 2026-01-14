/*
 * Copyright 2026,2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/

/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define FLASH_W25Q64    0
#define FLASH_MT35XU512 1

#ifndef EXAMPLE_FLASH_TYPE
/* support FLASH_W25Q64 and FLASH_MT35XU512 */
#define EXAMPLE_FLASH_TYPE FLASH_W25Q64
#endif

#define EXAMPLE_FLEXSPI           FLEXSPI0
#define EXAMPLE_CACHE             CACHE64_CTRL0
#define FLASH_SIZE                0x10000 /* 512Mb/KByte */
#define EXAMPLE_FLEXSPI_AMBA_BASE 0x80000000U
#define FLASH_PAGE_SIZE           256
#define EXAMPLE_SECTOR            1000
#define SECTOR_SIZE               0x1000 /* 4K */
#define EXAMPLE_FLEXSPI_CLOCK     kCLOCK_Flexspi
#define FLASH_PORT                kFLEXSPI_PortA1

#if EXAMPLE_FLASH_TYPE == FLASH_W25Q64
#define EXAMPLE_FLEXSPI_RX_SAMPLE_CLOCK kFLEXSPI_ReadSampleClkLoopbackFromDqsPad
#elif EXAMPLE_FLASH_TYPE == FLASH_MT35XU512
#define EXAMPLE_FLEXSPI_RX_SAMPLE_CLOCK kFLEXSPI_ReadSampleClkExternalInputFromDqsPad
#endif

#define NOR_CMD_LUT_SEQ_IDX_READ            0
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS      1
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE     2
#define NOR_CMD_LUT_SEQ_IDX_READID_OPI      3
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI 4
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR     5
#define NOR_CMD_LUT_SEQ_IDX_CHIPERASE       6
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM     7
#define NOR_CMD_LUT_SEQ_IDX_ENTEROPI        8
#define NOR_CMD_LUT_SEQ_IDX_WRITE           9
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI  10

#define CUSTOM_LUT_LENGTH        60
#define FLASH_BUSY_STATUS_POL    1
#define FLASH_BUSY_STATUS_OFFSET 0
#define FLASH_ERROR_STATUS_MASK  0x0e
#define FLASH_ENABLE_OCTAL_CMD   0xE7
#define CACHE_MAINTAIN           0

#if EXAMPLE_FLASH_TYPE == FLASH_W25Q64
#define FLASH_ENABLE_OCTAL_CMD       0xE7
#define EXAMPLE_FLASH_RESET_CONFIG() Flash_Reset()
#elif EXAMPLE_FLASH_TYPE == FLASH_MT35XU512
#define FLASH_ENABLE_OCTAL_CMD 0xE7
#endif

/* DMA related. */
#define EXAMPLE_FLEXSPI_DMA (DMA0)

#define FLEXSPI_TX_DMA_REQUEST_SOURCE kDma0RequestMuxFlexSpi0Tx
#define FLEXSPI_RX_DMA_REQUEST_SOURCE kDma0RequestMuxFlexSpi0Rx

#define FLEXSPI_TX_DMA_CHANNEL 0U
#define FLEXSPI_RX_DMA_CHANNEL 1U

/*${macro:end}*/
/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
typedef struct _flexspi_cache_status
{
    volatile bool CacheEnableFlag;
} flexspi_cache_status_t;
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

#if EXAMPLE_FLASH_TYPE == FLASH_W25Q64
status_t Flash_Reset(void);
#endif

/*${prototype:end}*/

#endif /* _APP_H_ */
