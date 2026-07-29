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
#include "fsl_reformatter.h"
#include "fsl_pcal6524.h"
#include "fsl_reformatter.h"
#include "fsl_debug_console.h"

typedef struct
{
    uint32_t resolution;
    uint32_t fps;
    uint8_t  tHsSettle_EscClk;
} ov5640_settle_t;

static const ov5640_settle_t s_ov5640Settle[] = {
    { FSL_VIDEO_RESOLUTION(320,  240),  30U, 0x24 },
    { FSL_VIDEO_RESOLUTION(320,  240),  15U, 0x24 },
    { FSL_VIDEO_RESOLUTION(640,  480),  30U, 0x1F },
    { FSL_VIDEO_RESOLUTION(640,  480),  15U, 0x1F },
    { FSL_VIDEO_RESOLUTION(1280, 720),  30U, 0x11 },
    { FSL_VIDEO_RESOLUTION(1280, 720),  15U, 0x17 },
    { FSL_VIDEO_RESOLUTION(1920, 1080), 30U, 0x6 },
    { FSL_VIDEO_RESOLUTION(1920, 1080), 15U, 0x8 },
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

/* On mimxrt2660evk the OV5640 camera reset (CSI_RST_B) and power control
 * (CSI_PWR_CTL) are routed through the PCAL6524 I/O expander, not MCU GPIO. */
static void BOARD_PullCameraResetPin(bool pullUp)
{
    pcal6524_handle_t *h = BOARD_GetPCAL6524Handle();
    if (pullUp)
    {
        (void)PCAL6524_SetPins(h, 1UL << BOARD_PCAL6524_CSI_RST_B);
    }
    else
    {
        (void)PCAL6524_ClearPins(h, 1UL << BOARD_PCAL6524_CSI_RST_B);
    }
}

static void BOARD_PullCameraPowerDownPin(bool pullUp)
{
    pcal6524_handle_t *h = BOARD_GetPCAL6524Handle();
    if (pullUp)
    {
        (void)PCAL6524_SetPins(h, 1UL << BOARD_PCAL6524_CSI_PWR_CTL);
    }
    else
    {
        (void)PCAL6524_ClearPins(h, 1UL << BOARD_PCAL6524_CSI_PWR_CTL);
    }
}

/*!
 * @brief Prepare camera hardware: I2C, GPIO, MCLK.
 */
void BOARD_PrepareCamera(void)
{
    BOARD_Camera_I2C_Init();

    /* CSI camera reset and power control are on the PCAL6524 I/O expander;
     * configure both as outputs before the OV5640 adapter drives them. */
    pcal6524_handle_t *h = BOARD_GetPCAL6524Handle();
    (void)PCAL6524_SetDirection(h, (1UL << BOARD_PCAL6524_CSI_RST_B) | (1UL << BOARD_PCAL6524_CSI_PWR_CTL),
                                kPCAL6524_Output);

    {
        clock_root_config_t mclkCfg = {0};
        mclkCfg.sndDiv = 1U;
        mclkCfg.mux    = kCLOCK_CSI_MCLKOUT_ClockRoot_SXOSC;
        mclkCfg.div    = 1U;
        CLOCK_SetRootClock(kCLOCK_Root_MEDIA_csi_mclkout, &mclkCfg);
        CLOCK_PowerOnRootClock(kCLOCK_Root_MEDIA_csi_mclkout);
    }

    CLOCK_EnableClock(kCLOCK_MEDIA_isi);
    EnableIRQ(APP_ISI_IRQn);
}

void BOARD_InitCameraInterface(void)
{
    csi2rx_config_t csiConfig;
    uint8_t dataSettle = APP_MIPI_CSI_DATA_SETTLE; /* default */
    reformatter_config_t refConfig;

    {
        clock_root_config_t lpi2cCfg = {0};
        lpi2cCfg.sndDiv = 1U;
        lpi2cCfg.mux    = kCLOCK_LPI2C0_ClockRoot_SXOSC;
        lpi2cCfg.div    = 2U;
        CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpi2c0_fclk, &lpi2cCfg);
        CLOCK_PowerOnRootClock(kCLOCK_Root_MAIN_lpi2c0_fclk);
    }

    RESET_PeripheralReset(kModCon_MEDIA_MIPI_CSI);

    /* Set ESC clock to 54MHZ, and CSI core clock to 1080MHZ to adapter all resolution up to 1080p 30fps */
    {
        clock_root_config_t escCfg = {0};
        escCfg.sndDiv = 1U;
        escCfg.mux    = kCLOCK_MIPICSI_ESCCLK_ClockRoot_MEDIAPLL;
        escCfg.div    = 2;
        CLOCK_SetRootClock(kCLOCK_Root_MEDIA_mipicsi_escclk, &escCfg);
        CLOCK_PowerOnRootClock(kCLOCK_Root_MEDIA_mipicsi_escclk);
    }

    {
        clock_root_config_t csiClkCfg = {0};
        csiClkCfg.sndDiv = 1U;
        csiClkCfg.mux    = kCLOCK_MIPICSI_ClockRoot_MEDIAPLL;
        csiClkCfg.div    = 1U;
        CLOCK_SetRootClock(kCLOCK_Root_MEDIA_mipicsi_clk, &csiClkCfg);
        CLOCK_PowerOnRootClock(kCLOCK_Root_MEDIA_mipicsi_clk);
    }

    PRINTF("kCLOCK_Root_MEDIA_mipicsi_escclk: %d\r\n", CLOCK_GetRootClockFreq(kCLOCK_Root_MEDIA_mipicsi_escclk));
    PRINTF("kCLOCK_Root_MEDIA_mipicsi_clk: %d\r\n", CLOCK_GetRootClockFreq(kCLOCK_Root_MEDIA_mipicsi_clk));
    CLOCK_EnableClock(kCLOCK_MEDIA_mipi_csi);
    CLOCK_EnableClock(kCLOCK_MEDIA_isi);

    /* Settle timer lookup */
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
    csiConfig.laneNum           = APP_MIPI_CSI_LANES;
    csiConfig.tHsSettle_EscClk  = dataSettle;
    csiConfig.tClkSettle_EscClk = APP_MIPI_CSI_CLK_SETTLE;
    csiConfig.flushCount        = APP_MIPI_CSI_FLUSH_COUNT;

    status_t ret = CSI2RX_Init(APP_MIPI_CSI, &csiConfig);
    PRINTF("CSI2RX_Init: %s\r\n", ret == kStatus_Success ? "OK" : "TIMEOUT");

    REFORMATTER_GetDefaultConfig(&refConfig);
    refConfig.signalConfig.enable         = true;
    refConfig.signalConfig.validForceHigh = true;
    refConfig.enableConversion            = false;
    REFORMATTER_Init(MEDIA__REFORMATTER, &refConfig);

    /* Enable error interrupts for diagnostics. */
    CSI2RX_EnableInterrupts(APP_MIPI_CSI,
                              kCSI2RX_InterruptCrcError      |
                              kCSI2RX_InterruptEcc2BitError  |
                              kCSI2RX_InterruptLaneError     |
                              kCSI2RX_InterruptInternalError);
}

void APP_ISI_IRQHandler(void)
{
    ISI_ADAPTER_IRQHandler(&cameraReceiver);
}
