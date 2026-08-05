/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "mpp_config.h"

#if defined(HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670) && (HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670 == 1)
#include "flexio_camera.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_gpio.h"
#include "camera_config.h"
#include "fsl_ov7670.h"
#include "fsl_inputmux.h"
#include "fsl_ezhv.h"
#include "ezhv_para.h"
#include "ezhv_support.h"
#include "fsl_flexio_camera.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define VSYNC_IRQ_HANDLER GPIO20_IRQHandler
#define DEMO_FLEXIO_CLOCK_FREQ CLOCK_GetFlexioClkFreq()
#define FLEXIO_MAX_FREQ (DEMO_FLEXIO_CLOCK_FREQ / 2U)
#define FLEXIO_MIN_FREQ (DEMO_FLEXIO_CLOCK_FREQ / 512U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void CAMERA_I2CInit(void);
static void CAMERA_XclkInit(uint32_t freq_Hz);
static void CAMERA_FlexioInit(void);
static void CAMERA_InterruptsInit(void);
static void CAMERA_ResetPinInit(void);
/*******************************************************************************
 * Variables
 *******************************************************************************/
/* camera buffers */
uint8_t s_buf[CAMERA_HEIGHT*CAMERA_STRIDE] __attribute__((section(".ezhv_camera"))) __attribute__((aligned(128)));
uint8_t s_buf1[CAMERA_HEIGHT*CAMERA_STRIDE] __attribute__((section(".ezhv_camera"))) __attribute__((aligned(128)));

volatile uint32_t g_newVideoFrame = 0;

volatile CameraDvpTransfer g_dvpTransfer = {
  .driverIdx = 0,
  .userIdx = 1,
  .queue[0] = {
    .len = CAMERA_HEIGHT * CAMERA_STRIDE,
    .pBuf = NULL,
  },
  .queue[1] = {
    .len = CAMERA_HEIGHT* CAMERA_STRIDE,
    .pBuf = NULL,
  },
};

volatile CameraBuffer_t *g_stCamBuf = NULL;

static ov7670_resource_t ov7670Resource = {
    .i2cSendFunc    = BOARD_CAMERA_I2C_Send,
    .i2cReceiveFunc = BOARD_CAMERA_I2C_Receive,
    .xclock         = kOV7670_InputClock12MHZ,
};

camera_device_handle_t cameraDevice = {
    .resource = &ov7670Resource,
    .ops      = &ov7670_ops,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
void CAMERA_Init(void)
{
    const camera_config_t cameraConfig = {
        .pixelFormat = kVIDEO_PixelFormatRGB565,
	    .bytesPerPixel = CAMERA_BPP,
	    .resolution = FSL_VIDEO_RESOLUTION(CAMERA_WIDTH,CAMERA_HEIGHT),
	    .interface = kCAMERA_InterfaceGatedClock,
	    .controlFlags = CAMERA_CTRL_FLAG,
	    .framePerSec = CAMERA_FRAME_RATE
    };

    g_dvpTransfer.queue[0].pBuf = (void *)s_buf;
    g_dvpTransfer.queue[1].pBuf = (void *)s_buf1;

    /* make sure flexio root clock freq >= 4 * XCLK freq */
    CLOCK_AttachClk(kMAIN_PLL_PFD3_to_FLEXIO);
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 6U);
    RESET_PeripheralReset(kFLEXIO0_RST_SHIFT_RSTn);

    CAMERA_ResetPinInit();
    CAMERA_I2CInit();
    CAMERA_FlexioInit();
    CAMERA_InterruptsInit();
    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);

    return;
}

/*!
 * brief Initialize Camera SCCB interface using board blocking I2C.
 * LPI2C8 is shared with the display TC358762 bridge (RPi 7inch panel).
 *
 * Both display_support.c::BOARD_InitLcdPanel() and this function call
 * BOARD_CAMERA_I2C_Init() which internally calls LPI2C_MasterInit().
 * A second LPI2C_MasterInit() resets the peripheral and causes
 * LPI2C_MasterTransferBlocking() to hang forever.
 *
 * The static bool ensures LPI2C8 is initialized exactly once:
 *   - Camera + Display: display calls BOARD_CAMERA_I2C_Init() first,
 *     sets the flag; camera path skips re-init.
 *   - Display only:     display initializes, flag set, camera never runs.
 *   - Camera only:      camera initializes, flag set.
 */
static void CAMERA_I2CInit(void)
{
    static bool s_i2cInitialized = false;
    if (!s_i2cInitialized)
    {
        BOARD_CAMERA_I2C_Init();
        s_i2cInitialized = true;
    }
}

static void CAMERA_ResetPinInit(void)
{
    gpio_pin_config_t outConfig = {kGPIO_DigitalOutput, 1};

    GPIO_PinInit(DEMO_RESET_PORT, DEMO_RESET_PIN, &outConfig);
}

/*!
 * brief Initialize flexio for cameraIf.
 *
 */
static void CAMERA_FlexioInit(void)
{
    /* Initialize */
    FLEXIO_CAMERA_Type base = {
		.flexioBase = FLEXIO,
		.timerIdx = DEMO_FLEXIO_PCLK_TIMER,
		.datPinStartIdx = DEMO_FLEXIO_DATA0_IDX,
		.hrefPinIdx =DEMO_FLEXIO_HREF_IDX,
		.pclkPinIdx = DEMO_FLEXIO_PCLK_IDX,
		.shifterStartIdx = DEMO_FLEXIO_SHIFTER0_IDX,
		.shifterCount = DEMO_FLEXIO_SHIFTER_NUM,
    };

    flexio_camera_config_t config;
    FLEXIO_CAMERA_GetDefaultConfig(&config);
    config.enablecamera = true;
    config.enableInDebug = true;
    FLEXIO_CAMERA_Init(&base, &config);
    CAMERA_XclkInit(DEMO_FLEXIO_XCLK_FREQ_HZ);

    /* flexio's shifter0 interrupt is used by the EZH-V. */
    FLEXIO_CAMERA_EnableInterrupt(&base);
}

/*!
 * brief Configure the XCLK frequency.
 *
 */
static void CAMERA_XclkInit(uint32_t freq_Hz)
{
    assert((freq_Hz < FLEXIO_MAX_FREQ) && (freq_Hz > FLEXIO_MIN_FREQ));

    uint32_t lowerValue = 0;
    uint32_t upperValue = 0;
    uint32_t sum        = 0;
    flexio_timer_config_t timerConfig;

    /* Configure the timer DEMO_FLEXIO_TIMER_CH for generating PWM */
    timerConfig.triggerSelect   = 0U; /* any value, ok */
    timerConfig.triggerSource   = kFLEXIO_TimerTriggerSourceInternal;
    timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
    timerConfig.pinConfig       = kFLEXIO_PinConfigOutput;
    timerConfig.pinPolarity     = kFLEXIO_PinActiveHigh;
    timerConfig.pinSelect       = DEMO_FLEXIO_XCLK_IDX; /* Set pwm output */
    timerConfig.timerMode       = kFLEXIO_TimerModeDual8BitPWM;
    timerConfig.timerOutput     = kFLEXIO_TimerOutputOneNotAffectedByReset;
    timerConfig.timerDecrement  = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    timerConfig.timerDisable    = kFLEXIO_TimerDisableNever;
    timerConfig.timerEnable     = kFLEXIO_TimerEnabledAlways;
    timerConfig.timerReset      = kFLEXIO_TimerResetNever;
    timerConfig.timerStart      = kFLEXIO_TimerStartBitDisabled;
    timerConfig.timerStop       = kFLEXIO_TimerStopBitDisabled;

    /* Calculate timer lower and upper values of TIMCMP */
    /* Calculate the nearest integer value for sum, 
       using formula round(x) = (2 * floor(x) + 1) / 2 */
    /* sum = DEMO_FLEXIO_CLOCK_FREQ / freq_H */
    sum = (DEMO_FLEXIO_CLOCK_FREQ * 2 / freq_Hz + 1) / 2;

    /* Calculate the nearest integer value for lowerValue,
       the high period of the pwm output */
    lowerValue = (sum >> 1) - 1;
    /* Calculate upper value, the low period of the pwm output */
    upperValue = (sum >> 1) - 1;

    timerConfig.timerCompare = ((upperValue << 8U) | (lowerValue));

    FLEXIO_SetTimerConfig(FLEXIO, DEMO_FLEXIO_XCLK_TIMER, &timerConfig);
}

static void EZHV_Callback(void *userData)
{
    g_newVideoFrame = 1;
    g_dvpTransfer.driverIdx = (g_dvpTransfer.driverIdx+1) % QUEUE_SIZE;
    g_stCamBuf->len = g_dvpTransfer.queue[g_dvpTransfer.driverIdx].len;
    g_stCamBuf->pBuf = g_dvpTransfer.queue[g_dvpTransfer.driverIdx].pBuf;
}

static void CAMERA_InterruptsInit(void)
{
    /* XSYNC gpio interrupt setting for EZH-V */
    GPIO_SetPinInterruptConfig(DEMO_XSYNC_PORT, DEMO_XSYNC_PIN, kGPIO_InterruptRisingEdge);
    GPIO_EnableInterruptControlNonSecure(GPIO2, 0x3);
    GPIO_EnablePinControlNonSecure(GPIO2, 1 << DEMO_XSYNC_PIN);
    GPIO_EnablePinControlNonPrivilege(GPIO2, 1 << DEMO_XSYNC_PIN);
    GPIO_EnableInterruptControlNonPrivilege(GPIO2, 0x3);

    /* flexio irq to ezhv's trigger */
    INPUTMUX_Init(INPUTMUX0);
    RESET_ClearPeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);    
    INPUTMUX_AttachSignal(INPUTMUX0, 0, kINPUTMUX_Gpio2IrqToEzhv);
    INPUTMUX_AttachSignal(INPUTMUX0, 1, kINPUTMUX_FlexioIrqToEzhv);
    INPUTMUX_Deinit(INPUTMUX0);

    /* eanble ezhv interrupt to ARM */
    EZHV_EnableEzhv2ArmIntChan(kEZHV_EzhvToArmIntChan0); 
    EnableIRQ(EZHV_IRQn);
    NVIC_SetPriority(EZHV_IRQn, 2);
    EZHV_SetCallback(EZHV_Callback, 0, NULL);
}
#endif /* defined(HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670) && (HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670 == 1) */
