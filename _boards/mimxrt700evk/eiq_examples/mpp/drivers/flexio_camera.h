/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_FLEXIO_CAMERA_H_
#define _FSL_FLEXIO_CAMERA_H_

#include "fsl_common.h"
#include "fsl_flexio.h"
/*!
 * @addtogroup flexio_camera
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
extern volatile uint32_t g_ezhvIrqIdx;
extern volatile uint32_t g_ezhvIrq;
extern CameraBuffer_t *g_stCamBuf;
/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

void flexio_camera_init(void);
/*! @} */

#if defined(__cplusplus)
}
#endif /*_cplusplus*/

/*@}*/

#endif /*_FSL_FLEXIO_CAMERA_H_*/
