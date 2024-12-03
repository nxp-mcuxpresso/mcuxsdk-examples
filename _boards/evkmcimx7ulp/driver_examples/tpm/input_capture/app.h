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
#define DEMO_TPM_BASEADDR               TPM2
#define BOARD_TPM_INPUT_CAPTURE_CHANNEL kTPM_Chnl_1
#define TPM_INTERRUPT_NUMBER            TPM2_IRQn
#define TPM_INPUT_CAPTURE_HANDLER       TPM2_IRQHandler
#define TPM_CHANNEL_INTERRUPT_ENABLE    kTPM_Chnl1InterruptEnable
#define TPM_CHANNEL_FLAG                kTPM_Chnl1Flag
#define TPM_SOURCE_CLOCK                CLOCK_GetIpFreq(kCLOCK_Tpm2)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
