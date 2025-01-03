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
#define DEMO_I2S_MASTER_CLOCK_FREQUENCY (24576000)
#define DEMO_DMA_INSTANCE_INDEX         (0U)
#define DEMO_I2S_TX_INSTANCE_INDEX      (7U)
#define DEMO_I2S_TX_CHANNEL             (19)
#define DEMO_I2S_TX_MODE                kHAL_AudioMaster
#define DEMO_I2S_RX_INSTANCE_INDEX      (6U)
#define DEMO_I2S_RX_CHANNEL             (16)
#define DEMO_I2S_RX_MODE                kHAL_AudioSlave

#define DEMO_GINT0_PORT kGINT_Port0
#define DEMO_GINT1_PORT kGINT_Port1

/* Select one input, active low for GINT0 */
#define DEMO_GINT0_POL_MASK ~(1U << BOARD_SW1_GPIO_PIN)
#define DEMO_GINT0_ENA_MASK (1U << BOARD_SW1_GPIO_PIN)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
