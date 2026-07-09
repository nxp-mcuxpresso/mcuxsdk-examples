/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPADC_BASE                         HSADC_0
#define DEMO_LPADC_USER_CHANNEL                 3U
#define DEMO_LPADC_USER_CMDID                   1U
#define DEMO_LPADC_SAMPLE_COUNT                 16U
#define DEMO_LPADC_RESFIFO_REG_ADDR             (&DEMO_LPADC_BASE->RESFIFO)
#define DEMO_LPADC_VREF_SOURCE                  kLPADC_ReferenceVoltageAlt3
#define DEMO_LPADC_DO_OFFSET_CALIBRATION        true
#define DEMO_LPADC_OFFSET_VALUE                 0x10U

#define DEMO_DMA_BASE                           DMA0
#define DEMO_DMA_CHANNEL                        0U
#define DEMO_DMA_REQUEST                        kDmaRequestADCFifoA

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);

#endif /* _APP_H_ */
