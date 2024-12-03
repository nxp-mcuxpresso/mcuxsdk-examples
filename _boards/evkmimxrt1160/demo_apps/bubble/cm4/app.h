/*
 * Copyright 2021 NXP
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
#define BOARD_QTMR_BASEADDR      TMR2
#define BOARD_QTMR_FIRST_CHANNEL (kQTMR_Channel_0)
#define QTMR_IRQ_HANDLER         TMR2_IRQHandler
#define QTMR_IRQ_ID              TMR2_IRQn
#define BOARD_PWM_FREQ           (5000U)

/* Get source clock for QTMR driver */
#define QTMR_SOURCE_CLOCK (CLOCK_GetRootClockFreq(kCLOCK_Root_Bus))

/* LPI2C */
#define I2C_BAUDRATE (100000U)

#define I2C_RELEASE_SDA_GPIO  GPIO12
#define I2C_RELEASE_SDA_PIN   6U
#define I2C_RELEASE_SCL_GPIO  GPIO12
#define I2C_RELEASE_SCL_PIN   5U
#define I2C_RELEASE_BUS_COUNT 100U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_I2C_ReleaseBus(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
