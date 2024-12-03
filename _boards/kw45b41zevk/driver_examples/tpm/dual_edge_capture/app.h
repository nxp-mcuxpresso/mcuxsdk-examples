/*
 * Copyright 2020 NXP
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
#define DEMO_TPM_BASEADDR TPM0
/* TPM channel pair used for the dual-edge capture, channel pair 0 uses channels 1 */
#define BOARD_TPM_INPUT_CAPTURE_CHANNEL_PAIR kTPM_Chnl_0
/* Interrupt to enable and flag to read; depends on the TPM channel pair used */
#define TPM_FIRST_CHANNEL_INTERRUPT_ENABLE  kTPM_Chnl0InterruptEnable
#define TPM_FIRST_CHANNEL_FLAG              kTPM_Chnl0Flag
#define TPM_SECOND_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl1InterruptEnable
#define TPM_SECOND_CHANNEL_FLAG             kTPM_Chnl1Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER      TPM0_IRQn
#define TPM_INPUT_CAPTURE_HANDLER TPM0_IRQHandler
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
