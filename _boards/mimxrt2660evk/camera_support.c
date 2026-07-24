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

static void BOARD_PullCameraResetPin(bool pullUp)
{
    GPIO_PinWrite(BOARD_CAMERA_RST_GPIO, BOARD_CAMERA_RST_PIN, pullUp ? 1U : 0U);
}

static void BOARD_PullCameraPowerDownPin(bool pullUp)
{
    GPIO_PinWrite(BOARD_CAMERA_PWDN_GPIO, BOARD_CAMERA_PWDN_PIN, pullUp ? 1U : 0U);
}

void BOARD_EarlyPrepareCamera(void)
{
}

void BOARD_InitCameraResource(void)//todo
{
    BOARD_Camera_I2C_Init();

    /* Fixed clock source in simulation environment. */

    /* Set the pins for CSI reset and power down. */
    // gpio_pin_config_t pinConfig = {
    //     kGPIO_DigitalOutput,
    //     1,
    //     kGPIO_NoIntmode,
    // };

    // GPIO_PinInit(BOARD_CAMERA_PWDN_GPIO, BOARD_CAMERA_PWDN_PIN, &pinConfig);
}
