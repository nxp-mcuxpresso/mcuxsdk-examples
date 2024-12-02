/*
 * Copyright 2024 NXP
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
/* The Flextimer instance/channel used for board */
#define BOARD_TIMER_BASEADDR         TPM2
#define BOARD_FIRST_TIMER_CHANNEL    0U
#define BOARD_SECOND_TIMER_CHANNEL   1U
#define BOARD_FIRST_CHANNEL_INT      kTPM_Chnl0InterruptEnable
#define BOARD_SECOND_CHANNEL_INT     kTPM_Chnl1InterruptEnable
#define BOARD_TIMER_PRESCALE_DIVIDER kTPM_Prescale_Divide_128
/* Get source clock for TPM driver */
#define BOARD_TIMER_SOURCE_CLOCK   CLOCK_GetFreq(kCLOCK_McgIrc48MClk)
#define TIMER_CLOCK_MODE           1U
#define BOARD_TPM_IRQ_HANDLER_FUNC TPM2_IRQHandler
#define BOARD_TPM_IRQ_ID           TPM2_IRQn
/* I2C source clock */
#define I2C_BAUDRATE 100000U

#define I2C_RELEASE_SDA_PORT  PORTD
#define I2C_RELEASE_SCL_PORT  PORTD
#define I2C_RELEASE_SDA_GPIO  GPIOD
#define I2C_RELEASE_SDA_PIN   6U
#define I2C_RELEASE_SCL_GPIO  GPIOD
#define I2C_RELEASE_SCL_PIN   7U
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
