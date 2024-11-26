/*
 * Copyright 2024 NXP
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
#define BOARD_TPM_BASEADDR TPM6
/* TPM channel pair 1 works with TPM channels 0 and 1*/
#define BOARD_TPM_CHANNEL_PAIR kTPM_Chnl_0

/* Interrupt to enable and flag to read; depends on the TPM channel pair used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl0InterruptEnable
#define TPM_CHANNEL_FLAG             kTPM_Chnl0Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define BOARD_TPM_IRQ_NUM TPM6_IRQn
#define BOARD_TPM_HANDLER TPM6_IRQHandler
/* Get source clock for TPM driver */
#define LPTPM_CLOCK_ROOT hal_clock_tpm6
#define TPM_SOURCE_CLOCK HAL_ClockGetRate(LPTPM_CLOCK_ROOT)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
