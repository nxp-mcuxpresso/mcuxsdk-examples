/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "dcif_support.h"
#include "isi_example.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_HTOTAL (DEMO_PANEL_WIDTH + DEMO_HSW + DEMO_HBP + DEMO_HFP)
#define APP_VTOTAL (DEMO_PANEL_HEIGHT + DEMO_VSW + DEMO_VBP + DEMO_VFP)
/* Different settings(camera resolution, scale, frame rate, etc.) may require different PCLK delay */
#define APP_PDI_VSYNC_DELAY_PCLK ((APP_VTOTAL - 7U) * APP_HTOTAL)
/* ISI-REFORMATTER-PDI pipeline only support RGB888 format */
#define DEMO_PDI_FORMAT kDCIF_PdiRGB888
#define DEMO_ISI_FORMAT kISI_OutputRGB888
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
