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
#define DEMO_TPM_BASEADDR                    TPM2
#define BOARD_TPM_INPUT_CAPTURE_CHANNEL_PAIR kTPM_Chnl_0
#define TPM_INTERRUPT_NUMBER                 TPM2_IRQn

#ifdef ENABLE_RAM_VECTOR_TABLE
/* Put the IRQ handler in RAM to reduce latency. */
AT_QUICKACCESS_SECTION_CODE(void TPM_INPUT_CAPTURE_HANDLER(void));
#else
/* Map example IRQHandler to name of vector table */
#define TPM_INPUT_CAPTURE_HANDLER TPM2_IRQHandler
#endif

#define TPM_FIRST_CHANNEL_INTERRUPT_ENABLE  kTPM_Chnl0InterruptEnable
#define TPM_FIRST_CHANNEL_FLAG              kTPM_Chnl0Flag
#define TPM_SECOND_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl1InterruptEnable
#define TPM_SECOND_CHANNEL_FLAG             kTPM_Chnl1Flag
#define TPM_SOURCE_CLOCK                    CLOCK_GetIpFreq(kCLOCK_Tpm2)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
