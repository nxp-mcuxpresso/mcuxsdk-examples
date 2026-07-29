/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _ISI_EXAMPLE_H_
#define _ISI_EXAMPLE_H_

#include "fsl_common.h"

#define APP_CAMERA_CONTROL_FLAGS 0U

#define APP_MIPI_CSI_LANES       2U
#define APP_MIPI_CSI_VC          0U

#define APP_ISI          ((ISI_Type *)MEDIA__ISI_BASE)
#define APP_MIPI_CSI     CSI2_RX_BASE

#define APP_MIPI_CSI_ISI_PORT    2U

#define APP_ISI_IRQn             MEDIA_ISI_IRQn
#define APP_ISI_IRQHandler       MEDIA_ISI_IRQHandler

#define APP_FB_ALIGN_BYTE        16U

#define APP_MIPI_CSI_DATA_SETTLE 8U
#define APP_MIPI_CSI_CLK_SETTLE  16U

#define APP_MIPI_CSI_FLUSH_COUNT 7U

#if defined(__cplusplus)
extern "C" {
#endif

void BOARD_PrepareCamera(void);

void BOARD_InitCameraInterface(void);

#if defined(__cplusplus)
}
#endif

#endif /* _ISI_EXAMPLE_H_ */
