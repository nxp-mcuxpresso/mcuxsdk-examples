/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if defined(SDK_OS_FREE_RTOS)
#include "FreeRTOS.h"
#include "semphr.h"
#endif

#include "board.h"
#include "fsl_gpio.h"

#include "lvgl_support.h"
#include "lvgl.h"

#if BOARD_USE_FLEXIO_SMARTDMA
#include "fsl_dbi_flexio_smartdma.h"
#else
#include "fsl_dbi_flexio_edma.h"
#endif
#include "fsl_flexio_mculcd.h"
#include "fsl_lpi2c.h"
#include "fsl_port.h"

#include "fsl_st7796s.h"
#include "fsl_gt911.h"

#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_MS_TO_TICK(ms) ((ms * configTICK_RATE_HZ / 1000) + 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_InitLcd(void);

static void DEMO_InitLcdClock(void);

static status_t DEMO_InitLcdController(void);

static void DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p);

static void DEMO_InitTouch(void);

static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data);

static void DEMO_SetCSPin(bool set);

static void DEMO_SetRSPin(bool set);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static gt911_handle_t touchHandle;
static volatile bool s_touchPending = false;

#if defined(SDK_OS_FREE_RTOS)
static SemaphoreHandle_t s_memWriteDone;
#else
static volatile bool s_memWriteDone;
#endif

/* ST7796S LCD controller handle. */
st7796s_handle_t lcdHandle;

/* DBI XFER handle. */
#if BOARD_USE_FLEXIO_SMARTDMA
dbi_flexio_smartdma_xfer_handle_t g_dbiFlexioXferHandle;
#else
dbi_flexio_edma_xfer_handle_t g_dbiFlexioXferHandle;
#endif
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
    .setCSPin            = DEMO_SetCSPin,
    .setRSPin            = DEMO_SetRSPin,
    .setRDWRPin          = NULL /* Not used in 8080 mode. */
};

SDK_ALIGN(static uint8_t s_frameBuffer[CONFIG_LVGL_SUPPORT_VDB_COUNT][LCD_VIRTUAL_BUF_SIZE * LCD_FB_BYTE_PER_PIXEL], 4);

/*******************************************************************************
 * Code
 ******************************************************************************/
static void DEMO_SetCSPin(bool set)
{
    GPIO_PinWrite(BOARD_LCD_CS_GPIO, BOARD_LCD_CS_PIN, (uint8_t)set);
}

static void DEMO_SetRSPin(bool set)
{
    GPIO_PinWrite(BOARD_LCD_RS_GPIO, BOARD_LCD_RS_PIN, (uint8_t)set);
}

static void DEMO_DbiMemoryDoneCallback(status_t status, void *userData)
{
#if defined(SDK_OS_FREE_RTOS)
    BaseType_t taskAwake = pdFALSE;

    xSemaphoreGiveFromISR(s_memWriteDone, &taskAwake);

    portYIELD_FROM_ISR(taskAwake);
#else
    s_memWriteDone = true;
#endif
}

void DEMO_LCD_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_TOUCH_I2C, BOARD_TOUCH_I2C_CLOCK_FREQ);
}

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
                                    .pullSelect = kPORT_PullDown,
                                    /* Low internal pull resistor value is selected. */
                                    .pullValueSelect = kPORT_LowPullResistor,
                                    /* Fast slew rate is configured */
                                    .slewRate = kPORT_FastSlewRate,
                                    /* Passive input filter is disabled */
                                    .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                    /* Open drain output is disabled */
                                    .openDrainEnable = kPORT_OpenDrainDisable,
                                    /* Low drive strength is configured */
                                    .driveStrength = kPORT_LowDriveStrength,
#if defined(FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH1) && FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH1
                                    .driveStrength1 = kPORT_NormalDriveStrength,
#endif
                                    /* Pin is configured as GPIO */
                                    .mux = kPORT_MuxAlt0,
                                    /* Digital input enabled */
                                    .inputBuffer = kPORT_InputBufferEnable,
                                    /* Digital input is not inverted */
                                    .invertInput = kPORT_InputNormal,
                                    /* Pin Control Register fields [15:0] are not locked */
                                    .lockRegister = kPORT_UnlockRegister};

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

/* Clear the LCD controller video memory content. */
static void DEMO_ClearLcd(void)
{
    int32_t leftLinesToClear = LCD_HEIGHT;
    int32_t curLinesToClear;
    int32_t startLine = 0;

    while (leftLinesToClear > 0)
    {
        curLinesToClear = (leftLinesToClear > (CONFIG_LVGL_SUPPORT_VDB_COUNT * LCD_VIRTUAL_BUF_HEIGHT)) ?
                          (CONFIG_LVGL_SUPPORT_VDB_COUNT * LCD_VIRTUAL_BUF_HEIGHT) : leftLinesToClear;

        ST7796S_SelectArea(&lcdHandle, 0, startLine, LCD_WIDTH - 1, startLine + curLinesToClear - 1);

#if !defined(SDK_OS_FREE_RTOS)
        s_memWriteDone = false;
#endif

        ST7796S_WritePixels(&lcdHandle, (uint16_t *)s_frameBuffer, curLinesToClear * LCD_WIDTH);

#if defined(SDK_OS_FREE_RTOS)
        if (xSemaphoreTake(s_memWriteDone, portMAX_DELAY) != pdTRUE)
        {
            PRINTF("Wait semaphore error: s_memWriteDone\r\n");
            assert(0);
        }
#else
        while (false == s_memWriteDone)
        {
        }
#endif

        startLine += curLinesToClear;
        leftLinesToClear -= curLinesToClear;
    }
}

status_t DEMO_InitLcdController(void)
{
    status_t status;

    flexio_mculcd_config_t flexioMcuLcdConfig;

    const st7796s_config_t st7796sConfig = {.driverPreset    = kST7796S_DriverPresetLCDPARS035,
                                            .pixelFormat     = kST7796S_PixelFormatRGB565,
                                            .orientationMode = kST7796S_Orientation270,
                                            .teConfig        = kST7796S_TEDisabled,
                                            .invertDisplay   = true,
                                            .flipDisplay     = true,
                                            .bgrFilter       = true};

    const gpio_pin_config_t pinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1,
    };

#if BOARD_USE_FLEXIO_SMARTDMA
    flexio_mculcd_smartdma_config_t flexioEzhConfig = {
        .inputPixelFormat = kFLEXIO_MCULCD_RGB565,
        .outputPixelFormat = kFLEXIO_MCULCD_RGB565,
    };
#endif

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

#if BOARD_USE_FLEXIO_SMARTDMA
    /* Create the DBI XFER handle. */
    status = DBI_FLEXIO_SMARTDMA_CreateXferHandle(&g_dbiFlexioXferHandle, &flexioLcdDev, &flexioEzhConfig);
#else
    /* Create the DBI XFER handle. Because DMA transfer is not used, so don't
     * need to create DMA handle.
     */
    status = DBI_FLEXIO_EDMA_CreateXferHandle(&g_dbiFlexioXferHandle, &flexioLcdDev, NULL, NULL);
#endif

    if (kStatus_Success != status)
    {
        return status;
    }

    /* Reset the SSD1963 LCD controller. */
    GPIO_PinWrite(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, 0);

    /* Required for GT911 I2C address mode 0 */
    DEMO_TouchConfigIntPin(kGT911_IntPinPullDown);

#if defined(SDK_OS_FREE_RTOS)
    vTaskDelay(DEMO_MS_TO_TICK(1)); /* Delay at least 10ns. */
#else
    SDK_DelayAtLeastUs(1000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#endif
    GPIO_PinWrite(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, 1);

    DEMO_TouchConfigIntPin(kGT911_IntPinInput);

#if defined(SDK_OS_FREE_RTOS)
    vTaskDelay(DEMO_MS_TO_TICK(5)); /* Delay at 5ms. */
#else
    SDK_DelayAtLeastUs(5000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#endif

#if BOARD_USE_FLEXIO_SMARTDMA
    status = ST7796S_Init(&lcdHandle, &st7796sConfig, &g_dbiFlexioSmartdmaXferOps, &g_dbiFlexioXferHandle);
#else
    status = ST7796S_Init(&lcdHandle, &st7796sConfig, &g_dbiFlexioEdmaXferOps, &g_dbiFlexioXferHandle);
#endif

    if (status == kStatus_Success)
    {
        ST7796S_SetMemoryDoneCallback(&lcdHandle, DEMO_DbiMemoryDoneCallback, NULL);

        /* Clear the SSD1963 video ram. */
        DEMO_ClearLcd();

        ST7796S_EnableDisplay(&lcdHandle, true);
    }
    else
    {
        PRINTF("LCD controller initialization failed.\r\n");
    }

    return status;
}

static void DEMO_InitLcdClock(void)
{
}

static void DEMO_InitLcd(void)
{
#if defined(SDK_OS_FREE_RTOS)
    s_memWriteDone = xSemaphoreCreateBinary();
    if (NULL == s_memWriteDone)
    {
        PRINTF("Frame semaphore create failed\r\n");
        assert(0);
    }
#else
    s_memWriteDone = false;
#endif

    DEMO_InitLcdClock();
    DEMO_InitLcdController();
}

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_flush_ready()' has to be called when finished
 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h*/
static void DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p)
{
    lv_coord_t x1 = area->x1;
    lv_coord_t y1 = area->y1;
    lv_coord_t x2 = area->x2;
    lv_coord_t y2 = area->y2;

    int32_t length = (x2 - x1 + 1) * (y2 - y1 + 1) * LCD_FB_BYTE_PER_PIXEL;

    ST7796S_SelectArea(&lcdHandle, x1, y1, x2, y2);

#if !defined(SDK_OS_FREE_RTOS)
    s_memWriteDone = false;
#endif

    ST7796S_WritePixels(&lcdHandle, (uint16_t *)color_p, length / LCD_FB_BYTE_PER_PIXEL);

#if defined(SDK_OS_FREE_RTOS)
    if (xSemaphoreTake(s_memWriteDone, portMAX_DELAY) != pdTRUE)
    {
        PRINTF("Wait semaphore error: s_memWriteDone\r\n");
        assert(0);
    }
#else
    while (!s_memWriteDone)
    {
    }
#endif

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

void lv_port_disp_init(void)
{
    lv_display_t * disp_drv; /*Descriptor of a display driver*/

    memset(s_frameBuffer, 0, sizeof(s_frameBuffer));

    /*-------------------------
     * Initialize your display
     * -----------------------*/
    DEMO_InitLcd();

    disp_drv = lv_display_create(LCD_WIDTH, LCD_HEIGHT);

#if (CONFIG_LVGL_SUPPORT_VDB_COUNT == 2)
    lv_display_set_buffers(disp_drv, (void*)s_frameBuffer[0], (void*)s_frameBuffer[1], LCD_VIRTUAL_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
#else
    lv_display_set_buffers(disp_drv, (void*)s_frameBuffer[0], NULL, LCD_VIRTUAL_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif
    lv_display_set_flush_cb(disp_drv, DEMO_FlushDisplay);
}

void lv_port_indev_init(void)
{
    /*Initialize your touchpad */
    DEMO_InitTouch();

    /*Register a touchpad input device*/
    lv_indev_t * indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, DEMO_ReadTouch);
}

/*Initialize your touchpad*/
static void DEMO_InitTouch(void)
{
    const gpio_pin_config_t intPinConfig    = {.pinDirection = kGPIO_DigitalInput, .outputLogic = 0};

    DEMO_LCD_I2C_Init();

    s_touchPending = false;

    gt911_config_t touchConfig = {.I2C_SendFunc     = DEMO_LCD_I2C_Send,
                                  .I2C_ReceiveFunc  = DEMO_LCD_I2C_Receive,
                                  .timeDelayMsFunc  = DEMO_TouchDelayMs,
                                  .intPinFunc       = DEMO_TouchConfigIntPin,
                                  .pullResetPinFunc = DEMO_TouchConfigResetPin,
                                  .touchPointNum    = 5,
                                  .i2cAddrMode      = kGT911_I2cAddrMode0,
                                  .intTrigMode      = kGT911_IntFallingEdge};

    GT911_Init(&touchHandle, &touchConfig);

    GPIO_PinInit(BOARD_LCD_INT_GPIO, BOARD_LCD_INT_PIN, &intPinConfig);
    GPIO_SetPinInterruptConfig(BOARD_LCD_INT_GPIO, BOARD_LCD_INT_PIN, kGPIO_InterruptFallingEdge);
    GPIO_PinClearInterruptFlag(BOARD_LCD_INT_GPIO, BOARD_LCD_INT_PIN);
    NVIC_ClearPendingIRQ(BOARD_LCD_INT_IRQn);
    EnableIRQ(BOARD_LCD_INT_IRQn);
}

/* Will be called by the library to read the touchpad */
static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data)
{
    touch_point_t tp[5];

    uint8_t tp_count = 5;

    if (!s_touchPending)
    {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    GT911_GetMultiTouch(&touchHandle, &tp_count, tp);

    /**
     * GT911 supports 5 points tracking, we only tracks ID #0.
     *
     */

    bool found_track = false;

    for (uint8_t i = 0; i < tp_count; i++)
    {
        /* Found track ID #0 */
        if (tp[i].touchID == 0)
        {
            data->state = LV_INDEV_STATE_PRESSED;

            switch (lcdHandle.orientationMode)
            {
                case kST7796S_Orientation0:
                    data->point.x = (int16_t)tp[i].x;
                    data->point.y = (int16_t)tp[i].y;
                    break;
                case kST7796S_Orientation90:
                    data->point.x = (int16_t)(touchHandle.resolutionY - tp[i].y);
                    data->point.y = (int16_t)tp[i].x;
                    break;
                case kST7796S_Orientation180:
                    data->point.x = (int16_t)(touchHandle.resolutionX - tp[i].x);
                    data->point.y = (int16_t)(touchHandle.resolutionY - tp[i].y);
                    break;
                case kST7796S_Orientation270:
                    data->point.x = (int16_t)tp[i].y;
                    data->point.y = (int16_t)(touchHandle.resolutionX - tp[i].x);
                    break;
                default:
                    break;
            }

            found_track = true;
            break;
        }
    }

    s_touchPending = false;

    /* No track #0 found... */
    if (!found_track)
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void BOARD_TouchIntHandler(void)
{
    s_touchPending = true;
}
