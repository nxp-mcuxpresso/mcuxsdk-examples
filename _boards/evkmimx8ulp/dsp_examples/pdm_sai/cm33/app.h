/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "fsl_wm8960.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_AUDIO_MASTER_CLOCK CLOCK_GetIpFreq(kCLOCK_Sai0)

#define DEMO_SAI_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_Sai0)
#define DEMO_I2C_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_Lpi2c0)


#ifndef DEMO_CODEC_VOLUME
#define DEMO_CODEC_VOLUME 100U
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
