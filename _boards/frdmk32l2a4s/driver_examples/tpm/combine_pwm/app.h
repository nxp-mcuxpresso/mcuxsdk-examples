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
/* define instance */
#define BOARD_TPM_BASEADDR TPM0
/* TPM channel pair 1 works with TPM channels 2 and 3*/
#define BOARD_TPM_CHANNEL_PAIR kTPM_Chnl_1

/* Interrupt to enable and flag to read; depends on the TPM channel pair used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl2InterruptEnable
#define TPM_CHANNEL_FLAG             kTPM_Chnl2Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM0_IRQn
#define TPM_LED_HANDLER      TPM0_IRQHandler
/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetIpFreq(kCLOCK_Tpm0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
