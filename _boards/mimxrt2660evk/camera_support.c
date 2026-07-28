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

/* Parallel CSI camera module (connector J95, schematic SPF-96037 sheet 17):
 * its reset pin is not connected on the board, and its power-down (PWDN, via
 * net CSI_PWDN) is driven by the PCA9555 I/O expander P1_0 -- not the PCAL6524
 * (which carries the MIPI-CSI camera's CSI_RST_B / CSI_PWR_CTL, see isi_board.c). */
static void BOARD_PullCameraResetPin(bool pullUp)
{
    /* J95 RESET is left unconnected on this board (camera uses power-on reset). */
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

void BOARD_EarlyPrepareCamera(void)
{
}

void BOARD_InitCameraResource(void)//todo
{
    BOARD_Camera_I2C_Init();

    /* Parallel CSI power-down (CSI_PWDN) is on PCA9555 P1_0; configure it as an
     * output before the camera adapter drives it. */
    pca9555_handle_t *h = BOARD_GetPCA9555Handle();
    (void)PCA9555_SetDirection(h, 1UL << BOARD_PCA9555_CSI_PWDN, kPCA9555_Output);
}
