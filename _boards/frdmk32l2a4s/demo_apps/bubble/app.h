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
/* The Flextimer instance/channel used for board */
#define BOARD_TIMER_BASEADDR       TPM0
#define BOARD_FIRST_TIMER_CHANNEL  2U
#define BOARD_SECOND_TIMER_CHANNEL 3U
/* Get source clock for TPM driver */
#define BOARD_TIMER_SOURCE_CLOCK (CLOCK_GetIpFreq(kCLOCK_Tpm0))
/* LPI2C */
#define I2C_BAUDRATE 100000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

/*${prototype:end}*/

#endif /* _APP_H_ */
