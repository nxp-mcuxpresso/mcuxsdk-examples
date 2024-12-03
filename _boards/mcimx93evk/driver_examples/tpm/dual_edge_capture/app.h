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
#define DEMO_TPM_BASEADDR TPM2

/* TPM channel pair used for the dual-edge capture, channel pair 0 uses channels 0 and 1 */
#define BOARD_TPM_INPUT_CAPTURE_CHANNEL_PAIR kTPM_Chnl_0

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_FIRST_CHANNEL_INTERRUPT_ENABLE  kTPM_Chnl0InterruptEnable
#define TPM_FIRST_CHANNEL_FLAG              kTPM_Chnl0Flag
#define TPM_SECOND_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl1InterruptEnable
#define TPM_SECOND_CHANNEL_FLAG             kTPM_Chnl1Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER      TPM2_IRQn
#define TPM_INPUT_CAPTURE_HANDLER TPM2_IRQHandler

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
