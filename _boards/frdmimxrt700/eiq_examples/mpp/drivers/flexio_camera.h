/*
 * Copyright 2024-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLEXIO_CAMERA_H_
#define _FLEXIO_CAMERA_H_

#include "fsl_common.h"
#include "camera_config.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the FlexIO-based camera interface for OV7670.
 */
void FLEXIO_Camera_Init(void);

/*!
 * @brief Start camera capture into the provided frame buffer.
 * @param buf  Pointer to frame buffer (must be CAMERA_BYTES_PER_PIXEL * W * H bytes)
 */
void FLEXIO_Camera_StartCapture(uint8_t *buf);

/*!
 * @brief Stop camera capture.
 */
void FLEXIO_Camera_StopCapture(void);

#if defined(__cplusplus)
}
#endif

#endif /* _FLEXIO_CAMERA_H_ */
