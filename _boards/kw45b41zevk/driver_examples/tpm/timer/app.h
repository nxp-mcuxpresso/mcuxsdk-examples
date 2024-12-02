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
#define BOARD_TPM         TPM0
#define BOARD_TPM_IRQ_NUM TPM0_IRQn
#define BOARD_TPM_HANDLER TPM0_IRQHandler
/* Get source clock for LPIT driver */
#define TPM_SOURCE_CLOCK (CLOCK_GetFreq(kCLOCK_ScgSircClk))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
