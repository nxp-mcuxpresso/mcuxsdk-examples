/*
 * Copyright 2023 NXP
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
#define DEMO_TPM_BASEADDR     TPM2
#define BOARD_TPM_OUT_CHANNEL kTPM_Chnl_2

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
