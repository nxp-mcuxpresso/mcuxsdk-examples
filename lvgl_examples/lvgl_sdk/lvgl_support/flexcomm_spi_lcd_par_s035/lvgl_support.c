/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if defined(SDK_OS_FREE_RTOS)
#include "FreeRTOS.h"
#include "semphr.h"
#endif

#include "board.h"
#include "fsl_gpio.h"
#include "fsl_spi.h"
#include "fsl_spi_dma.h"
#include "fsl_i2c.h"
#include "fsl_dma.h"
#include "fsl_inputmux.h"
#include "fsl_dbi_spi_dma.h"
#include "fsl_st7796s.h"
#include "fsl_gt911.h"
#include "lvgl_support.h"
#include "lvgl_support_board.h"
#include "lvgl.h"

#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void     DEMO_InitLcd(void);
static status_t DEMO_InitLcdController(void);
static void     DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p);
static bool     DEMO_InitTouch(void);
static void     DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data);
static void     DEMO_DbiMemoryDoneCallback(status_t status, void *userData);

/* I2C callbacks for GT911. */
static status_t DEMO_TouchI2C_Send(uint8_t deviceAddress,
                                    uint32_t subAddress,
                                    uint8_t subAddressSize,
                                    const uint8_t *txBuff,
                                    uint8_t txBuffSize);
static status_t DEMO_TouchI2C_Receive(uint8_t deviceAddress,
                                       uint32_t subAddress,
                                       uint8_t subAddressSize,
                                       uint8_t *rxBuff,
                                       uint8_t rxBuffSize);
static void DEMO_TouchDelayMs(uint32_t delayMs);

/* DC pin function passed to DBI driver. */
static void DEMO_DbiPullDcPin(bool high);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static gt911_handle_t s_touchHandle;
static volatile bool  s_touchPending = false;

static st7796s_handle_t       s_lcdHandle;
static dbi_spi_dma_prv_data_t s_dbiSpiDmaPrvData;
static dbi_iface_t            s_dbiIface;

/* TX and RX DMA handles; rxHandle must be non-NULL for SPI_MasterTransferCreateHandleDMA. */
static dma_handle_t s_spiTxDmaHandle;
static dma_handle_t s_spiRxDmaHandle;

#if defined(SDK_OS_FREE_RTOS)
static SemaphoreHandle_t s_memWriteDone;
#else
static volatile bool s_memWriteDone;
#endif

SDK_ALIGN(static uint8_t s_frameBuffer[CONFIG_LVGL_SUPPORT_VDB_COUNT][LCD_VIRTUAL_BUF_SIZE * LCD_FB_BYTE_PER_PIXEL], 4);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void DEMO_DbiPullDcPin(bool high)
{
    GPIO_PinWrite(BOARD_LCD_DC_GPIO, BOARD_LCD_DC_PORT, BOARD_LCD_DC_PIN, high ? 1U : 0U);
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

static status_t DEMO_TouchI2C_Send(uint8_t deviceAddress,
                                    uint32_t subAddress,
                                    uint8_t subAddressSize,
                                    const uint8_t *txBuff,
                                    uint8_t txBuffSize)
{
    i2c_master_transfer_t xfer;

    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = (uint8_t *)(uintptr_t)txBuff;
    xfer.dataSize       = txBuffSize;
    xfer.flags          = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(BOARD_TOUCH_I2C, &xfer);
}

static status_t DEMO_TouchI2C_Receive(uint8_t deviceAddress,
                                       uint32_t subAddress,
                                       uint8_t subAddressSize,
                                       uint8_t *rxBuff,
                                       uint8_t rxBuffSize)
{
    i2c_master_transfer_t xfer;

    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kI2C_Read;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = rxBuff;
    xfer.dataSize       = rxBuffSize;
    xfer.flags          = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(BOARD_TOUCH_I2C, &xfer);
}

static void DEMO_TouchDelayMs(uint32_t delayMs)
{
#if defined(SDK_OS_FREE_RTOS)
    vTaskDelay(pdMS_TO_TICKS(delayMs));
#else
    SDK_DelayAtLeastUs(delayMs * 1000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#endif
}

static status_t DEMO_InitLcdController(void)
{
    status_t status;
    spi_master_config_t spiConfig;
    i2c_master_config_t i2cConfig;

    /* SPI initialisation for display: CPOL1/CPHA1 (mode 3), 8-bit. */
    SPI_MasterGetDefaultConfig(&spiConfig);
    spiConfig.polarity     = kSPI_ClockPolarityActiveLow;
    spiConfig.phase        = kSPI_ClockPhaseSecondEdge;
    spiConfig.baudRate_Bps = BOARD_LCD_SPI_BAUDRATE;
    spiConfig.sselNum      = kSPI_Ssel0;

    status = SPI_MasterInit(BOARD_LCD_SPI, &spiConfig, BOARD_LCD_SPI_CLOCK_FREQ);
    if (kStatus_Success != status)
    {
        return status;
    }

    /* Connect SPI TX and RX DMA requests via INPUTMUX. */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, BOARD_LCD_SPI_TX_DMA_CH, BOARD_LCD_SPI_TX_INPUTMUX_SIG);
    INPUTMUX_AttachSignal(INPUTMUX, BOARD_LCD_SPI_RX_DMA_CH, BOARD_LCD_SPI_RX_INPUTMUX_SIG);
    INPUTMUX_Deinit(INPUTMUX);

    /* DC pin. */
    {
        const gpio_pin_config_t dcPinCfg = {.pinDirection = kGPIO_DigitalOutput, .outputLogic = 0U};
        GPIO_PinInit(BOARD_LCD_DC_GPIO, BOARD_LCD_DC_PORT, BOARD_LCD_DC_PIN, &dcPinCfg);
    }

    /* RST pin (shared between display and touch controller). */
    {
        const gpio_pin_config_t rstPinCfg = {.pinDirection = kGPIO_DigitalOutput, .outputLogic = 1U};
        GPIO_PinInit(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PORT, BOARD_LCD_RST_PIN, &rstPinCfg);
    }

    /*
     * DMA: both TX and RX handles required.
     * SPI_MasterTransferCreateHandleDMA asserts if rxDmaHandle is NULL.
     * Actually rxDmaHandle is not used, this case won't read data from panel.
     */
    DMA_CreateHandle(&s_spiTxDmaHandle, BOARD_LCD_DMA, BOARD_LCD_SPI_TX_DMA_CH);
    DMA_CreateHandle(&s_spiRxDmaHandle, BOARD_LCD_DMA, BOARD_LCD_SPI_RX_DMA_CH);

    const dbi_spi_dma_config_t dbiConfig = {
        .spi         = BOARD_LCD_SPI,
        .txDmaHandle = &s_spiTxDmaHandle,
        .rxDmaHandle = &s_spiRxDmaHandle,
        .dcPinFunc   = DEMO_DbiPullDcPin,
        .dataWidth   = kSPI_Data8Bits,
    };

    status = DBI_SPI_DMA_CreateHandle(&s_dbiIface, &dbiConfig, &s_dbiSpiDmaPrvData);
    if (kStatus_Success != status)
    {
        return status;
    }

    /* Hardware reset for display and touch controller. */
    GPIO_PinWrite(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PORT, BOARD_LCD_RST_PIN, 0U);
    SDK_DelayAtLeastUs(1000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    GPIO_PinWrite(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PORT, BOARD_LCD_RST_PIN, 1U);
    SDK_DelayAtLeastUs(5000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* ST7796S display controller init: landscape 480x320, RGB565. */
    const st7796s_config_t st7796sCfg = {
        .driverPreset    = kST7796S_DriverPresetLCDPARS035,
        .pixelFormat     = kST7796S_PixelFormatRGB565,
        .orientationMode = kST7796S_Orientation270,
        .teConfig        = kST7796S_TEDisabled,
        .invertDisplay   = true,
        .flipDisplay     = true,
        .bgrFilter       = true,
    };

    status = ST7796S_Init(&s_lcdHandle, &st7796sCfg, &s_dbiIface);
    if (kStatus_Success != status)
    {
        return status;
    }

    DBI_IFACE_SetMemoryDoneCallback(s_lcdHandle.dbiIface, DEMO_DbiMemoryDoneCallback, NULL);
    ST7796S_EnableDisplay(&s_lcdHandle, true);

    /* I2C initialisation for touch controller. */
    I2C_MasterGetDefaultConfig(&i2cConfig);
    i2cConfig.baudRate_Bps = BOARD_TOUCH_I2C_BAUDRATE;
    I2C_MasterInit(BOARD_TOUCH_I2C, &i2cConfig, BOARD_TOUCH_I2C_CLOCK_FREQ);

    return kStatus_Success;
}

static void DEMO_InitLcd(void)
{
#if defined(SDK_OS_FREE_RTOS)
    s_memWriteDone = xSemaphoreCreateBinary();
    if (NULL == s_memWriteDone)
    {
        assert(0);
    }
#else
    s_memWriteDone = false;
#endif

    if (kStatus_Success != DEMO_InitLcdController())
    {
        assert(0);
    }
}

static void DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p)
{
    lv_coord_t x1 = area->x1;
    lv_coord_t y1 = area->y1;
    lv_coord_t x2 = area->x2;
    lv_coord_t y2 = area->y2;

    int32_t pixelCount = (int32_t)(x2 - x1 + 1) * (int32_t)(y2 - y1 + 1);

    ST7796S_SelectArea(&s_lcdHandle, x1, y1, x2, y2);

    /* Swap the 2 bytes of RGB565 color */
    lv_draw_sw_rgb565_swap(color_p, pixelCount * sizeof(uint16_t));

#if !defined(SDK_OS_FREE_RTOS)
    s_memWriteDone = false;
#endif

    ST7796S_WritePixels(&s_lcdHandle, (uint16_t *)color_p, (uint32_t)pixelCount);

#if defined(SDK_OS_FREE_RTOS)
    if (xSemaphoreTake(s_memWriteDone, portMAX_DELAY) != pdTRUE)
    {
        assert(0);
    }
#else
    while (!s_memWriteDone)
    {
    }
#endif

    lv_disp_flush_ready(disp_drv);
}

void lv_port_disp_init(void)
{
    memset(s_frameBuffer, 0, sizeof(s_frameBuffer));

    DEMO_InitLcd();

    lv_display_t *disp = lv_display_create(LCD_WIDTH, LCD_HEIGHT);

#if (CONFIG_LVGL_SUPPORT_VDB_COUNT == 2)
    lv_display_set_buffers(disp, (void *)s_frameBuffer[0], (void *)s_frameBuffer[1],
                           LCD_VIRTUAL_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
#else
    lv_display_set_buffers(disp, (void *)s_frameBuffer[0], NULL,
                           LCD_VIRTUAL_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif
    lv_display_set_flush_cb(disp, DEMO_FlushDisplay);
}

/* Called from the board touch GPIO interrupt handler. */
void BOARD_TouchIntHandler(void)
{
    s_touchPending = true;
}

static bool DEMO_InitTouch(void)
{
    const gpio_pin_config_t intPinCfg = {.pinDirection = kGPIO_DigitalInput, .outputLogic = 0U};

    s_touchPending = false;

    /* Don't reset again, LCD IC and Touch IC share the same reset pin,
     * which has already been reset in DEMO_InitLcdController().
     */

    GPIO_PinInit(BOARD_LCD_INT_GPIO, BOARD_LCD_INT_PORT, BOARD_LCD_INT_PIN, &intPinCfg);
    /* Board header provides BOARD_LCD_ENABLE_TOUCH_INT() for platform-specific
     * interrupt routing (e.g. INPUTMUX+PINT on LPC, GPIO IRQ on MCX). */
    BOARD_LCD_ENABLE_TOUCH_INT();

    gt911_config_t touchCfg = {
        .I2C_SendFunc     = DEMO_TouchI2C_Send,
        .I2C_ReceiveFunc  = DEMO_TouchI2C_Receive,
        .timeDelayMsFunc  = DEMO_TouchDelayMs,
        .intPinFunc       = NULL,
        .pullResetPinFunc = NULL,
        .touchPointNum    = 1U,
        .i2cAddrMode      = kGT911_I2cAddrAny,
        .intTrigMode      = kGT911_IntFallingEdge,
    };

    status_t ts = GT911_Init(&s_touchHandle, &touchCfg);
    if (kStatus_Success != ts)
    {
        return false;
    }

    return true;
}

static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data)
{
    touch_point_t tp[1];
    uint8_t       tp_count = 1U;

    if (!s_touchPending)
    {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }
    PRINTF("Touch pending\r\n");

    GT911_GetMultiTouch(&s_touchHandle, &tp_count, tp);

    s_touchPending = false;

    if ((tp_count == 0U) || !tp[0].valid)
    {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    data->state   = LV_INDEV_STATE_PRESSED;

    /* Orientation270 coordinate mapping. */
    data->point.x = (int16_t)tp[0].y;
    data->point.y = (int16_t)(s_touchHandle.resolutionX - (int32_t)tp[0].x);
}

void lv_port_indev_init(void)
{
    if (DEMO_InitTouch())
    {
        lv_indev_t *indev = lv_indev_create();
        lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
        lv_indev_set_read_cb(indev, DEMO_ReadTouch);
    }
}
