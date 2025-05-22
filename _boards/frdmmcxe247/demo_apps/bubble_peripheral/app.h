/*
 * Copyright 2025 NXP
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
#define BOARD_FTM_BASEADDR         FTM3
#define BOARD_FIRST_TIMER_CHANNEL  7U
#define BOARD_SECOND_TIMER_CHANNEL 3U
#define BOARD_FIRST_CHANNEL_INT    kFTM_Chnl0InterruptEnable
#define BOARD_SECOND_CHANNEL_INT   kFTM_Chnl3InterruptEnable
#define BOARD_FTM_IRQ_HANDLER_FUNC FTM3_IRQHandler

#define I2C_RELEASE_SDA_PORT  PORTA
#define I2C_RELEASE_SCL_PORT  PORTA
#define I2C_RELEASE_SDA_GPIO  GPIOA
#define I2C_RELEASE_SDA_PIN   2U
#define I2C_RELEASE_SCL_GPIO  GPIOA
#define I2C_RELEASE_SCL_PIN   3U
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
