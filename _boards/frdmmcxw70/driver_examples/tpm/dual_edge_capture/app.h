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
/*${macro:start}*/
#define DEMO_TPM_BASEADDR TPM_0
/* TPM channel pair used for the dual-edge capture, channel pair 2 uses channels 4 */
#define BOARD_TPM_INPUT_CAPTURE_CHANNEL_PAIR kTPM_Chnl_2
/* Interrupt to enable and flag to read; depends on the TPM channel pair used */
#define TPM_FIRST_CHANNEL_INTERRUPT_ENABLE  kTPM_Chnl4InterruptEnable
#define TPM_FIRST_CHANNEL_FLAG              kTPM_Chnl4Flag
#define TPM_SECOND_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl5InterruptEnable
#define TPM_SECOND_CHANNEL_FLAG             kTPM_Chnl5Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER      TPM_0_IRQn
#define TPM_INPUT_CAPTURE_HANDLER TPM_0_IRQHandler
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
