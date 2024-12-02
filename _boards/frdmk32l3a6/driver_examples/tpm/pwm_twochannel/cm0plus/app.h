/*
 * Copyright 2017 NXP
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
#define BOARD_TPM_BASEADDR TPM3

#define BOARD_FIRST_TPM_CHANNEL  1U
#define BOARD_SECOND_TPM_CHANNEL 0U

/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetIpFreq(kCLOCK_Tpm3)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
