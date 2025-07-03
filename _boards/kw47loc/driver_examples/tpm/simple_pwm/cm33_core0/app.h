/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define TPM_LED_ON_LEVEL   kTPM_HighTrue
#define BOARD_TPM_BASEADDR TPM1
#define TPM_SOURCE_CLOCK   CLOCK_GetIpFreq(kCLOCK_Tpm1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
