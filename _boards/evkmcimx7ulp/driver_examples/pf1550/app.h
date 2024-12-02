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
#define DEMO_PF1550_INTB_GPIO         GPIOB
#define DEMO_PF1550_INTB_PORT         PORTB
#define DEMO_PF1550_INTB_PIN          (11U)
#define DEMO_PF1550_INTB_HANDLER      PCTLB_IRQHandler
#define DEMO_PF1550_PORT_IRQ          PCTLB_IRQn
#define DEMO_PF1550_EWM_OUT_B_GPIO    GPIOA
#define DEMO_PF1550_EWM_OUT_B_PIN     (22U)
#define DEMO_BATT_ADC_EN_GPIO         GPIOA
#define DEMO_BATT_ADC_EN_PIN          (12U)
#define DEMO_LPADC_BASE               ADC0
#define DEMO_PF1550_LPI2C             LPI2C3
#define DEMO_PF1550_LPI2C_CLKSRC_FREQ CLOCK_GetIpFreq(kCLOCK_Lpi2c3)
#define DEMO_PF1550_LPI2C_BAUDRATE    (100000U)
#define LPIT_SOURCECLOCK              CLOCK_GetIpFreq(kCLOCK_Lpit0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
