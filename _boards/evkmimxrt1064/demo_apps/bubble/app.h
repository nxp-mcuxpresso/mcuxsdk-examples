/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_QTMR_BASEADDR      TMR3
#define BOARD_QTMR_FIRST_CHANNEL (kQTMR_Channel_0)
#define QTMR_IRQ_HANDLER         TMR3_IRQHandler
#define QTMR_IRQ_ID              TMR3_IRQn

/* Get source clock for QTMR driver */
#define QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)

/* LPI2C */
#define I2C_BAUDRATE (100000U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
