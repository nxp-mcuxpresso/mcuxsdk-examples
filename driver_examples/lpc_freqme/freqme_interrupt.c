/*
 * Copyright 2021, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_freqme.h"
#include "fsl_inputmux.h"

#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_CLOCK_SOURCE_COUNT 3U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* Measure target clock's frequency. */
static void DEMO_DoFreqMeasurement(void);
/* Measure reference clock's pulse. */
static void DEMO_DoPulseMeasurement(void);
/* Select clock source. */
static void DEMO_GetClockSelection(freq_measure_config_t *config);
/* Get pulse polarity from user input. */
static void DEMO_GetPulsePolarity(freq_measure_config_t *config);
/* Get reference clock scale factor from user input. */
static void DEMO_GetReferenceClockScaleFactor(freq_measure_config_t *config);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static const char *g_freqClockNameArray[DEMO_CLOCK_SOURCE_COUNT] = DEMO_FREQ_TAR_CLOCK_SOURCE_NAME;
static const char *g_pulseClockNameArray[DEMO_CLOCK_SOURCE_COUNT] = DEMO_PULSE_TAR_CLOCK_SOURCE_NAME;

static const inputmux_connection_t g_freqClockSourceSignalArray[DEMO_CLOCK_SOURCE_COUNT] =
    DEMO_FREQ_TAR_CLOCK_SOURCE_SIGNAL;
static const inputmux_connection_t g_pulseClockSourceSignalArray[DEMO_CLOCK_SOURCE_COUNT] =
    DEMO_PULSE_TAR_CLOCK_SOURCE_SIGNAL;

volatile bool g_measurementCompleted = false;
volatile bool g_errorOccurred        = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

void FREQME_IRQHANDLER(void)
{
    if (FREQME_GetInterruptStatusFlags(DEMO_FREQME) &
        (kFREQME_UnderflowInterruptStatusFlag | kFREQME_OverflowInterruptStatusFlag))
    {
        FREQME_ClearInterruptStatusFlags(DEMO_FREQME,
                                         (kFREQME_UnderflowInterruptStatusFlag | kFREQME_OverflowInterruptStatusFlag));
        g_errorOccurred        = true;
        g_measurementCompleted = true;
        return;
    }
    if ((FREQME_GetInterruptStatusFlags(DEMO_FREQME) & kFREQME_ReadyInterruptStatusFlag) != 0UL)
    {
        FREQME_ClearInterruptStatusFlags(DEMO_FREQME, kFREQME_ReadyInterruptStatusFlag);
        g_measurementCompleted = true;
    }
}

int main(void)
{
    char inputCh;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    INPUTMUX_Init(INPUTMUX);
    PRINTF("FREQME Interrupt Example!\r\n");

    EnableIRQ(Freqme_IRQn);
    while (1)
    {
        PRINTF("Please select operate mode...\r\n");
        PRINTF("\tA -- Frequency Measurement Mode.\r\n");
        PRINTF("\tB -- Pulse Width Measurement Mode.\r\n");
        PRINTF("Operate mode: ");

        inputCh = GETCHAR();

        if ((inputCh >= 'a') && (inputCh <= 'z'))
        {
            inputCh -= 'a' - 'A';
        }
        PRINTF("%c \r\n", inputCh);

        switch (inputCh)
        {
            case 'A':
            {
                FREQME_SetMaxExpectedValue(DEMO_FREQME, DEMO_MAXEXPECTVALUE);
                FREQME_SetMinExpectedValue(DEMO_FREQME, DEMO_MINEXPECTVALUE);
                DEMO_DoFreqMeasurement();
                break;
            }
            case 'B':
            {
                DEMO_DoPulseMeasurement();
                break;
            }
            default:
            {
                PRINTF("Wrong input, please retry!\r\n");
                break;
            }
        }
    }
}

static void DEMO_DoFreqMeasurement(void)
{
    freq_measure_config_t config;
    uint32_t targetFreq = 0UL;

    PRINTF("Frequency Measurement Mode Selected!\r\n");

    /* 
     * Set reference clock source for Frequency Measurement mode.
     * In Frequency Measurement mode, FREQME counts target clock pulses that occurs during 
     * the time defined by 2^CTRL_W[REF_SCALE] periods of the reference clock, so recommend
     * to set the reference clock frequency greater than target clock frequency.
     */
    INPUTMUX_AttachSignal(INPUTMUX, 0UL, DEMO_FREQ_REF_CLK_SOURCE);

    /*
     * config->operateMode = kFREQME_FreqMeasurementMode;
     * config->operateModeAttribute.refClkScaleFactor = 0U;
     * config->enableContinuousMode                   = false;
     * config->startMeasurement                       = false;
     */
    FREQME_GetDefaultConfig(&config);
    DEMO_GetClockSelection(&config);
    DEMO_GetReferenceClockScaleFactor(&config);
    FREQME_Init(DEMO_FREQME, &config);

    FREQME_EnableInterrupts(
        DEMO_FREQME, kFREQME_ReadyInterruptEnable | kFREQME_UnderflowInterruptEnable | kFREQME_OverflowInterruptEnable);
    FREQME_StartMeasurementCycle(DEMO_FREQME);

    while (!g_measurementCompleted)
    {
    }
    g_measurementCompleted = false;
    if (g_errorOccurred)
    {
        PRINTF("Error Occurred! please retry by changing reference clock scaling factor.\r\n");
        g_errorOccurred = false;
    }
    else
    {
        targetFreq = FREQME_CalculateTargetClkFreq(DEMO_FREQME, DEMO_REF_CLK_FREQ);
        PRINTF("Target clock frequency is %d Hz.\r\n", targetFreq);
    }
}

static void DEMO_DoPulseMeasurement(void)
{
    freq_measure_config_t config;
    PRINTF("Pulse Width Measurement Mode.\r\n");
    uint32_t pulseCount;

    /* 
     * Set reference clock source for Pulse Measurement mode.
     * In Pulse Width Measurement mode, FREQME counts target clock pulses while the reference 
     * clock is in a specific state (high or low), so the reference clock frequency must less
     * than target clock frequency.
     * By combining the known duration of the reference clock's high or low period with the
     * counted target pulses, the target clock frequency can be calculated.
     * If the reference clock has a high period of 1 ms and 1000 target pulses occur during
     * that time, the target frequency is 1 MHz.
     */
    INPUTMUX_AttachSignal(INPUTMUX, 0U, DEMO_PULSE_REF_CLK_SOURCE);

    /*
     * config->operateMode = kFREQME_FreqMeasurementMode;
     * config->operateModeAttribute.refClkScaleFactor = 0U;
     * config->enableContinuousMode                   = false;
     */
    FREQME_GetDefaultConfig(&config);
    config.operateMode = kFREOME_PulseWidthMeasurementMode;
    DEMO_GetClockSelection(&config);
    DEMO_GetPulsePolarity(&config);
    FREQME_Init(DEMO_FREQME, &config);

    FREQME_EnableInterrupts(
        DEMO_FREQME, kFREQME_ReadyInterruptEnable | kFREQME_UnderflowInterruptEnable | kFREQME_OverflowInterruptEnable);
    FREQME_StartMeasurementCycle(DEMO_FREQME);

    while (!g_measurementCompleted)
    {
    }
    g_measurementCompleted = false;
    if (g_errorOccurred)
    {
        PRINTF("Error Occurred! please retry by changing reference clock scaling factor.\r\n");
        g_errorOccurred = false;
    }
    else
    {
        pulseCount = FREQME_GetMeasurementResult(DEMO_FREQME);
        PRINTF("Pulse count is %d.\r\n", pulseCount);
    }
}

static void DEMO_GetClockSelection(freq_measure_config_t *config)
{
    uint8_t i;
    char chInput;

    while (1)
    {
        PRINTF("Please select the target clock:\r\n");
        for (i = 0U; i < DEMO_CLOCK_SOURCE_COUNT; i++)
        {
            if (config->operateMode == kFREQME_FreqMeasurementMode)
            {                
                PRINTF("\t\t%c -- %s\r\n", 'A' + i, g_freqClockNameArray[i]);
            }
            else
            {
                PRINTF("\t\t%c -- %s\r\n", 'A' + i, g_pulseClockNameArray[i]);
            }
        }
        PRINTF("Target clock: ");
        chInput = GETCHAR();
        if ((chInput >= 'a') && (chInput <= 'z'))
        {
            chInput -= 'a' - 'A';
        }
        PRINTF("%c \r\n", chInput);
        chInput -= 'A';
        if (chInput < DEMO_CLOCK_SOURCE_COUNT)
        {
            if (config->operateMode == kFREQME_FreqMeasurementMode)
            {
                INPUTMUX_AttachSignal(INPUTMUX, 0UL, g_freqClockSourceSignalArray[(uint8_t)chInput]);
            }
            else
            {
                INPUTMUX_AttachSignal(INPUTMUX, 0UL, g_pulseClockSourceSignalArray[(uint8_t)chInput]);
            }
            break;
        }
        else
        {
            PRINTF("Wrong Input Please Retry!\r\n");
            continue;
        }
    }
}

static void DEMO_GetReferenceClockScaleFactor(freq_measure_config_t *config)
{
    char chInput;
    uint8_t scaleFactor;
    for (;;)
    {
        scaleFactor = 0UL;
        PRINTF("Please input the scale factor of reference clock(Ranges from 0 to 31).\r\n");
        PRINTF("Please end line with CR+LF.\r\n");
        while (1)
        {
            chInput = GETCHAR();
            if ((chInput >= '0') && (chInput <= '9'))
            {
                PUTCHAR(chInput);
                chInput -= '0';
                scaleFactor = scaleFactor * 10U + chInput;
            }
            if (chInput == 0x0D)
            {
                chInput = GETCHAR();
                if (chInput == 0x0A)
                {
                    break;
                }
            }
        }
        PRINTF("\r\n");
        if (scaleFactor > 31U)
        {
            PRINTF("Wrong Scale Factor Input, Please Retry!\r\n");
            continue;
        }
        else
        {
            config->operateModeAttribute.refClkScaleFactor = scaleFactor;
            break;
        }
    }
}

static void DEMO_GetPulsePolarity(freq_measure_config_t *config)
{
    char chInput;

    for (;;)
    {
        PRINTF("Please select pulse polarity.\r\n");
        PRINTF("\t\tA -- High Period\r\n");
        PRINTF("\t\tB -- Low Period\r\n");
        PRINTF("Pulse polarity: ");
        chInput = GETCHAR();

        if ((chInput >= 'a') && (chInput <= 'z'))
        {
            chInput -= 'a' - 'A';
        }
        PRINTF("%c \r\n", chInput);
        if (chInput > 'B')
        {
            PRINTF("Wrong Pulse Polarity Input, Please Retry!\r\n");
            continue;
        }
        else
        {
            config->operateModeAttribute.pulsePolarity = (freqme_pulse_polarity_t)(chInput - 'A');
            break;
        }
    }
}
