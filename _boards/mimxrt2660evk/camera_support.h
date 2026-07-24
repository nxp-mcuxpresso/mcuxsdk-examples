/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CAMERA_SUPPORT_H_
#define _CAMERA_SUPPORT_H_

#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "fsl_camera_device.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_CAMERA_OV5640  0

#ifndef DEMO_CAMERA
#define DEMO_CAMERA DEMO_CAMERA_MT9M114
#endif

/* Use QVGA resolution for simulation due to limited RAM space. */
#define DEMO_CAMERA_WIDTH         320
#define DEMO_CAMERA_HEIGHT        240
#define DEMO_CAMERA_FRAME_RATE    30
#define DEMO_CAMERA_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)
#define DEMO_CAMERA_BUFFER_ALIGN  64 /* Buffer should be 64 byte aligned. */

extern camera_device_handle_t cameraDevice;
extern camera_receiver_handle_t cameraReceiver;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/* This function should be called before camera pins initialization */
void BOARD_EarlyPrepareCamera(void);

void BOARD_InitCameraResource(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _CAMERA_SUPPORT_H_ */
