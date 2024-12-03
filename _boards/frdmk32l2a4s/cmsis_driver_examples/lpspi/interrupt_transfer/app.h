/*
 * Copyright 2019 NXP
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
/*Master related*/
#define EXAMPLE_LPSPI_MASTER_IRQN         (LPSPI0_IRQn)
#define EXAMPLE_LPSPI_MASTER_CLOCK_NAME   (kCLOCK_Lpspi0)
#define EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)
/*Slave related*/
#define EXAMPLE_LPSPI_SLAVE_IRQN         (LPSPI2_IRQn)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_NAME   (kCLOCK_Lpspi2)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)
/*INTMUX setting*/
#define EXAMPLE_LPSPI_INTMUX_CHANNLE (0U)
/* Driver mapping */
#define DRIVER_MASTER_SPI Driver_SPI0
#define DRIVER_SLAVE_SPI  Driver_SPI2
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
