/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _ISI_EXAMPLE_H_
#define _ISI_EXAMPLE_H_

#include "fsl_video_common.h"
#include "fsl_camera_receiver.h"
#include "fsl_camera.h"
#include "fsl_camera_csr.h"
/*******************************************************************************
 * Configuration
 ******************************************************************************/
/* Configuration for camera. */
#define APP_CAMERA_CONTROL_FLAGS        0U
#define APP_CAMERA_FRAME_RATE           30U

#define APP_CAMERA_HEIGHT               1080U
#define APP_CAMERA_WIDTH                1920U

#define APP_ISI_BASE                    ISI
#define APP_ISI                         ((ISI_Type *)CAMERA__ISI_BASE)

#define APP_MIPI_CSI2_BASE              MIPI_CSI2_BASE
#define APP_MIPI_CSI2                   ((CAMERA_MIPI_CSI2_Type *)MIPI_CSI2_BASE)
#define APP_MIPI_CSI2_PHY               ((CAMERA_DSI_OR_CSI_PHY_CSR_Type *)CAMERA__DSI_OR_CSI_PHY_CSR_BASE)
   
#define APP_CAMERA_I2C_BASEADDR         BOARD_PCAL6408_I2C4

#define APP_MIPI_CSI2_INTERFACE         0U

#define APP_MIPI_CSI_LANES              2U
#define APP_MIPI_CSI_VC                 0U

/* whether resize camera image to adapt display panel */
#define APP_RESIZE_CAMERA_IMAGE         0U

#define APP_ISI_IRQn                    ISI_IRQn

#define APP_DisplayIRQHandler           DPU_IRQHandler

/* The frame buffer alignment. */
#define APP_FB_ALIGN_BYTE               1U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Init Camera sensor connected interface.
 *
 * @param camera_config .
 */
status_t BOARD_InitCameraInterface(const camera_config_t *camera_config);

/*!
 * @brief Init MIPI-CSI Pixel Formatting moduel to adapter Pixel Link interface.
 *
 * @param camear_csr_pixel_formatting_config_t User defined CSI pixel Formatting module.
 * @param camera_config User defined camera basic setting.
 */
void BOARD_InitPixelLinkInterface(camera_csr_pixel_formatting_config_t cameracsr_config, const camera_config_t *camera_config);

void APP_PrepareCamera();

/*${prototype:end}*/
#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _ISI_EXAMPLE_H_ */
