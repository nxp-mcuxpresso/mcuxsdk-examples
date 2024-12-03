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
#define DEMO_DAC_BASE               GAU_DAC0
#define DEMO_DAC_CHANNEL            kDAC_ChannelB
#define DEMO_DAC_TRIANGLE_MAX_VALUE kDAC_TriangleAmplitude1023
#define DEMO_DAC_TRIANGLE_STEP_SIZE kDAC_TriangleStepSize1
#define DEMO_DAC_TRIANGLE_MIN_VALUE 0U

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
