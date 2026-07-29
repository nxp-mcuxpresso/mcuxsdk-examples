/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "camera_support.h"
#include "fsl_gpio.h"
#include "fsl_csi.h"
#include "fsl_csi_camera_adapter.h"
#include "fsl_ov5640.h"
#include "fsl_iomuxc.h"
#include "board.h"
#include "fsl_pca9555.h"
#include "fsl_debug_console.h"
#include "fsl_reset.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void BOARD_PullCameraPowerDownPin(bool pullUp);
static void BOARD_PullCameraResetPin(bool pullUp);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Camera connect to CSI. */
static csi_resource_t csiResource = {
    .csiBase = MEDIA__CSI,
    .dataBus = kCSI_DataBus8Bit,
};

static csi_private_data_t csiPrivateData;

camera_receiver_handle_t cameraReceiver = {
    .resource    = &csiResource,
    .ops         = &csi_ops,
    .privateData = &csiPrivateData,
};

static ov5640_resource_t ov5640Resource = {
    .i2cSendFunc      = BOARD_Camera_I2C_SendSCCB,
    .i2cReceiveFunc   = BOARD_Camera_I2C_ReceiveSCCB,
    .pullResetPin     = BOARD_PullCameraResetPin,
    .pullPowerDownPin = BOARD_PullCameraPowerDownPin,
};

camera_device_handle_t cameraDevice = {
    .resource = &ov5640Resource,
    .ops      = &ov5640_ops,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
extern void CSI_DriverIRQHandler(void);

void CSI_IRQHandler(void)
{
    CSI_DriverIRQHandler();
    __DSB();
}

static void BOARD_PullCameraResetPin(bool pullUp)
{
    /* The reset pin is directly connected to a certain level and will remain in a high state continuously.
     * not need to set in there. */
    (void)pullUp;
}

static void BOARD_PullCameraPowerDownPin(bool pullUp)
{
    pca9555_handle_t *h = BOARD_GetPCA9555Handle();
    if (pullUp)
    {
        (void)PCA9555_SetPins(h, 1UL << BOARD_PCA9555_CSI_PWDN);
    }
    else
    {
        (void)PCA9555_ClearPins(h, 1UL << BOARD_PCA9555_CSI_PWDN);
    }
}

void BOARD_InitCameraResource(void)
{
    BOARD_Camera_I2C_Init();

    /* Parallel CSI power-down (CSI_PWDN) is on PCA9555 P1_0; configure it as an
     * output before the camera adapter drives it. */
    pca9555_handle_t *h = BOARD_GetPCA9555Handle();
    (void)PCA9555_SetDirection(h, 1UL << BOARD_PCA9555_CSI_PWDN, kPCA9555_Output);

    RESET_PeripheralReset(kModCon_MEDIA_CSI);

    {
        clock_root_config_t mclkCfg = {0};
        mclkCfg.sndDiv = 1U;
        mclkCfg.mux    = kCLOCK_CSI_MCLKOUT_ClockRoot_SXOSC;
        mclkCfg.div    = 1U;
        CLOCK_SetRootClock(kCLOCK_Root_MEDIA_csi_mclkout, &mclkCfg);
        CLOCK_PowerOnRootClock(kCLOCK_Root_MEDIA_csi_mclkout);
    }

    CLOCK_EnableClock(kCLOCK_MEDIA_csi);
    CLOCK_EnableClock(kCLOCK_MEDIA_mipi_csi);

    {
        clock_root_config_t lpi2cCfg = {0};
        lpi2cCfg.sndDiv = 1U;
        lpi2cCfg.mux    = kCLOCK_LPI2C0_ClockRoot_SXOSC;
        lpi2cCfg.div    = 2U;
        CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpi2c0_fclk, &lpi2cCfg);
        CLOCK_PowerOnRootClock(kCLOCK_Root_MAIN_lpi2c0_fclk);
    }
}
