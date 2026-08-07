/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_common.h"
#include "fsl_jpegdec.h"
#include "display_support.h"

/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

extern JPEG_DECODER_Type g_appJpegDec;
#define APP_JPEGDEC (&g_appJpegDec)
#ifndef DEMO_BUFFER1_ADDR
#define DEMO_BUFFER1_ADDR 0x89410000U
#endif
#define DEMO_FB_ADDR 0x89210000U
#define APP_FB_USE_NV12 1

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
