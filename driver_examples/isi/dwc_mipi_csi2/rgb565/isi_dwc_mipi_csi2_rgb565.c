/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "isi_config.h"
#include "isi_example.h"
#include "fsl_debug_console.h"
#include "fsl_camera_device.h"
#include "app_display.h"

#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_FRAME_BUFFER_COUNT 4
#define APP_BPP                2 /* In this example, the camera pixel format is RGB565. */
#ifndef APP_FB_STRIDE_ALIGN_BYTE
#define APP_FB_STRIDE_ALIGN_BYTE APP_BPP
#endif

#define APP_FB_STRIDE_BYTE (SDK_SIZEALIGN(APP_CAMERA_WIDTH * APP_BPP, APP_FB_STRIDE_ALIGN_BYTE))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_frameBuffer[APP_FRAME_BUFFER_COUNT][SDK_SIZEALIGN(
                                  APP_CAMERA_HEIGHT * APP_FB_STRIDE_BYTE, APP_FB_ALIGN_BYTE)],
                              APP_FB_ALIGN_BYTE);

extern camera_device_handle_t cameraDevice;
extern camera_receiver_handle_t cameraReceiver;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*
 * The callback function is called when the display controller updated the use
 * the new frame buffer. The previous active frame buffer address is returned
 * so that it could be submitted to the camera buffer queue.
 */
void APP_DisplayFrameDoneCallback(uint32_t frameBuffer)
{
    CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, frameBuffer);
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t fullCameraBufferAddr;
    status_t status;
    camera_csr_pixel_formatting_config_t cameracsrConfig = {0};

    const camera_config_t cameraConfig = {
        .pixelFormat                = kVIDEO_PixelFormatRGB565,
        .bytesPerPixel              = APP_BPP,
        .resolution                 = FSL_VIDEO_RESOLUTION(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT),
        .frameBufferLinePitch_Bytes = APP_CAMERA_WIDTH * APP_BPP,
        .interface                  = kCAMERA_InterfaceMIPI,
        .controlFlags               = APP_CAMERA_CONTROL_FLAGS,
        .framePerSec                = APP_CAMERA_FRAME_RATE,
        .mipiChannel                = APP_MIPI_CSI_VC,
        .csiLanes                   = APP_MIPI_CSI_LANES,
    };

    BOARD_InitHardware();

    memset(s_frameBuffer, 0, sizeof(s_frameBuffer));
    
    PRINTF("ISI MIPI-CSI2 example start...\r\n");

    status = CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);

    if (kStatus_Success != status)
    {
        PRINTF("ISI initialize failed.\r\n");
        return -1;
    }
    EnableIRQ(APP_ISI_IRQn);
    
    status = BOARD_InitCameraInterface(&cameraConfig);
    if (kStatus_Success != status)
    {
        PRINTF("CSI host interface initialize failed.\r\n");
        return -1;
    }
    else
    {
        PRINTF("CSI interface init pass.\r\n");
    }

    BOARD_InitPixelLinkInterface(cameracsrConfig, &cameraConfig);

    /*
     * Initialize the display interface.
     * The first frame buffer is shown.
     */
    APP_InitDisplay((uint32_t)s_frameBuffer[0], APP_DisplayFrameDoneCallback);
    
    PRINTF("DISPLAY Device init pass.\r\n");
    
    /*
     * Submit the empty buffer to camera buffer queue.
     * The first buffer is used by the display. So submit from the second buffer.
     */
    for (uint32_t i = 1; i < APP_FRAME_BUFFER_COUNT; i++)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(s_frameBuffer[i]));
    }
    
    CAMERA_RECEIVER_Start(&cameraReceiver);

    status = CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);

    if (kStatus_Success != status)
    {
        PRINTF("Camera initialize failed.\r\n");
        return -1;
    }

    CAMERA_DEVICE_Start(&cameraDevice);

    while (1)
    {
        /* Wait to get the full frame buffer to show. */
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))
        {
        }
        /* Wait for the display controller available to accept new frame buffer. */
        while (APP_IsDisplayFramePending())
        {
        }
        
        APP_SetDisplayFrameBuffer(fullCameraBufferAddr);
    }
}
