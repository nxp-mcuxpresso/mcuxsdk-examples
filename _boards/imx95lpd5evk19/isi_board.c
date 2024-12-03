/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "isi_example.h"
#include "fsl_isi_camera_adapter.h"
#include "fsl_dwc_mipi_csi2rx.h"
#include "fsl_camera_device.h"
#include "fsl_ap1302.h"
#include "fsl_lpi2c.h"
#include "dpu_example.h"
#include "isi_config.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define APP_ISI_IRQHandler           ISI_IRQHandler

/* ADP5585 */
#define APP_CAMERA_ADP5585_I2C            LPI2C3
#define APP_CAMERA_ADP5585_I2C_ADDR       (0x34U)
#define APP_CAMERA_ADP5585_I2C_CLOCK_ROOT hal_clock_lpi2c3
#define APP_CAMERA_ADP5585_I2C_CLOCK_FREQ HAL_ClockGetIpFreq(APP_CAMERA_ADP5585_I2C_CLOCK_ROOT)

#define ADP5585_DVDD_SEL        0
#define ADP5585_VAA_SEL         1
#define ADP5585_ISP_BYP         2
#define ADP5585_FLED_TOR        3
#define ADP5585_VCM_PWREN       4
#define ADP5585_PWREN1          8
#define ADP5585_PWREN2          9
#define ADP5585_PWREN3          10
#define ADP5585_PWREN4          11
#define ADP5585_PWREN5          12

pcal6408_handle_t pcal_handle;
adp5585_handle_t handle;
isi_private_data_t isiPrivateData;

isi_resource_t isiResource = {
    .isiBase = APP_ISI,
    /* MIPI-CSI0 interface ISI port: 2 */
    .isiInputPort = 2,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined(BOARD_USE_ADP5585) && BOARD_USE_ADP5585
void APP_CAMERA_ADP5585_I2C_Init(void)
{
    BOARD_LPI2C_Init(APP_CAMERA_ADP5585_I2C, APP_CAMERA_ADP5585_I2C_CLOCK_FREQ);
}

status_t APP_CAMERA_ADP5585_I2C_Send(uint8_t deviceAddress,
                                      uint32_t subAddress,
                                      uint8_t subAddressSize,
                                      const uint8_t *txBuff,
                                      uint8_t txBuffSize,
                                      uint32_t flags)
{
    return BOARD_LPI2C_Send(APP_CAMERA_ADP5585_I2C, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff, txBuffSize,
                            flags);
}

status_t APP_CAMERA_ADP5585_I2C_Receive(uint8_t deviceAddress,
                                         uint32_t subAddress,
                                         uint8_t subAddressSize,
                                         uint8_t *rxBuff,
                                         uint8_t rxBuffSize,
                                         uint32_t flags)
{
    return BOARD_LPI2C_Receive(APP_CAMERA_ADP5585_I2C, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize, flags);
}

void APP_CAMERA_InitADP5585(adp5585_handle_t *handle)
{
    APP_CAMERA_ADP5585_I2C_Init();

    static const adp5585_config_t config = {
        .i2cAddr         = APP_CAMERA_ADP5585_I2C_ADDR,
        .I2C_SendFunc    = APP_CAMERA_ADP5585_I2C_Send,
        .I2C_ReceiveFunc = APP_CAMERA_ADP5585_I2C_Receive,
    };

    ADP5585_Init(handle, &config);
}

#endif /* BOARD_USE_ADP5585. */

status_t APP_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subAddressSize,
                          uint32_t *txBuff,
                          uint32_t txBuffSize,
                          uint32_t flags)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = flags;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t APP_CAMERA_LPI2C_Send(LPI2C_Type *base,
                              uint8_t deviceAddress,
                              uint32_t subAddress,
                              uint8_t subAddressSize,
                              uint8_t *txBuff,
                              uint32_t txBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t APP_CAMERA_LPI2C_Receive(LPI2C_Type *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 uint8_t *rxBuff,
                                 uint8_t rxBuffSize)
{
    status_t status;
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = NULL;
    xfer.dataSize       = 0;

    status = LPI2C_MasterTransferBlocking(base, &xfer);

    if (kStatus_Success == status)
    {
        xfer.subaddressSize = 0;
        xfer.direction      = kLPI2C_Read;
        xfer.data           = rxBuff;
        xfer.dataSize       = rxBuffSize;

        status = LPI2C_MasterTransferBlocking(base, &xfer);
    }

    return status;
}

status_t APP_Camera_I2C_Send_8bit(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{

    return APP_CAMERA_LPI2C_Send(APP_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize,
                                (uint8_t *)txBuff, txBuffSize);
}

status_t APP_Camera_I2C_Send_32bit(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint32_t txBuffSize)
{

    return APP_CAMERA_LPI2C_Send(APP_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize,
                                (uint8_t *)txBuff, txBuffSize);
}

status_t APP_Camera_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return APP_CAMERA_LPI2C_Receive(APP_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff,
                                   rxBuffSize);
}

void APP_Camera_I2C_PowerUp(bool powerup)
{
    if (powerup)
    {
        APP_CAMERA_InitADP5585(&handle);
        /* Set DVDO_1V2 */
        ADP5585_SetDirection(&handle, (1 << ADP5585_PWREN2), kADP5585_Output);
        ADP5585_SetPins(&handle, (1 << ADP5585_PWREN2));

        /* Set VDDIO_1V8 */
        ADP5585_SetDirection(&handle, (1 << ADP5585_PWREN1), kADP5585_Output);
        ADP5585_SetPins(&handle, (1 << ADP5585_PWREN1));

        /* Set AVDD_2V8 */
        ADP5585_SetDirection(&handle, (1 << ADP5585_PWREN3), kADP5585_Output);
        ADP5585_SetPins(&handle, (1 << ADP5585_PWREN3));

        /* Set HMISC_VDDIO_1V8 */
        ADP5585_SetDirection(&handle, (1 << ADP5585_PWREN5), kADP5585_Output);
        ADP5585_SetPins(&handle, (1 << ADP5585_PWREN5));

        ADP5585_SetDirection(&handle, (1 << ADP5585_ISP_BYP), kADP5585_Output);
        ADP5585_SetPins(&handle, (1 << ADP5585_ISP_BYP));
    }
}

void APP_PullCameraReset(bool reset)
{
    if (reset != false)
    {
        PCAL6408_SetDirection(&pcal_handle, (1 << BOARD_PCAL6408_CSI1_RST_B), kPCAL6408_Output);
        PCAL6408_SetPins(&pcal_handle, (1 << BOARD_PCAL6408_CSI1_RST_B));
    }
    else
    {
        PCAL6408_SetDirection(&pcal_handle, (1 << BOARD_PCAL6408_CSI1_RST_B), kPCAL6408_Output);
        PCAL6408_ClearPins(&pcal_handle, (1 << BOARD_PCAL6408_CSI1_RST_B));
    }
}

void APP_PullCameraPowerDown(bool powerdown)
{
    if (powerdown != false)
    {
        PCAL6408_SetDirection(&pcal_handle, (1 << BOARD_PCAL6408_CSI1_PWDN), kPCAL6408_Output);
        PCAL6408_SetPins(&pcal_handle, (1 << BOARD_PCAL6408_CSI1_PWDN));
    }
    else
    {
        PCAL6408_SetDirection(&pcal_handle, (1 << BOARD_PCAL6408_CSI1_PWDN), kPCAL6408_Output);
        PCAL6408_ClearPins(&pcal_handle, (1 << BOARD_PCAL6408_CSI1_PWDN));
    }
}

void APP_PullCameraEnableIsp(bool isp_en)
{
    if (isp_en != false)
    {
        ADP5585_SetDirection(&handle, (1 << ADP5585_ISP_BYP), kADP5585_Output);
        ADP5585_SetPins(&handle, (1 << ADP5585_ISP_BYP));
    }
}

camera_receiver_handle_t cameraReceiver = {
    .resource    = &isiResource,
    .ops         = &isi_ops,
    .privateData = &isiPrivateData,
};

ap1302_resource_t ap1302Resource = {
    .i2cSendFunc         = APP_Camera_I2C_Send_8bit, /* send 8bit tx buffersize */
    .i2cSendFunc32bit    = APP_Camera_I2C_Send_32bit, /* send 32bit tx buffersize */
    .i2cReceiveFunc      = APP_Camera_I2C_Receive,
    .powerUp             = APP_Camera_I2C_PowerUp,
    .pullResetPin        = APP_PullCameraReset,
    .pullPowerDownPin    = APP_PullCameraPowerDown,
};

camera_device_handle_t cameraDevice = {
    .resource = &ap1302Resource,
    .ops      = &ap1302_ops,
};

status_t BOARD_InitCameraInterface(const camera_config_t *camera_config)
{
    status_t result = kStatus_Success;

    csi2rx_config_t csi2rxConfig = {0};
    csi2rxConfig.laneNum          = camera_config->csiLanes;
    csi2rxConfig.datatype = CAMERA_DEVICE_OUTPUT_FORMAT;
    csi2rxConfig.pg_enable = false;
    csi2rxConfig.vcnum = camera_config->mipiChannel;
    csi2rxConfig.hsfreqrange = 0x29;

    csi2rxConfig.cfgclkfreqrange = 0x1C; /* according to dphy document, 86 page setting */

    result = MIPI_CSI2RX_Init(APP_MIPI_CSI2, APP_MIPI_CSI2_PHY, &csi2rxConfig);

    return result;
}

void BOARD_InitPixelLinkInterface(camera_csr_pixel_formatting_config_t cameracsr_config, const camera_config_t *camera_config)
{
    assert(NULL != camera_config);

    cameracsr_config.csiinterface = APP_MIPI_CSI2_INTERFACE;
    cameracsr_config.mipiVc = camera_config->mipiChannel;
    cameracsr_config.PixelDataType = CAMERA_DEVICE_OUTPUT_FORMAT;
    cameracsr_config.NonPixelDataNewVc = camera_config->mipiChannel;
    cameracsr_config.enableRAW32 = false;
    cameracsr_config.enablePixelDataRoute = false;
    cameracsr_config.enableNonPixelDataRoute = false;
    cameracsr_config.PixelDataNewVc = camera_config->mipiChannel;
    cameracsr_config.NonPixelDataType = 0x0;

    CAMERACSR_PixelFormatting(BLK_CTRL_CAMERAMIX, &cameracsr_config);
}

void APP_ISI_IRQHandler(void)
{
    ISI_ADAPTER_IRQHandler(&cameraReceiver);
}

/*
 * Prepare for the camera:
 *
 * Initialize the MIPI CSI2, the input frames are sent to ISI.
 * Initialize the I2C to communicate with camera.
 * Enable the ISI interrupt.
 */
void APP_PrepareCamera()
{
    /* Initialize the camera I2C. */
    BOARD_InitPCAL6408_I2C3(&pcal_handle);
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

