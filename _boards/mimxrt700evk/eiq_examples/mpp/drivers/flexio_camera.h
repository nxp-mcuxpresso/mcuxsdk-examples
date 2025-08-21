/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "camera_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern volatile CameraDvpTransfer g_dvpTransfer;
extern volatile uint32_t g_newVideoFrame;
extern volatile CameraBuffer_t *g_stCamBuf;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

void CAMERA_Init(void);
/*! @} */

#if defined(__cplusplus)
}
#endif /*_cplusplus*/
