/*
 * Copyright 2019-2021, 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lvgl_support.h"
#include "lvgl.h"
#if defined(SDK_OS_FREE_RTOS)

#include "FreeRTOS.h"
#include "semphr.h"

#endif

#include "board.h"
#include "fsl_video_common.h"
#include "fsl_lcdc.h"
#include "fsl_gpio.h"
#include "fsl_i2c.h"
#if (LVGL_PANEL == DEMO_PANEL_RK043FN66HS)
#include "fsl_gt911.h"
#else
#include "fsl_ft5406.h"
#endif
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Port Me. Start */
#define TOUCH_I2C                 I2C2
#define TOUCH_I2C_CLOCK_FREQUENCY CLOCK_GetFlexCommClkFreq(2)
#define TOUCH_I2C_SLAVE_ADDR_7BIT 0x7EU
#define TOUCH_I2C_BAUDRATE        100000U
/* Port Me. End */

#define LCD_PANEL_CLK 9000000
#if (LVGL_PANEL == DEMO_PANEL_RK043FN66HS)

#define LCD_HSW 4
#define LCD_HFP 8
#define LCD_HBP 43
#define LCD_VSW 4
#define LCD_VFP 8
#define LCD_VBP 12

#else

#define LCD_HSW 2
#define LCD_HFP 8
#define LCD_HBP 43
#define LCD_VSW 10
#define LCD_VFP 4
#define LCD_VBP 12
#endif
#define LCD_POL_FLAGS      (kLCDC_InvertVsyncPolarity | kLCDC_InvertHsyncPolarity)
#define LCD_INPUT_CLK_FREQ CLOCK_GetLcdClkFreq()

#if (LV_COLOR_DEPTH == 16)
#define LCD_FB_BYTE_PER_PIXEL 2
#define LCDC_BPP              kLCDC_16BPP565
#elif ((LV_COLOR_DEPTH == 8) || (LV_COLOR_DEPTH == 1))
#define LCD_FB_BYTE_PER_PIXEL 1
#define LCDC_BPP              kLCDC_8BPP
#else
#error Color Depth not supported
#endif

#define DEMO_BUFFER0_ADDR 0xa0000000
#define DEMO_BUFFER1_ADDR (DEMO_BUFFER0_ADDR + LCD_WIDTH * LCD_HEIGHT * LCD_FB_BYTE_PER_PIXEL)

#if (LV_COLOR_DEPTH == 8)
typedef union
{
    struct
    {
      uint8_t B : 2;
      uint8_t G : 3;
      uint8_t R : 3;
      uint8_t : 1;
    } ch;
    uint8_t color;
} color_t;
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_InitLcd(void);

static void DEMO_InitLcdClock(void);

static void DEMO_InitLcdBackLight(void);

static void DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p);

static void DEMO_InitTouch(void);

static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data);

#if ((LV_COLOR_DEPTH == 8) || (LV_COLOR_DEPTH == 1))
/*
 * To support 8 color depth and 1 color depth with this board, color palette is
 * used to map 256 color to 2^16 color.
 */
static void DEMO_SetLcdColorPalette(void);
#endif

#if (LVGL_PANEL == DEMO_PANEL_RK043FN66HS)
static void BOARD_PullTouchResetPin(bool pullUp);

static void BOARD_ConfigTouchIntPin(gt911_int_pin_mode_t mode);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool s_framePending;
#if defined(SDK_OS_FREE_RTOS)
static SemaphoreHandle_t s_frameSema;
#endif

#if (LVGL_PANEL == DEMO_PANEL_RK043FN66HS)
static gt911_handle_t s_touchHandle;
static const gt911_config_t s_touchConfig = {
    .I2C_SendFunc     = BOARD_Touch_I2C_Send,
    .I2C_ReceiveFunc  = BOARD_Touch_I2C_Receive,
    .pullResetPinFunc = BOARD_PullTouchResetPin,
    .intPinFunc       = BOARD_ConfigTouchIntPin,
    .timeDelayMsFunc  = VIDEO_DelayMs,
    .touchPointNum    = 1,
    .i2cAddrMode      = kGT911_I2cAddrMode0,
    .intTrigMode      = kGT911_IntRisingEdge,
};
static int s_touchResolutionX;
static int s_touchResolutionY;
#else
static ft5406_handle_t touchHandle;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

void lv_port_disp_init(void)
{
    lv_display_t * disp_drv; /*Descriptor of a display driver*/

    memset((void *)DEMO_BUFFER0_ADDR, 0, LCD_WIDTH * LCD_HEIGHT * LCD_FB_BYTE_PER_PIXEL);
    memset((void *)DEMO_BUFFER1_ADDR, 0, LCD_WIDTH * LCD_HEIGHT * LCD_FB_BYTE_PER_PIXEL);

    /*-------------------------
     * Initialize your display
     * -----------------------*/
    DEMO_InitLcd();

    disp_drv = lv_display_create(LCD_WIDTH, LCD_HEIGHT);

    lv_display_set_buffers(disp_drv, (void*)DEMO_BUFFER0_ADDR, (void*)DEMO_BUFFER1_ADDR, LCD_WIDTH * LCD_HEIGHT, LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_flush_cb(disp_drv, DEMO_FlushDisplay);
}

void LCD_IRQHandler(void)
{
#if defined(SDK_OS_FREE_RTOS)
    BaseType_t taskAwake = pdFALSE;
#endif

    uint32_t intStatus = LCDC_GetEnabledInterruptsPendingStatus(LCD);

    LCDC_ClearInterruptsStatus(LCD, intStatus);

    if (s_framePending)
    {
        if (intStatus & kLCDC_BaseAddrUpdateInterrupt)
        {
            s_framePending = false;

#if defined(SDK_OS_FREE_RTOS)
            xSemaphoreGiveFromISR(s_frameSema, &taskAwake);

            portYIELD_FROM_ISR(taskAwake);
#endif
        }
    }

    __DSB();
}

static void DEMO_InitLcdClock(void)
{
    /* Route Main clock to LCD. */
    CLOCK_AttachClk(kMAIN_CLK_to_LCD_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivLcdClk, 1, true);
}

static void DEMO_InitLcdBackLight(void)
{
    const gpio_pin_config_t config = {
        kGPIO_DigitalOutput,
        1,
    };

    /* LCD back light. */
    GPIO_PinInit(GPIO, BOARD_LCD_BL_GPIO, BOARD_LCD_BL_PIN, &config);
}

#if ((LV_COLOR_DEPTH == 8) || (LV_COLOR_DEPTH == 1))
static void DEMO_SetLcdColorPalette(void)
{
    /*
     * To support 8 color depth and 1 color depth with this board, color palette is
     * used to map 256 color to 2^16 color.
     *
     * LVGL 1-bit color depth still uses 8-bit per pixel, so the palette size is the
     * same with 8-bit color depth.
     * Each color palette item is 16-bit.
     *
     * Color palette format:
     *
     * RGB format: (lcdConfig.swapRedBlue = false)
     * Bit(s)   Name     Description
     * 4:0      R[4:0]   Red palette data B[4:0] Blue palette data
     * 9:5      G[4:0]   Green palette data G[4:0] Green palette data
     * 14:10    B[4:0]   Blue palette data R[4:0] Red palette data
     * 15       I        Intensity / unused I Intensity / unused
     * 20:16    R[4:0]   Red palette data B[4:0] Blue palette data
     * 25:21    G[4:0]   Green palette data G[4:0] Green palette data
     * 30:26    B[4:0]   Blue palette data R[4:0] Red palette data
     * 31       I        Intensity / unused I Intensity / unused
     *
     * BGR format: (lcdConfig.swapRedBlue = true)
     * Bit(s)   Name     Description
     * 4:0      B[4:0]   Blue palette data
     * 9:5      G[4:0]   Green palette data
     * 14:10    R[4:0]   Red palette data
     * 15       I        Intensity / unused
     * 20:16    B[4:0]   Blue palette data
     * 25:21    G[4:0]   Green palette data
     * 30:26    R[4:0]   Red palette data
     * 31       I        Intensity / unused
     */
    uint32_t palette[256U / 2U];

#if (LV_COLOR_DEPTH == 8)
    color_t color;
    color.color = 0U;

    for (int i = 0; i < ARRAY_SIZE(palette); i++)
    {
        palette[i] =
            ((uint32_t)color.ch.B << 3U) | ((uint32_t)color.ch.G << 7U) | ((uint32_t)color.ch.R << 12U);
        color.color++;
        palette[i] |=
            (((uint32_t)color.ch.B << 3U) | ((uint32_t)color.ch.G << 7U) | ((uint32_t)color.ch.R << 12U))
            << 16U;
        color.color++;
    }
#elif (LV_COLOR_DEPTH == 1)
    for (int i = 0; i < ARRAY_SIZE(palette); i++)
    {
        /*
         * Pixel map:
         * 0bXXXXXXX1 -> 0x7FFF
         * 0bXXXXXXX0 -> 0x0000
         */
        palette[i] = 0x7FFF0000;
    }
#endif

    LCDC_SetPalette(LCD, palette, sizeof(palette) / 4U);
}
#endif

static void DEMO_InitLcd(void)
{
    /* Initialize the display. */
    lcdc_config_t lcdConfig;

#if defined(SDK_OS_FREE_RTOS)
    s_frameSema = xSemaphoreCreateBinary();
    if (NULL == s_frameSema)
    {
        PRINTF("Frame semaphore create failed\r\n");
        assert(0);
    }
#endif

    /* No frame pending. */
    s_framePending = false;
#if defined(SDK_OS_FREE_RTOS)
    NVIC_SetPriority(LCD_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
#endif

    DEMO_InitLcdClock();

    LCDC_GetDefaultConfig(&lcdConfig);

    lcdConfig.panelClock_Hz = LCD_PANEL_CLK;
    lcdConfig.ppl           = LCD_WIDTH;
    lcdConfig.hsw           = LCD_HSW;
    lcdConfig.hfp           = LCD_HFP;
    lcdConfig.hbp           = LCD_HBP;
    lcdConfig.lpp           = LCD_HEIGHT;
    lcdConfig.vsw           = LCD_VSW;
    lcdConfig.vfp           = LCD_VFP;
    lcdConfig.vbp           = LCD_VBP;
    lcdConfig.polarityFlags = LCD_POL_FLAGS;
    /* lvgl starts render in frame buffer 0, so show frame buffer 1 first. */
    lcdConfig.upperPanelAddr = DEMO_BUFFER1_ADDR;
    lcdConfig.bpp            = LCDC_BPP;
    lcdConfig.display        = kLCDC_DisplayTFT;
    lcdConfig.swapRedBlue    = true;

    LCDC_Init(LCD, &lcdConfig, LCD_INPUT_CLK_FREQ);

#if ((LV_COLOR_DEPTH == 8) || (LV_COLOR_DEPTH == 1))
    DEMO_SetLcdColorPalette();
#endif

    /* Trigger interrupt at start of every vertical back porch. */
    LCDC_EnableInterrupts(LCD, kLCDC_BaseAddrUpdateInterrupt);
    NVIC_EnableIRQ(LCD_IRQn);

    LCDC_Start(LCD);
    LCDC_PowerUp(LCD);

    /* Back light. */
    DEMO_InitLcdBackLight();
}

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_flush_ready()' has to be called when finished
 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h*/
static void DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p)
{
    LCDC_SetPanelAddr(LCD, kLCDC_UpperPanel, (uint32_t)color_p);

    s_framePending = true;

#if defined(SDK_OS_FREE_RTOS)
    if (xSemaphoreTake(s_frameSema, portMAX_DELAY) != pdTRUE)
    {
        PRINTF("Wait semaphore error: s_frameSema\r\n");
        assert(0);
    }
#else
    while (s_framePending)
    {
    }
#endif

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

#if (LVGL_PANEL == DEMO_PANEL_RK043FN66HS)
static void BOARD_PullTouchResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_TOUCH_RST_GPIO, BOARD_TOUCH_RST_PORT, BOARD_TOUCH_RST_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(BOARD_TOUCH_RST_GPIO, BOARD_TOUCH_RST_PORT, BOARD_TOUCH_RST_PIN, 0);
    }
}

static void BOARD_ConfigTouchIntPin(gt911_int_pin_mode_t mode)
{
    if (mode == kGT911_IntPinInput)
    {
        BOARD_TOUCH_INT_GPIO->DIR[BOARD_TOUCH_INT_PORT] &= ~(1UL << BOARD_TOUCH_INT_PIN);
    }
    else
    {
        if (mode == kGT911_IntPinPullDown)
        {
            GPIO_PinWrite(BOARD_TOUCH_INT_GPIO, BOARD_TOUCH_INT_PORT, BOARD_TOUCH_INT_PIN, 0);
        }
        else
        {
            GPIO_PinWrite(BOARD_TOUCH_INT_GPIO, BOARD_TOUCH_INT_PORT, BOARD_TOUCH_INT_PIN, 1);
        }

        BOARD_TOUCH_INT_GPIO->DIR[BOARD_TOUCH_INT_PORT] |= (1UL << BOARD_TOUCH_INT_PIN);
    }
}

/*Initialize your touchpad*/
static void DEMO_InitTouch(void)
{
    status_t status;

    const gpio_pin_config_t resetPinConfig = {.pinDirection = kGPIO_DigitalOutput, .outputLogic = 0};
    GPIO_PinInit(BOARD_TOUCH_INT_GPIO, BOARD_TOUCH_INT_PORT, BOARD_TOUCH_INT_PIN, &resetPinConfig);
    GPIO_PinInit(BOARD_TOUCH_RST_GPIO, BOARD_TOUCH_RST_PORT, BOARD_TOUCH_RST_PIN, &resetPinConfig);

    /* attach 12 MHz clock to FLEXCOMM2 (I2C master for touch controller) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    BOARD_I2C_Init(TOUCH_I2C, TOUCH_I2C_CLOCK_FREQUENCY);

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
    data->point.x = touch_x * LCD_WIDTH / s_touchResolutionX;
    data->point.y = touch_y * LCD_HEIGHT / s_touchResolutionY;
}
#else
/*Initialize your touchpad*/
static void DEMO_InitTouch(void)
{
    status_t status;
    i2c_master_config_t masterConfig;
    gpio_pin_config_t pin_config = {
        kGPIO_DigitalOutput,
        0,
    };

    CLOCK_EnableClock(kCLOCK_Gpio2);

    /* Clock setting for I2C */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    I2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Bps = TOUCH_I2C_BAUDRATE;

    /* Initialize the I2C master peripheral */
    I2C_MasterInit(TOUCH_I2C, &masterConfig, TOUCH_I2C_CLOCK_FREQUENCY);

    /* Enable touch panel controller */
    GPIO_PinInit(GPIO, BOARD_TOUCH_PANEL_RST_GPIO, BOARD_TOUCH_PANEL_RST_PIN, &pin_config);
    GPIO_PinWrite(GPIO, BOARD_TOUCH_PANEL_RST_GPIO, BOARD_TOUCH_PANEL_RST_PIN, 1);

    /* Initialize touch panel controller */
    status = FT5406_Init(&touchHandle, TOUCH_I2C);
    if (status != kStatus_Success)
    {
        PRINTF("Touch panel init failed\n");
        assert(0);
    }
}

/* Will be called by the library to read the touchpad */
static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data)
{
    touch_event_t touch_event;
    static int touch_x = 0;
    static int touch_y = 0;

    data->state = LV_INDEV_STATE_REL;

    if (kStatus_Success == FT5406_GetSingleTouch(&touchHandle, &touch_event, &touch_x, &touch_y))
    {
        if ((touch_event == kTouch_Down) || (touch_event == kTouch_Contact))
        {
            data->state = LV_INDEV_STATE_PR;
        }
    }

    /*Set the last pressed coordinates*/
    data->point.x = touch_y;
    data->point.y = touch_x;
}
#endif
