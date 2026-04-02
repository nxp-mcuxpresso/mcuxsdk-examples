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
#define DEMO_TPM_BASEADDR               TPM_0
#define BOARD_TPM_INPUT_CAPTURE_CHANNEL kTPM_Chnl_3
/* Interrupt to enable and flag to read; depends on the TPM channel pair used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl3InterruptEnable
#define TPM_CHANNEL_FLAG             kTPM_Chnl3Flag
/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER      TPM_0_IRQn
#define TPM_INPUT_CAPTURE_HANDLER TPM_0_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
