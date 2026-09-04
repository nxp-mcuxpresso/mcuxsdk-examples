/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DCIF_SUPPORT_H_
#define _DCIF_SUPPORT_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_DCIF            MEDIA__DCIF
#define DEMO_DCIF_IRQn       MEDIA_DCIF_CH0_IRQn
#define DEMO_DCIF_IRQHandler MEDIA_DCIF_CH0_IRQHandler
#define DEMO_DOMAIN          0
#define DEMO_FRAME_RATE      30

/* The original parameters are changed to increase pixel clock.  */
#define DEMO_PANEL_WIDTH  1280U
#define DEMO_HSW 22
#define DEMO_HBP 80
#define DEMO_HFP 80
#define DEMO_PANEL_HEIGHT 800U
#define DEMO_VSW 3
#define DEMO_VBP 10
#define DEMO_VFP 10

#define DEMO_POL_FLAGS \
    (kDCIF_DpiDataEnableActiveHigh | kDCIF_DpiVsyncActiveLow | kDCIF_DpiHsyncActiveLow | \
     kDCIF_DpiDriveDataOnRisingClkEdge)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void DEMO_InitInterface(void);
void DEMO_UpdateFrame(uint32_t address);
status_t BOARD_InitDisplayInterface(void);
void BOARD_InitDcifPowerClockReset(void);
void BOARD_InitMipiDsiClock(void);

#endif /* _DCIF_SUPPORT_H_ */
