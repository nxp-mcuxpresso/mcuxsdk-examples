/*
 * Copyright 2026 NXP
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
#include "fsl_pcal6524.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Panel touch (GT911) wiring, from the board schematic (SCH/SPF-96037):
 *   - MIPI RK055MHD091A0 : GT911 on I2C0; reset + INT on the PCAL6524 expander
 *                          (CTP_RST_B / CTP_INT).
 *   - RGB  LCM_RGB_5INCH  : GT911 on I2C1; reset + INT on the PCAL6524 expander
 *                          (LCD_RST / LCD_TOUCH_INT).
 *   - DBI  LCD_PAR_S035   : GT911 on I2C1; reset + INT on MCU GPIO
 *                          (DBI_PANEL_LCD_RST / DBI_PANEL_INT).
 * The I2C bus itself is abstracted by BOARD_PanelTouch_I2C_* (see board.c). */
#if (DEMO_PANEL == DEMO_PANEL_RK055MHD091A0)
#define DEMO_TOUCH_VIA_EXPANDER 1
#define DEMO_TOUCH_RST_EXP_PIN  BOARD_PCAL6524_CTP_RST_B
#define DEMO_TOUCH_INT_EXP_PIN  BOARD_PCAL6524_CTP_INT
#elif (DEMO_PANEL == DEMO_PANEL_LCM_RGB_5INCH)
#define DEMO_TOUCH_VIA_EXPANDER 1
#define DEMO_TOUCH_RST_EXP_PIN  BOARD_PCAL6524_LCD_RST
#define DEMO_TOUCH_INT_EXP_PIN  BOARD_PCAL6524_LCD_TOUCH_INT
#elif (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
#define DEMO_TOUCH_VIA_EXPANDER 0
/* LCD_PAR_S035: the GT911 touch reset is shared with the panel LCD reset
 * (BOARD_DBI_PANEL_LCD_RST), which BOARD_PrepareDisplayController already pulses
 * during display init. The touch layer must NOT drive it again -- doing so
 * resets the ST7796S and blanks the panel. Only the touch INT line is used. */
#define DEMO_TOUCH_INT_GPIO     BOARD_DBI_PANEL_INT_GPIO
#define DEMO_TOUCH_INT_PIN      BOARD_DBI_PANEL_INT_PIN
#endif

#define DEMO_FB_SIZE (DEMO_BUFFER_STRIDE_BYTE * DEMO_BUFFER_HEIGHT)

#ifndef DEMO_BUFFER_FIXED_ADDRESS
#define DEMO_BUFFER_FIXED_ADDRESS 0
#endif

/* This support layer always double-buffers for LVGL, independent of the panel's
 * DC buffer count (DEMO_BUFFER_COUNT). */
#define DEMO_LVGL_BUFFER_COUNT 2

#if !DEMO_BUFFER_FIXED_ADDRESS
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_frameBuffer[DEMO_LVGL_BUFFER_COUNT][DEMO_BUFFER_HEIGHT][DEMO_BUFFER_WIDTH][DEMO_BUFFER_BYTE_PER_PIXEL],
    FRAME_BUFFER_ALIGN);
#define DEMO_BUFFER0_ADDR (uint32_t) s_frameBuffer[0]
#define DEMO_BUFFER1_ADDR (uint32_t) s_frameBuffer[1]
#endif

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

static void DEMO_TouchDelayMs(uint32_t ms)
{
    SDK_DelayAtLeastUs(ms * 1000U, SystemCoreClock);
}

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
    .timeDelayMsFunc  = DEMO_TouchDelayMs,
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
    fbInfo.strideBytes = DEMO_BUFFER_STRIDE_BYTE;
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

    lv_display_set_buffers_with_stride(disp_drv, (void *)s_frameBufferAddr[0],
        (void *)s_frameBufferAddr[1], DEMO_FB_SIZE,
        DEMO_BUFFER_STRIDE_BYTE, LV_DISPLAY_RENDER_MODE_FULL);

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
#if DEMO_TOUCH_VIA_EXPANDER
    pcal6524_handle_t *h = BOARD_GetPCAL6524Handle();
    const uint32_t mask  = 1U << DEMO_TOUCH_RST_EXP_PIN;
    if (pullUp)
    {
        (void)PCAL6524_SetPins(h, mask);
    }
    else
    {
        (void)PCAL6524_ClearPins(h, mask);
    }
#else
    /* DBI panel: touch reset shares the LCD reset (owned by display init); do
     * not drive it here or the ST7796S resets and the panel blanks. */
    (void)pullUp;
#endif
}

static void BOARD_ConfigPanelTouchIntPin(gt911_int_pin_mode_t mode)
{
    /* GT911 reset sequence drives INT to select the I2C address, then releases
     * it to input. On this board MIPI/RGB route INT through the PCAL6524
     * expander and DBI through an MCU GPIO. */
#if DEMO_TOUCH_VIA_EXPANDER
    pcal6524_handle_t *h = BOARD_GetPCAL6524Handle();
    const uint32_t mask  = 1U << DEMO_TOUCH_INT_EXP_PIN;
    if (mode == kGT911_IntPinInput)
    {
        (void)PCAL6524_SetDirection(h, mask, kPCAL6524_Input);
    }
    else
    {
        if (mode == kGT911_IntPinPullDown)
        {
            (void)PCAL6524_ClearPins(h, mask);
        }
        else
        {
            (void)PCAL6524_SetPins(h, mask);
        }
        (void)PCAL6524_SetDirection(h, mask, kPCAL6524_Output);
    }
#else
    if (mode == kGT911_IntPinInput)
    {
        DEMO_TOUCH_INT_GPIO->PDDR &= ~(1UL << DEMO_TOUCH_INT_PIN);
    }
    else
    {
        if (mode == kGT911_IntPinPullDown)
        {
            GPIO_PinWrite(DEMO_TOUCH_INT_GPIO, DEMO_TOUCH_INT_PIN, 0);
        }
        else
        {
            GPIO_PinWrite(DEMO_TOUCH_INT_GPIO, DEMO_TOUCH_INT_PIN, 1);
        }

        DEMO_TOUCH_INT_GPIO->PDDR |= (1UL << DEMO_TOUCH_INT_PIN);
    }
#endif
}

static void DEMO_InitTouch(void)
{
    status_t status;

#if DEMO_TOUCH_VIA_EXPANDER
    /* Reset is an expander output; INT direction is managed by the GT911 reset
     * sequence through BOARD_ConfigPanelTouchIntPin(). */
    (void)PCAL6524_SetDirection(BOARD_GetPCAL6524Handle(), 1U << DEMO_TOUCH_RST_EXP_PIN, kPCAL6524_Output);
#else
    /* DBI panel: configure only the touch INT line. The shared LCD/touch reset
     * is owned by BOARD_PrepareDisplayController and must not be driven here, or
     * the ST7796S resets and the panel blanks. */
    const gpio_pin_config_t intPinConfig = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic  = 0,
    };
    GPIO_PinInit(DEMO_TOUCH_INT_GPIO, DEMO_TOUCH_INT_PIN, &intPinConfig);
#endif

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
#if (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
    /* The LCD_PAR_S035 display is driven at ST7796S Orientation270 (the panel's
     * native 320x480 shown as 480x320 landscape), so rotate the GT911 touch
     * coordinates by 270 to match: swap the axes and flip X.
     * If a hardware axis turns out inverted, flip the affected term:
     *   x flip: (s_touchResolutionY - touch_y) instead of touch_y
     *   y flip: touch_x instead of (s_touchResolutionX - touch_x) */
    data->point.x = touch_y * DEMO_PANEL_WIDTH / s_touchResolutionY;
    data->point.y = (s_touchResolutionX - touch_x) * DEMO_PANEL_HEIGHT / s_touchResolutionX;
#else
    data->point.x = touch_x * DEMO_PANEL_WIDTH / s_touchResolutionX;
    data->point.y = touch_y * DEMO_PANEL_HEIGHT / s_touchResolutionY;
#endif
}
