/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#define DEMO_DMA_INSTANCE_INDEX    (0U)
#define DEMO_I2S_TX_INSTANCE_INDEX (0U)
#define DEMO_I2S_TX_MODE           kHAL_AudioMaster
#define DEMO_I2S_TX_CHANNEL        (13)
#define DEMO_I2S_RX_INSTANCE_INDEX (1U)
#define DEMO_I2S_RX_CHANNEL        (14)
#define DEMO_I2S_RX_MODE           kHAL_AudioSlave

#define DEMO_GINT0_PORT     kGINT_Port0
#define DEMO_GINT0_POL_MASK ~(1U << BOARD_SW2_GPIO_PIN)
#define DEMO_GINT0_ENA_MASK (1U << BOARD_SW2_GPIO_PIN)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
