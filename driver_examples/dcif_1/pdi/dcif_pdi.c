/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "fsl_camera_device.h"
#include "fsl_isi.h"
#include "fsl_isi_camera_adapter.h"
#include "fsl_dcif.h"
#include "isi_config.h"
#include "isi_example.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*
 * DCIF PDI (parallel data interface) low-latency camera preview.
 *
 *   OV5640 -> MIPI CSI2 RX -> ISI (scaling, RGB888) -> pixel-link
 *          -> REFORMATTER (PL_TO_PDI) -> DCIF PDI input (layer 0)
 *          -> DCIF DPI output -> panel
 *
 * The camera sensor (OV5640) output is RGB565; the ISI output frame is RGB888
 * (the REFORMATTER only supports RGB888 on the ISI-to-PDI pixel-link path).
 *
 * The ISI channel output destination is display-controller only
 * (kISI_OutputDestDisplay), so the ISI never writes a frame to memory: no
 * frame buffer is allocated, and none is ever submitted to or retrieved from
 * the ISI. This bypasses the camera_receiver/isi_ops adapter
 * (fsl_isi_camera_adapter.c) for the ISI channel, since that adapter's
 * ISI_ADAPTER_Start() refuses to start the channel with fewer than 3 buffers
 * (1 drop-frame + ISI_MAX_ACTIVE_BUF), and its ISI_ADAPTER_InitExt() only ever
 * selects memory or memory+display output, never display-only.
 */
#define APP_CAMERA_BPP     2 /* OV5640 MIPI sensor output is RGB565. */
#define APP_ISI_OUTPUT_BPP 3 /* ISI RGB888 output is packed into 24-bit words. */

/* The DCIF layer that shows the PDI stream, only support layer 0/1. */
#define APP_PDI_LAYER 0U

/* PDI VSYNC detection timeout (PDI_TO_CNT), in DPI HSYNC cycles. 0 disables it. */
#define APP_PDI_VSYNC_TIMEOUT_HSYNC 8000U

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern camera_device_handle_t cameraDevice;
extern camera_receiver_handle_t cameraReceiver;
static ISI_Type *s_isiBase;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void APP_StartIsiDisplay(const camera_config_t *config)
{
    isi_config_t isiConfig;
    isi_resource_t *resource = (isi_resource_t *)cameraReceiver.resource;

    s_isiBase = resource->isiBase;

    ISI_GetDefaultConfig(&isiConfig);
    isiConfig.isChannelBypassed    = false;
    isiConfig.isSourceMemory       = false;
    isiConfig.isYCbCr              = true;
    isiConfig.sourcePort           = resource->isiInputPort;
    isiConfig.mipiChannel          = config->mipiChannel;
    isiConfig.inputHeight          = FSL_VIDEO_EXTRACT_HEIGHT(config->resolution);
    isiConfig.inputWidth           = FSL_VIDEO_EXTRACT_WIDTH(config->resolution);
    isiConfig.outputFormat         = DEMO_ISI_FORMAT;
    isiConfig.outputDest           = kISI_OutputDestDisplay;
    isiConfig.outputLinePitchBytes = config->frameBufferLinePitch_Bytes;

    ISI_Init(s_isiBase);
    ISI_SetConfig(s_isiBase, &isiConfig);
    ISI_SetScalerConfig(s_isiBase, isiConfig.inputWidth, isiConfig.inputHeight, DEMO_PANEL_WIDTH,
        DEMO_PANEL_HEIGHT);

    /* Neither the sensor output (RGB565) nor the ISI output (RGB888) is YUV. */
    ISI_EnableColorSpaceConversion(s_isiBase, false);
    ISI_SetFlipMode(s_isiBase, kISI_FlipDisable);

    ISI_ClearInterruptStatus(s_isiBase, (uint32_t)kISI_FrameReceivedInterrupt);
    ISI_Start(s_isiBase);
}

/*
 * Configure the DCIF DPI output and the DCIF PDI input (layer 0) so the
 * REFORMATTER's pixel-link output is displayed on the panel.
 */
static void APP_InitDisplayPdi(void)
{
    dcif_layer_config_t layerConfig;
    dcif_pdi_config_t pdiConfig;

    DCIF_Init(DEMO_DCIF);

    const dcif_output_config_t outputConfig = {
        .interface = kDCIF_OutputDpi,
        .width     = DEMO_PANEL_WIDTH,
        .height    = DEMO_PANEL_HEIGHT,
    };
    DCIF_SetOutputConfig(DEMO_DCIF, &outputConfig);

    /* Configure the DPI timing for the panel. */
    DEMO_InitInterface();

    DCIF_SetBackGroundLayerColor(DEMO_DCIF, 0x0U);

    (void)memset(&layerConfig, 0, sizeof(layerConfig));
    layerConfig.enable         = true;
    layerConfig.format         = kDCIF_LayerPixelFormatRGB888;
    layerConfig.topLeftX       = 0U;
    layerConfig.topLeftY       = 0U;
    layerConfig.width          = DEMO_PANEL_WIDTH;
    layerConfig.height         = DEMO_PANEL_HEIGHT;
    layerConfig.background     = 0U;
    layerConfig.globalAlpha    = 0xFFU;
    layerConfig.alphaBlendMode = kDCIF_AlphaBlendOverride;
    (void)DCIF_SetLayerConfig(DEMO_DCIF, APP_PDI_LAYER, &layerConfig);

    DCIF_PdiGetDefaultConfig(&pdiConfig);
    pdiConfig.enable             = true;
    pdiConfig.format             = DEMO_PDI_FORMAT;
    pdiConfig.layer              = (dcif_pdi_layer_select_t)APP_PDI_LAYER;
    pdiConfig.vsyncDelay_pclk    = APP_PDI_VSYNC_DELAY_PCLK;
    pdiConfig.vsyncTimeout_hsync = APP_PDI_VSYNC_TIMEOUT_HSYNC;
    pdiConfig.polarityFlags      = kDCIF_PdiVsyncActiveLow;
    DCIF_PdiSetConfig(DEMO_DCIF, &pdiConfig);

    DCIF_EnableOutput(DEMO_DCIF, true);

    /* Initialize panel power/backlight. */
    if (kStatus_Success != BOARD_InitDisplayInterface())
    {
        PRINTF("Display panel initialize failed\r\n");
    }
}

int main(void)
{
    status_t status;

    const camera_config_t cameraConfig = {
        .pixelFormat                = kVIDEO_PixelFormatRGB565,
        .bytesPerPixel              = APP_CAMERA_BPP,
        .resolution                 = FSL_VIDEO_RESOLUTION(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT),
        .frameBufferLinePitch_Bytes = DEMO_PANEL_WIDTH * APP_ISI_OUTPUT_BPP,
        .interface                  = kCAMERA_InterfaceMIPI,
        .controlFlags               = APP_CAMERA_CONTROL_FLAGS,
        .framePerSec                = APP_CAMERA_FRAME_RATE,
        .mipiChannel                = APP_MIPI_CSI_VC,
        .csiLanes                   = APP_MIPI_CSI_LANES,
    };

    BOARD_InitHardware();
    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    PRINTF("DCIF PDI example start...\r\n");

    /*
     * Configure and start the ISI channel. First enable ISI, then MIPI_CSI, at
     * last the sensor.
     */
    APP_StartIsiDisplay(&cameraConfig);

    /*
     * Initialize the CSI2RX and enable the REFORMATTER PL_TO_PDI conversion so the
     * ISI pixel-link output is fed to the DCIF PDI input.
     */
    BOARD_InitCameraInterface();

    status = CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);
    if (kStatus_Success != status)
    {
        PRINTF("Camera initialize failed.\r\n");
        return -1;
    }

    CAMERA_DEVICE_Start(&cameraDevice);

    /* Wait for the first ISI frame (polling the status register directly,
     * no buffer needed) so the PDI input has valid data before the display
     * layer is enabled. */
    while (0U == (ISI_GetInterruptStatus(s_isiBase) & (uint32_t)kISI_FrameReceivedInterrupt))
    {
    }
    ISI_ClearInterruptStatus(s_isiBase, (uint32_t)kISI_FrameReceivedInterrupt);

    APP_InitDisplayPdi();

    /*
     * The displayed pixels come entirely from the pixel-link/PDI path, and the
     * ISI channel never writes to memory, so there is no per-frame handling
     * left to do in the main loop.
     */
    while (1)
    {
    }
}
