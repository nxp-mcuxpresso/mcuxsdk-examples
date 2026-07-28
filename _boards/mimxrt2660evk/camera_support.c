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

void BOARD_EarlyPrepareCamera(void)
{
}

void BOARD_InitCameraResource(void)//todo
{
    BOARD_Camera_I2C_Init();

    /* CSI camera reset and power control are on the PCAL6524 I/O expander;
     * configure both as outputs before the OV5640 adapter drives them. */
    pcal6524_handle_t *h = BOARD_GetPCAL6524Handle();
    (void)PCAL6524_SetDirection(h, (1UL << BOARD_PCAL6524_CSI_RST_B) | (1UL << BOARD_PCAL6524_CSI_PWR_CTL),
                                kPCAL6524_Output);
}
