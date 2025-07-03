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
#define BOARD_TPM_BASEADDR     TPM1
#define TPM_SOURCE_CLOCK       CLOCK_GetIpFreq(kCLOCK_Tpm1)
#define BOARD_TPM_CHANNEL_PAIR kTPM_Chnl_0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
