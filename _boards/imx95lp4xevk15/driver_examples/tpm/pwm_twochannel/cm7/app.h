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
#define BOARD_TPM_BASEADDR       TPM2
#define BOARD_FIRST_TPM_CHANNEL  kTPM_Chnl_2
#define BOARD_SECOND_TPM_CHANNEL kTPM_Chnl_3

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
