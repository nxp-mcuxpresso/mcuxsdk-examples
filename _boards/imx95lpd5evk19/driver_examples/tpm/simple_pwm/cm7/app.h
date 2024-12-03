/*
 * Copyright 2023 NXP
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
#define BOARD_TPM_BASEADDR TPM2
#define BOARD_TPM_CHANNEL  kTPM_Chnl_2

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl2InterruptEnable
#define TPM_CHANNEL_FLAG             kTPM_Chnl2Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM2IRQn
#define TPM_LED_HANDLER      TPM2_IRQHandler

/* Get source clock for TPM driver */
#define LPTPM_CLOCK_ROOT hal_clock_tpm2
#define TPM_SOURCE_CLOCK HAL_ClockGetIpFreq(LPTPM_CLOCK_ROOT)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
