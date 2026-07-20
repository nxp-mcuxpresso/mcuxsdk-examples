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
#define BOARD_TPM         TPM_0
#define BOARD_TPM_IRQ_NUM TPM_0_IRQn
#define BOARD_TPM_HANDLER TPM_0_IRQHandler
/* Get source clock for LPIT driver */
#define TPM_SOURCE_CLOCK (CLOCK_GetIpFreq(kCLOCK_Tpm0))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
