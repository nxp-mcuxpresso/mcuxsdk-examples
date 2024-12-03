/*
 * Copyright 2022 NXP
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
#define BOARD_TPM TPM2

/* Interrupt number and interrupt handler for the TPM instance used */
#define BOARD_TPM_IRQ_NUM TPM2_IRQn
#define BOARD_TPM_HANDLER TPM2_IRQHandler

/* Get source clock for TPM driver */
#define LPTPM_CLOCK_ROOT kCLOCK_Root_Tpm2
#define LPTPM_CLOCK_GATE kCLOCK_Tpm2
#define TPM_SOURCE_CLOCK CLOCK_GetIpFreq(LPTPM_CLOCK_ROOT)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
