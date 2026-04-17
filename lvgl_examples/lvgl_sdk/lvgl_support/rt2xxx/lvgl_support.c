/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lvgl_support.h"
#include "lvgl.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"
#include "fsl_dcif.h"
#include "clock_config.h"
#include "fsl_lpi2c.h"
#include "fsl_gt911.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_FB_STRIDE(x) (x * 2U) //RGB565

#define DEMO_FB_SIZE (DEMO_FB_STRIDE(DEMO_BUFFER_WIDTH) * DEMO_BUFFER_HEIGHT)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p);
static void DEMO_InitTouch(void);
static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data);
static void DEMO_BufferSwitchOffCallback(void *param, void *switchOffBuffer);
static void BOARD_PullPanelTouchResetPin(bool pullUp);
static void BOARD_ConfigPanelTouchIntPin(gt911_int_pin_mode_t mode);
static void DEMO_WaitBufferSwitchOff(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint32_t s_frameBufferAddr[2] = {DEMO_BUFFER0_ADDR, DEMO_BUFFER1_ADDR};

static volatile bool s_transferDone;

static gt911_handle_t s_touchHandle;

static const gt911_config_t s_touchConfig = {
    .I2C_SendFunc     = BOARD_PanelTouch_I2C_Send,
    .I2C_ReceiveFunc  = BOARD_PanelTouch_I2C_Receive,
    .pullResetPinFunc = BOARD_PullPanelTouchResetPin,
    .intPinFunc       = BOARD_ConfigPanelTouchIntPin,
    .timeDelayMsFunc  = VIDEO_DelayMs,
    .touchPointNum    = 1,
    .i2cAddrMode      = kGT911_I2cAddrMode0,
    .intTrigMode      = kGT911_IntRisingEdge,
};

static int s_touchResolutionX;
static int s_touchResolutionY;

/*******************************************************************************
 * Code
 ******************************************************************************/
void lv_port_disp_init(void)
{
    lv_display_t * disp_drv; /*Descriptor of a display driver*/

    status_t status;
    dc_fb_info_t fbInfo;

    /*-------------------------
     * Initialize your display
     * -----------------------*/
    BOARD_PrepareDisplayController();

    status = g_dc.ops->init(&g_dc);
    if (kStatus_Success != status)
    {
        assert(0);
    }

    g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &fbInfo);
    fbInfo.pixelFormat = DEMO_BUFFER_PIXEL_FORMAT;
    fbInfo.width       = DEMO_BUFFER_WIDTH;
    fbInfo.height      = DEMO_BUFFER_HEIGHT;
    fbInfo.startX      = DEMO_BUFFER_START_X;
    fbInfo.startY      = DEMO_BUFFER_START_Y;
    fbInfo.strideBytes = DEMO_FB_STRIDE(DEMO_BUFFER_WIDTH);
    g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo);

    g_dc.ops->setCallback(&g_dc, 0, DEMO_BufferSwitchOffCallback, NULL);

    s_transferDone = false;

    /* lvgl starts render in frame buffer 0, so show frame buffer 1 first. */
    g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)s_frameBufferAddr[1]);

    /* Wait for frame buffer sent to display controller video memory. */
    if ((g_dc.ops->getProperty(&g_dc) & kDC_FB_ReserveFrameBuffer) == 0)
    {
        DEMO_WaitBufferSwitchOff();
    }

    g_dc.ops->enableLayer(&g_dc, 0);

    disp_drv = lv_display_create(DEMO_BUFFER_WIDTH, DEMO_BUFFER_HEIGHT);

    memset((void *)s_frameBufferAddr[0], 0, DEMO_FB_SIZE);
    memset((void *)s_frameBufferAddr[1], 0, DEMO_FB_SIZE);

    lv_display_set_buffers_with_stride(disp_drv, (void *)s_frameBufferAddr[0], (void *)s_frameBufferAddr[1], DEMO_FB_SIZE, DEMO_FB_STRIDE(DEMO_BUFFER_WIDTH), LV_DISPLAY_RENDER_MODE_FULL);

    lv_display_set_flush_cb(disp_drv, DEMO_FlushDisplay);
}

static void DEMO_BufferSwitchOffCallback(void *param, void *switchOffBuffer)
{
    s_transferDone = true;
}

static void DEMO_WaitBufferSwitchOff(void)
{
    while (false == s_transferDone)
    {
    }
    s_transferDone = false;
}

static void DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p)
{
    g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)color_p);

    DEMO_WaitBufferSwitchOff();

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
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

static void BOARD_PullPanelTouchResetPin(bool pullUp)
{
    GPIO_PinWrite(BOARD_TOUCH_RST_GPIO, BOARD_TOUCH_RST_PIN, pullUp ? 1U : 0U);
}

static void BOARD_ConfigPanelTouchIntPin(gt911_int_pin_mode_t mode)
{
    if (mode == kGT911_IntPinInput)
    {
        BOARD_TOUCH_INT_GPIO->PDDR &= ~(1UL << BOARD_TOUCH_INT_PIN);
    }
    else
    {
        if (mode == kGT911_IntPinPullDown)
        {
            GPIO_PinWrite(BOARD_TOUCH_INT_GPIO, BOARD_TOUCH_INT_PIN, 0);
        }
        else
        {
            GPIO_PinWrite(BOARD_TOUCH_INT_GPIO, BOARD_TOUCH_INT_PIN, 1);
        }

        BOARD_TOUCH_INT_GPIO->PDDR |= (1UL << BOARD_TOUCH_INT_PIN);
    }
}

static void DEMO_InitTouch(void)
{
    status_t status;

    const gpio_pin_config_t resetPinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0,
    };

    GPIO_PinInit(BOARD_TOUCH_INT_GPIO, BOARD_TOUCH_INT_PIN, &resetPinConfig);
    GPIO_PinInit(BOARD_TOUCH_RST_GPIO, BOARD_TOUCH_RST_PIN, &resetPinConfig);

    BOARD_PanelTouch_I2C_Init();

    status = GT911_Init(&s_touchHandle, &s_touchConfig);

    if (kStatus_Success != status)
    {
        PRINTF("Touch IC initialization failed\r\n");
        assert(false);
    }

    GT911_GetResolution(&s_touchHandle, &s_touchResolutionX, &s_touchResolutionY);
}

/* Will be called by the library to read the touchpad */
static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data)
{
    static int touch_x = 0;
    static int touch_y = 0;

    if (kStatus_Success == GT911_GetSingleTouch(&s_touchHandle, &touch_x, &touch_y))
    {
        data->state = LV_INDEV_STATE_PR;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Set the last pressed coordinates*/
    data->point.x = touch_x * DEMO_PANEL_WIDTH / s_touchResolutionX;
    data->point.y = touch_y * DEMO_PANEL_HEIGHT / s_touchResolutionY;
}
