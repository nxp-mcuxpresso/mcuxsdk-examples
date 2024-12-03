/*
 * Copyright 2024 NXP
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
#define DEMO_TPM_BASEADDR TPM2

/* TPM channel pair used for the dual-edge capture, channel pair 1 uses channels 2 and 3 */
#define BOARD_TPM_INPUT_CAPTURE_CHANNEL_PAIR kTPM_Chnl_1

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_FIRST_CHANNEL_INTERRUPT_ENABLE  kTPM_Chnl2InterruptEnable
#define TPM_FIRST_CHANNEL_FLAG              kTPM_Chnl2Flag
#define TPM_SECOND_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl3InterruptEnable
#define TPM_SECOND_CHANNEL_FLAG             kTPM_Chnl3Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER      TPM2_IRQn
#define TPM_INPUT_CAPTURE_HANDLER TPM2_IRQHandler

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
