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
#define BOARD_TPM_BASEADDR       TPM_0
#define TPM_SOURCE_CLOCK         CLOCK_GetIpFreq(kCLOCK_Tpm0)
#define BOARD_FIRST_TPM_CHANNEL  kTPM_Chnl_2
#define BOARD_SECOND_TPM_CHANNEL kTPM_Chnl_3
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
