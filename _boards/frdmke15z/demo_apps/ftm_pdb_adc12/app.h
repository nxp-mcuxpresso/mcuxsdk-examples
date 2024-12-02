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
/* For ADC. */
#define DEMO_ADC_BASE          ADC0
#define DEMO_ADC_USER_CHANNEL  1U
#define DEMO_ADC_CHANNEL_GROUP 0U
#define DEMO_ADC_IRQ_ID        ADC0_IRQn
#define DEMO_ADC_IRQ_HANDLER   ADC0_IRQHandler

/* For PDB. */
#define DEMO_PDB_BASE                   PDB0
#define DEMO_PDB_TRIGGER_CHANNEL        kPDB_ADCTriggerChannel0
#define DEMO_PDB_PRETRIGGER_CHANNEL     kPDB_ADCPreTrigger0
#define DEMO_PDB_MODULO_VALUE           2000U
#define DEMO_PDB_PRETRIGGER_DELAY_VALUE 500U

/* For FTM. */
#define DEMO_FTM_BASE                 FTM0
#define DEMO_FTM_COUNTER_CLOCK_HZ     CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define DEMO_FTM_COUNTER_CLOCK_SOURCE kFTM_SystemClock
#define DEMO_FTM_PWM_HZ               24000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
