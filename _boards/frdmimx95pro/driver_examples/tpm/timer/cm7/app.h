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
#define BOARD_TPM TPM3

/* Interrupt number and interrupt handler for the TPM instance used */
#define BOARD_TPM_IRQ_NUM TPM3_IRQn
#define BOARD_TPM_HANDLER TPM3_IRQHandler

/* Get source clock for TPM driver */
#define LPTPM_CLOCK_ROOT hal_clock_buswakeup
#define TPM_SOURCE_CLOCK HAL_ClockGetIpFreq(LPTPM_CLOCK_ROOT)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
