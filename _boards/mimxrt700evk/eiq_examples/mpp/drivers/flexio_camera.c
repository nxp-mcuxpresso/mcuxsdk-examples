/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_flexio.h"
#include "camera_config.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_camera_device.h"
#include "fsl_ov7670.h"
#include "fsl_camera.h"
#include "fsl_power.h"
#include "fsl_camera_i2c.h"
#include "fsl_inputmux.h"
#include "fsl_ezhv.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define VSYNC_IRQ_HANDLER GPIO20_IRQHandler

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void CameraIf_XclkInit(uint32_t freq_Hz);
static void CameraIf_DataPathInit(FLEXIO_CAMERA_Type *base);
static void CameraIf_SccbInit(void);
static void CameraIf_FlexioInit(void);
void CameraIf_InterruptsInit(void);
static void CameraIf_ResetPinInit(void);

/*******************************************************************************
 * Variables
 *******************************************************************************/
/* camera buffers */
#if (CAM_PSRAM == 1)
uint8_t *g_CamBufA = CAMERA_BUFFER_ADDR0;
uint8_t *g_CamBufB = CAMERA_BUFFER_ADDR1;
#else
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t g_CamBufA[CAMERA_HEIGHT*CAMERA_LINE_DATA_SIZE], 128);
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t g_CamBufB[CAMERA_HEIGHT*CAMERA_LINE_DATA_SIZE], 128);
#endif

volatile uint32_t g_ezhvIrqIdx = 0;
volatile uint32_t g_ezhvIrq = 0;

volatile FlexioDvpTransfer g_cameraQueue = {
//  .flag = 1,
  .driverIdx = 0,
  .userIdx = 1,
  .queue[0] = {
//    .valid = 0,
//    .lock = 0,
    .len = CAMERA_WIDTH*CAMERA_HEIGHT*CAMERA_BPP,
    .pBuf = (void*)0,
  },
  .queue[1] = {
//    .valid = 0,
//    .lock = 0,
    .len = CAMERA_WIDTH*CAMERA_HEIGHT*CAMERA_BPP,
    .pBuf = (void*)0,
  },
};

CameraBuffer_t *g_stCamBuf = NULL;
static ov7670_resource_t ov7670Resource = {
    .i2cSendFunc    = BOARD_Camera_I2C_Send,
    .i2cReceiveFunc = BOARD_Camera_I2C_Receive,
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
void flexio_camera_init(void)
{
    const camera_config_t cameraConfig = {
        .pixelFormat = kVIDEO_PixelFormatRGB565,
	.bytesPerPixel = CAMERA_BPP,
	.resolution = FSL_VIDEO_RESOLUTION(CAMERA_WIDTH,CAMERA_HEIGHT),
	.interface = kCAMERA_InterfaceGatedClock,
	.controlFlags = CAMERA_CONTROL_FLAGS,
	.framePerSec = CAMERA_FRAME_RATE,
    };
    
    g_cameraQueue.queue[0].pBuf = (void *)g_CamBufA;
    g_cameraQueue.queue[1].pBuf = (void *)g_CamBufB;

    /* Reset */
    CameraIf_ResetPinInit();
    /* Init the I2C to configure ov7670 */
    CameraIf_SccbInit();
    /* Init Flexio to receive data from ov7670 */
    CameraIf_FlexioInit();
    /* Init Interrupts */
    CameraIf_InterruptsInit();
    /* Configure the camera sensor OV7670 */
    status_t ov7670_status = kStatus_Success;
    do
    {
        ov7670_status = CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);
    } while(ov7670_status != kStatus_Success);


    return;
}

void CameraIf_ResetPinInit(void)
{
    gpio_pin_config_t outConfig = {kGPIO_DigitalOutput, 1};

    GPIO_PinInit(DEMO_RESET_PORT, DEMO_RESET_PIN, &outConfig);
    outConfig.outputLogic = 0;
    GPIO_PinInit(GPIO3, 1, &outConfig);
}

/*!
 * brief Initialize flexio for cameraIf.
 *
 */
void CameraIf_FlexioInit(void)
{
    flexio_config_t fxioUserConfig;

    /* Clock setting for FlexIO */
    CLOCK_AttachClk(kMAIN_PLL_PFD3_to_FLEXIO); //
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 5U);

    /* Init flexio, use default configure
     * Disable doze and fast access mode
     * Enable in debug mode
     */
    FLEXIO_GetDefaultConfig(&fxioUserConfig);
    FLEXIO_Init(FLEXIO, &fxioUserConfig);
    /* XCLK */
    CameraIf_XclkInit(DEMO_FLEXIO_PWM_FREQ_HZ);

    /* Initialize */
    FLEXIO_CAMERA_Type CameraBase = {
		.flexioBase = FLEXIO,
		.timerIdx = DEMO_FLEXIO_TIMER_PCLK,
		.datPinStartIdx = DEMO_FLEXIO_DATA_START_IDX,
		.hrefPinIdx =DEMO_FELXIO_HREF_IDX,
		.pclkPinIdx = DEMO_FELXIO_PCLK_IDX,
		.shifterStartIdx = DEMO_FLEXIO_SHIFTER_START_IDX,
		.shifterCount = DEMO_FLEXIO_SHIFTER_NUM,
    };
    CameraIf_DataPathInit(&CameraBase);

    /* The shifter0 interrupt is used by the SMARTDMA. */
    FLEXIO_EnableShifterStatusInterrupts(FLEXIO, (1UL << 0));
}

/*!
 * brief Initialize flexcomm4 for the SSCB interface.
 *
 */
void CameraIf_SccbInit(void)
{
    BOARD_Camera_I2C_Init(SCCB_BASE);
}

/*!
 * brief Configure the XCLK frequency.
 *
 */
void CameraIf_XclkInit(uint32_t freq_Hz)
{
    assert((freq_Hz < FLEXIO_MAX_FREQUENCY) && (freq_Hz > FLEXIO_MIN_FREQUENCY));

    uint32_t lowerValue = 0; /* Number of clock cycles in high logic state in one period */
    uint32_t upperValue = 0; /* Number of clock cycles in low logic state in one period */
    uint32_t sum        = 0; /* Number of clock cycles in one period */
    flexio_timer_config_t timerConfig;

    /* Configure the timer DEMO_FLEXIO_TIMER_CH for generating PWM */
    timerConfig.triggerSelect   = 0U; /* any value, ok */
    timerConfig.triggerSource   = kFLEXIO_TimerTriggerSourceInternal;
    timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
    timerConfig.pinConfig       = kFLEXIO_PinConfigOutput;
    timerConfig.pinPolarity     = kFLEXIO_PinActiveHigh;
    timerConfig.pinSelect       = DEMO_FLEXIO_PWM_TO_XCLK; /* Set pwm output */
    timerConfig.timerMode       = kFLEXIO_TimerModeDual8BitPWM;
    timerConfig.timerOutput     = kFLEXIO_TimerOutputOneNotAffectedByReset;
    timerConfig.timerDecrement  = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    timerConfig.timerDisable    = kFLEXIO_TimerDisableNever;
    timerConfig.timerEnable     = kFLEXIO_TimerEnabledAlways;
    timerConfig.timerReset      = kFLEXIO_TimerResetNever;
    timerConfig.timerStart      = kFLEXIO_TimerStartBitDisabled;
    timerConfig.timerStop       = kFLEXIO_TimerStopBitDisabled;

    /* Calculate timer lower and upper values of TIMCMP */
    /* Calculate the nearest integer value for sum, using formula round(x) = (2 * floor(x) + 1) / 2 */
    /* sum = DEMO_FLEXIO_CLOCK_FREQUENCY / freq_H */
    sum = (DEMO_FLEXIO_CLOCK_FREQUENCY * 2 / freq_Hz + 1) / 2;

    /* Calculate the nearest integer value for lowerValue, the high period of the pwm output */
    lowerValue = (sum >> 1) - 1;
    /* Calculate upper value, the low period of the pwm output */
    upperValue = (sum >> 1) - 1;

    timerConfig.timerCompare = ((upperValue << 8U) | (lowerValue));

    FLEXIO_SetTimerConfig(FLEXIO, DEMO_FLEXIO_TIMER_XCLK, &timerConfig);
}

/*!
 * brief Initialize the data path.
 *
 */
void CameraIf_DataPathInit(FLEXIO_CAMERA_Type *base)
{
    flexio_shifter_config_t shifterConfig;
    flexio_timer_config_t timerConfig;

    /* Clear the shifterConfig & timerConfig struct. */
    (void)memset(&shifterConfig, 0, sizeof(shifterConfig));
    (void)memset(&timerConfig, 0, sizeof(timerConfig));
	
    /* FLEXIO_CAMERA shifter config */
    shifterConfig.timerSelect = base->timerIdx;
    shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnPositive;
    shifterConfig.pinConfig = kFLEXIO_PinConfigOutputDisabled;
    shifterConfig.pinSelect = base->datPinStartIdx;
    shifterConfig.pinPolarity = kFLEXIO_PinActiveHigh;
    shifterConfig.shifterMode = kFLEXIO_ShifterModeReceive;
    shifterConfig.parallelWidth = DEMO_FLEXIO_CAMERA_DATA_WIDTH - 1U;
    shifterConfig.inputSource = kFLEXIO_ShifterInputFromNextShifterOutput;
    shifterConfig.shifterStop = kFLEXIO_ShifterStopBitDisable;
    shifterConfig.shifterStart = kFLEXIO_ShifterStartBitDisabledLoadDataOnEnable;
    /* Configure the shifters as FIFO buffer. */
    int i = 0;
    for (i = base->shifterStartIdx; i < (base->shifterStartIdx + base->shifterCount - 1U); i++)
    {
        FLEXIO_SetShifterConfig(base->flexioBase, i, &shifterConfig);
    }
    shifterConfig.inputSource = kFLEXIO_ShifterInputFromPin;
    FLEXIO_SetShifterConfig(base->flexioBase, i, &shifterConfig);

    /* FLEXIO_CAMERA timer config */
    timerConfig.triggerSelect = FLEXIO_TIMER_TRIGGER_SEL_PININPUT(base->hrefPinIdx);
    timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveHigh;
    timerConfig.triggerSource = kFLEXIO_TimerTriggerSourceInternal;
    timerConfig.pinConfig = kFLEXIO_PinConfigOutputDisabled;
    timerConfig.pinSelect = base->pclkPinIdx;
    timerConfig.pinPolarity = kFLEXIO_PinActiveHigh;
    timerConfig.timerMode = kFLEXIO_TimerModeSingle16Bit;
    timerConfig.timerOutput = kFLEXIO_TimerOutputZeroNotAffectedByReset;
    timerConfig.timerDecrement = kFLEXIO_TimerDecSrcOnPinInputShiftPinInput;
    timerConfig.timerReset = kFLEXIO_TimerResetOnTimerTriggerRisingEdge;
    timerConfig.timerDisable = kFLEXIO_TimerDisableOnTriggerFallingEdge;
    timerConfig.timerEnable = kFLEXIO_TimerEnableOnTriggerRisingEdge;
    timerConfig.timerStop = kFLEXIO_TimerStopBitDisabled;
    timerConfig.timerStart = kFLEXIO_TimerStartBitDisabled;
    timerConfig.timerCompare = 8U * base->shifterCount - 1U;
    FLEXIO_SetTimerConfig(base->flexioBase, base->timerIdx, &timerConfig);
    
    return;
}


void EZHV_Callback(void *param)
{
    g_ezhvIrqIdx = SYSCON4->EZHV2ARM_INT_CHAN;
    g_ezhvIrq++;

    if (g_ezhvIrqIdx)
    {
        SYSCON4->EZHV2ARM_INT_CHAN = g_ezhvIrqIdx;
        g_cameraQueue.driverIdx = (g_cameraQueue.driverIdx+1)%QUEUE_SIZE;
        g_stCamBuf->len = g_cameraQueue.queue[g_cameraQueue.driverIdx].len;
        g_stCamBuf->pBuf = g_cameraQueue.queue[g_cameraQueue.driverIdx].pBuf;
    }
}

void CameraIf_InterruptsInit(void)
{
    gpio_pin_config_t sw_config = {kGPIO_DigitalInput, 0};

    GPIO_PinInit(DEMO_XSYNC_PORT, DEMO_XSYNC_PIN, &sw_config);
    GPIO_SetPinInterruptConfig(DEMO_XSYNC_PORT, DEMO_XSYNC_PIN, kGPIO_InterruptRisingEdge);

    GPIO2->ICNS = 0x3;
    GPIO2->PCNS = 1 << DEMO_XSYNC_PIN;
    GPIO2->PCNP = 1 << DEMO_XSYNC_PIN;
    GPIO2->ICNP = 0x3;

    /* flexio irq to ezhv's trigger */
    INPUTMUX_Init(INPUTMUX0);
    RESET_ClearPeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);    
    INPUTMUX_AttachSignal(INPUTMUX0, 0, kINPUTMUX_Gpio2IrqToEzhv);
    INPUTMUX_AttachSignal(INPUTMUX0, 1, kINPUTMUX_FlexioIrqToEzhv);
    INPUTMUX_Deinit(INPUTMUX0);

    /* eanble ezhv interrupt to ARM */
    SYSCON4->EZHV2ARM_INT_EN = 0x1; 
    EnableIRQ(EZHV_IRQn);
    NVIC_SetPriority(EZHV_IRQn, 2);
    EZHV_InstallCallback(EZHV_Callback, NULL);
}
