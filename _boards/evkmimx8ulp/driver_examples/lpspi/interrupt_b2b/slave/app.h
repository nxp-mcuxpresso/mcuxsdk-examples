/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_LPSPI_SLAVE_BASEADDR         LPSPI1
#define EXAMPLE_LPSPI_SLAVE_IRQN             LPSPI1_IRQn
#define EXAMPLE_LPSPI_SLAVE_IRQHandler       LPSPI1_IRQHandler
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     kLPSPI_Pcs0
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER kLPSPI_SlavePcs0

#define EXAMPLE_LPSPI_SLAVE_CLOCK_NAME   (kCLOCK_Lpspi1)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE (kCLOCK_Pcc0BusIpSrcSysOscDiv2)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
#ifdef ENABLE_RAM_VECTOR_TABLE
#include "fsl_lpspi.h"

#if (defined(__ICCARM__))
#pragma inline = forced
static inline uint32_t LPSPI_GetRxFifoCount(LPSPI_Type *base);
#pragma inline = forced
static inline uint32_t LPSPI_ReadData(LPSPI_Type *base);
#pragma inline = forced
static inline void LPSPI_DisableInterrupts(LPSPI_Type *base, uint32_t mask);
#pragma inline = forced
static inline uint32_t LPSPI_GetTxFifoCount(LPSPI_Type *base);
#pragma inline = forced
static inline void LPSPI_WriteData(LPSPI_Type *base, uint32_t data);
#elif (defined(__GNUC__))
static inline uint32_t LPSPI_GetRxFifoCount(LPSPI_Type *base) __attribute__((always_inline));
static inline uint32_t LPSPI_ReadData(LPSPI_Type *base) __attribute__((always_inline));
static inline void LPSPI_DisableInterrupts(LPSPI_Type *base, uint32_t mask) __attribute__((always_inline));
static inline uint32_t LPSPI_GetTxFifoCount(LPSPI_Type *base) __attribute__((always_inline));
static inline void LPSPI_WriteData(LPSPI_Type *base, uint32_t data) __attribute__((always_inline));
#else
#error Toolchain not supported.
#endif
/* Put the IRQ handler in RAM to reduce latency. */
#endif
AT_QUICKACCESS_SECTION_CODE(void EXAMPLE_LPSPI_SLAVE_IRQHandler(void));
/*${prototype:end}*/

#endif /* _APP_H_ */
