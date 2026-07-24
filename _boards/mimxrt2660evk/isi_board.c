/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "isi_config.h"
#include "isi_example.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"
#include "fsl_camera_receiver.h"
#include "fsl_isi_camera_adapter.h"
#include "fsl_mipi_csi2rx.h"
#include "fsl_ov5640.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_reset.h"

typedef struct
{
    uint32_t resolution;
    uint32_t fps;
    uint8_t  tHsSettle_EscClk;
} ov5640_settle_t;

static const ov5640_settle_t s_ov5640Settle[] = {
    { FSL_VIDEO_RESOLUTION(320,  240),  30U, 8U },  /* QVGA 30fps  ~184 Mbit/s */
    { FSL_VIDEO_RESOLUTION(320,  240),  15U, 8U },  /* QVGA 15fps  ~92  Mbit/s */
    { FSL_VIDEO_RESOLUTION(640,  480),  30U, 8U },  /* VGA  30fps  ~737 Mbit/s */
    { FSL_VIDEO_RESOLUTION(640,  480),  15U, 8U },  /* VGA  15fps  ~368 Mbit/s */
    { FSL_VIDEO_RESOLUTION(1280, 720),  30U, 9U },  /* 720P 30fps ~1382 Mbit/s */
    { FSL_VIDEO_RESOLUTION(1280, 720),  15U, 9U },  /* 720P 15fps  ~691 Mbit/s */
    { FSL_VIDEO_RESOLUTION(1920, 1080), 30U, 9U },  /* 1080P 30fps ~3110 Mbit/s */
    { FSL_VIDEO_RESOLUTION(1920, 1080), 15U, 9U },  /* 1080P 15fps ~1555 Mbit/s */
};

static void BOARD_PullCameraResetPin(bool pullUp);
static void BOARD_PullCameraPowerDownPin(bool pullUp);

static isi_private_data_t s_isiPrivateData;

static isi_resource_t s_isiResource = {
    .isiBase      = APP_ISI,
    .isiInputPort = APP_MIPI_CSI_ISI_PORT,
};

camera_receiver_handle_t cameraReceiver = {
    .resource    = &s_isiResource,
    .ops         = &isi_ops,
    .privateData = &s_isiPrivateData,
};

static ov5640_resource_t s_ov5640Resource = {
    .i2cSendFunc      = BOARD_Camera_I2C_SendSCCB,
    .i2cReceiveFunc   = BOARD_Camera_I2C_ReceiveSCCB,
    .pullResetPin     = BOARD_PullCameraResetPin,
    .pullPowerDownPin = BOARD_PullCameraPowerDownPin,
};

camera_device_handle_t cameraDevice = {
    .resource = &s_ov5640Resource,
    .ops      = &ov5640_ops,
};

static void BOARD_PullCameraResetPin(bool pullUp)
{
    GPIO_PinWrite(BOARD_CAMERA_RST_GPIO, BOARD_CAMERA_RST_PIN, pullUp ? 1U : 0U);
}

static void BOARD_PullCameraPowerDownPin(bool pullUp)
{
    GPIO_PinWrite(BOARD_CAMERA_PWDN_GPIO, BOARD_CAMERA_PWDN_PIN, pullUp ? 1U : 0U);
}

/*!
 * @brief Prepare camera hardware: I2C, GPIO, MCLK.
 */
void BOARD_PrepareCamera(void)
{
    const gpio_pin_config_t pinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1U,
    };

    BOARD_Camera_I2C_Init();

    GPIO_PinInit(BOARD_CAMERA_PWDN_GPIO, BOARD_CAMERA_PWDN_PIN, &pinConfig);
    GPIO_PinInit(BOARD_CAMERA_RST_GPIO,  BOARD_CAMERA_RST_PIN,  &pinConfig);

    {
        clock_root_config_t mclkCfg = {0};
        mclkCfg.sndDiv = 1U;
        mclkCfg.mux    = kCLOCK_CSI_MCLKOUT_ClockRoot_SXOSC; /* SXOSC = 24 MHz */
        mclkCfg.div    = 1U;                                   /* 24 MHz / 1 = 24 MHz */
        CLOCK_SetRootClock(kCLOCK_Root_MEDIA_csi_mclkout, &mclkCfg);
        CLOCK_PowerOnRootClock(kCLOCK_Root_MEDIA_csi_mclkout);
    }
}

void BOARD_InitCameraInterface(void)
{
    csi2rx_config_t csiConfig;
    uint8_t dataSettle = APP_MIPI_CSI_DATA_SETTLE; /* default */

    RESET_PeripheralReset(kModCon_MEDIA_MIPI_CSI);

    {
        clock_root_config_t escCfg = {0};
        escCfg.sndDiv = 1U;
        escCfg.mux    = kCLOCK_MIPICSI_ESCCLK_ClockRoot_PERI5;
        escCfg.div    = 7U; /* 666.67 / 7 = 95.2 MHz */
        CLOCK_SetRootClock(kCLOCK_Root_MEDIA_mipicsi_escclk, &escCfg);
        CLOCK_PowerOnRootClock(kCLOCK_Root_MEDIA_mipicsi_escclk);
    }
    {
        clock_root_config_t csiClkCfg = {0};
        csiClkCfg.sndDiv = 1U;
        csiClkCfg.mux    = kCLOCK_MIPICSI_ClockRoot_PERI5;
        csiClkCfg.div    = 2U; /* 666.67 / 2 = 333 MHz */
        CLOCK_SetRootClock(kCLOCK_Root_MEDIA_mipicsi_clk, &csiClkCfg);
        CLOCK_PowerOnRootClock(kCLOCK_Root_MEDIA_mipicsi_clk);
    }
    CLOCK_EnableClock(kCLOCK_MEDIA_mipi_csi);

    CLOCK_EnableClock(kCLOCK_MEDIA_isi);

    for (uint32_t i = 0U; i < ARRAY_SIZE(s_ov5640Settle); i++)
    {
        if ((s_ov5640Settle[i].resolution ==
             FSL_VIDEO_RESOLUTION(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT)) &&
            (s_ov5640Settle[i].fps == APP_CAMERA_FRAME_RATE))
        {
            dataSettle = s_ov5640Settle[i].tHsSettle_EscClk;
            break;
        }
    }

    CSI2RX_GetDefaultConfig(&csiConfig);
    csiConfig.laneNum        = APP_MIPI_CSI_LANES;
    csiConfig.tHsSettle_EscClk = dataSettle;
    csiConfig.tClkSettle_EscClk  = APP_MIPI_CSI_CLK_SETTLE;
    csiConfig.flushCount     = APP_MIPI_CSI_FLUSH_COUNT;

    (void)CSI2RX_Init(APP_MIPI_CSI, &csiConfig);

    MEDIA__REFORMATTER->PLM_CTRL.SET = REFORMATTER_PLM_CTRL_ENABLE_MASK;

    /* Enable error interrupts for diagnostics. */
    CSI2RX_EnableInterrupts(APP_MIPI_CSI,
                              kCSI2RX_InterruptCrcError      |
                              kCSI2RX_InterruptEcc2BitError  |
                              kCSI2RX_InterruptLaneError     |
                              kCSI2RX_InterruptInternalError);
}
