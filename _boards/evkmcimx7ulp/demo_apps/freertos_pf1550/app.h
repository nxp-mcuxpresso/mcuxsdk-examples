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
#define DEMO_PF1550_LPI2C_BASE        LPI2C3
#define DEMO_PF1550_LPI2C_IRQ         LPI2C3_IRQn
#define DEMO_PF1550_LPI2C_PRIO        (5U)
#define DEMO_PF1550_LPI2C_CLKSRC_FREQ CLOCK_GetIpFreq(kCLOCK_Lpi2c3)
#define DEMO_PF1550_LPI2C_BAUDRATE    (400000U)
#define DEMO_PF1550_LPI2C_SCL_GPIO    GPIOB
#define DEMO_PF1550_LPI2C_SCL_PIN     (12U)
#define DEMO_PF1550_LPI2C_SDA_GPIO    GPIOB
#define DEMO_PF1550_LPI2C_SDA_PIN     (13U)
#define DEMO_PF1550_LPI2C_DELAY       (100U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_I2C_ReleaseBus(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
