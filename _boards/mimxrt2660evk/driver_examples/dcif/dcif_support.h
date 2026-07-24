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

/* Interface type. */
#define DEMO_INTERFACE_DPI 0
#define DEMO_INTERFACE_DBI 1

#define DEMO_INTERFACE_TYPE DEMO_INTERFACE_DPI

#define DEMO_DCIF            MEDIA__DCIF
#define DEMO_DCIF_IRQn       MEDIA_DCIF_CH0_IRQn
#define DEMO_DCIF_IRQHandler MEDIA_DCIF_CH0_IRQHandler
#define DEMO_DOMAIN          0

#define DEMO_FRAME_RATE 40

#if DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DPI
#define DEMO_PANEL_HEIGHT 480
#define DEMO_PANEL_WIDTH  800
#define DEMO_HSW 4
#define DEMO_HFP 8
#define DEMO_HBP 8
#define DEMO_VSW 4
#define DEMO_VFP 8
#define DEMO_VBP 8
#define DEMO_POL_FLAGS \
    (kDCIF_DpiDataEnableActiveHigh | kDCIF_DpiVsyncActiveLow | kDCIF_DpiHsyncActiveLow | \
     kDCIF_DpiDriveDataOnRisingClkEdge)

#else
/* LCD_PAR_S035 8080 panel */
#define DEMO_PANEL_WIDTH    480U
#define DEMO_PANEL_HEIGHT   320U
#define DEMO_BUFFER_START_X 0U
#define DEMO_BUFFER_START_Y 0U
#define DEMO_BUFFER_END_X   479U
#define DEMO_BUFFER_END_Y   319U
#define DBI_CMD_WRITE_MEMORY_START    0x2CU
#define DBI_CMD_WRITE_MEMORY_CONTINUE 0x3CU
#define DBI_CMD_SET_COLUMN_ADDRESS    0x2AU
#define DBI_CMD_SET_PAGE_ADDRESS      0x2BU
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void DEMO_InitInterface(void);
void DEMO_UpdateFrame(uint32_t address);
status_t BOARD_InitDisplayInterface(void);
void BOARD_InitDcifPowerClockReset(void);
void BOARD_InitMipiDsiClock(void);
#if DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DBI
void DEMO_DbiSelectUpdateArea(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);
#endif

#endif /* _DCIF_SUPPORT_H_ */
