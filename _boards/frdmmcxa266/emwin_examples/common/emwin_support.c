/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "GUI.h"
#include "WM.h"
#include "GUIDRV_FlexColor.h"
#include "emwin_support.h"

#include "fsl_debug_console.h"
#include "fsl_gpio.h"

#include "fsl_dbi_flexio_edma.h"
#include "fsl_flexio_mculcd.h"

#include "fsl_port.h"
#include "fsl_lpi2c.h"
#include "fsl_st7796s.h"
#include "fsl_gt911.h"
#include "board.h"

#ifndef GUI_MEMORY_ADDR
static uint32_t s_gui_memory[(GUI_NUMBYTES + 3) / 4]; /* needs to be word aligned */
#define GUI_MEMORY_ADDR ((uint32_t)s_gui_memory)
#endif

#define I2C_RELEASE_SDA_PORT  PORT3
#define I2C_RELEASE_SCL_PORT  PORT3
#define I2C_RELEASE_SDA_GPIO  GPIO3
#define I2C_RELEASE_SDA_PIN   28U
#define I2C_RELEASE_SCL_GPIO  GPIO3
#define I2C_RELEASE_SCL_PIN   27U
#define I2C_RELEASE_BUS_COUNT 100U

void DEMO_TouchConfigIntPin(gt911_int_pin_mode_t mode);

static void DEMO_DbiMemoryDoneCallback(status_t status, void *userData)
{
}

/*******************************************************************************
 * Implementation of PortAPI for emWin LCD driver
 ******************************************************************************/

/* ST7796S LCD controller handle. */
st7796s_handle_t lcdHandle;

/* DBI XFER handle. */
dbi_flexio_edma_xfer_handle_t g_dbiFlexioEdmaXferHandle;
/* The FlexIO MCU LCD device. */
FLEXIO_MCULCD_Type flexioLcdDev = {
    .flexioBase          = BOARD_FLEXIO,
    .busType             = kFLEXIO_MCULCD_8080,
    .dataPinStartIndex   = BOARD_FLEXIO_DATA_PIN_START,
    .ENWRPinIndex        = BOARD_FLEXIO_WR_PIN,
    .RDPinIndex          = BOARD_FLEXIO_RD_PIN,
    .txShifterStartIndex = BOARD_FLEXIO_TX_START_SHIFTER,
    .txShifterEndIndex   = BOARD_FLEXIO_TX_END_SHIFTER,
    .rxShifterStartIndex = BOARD_FLEXIO_RX_START_SHIFTER,
    .rxShifterEndIndex   = BOARD_FLEXIO_RX_END_SHIFTER,
    .timerIndex          = BOARD_FLEXIO_TIMER,
    .setCSPin            = BOARD_SetCSPin,
    .setRSPin            = BOARD_SetRSPin,
    .setRDWRPin          = NULL /* Not used in 8080 mode. */
};

status_t BOARD_LCD_Init(void)
{
    status_t status;

    flexio_mculcd_config_t flexioMcuLcdConfig;

    const st7796s_config_t st7796sConfig = {.driverPreset    = kST7796S_DriverPresetLCDPARS035,
                                            .pixelFormat     = kST7796S_PixelFormatRGB565,
                                            .orientationMode = kST7796S_Orientation0,
                                            .teConfig        = kST7796S_TEDisabled,
                                            .invertDisplay   = true,
                                            .flipDisplay     = true,
                                            .bgrFilter       = false};

    const gpio_pin_config_t pinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1,
    };

    /* Set SSD1963 CS, RS, and reset pin to output. */
    GPIO_PinInit(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, &pinConfig);
    GPIO_PinInit(BOARD_LCD_CS_GPIO, BOARD_LCD_CS_PIN, &pinConfig);
    GPIO_PinInit(BOARD_LCD_RS_GPIO, BOARD_LCD_RS_PIN, &pinConfig);

    /* Initialize the flexio MCU LCD. */
    /*
     * flexioMcuLcdConfig.enable = true;
     * flexioMcuLcdConfig.enableInDoze = false;
     * flexioMcuLcdConfig.enableInDebug = true;
     * flexioMcuLcdConfig.enableFastAccess = true;
     * flexioMcuLcdConfig.baudRate_Bps = 96000000U;
     */
    FLEXIO_MCULCD_GetDefaultConfig(&flexioMcuLcdConfig);
    flexioMcuLcdConfig.baudRate_Bps = BOARD_FLEXIO_BAUDRATE_BPS;

    status = FLEXIO_MCULCD_Init(&flexioLcdDev, &flexioMcuLcdConfig, BOARD_FLEXIO_CLOCK_FREQ);

    if (kStatus_Success != status)
    {
        return status;
    }

    /* Create the DBI XFER handle. Because DMA transfer is not used, so don't
     * need to create DMA handle.
     */
    status = DBI_FLEXIO_EDMA_CreateXferHandle(&g_dbiFlexioEdmaXferHandle, &flexioLcdDev, NULL, NULL);

    if (kStatus_Success != status)
    {
        return status;
    }

    /* Reset the SSD1963 LCD controller. */
    GPIO_PinWrite(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, 0);
    DEMO_TouchConfigIntPin(kGT911_IntPinPullDown);

    SDK_DelayAtLeastUs(1000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    GPIO_PinWrite(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, 1);
    DEMO_TouchConfigIntPin(kGT911_IntPinInput);

    SDK_DelayAtLeastUs(5000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    status = ST7796S_Init(&lcdHandle, &st7796sConfig, &g_dbiFlexioEdmaXferOps, &g_dbiFlexioEdmaXferHandle);

    ST7796S_SetMemoryDoneCallback(&lcdHandle, DEMO_DbiMemoryDoneCallback, NULL);

    ST7796S_SelectArea(&lcdHandle, 0, 0, LCD_WIDTH, LCD_HEIGHT);
    return status;
}

static void APP_pfWrite16_A0(U16 Data)
{
    FLEXIO_MCULCD_StartTransfer(&flexioLcdDev);
    FLEXIO_MCULCD_WriteCommandBlocking(&flexioLcdDev, Data);
    FLEXIO_MCULCD_StopTransfer(&flexioLcdDev);
}

static void APP_pfWrite16_A1(U16 Data)
{
    FLEXIO_MCULCD_StartTransfer(&flexioLcdDev);
    FLEXIO_MCULCD_WriteDataArrayBlocking(&flexioLcdDev, &Data, 2);
    FLEXIO_MCULCD_StopTransfer(&flexioLcdDev);
}

static void APP_pfWriteM16_A1(U16 *pData, int NumItems)
{
    FLEXIO_MCULCD_StartTransfer(&flexioLcdDev);
    FLEXIO_MCULCD_WriteDataArrayBlocking(&flexioLcdDev, pData, NumItems * 2);
    FLEXIO_MCULCD_StopTransfer(&flexioLcdDev);
}

static U16 APP_pfRead16_A1(void)
{
    uint16_t Data = 0;
#if defined(BOARD_LCD_READABLE) && (BOARD_LCD_READABLE == 0)
    PRINTF("Warning: LCD does not support read operation, the image may get distorted.\r\n");
    assert(0);
#endif
    FLEXIO_MCULCD_StartTransfer(&flexioLcdDev);
    FLEXIO_MCULCD_ReadDataArrayBlocking(&flexioLcdDev, &Data, 2);
    FLEXIO_MCULCD_StopTransfer(&flexioLcdDev);
    return Data;
}

static void APP_pfReadM16_A1(U16 *pData, int NumItems)
{
#if defined(BOARD_LCD_READABLE) && (BOARD_LCD_READABLE == 0)
    PRINTF("Warning: LCD does not support read operation, the image may get distorted.\r\n");
    assert(0);
#endif
    FLEXIO_MCULCD_StartTransfer(&flexioLcdDev);
    FLEXIO_MCULCD_ReadDataArrayBlocking(&flexioLcdDev, pData, NumItems * 2);
    FLEXIO_MCULCD_StopTransfer(&flexioLcdDev);
}

/*******************************************************************************
 * Implementation of communication with the touch controller
 ******************************************************************************/

/* Touch driver handle. */
static gt911_handle_t touchHandle;

status_t DEMO_LCD_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_TOUCH_I2C, deviceAddress, subAddress, subaddressSize, (uint8_t *)txBuff, txBuffSize);
}

status_t DEMO_LCD_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_TOUCH_I2C, deviceAddress, subAddress, subaddressSize, rxBuff, rxBuffSize);
}

void DEMO_TouchDelayMs(uint32_t delayMs)
{
#if defined(SDK_OS_FREE_RTOS)
    vTaskDelay(pdMS_TO_TICKS(delayMs));
#else
    SDK_DelayAtLeastUs(delayMs * 1000, CLOCK_GetCoreSysClkFreq());
#endif
}

void DEMO_TouchConfigIntPin(gt911_int_pin_mode_t mode)
{
    port_pin_config_t int_config = {/* Internal pull-up/down resistor is disabled */
                                    kPORT_PullDown,
                                    /* Low internal pull resistor value is selected. */
                                    kPORT_LowPullResistor,
                                    /* Fast slew rate is configured */
                                    kPORT_FastSlewRate,
                                    /* Passive input filter is disabled */
                                    kPORT_PassiveFilterDisable,
                                    /* Open drain output is disabled */
                                    kPORT_OpenDrainDisable,
                                    /* Low drive strength is configured */
                                    kPORT_LowDriveStrength,
                                    /* Pin is configured as GPIO */
                                    kPORT_MuxAlt0,
                                    /* Digital input enabled */
                                    kPORT_InputBufferEnable,
                                    /* Digital input is not inverted */
                                    kPORT_InputNormal,
                                    /* Pin Control Register fields [15:0] are not locked */
                                    kPORT_UnlockRegister};

    switch (mode)
    {
        case kGT911_IntPinPullUp:
            int_config.pullSelect = kPORT_PullUp;
            break;
        case kGT911_IntPinPullDown:
            int_config.pullSelect = kPORT_PullDown;
            break;
        case kGT911_IntPinInput:
            int_config.pullSelect = kPORT_PullDisable;
            break;
        default:
            break;
    };

    PORT_SetPinConfig(BOARD_LCD_INT_PORT, BOARD_LCD_INT_PIN, &int_config);
}

void DEMO_TouchConfigResetPin(bool pullUp)
{
    /*
     * As touch controller and display controller shares the same reset pin,
     * we do not do actual reset / address configuration here. Please check below for
     * the relationship between RST pin and INT pin.
     *
     */
}

static void BOARD_Touch_Init(void)
{
    DEMO_TouchConfigIntPin(kGT911_IntPinInput);

    BOARD_LPI2C_Init(BOARD_TOUCH_I2C, BOARD_TOUCH_I2C_CLOCK_FREQ);

    gt911_config_t touchConfig = {.I2C_SendFunc     = DEMO_LCD_I2C_Send,
                                  .I2C_ReceiveFunc  = DEMO_LCD_I2C_Receive,
                                  .timeDelayMsFunc  = DEMO_TouchDelayMs,
                                  .intPinFunc       = DEMO_TouchConfigIntPin,
                                  .pullResetPinFunc = DEMO_TouchConfigResetPin,
                                  .touchPointNum    = 5,
                                  .i2cAddrMode      = kGT911_I2cAddrMode0,
                                  .intTrigMode      = kGT911_IntFallingEdge};

    GT911_Init(&touchHandle, &touchConfig);
}

void BOARD_Touch_Deinit(void)
{
    LPI2C_MasterDeinit(BOARD_TOUCH_I2C);
}

int BOARD_Touch_Poll(void)
{
    GUI_PID_STATE pid_state;
    int32_t touch_x      = 0;
    int32_t touch_y      = 0;
    static int32_t pre_x = 0;
    static int32_t pre_y = 0;

    pid_state.Layer   = 0;
    pid_state.Pressed = false;

    if (kStatus_Success != GT911_GetSingleTouch(&touchHandle, (int *)(&touch_x), (int *)(&touch_y)))
    {
        GUI_TOUCH_StoreState(-1, -1);
        return 0;
    }
    else
    {
        pid_state.Pressed = true;
        pid_state.Layer   = 0;
        pid_state.x       = 0;
        pid_state.y       = 0;

        if ((touch_x == pre_x) && (touch_y == pre_y))
        {
            return 0;
        }
        else
        {
            pre_x = touch_x;
            pre_y = touch_y;
        }

        switch (lcdHandle.orientationMode)
        {
            case kST7796S_Orientation0:
                pid_state.x = touch_x;
                pid_state.y = touch_y;
                break;
            case kST7796S_Orientation90:
                pid_state.x = (int16_t)(touchHandle.resolutionY - touch_y);
                pid_state.y = (int16_t)touch_x;
                break;
            case kST7796S_Orientation180:
                pid_state.x = (int16_t)(touchHandle.resolutionX - touch_x);
                pid_state.y = (int16_t)(touchHandle.resolutionY - touch_y);
                break;
            case kST7796S_Orientation270:
                pid_state.x = (int16_t)touch_y;
                pid_state.y = (int16_t)(touchHandle.resolutionX - touch_x);
                break;
            default:
                break;
        }
        GUI_TOUCH_StoreState(pid_state.x, pid_state.y);
        return 1;
    }
}

/*******************************************************************************
 * Application implemented functions required by emWin library
 ******************************************************************************/
void LCD_X_Config(void)
{
    BOARD_LCD_Init();
    BOARD_Touch_Init();

    ST7796S_SelectArea(&lcdHandle, 0, 0, 480, 320);

    GUI_DEVICE *pDevice;
    GUI_PORT_API PortAPI;
    CONFIG_FLEXCOLOR Config = {0, 0, 0, 0, 1};

    pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_565, 0, 0);

    GUIDRV_FlexColor_Config(pDevice, &Config);

    if (LCD_GetSwapXY())
    {
        LCD_SetSizeEx(0, LCD_HEIGHT, LCD_WIDTH);
        LCD_SetVSizeEx(0, LCD_HEIGHT, LCD_WIDTH);
    }
    else
    {
        LCD_SetSizeEx(0, LCD_WIDTH, LCD_HEIGHT);
        LCD_SetVSizeEx(0, LCD_WIDTH, LCD_HEIGHT);
    }

    GUI_SetOrientation(GUI_MIRROR_X);

    PortAPI.pfWrite16_A0  = APP_pfWrite16_A0;
    PortAPI.pfWrite16_A1  = APP_pfWrite16_A1;
    PortAPI.pfWriteM16_A1 = APP_pfWriteM16_A1;
    PortAPI.pfRead16_A1   = APP_pfRead16_A1;
    PortAPI.pfReadM16_A1  = APP_pfReadM16_A1;
    GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66720, GUIDRV_FLEXCOLOR_M16C0B16);
}

int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void *pData)
{
    int result = 0;

    switch (Cmd)
    {
        case LCD_X_INITCONTROLLER:
            ST7796S_EnableDisplay(&lcdHandle, true);
            break;
        default:
            result = -1;
            break;
    }

    return result;
}

void GUI_X_Config(void)
{
    /* Assign work memory area to emWin */
    GUI_ALLOC_AssignMemory((void *)GUI_MEMORY_ADDR, GUI_NUMBYTES);

    /* Select default font */
    GUI_SetDefaultFont(GUI_FONT_6X8);
}

void GUI_X_Init(void)
{
}

/* Dummy RTOS stub required by emWin */
void GUI_X_InitOS(void)
{
}

/* Dummy RTOS stub required by emWin */
void GUI_X_Lock(void)
{
}

/* Dummy RTOS stub required by emWin */
void GUI_X_Unlock(void)
{
}

/* Dummy RTOS stub required by emWin */
U32 GUI_X_GetTaskId(void)
{
    return 0;
}

void GUI_X_ExecIdle(void)
{
}

GUI_TIMER_TIME GUI_X_GetTime(void)
{
    return 0;
}

void GUI_X_Delay(int Period)
{
    volatile int i;
    for (; Period > 0; Period--)
    {
        for (i = 15000; i > 0; i--)
            ;
    }
}

void DEMO_LCD_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_TOUCH_I2C, BOARD_TOUCH_I2C_CLOCK_FREQ);
}

void BOARD_SetCSPin(bool set)
{
    GPIO_PinWrite(BOARD_LCD_CS_GPIO, BOARD_LCD_CS_PIN, (uint8_t)set);
}

void BOARD_SetRSPin(bool set)
{
    GPIO_PinWrite(BOARD_LCD_RS_GPIO, BOARD_LCD_RS_PIN, (uint8_t)set);
}

static void i2c_release_bus_delay(void)
{
    SDK_DelayAtLeastUs(100U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0;
    gpio_pin_config_t pin_config;
    port_pin_config_t i2c_pin_config = {0};

    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux        = kPORT_MuxAsGpio;

    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic  = 1U;
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SDA_PIN, &i2c_pin_config);

    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();
}
