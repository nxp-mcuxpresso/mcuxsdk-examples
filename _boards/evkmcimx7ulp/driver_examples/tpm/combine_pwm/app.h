/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define TPM_LED_ON_LEVEL       kTPM_HighTrue
#define BOARD_TPM_BASEADDR     TPM2
#define BOARD_TPM_CHANNEL_PAIR kTPM_Chnl_0
#define TPM_SOURCE_CLOCK       CLOCK_GetIpFreq(kCLOCK_Tpm2)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
