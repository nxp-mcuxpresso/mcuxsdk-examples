/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_rtc.h"
#include "fsl_ostimer.h"
#include "fsl_ctimer.h"
#include "fsl_pint.h"
#include "fsl_inputmux.h"
#include "fsl_gint.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define kRESET_CAUSE_NOT_RELEVANT (0x0)
#define kLOWPOWERCFG_NONE         (0x0)
#define SLEEP_TIME_MS             (1500u)

#define GINT0_PORT     kGINT_Port0
#define GINT0_POL_MASK ~(1U << BOARD_BUTTON1_GPIO_PIN)
#define GINT0_ENA_MASK (1U << BOARD_BUTTON1_GPIO_PIN)

/** wakeupVectorIndex Enum */
typedef enum
{
    kWAKEUP_VECTOR_RTC = 0U,
    kWAKEUP_VECTOR_OS_TIMER,
    kWAKEUP_VECTOR_CTIMER,
    kWAKEUP_VECTOR_PINT,
    kWAKEUP_VECTOR_GINT,
    kWAKEUP_VECTOR_WAKEUP_PIN,
} wakeup_vector_index_t;

/** Configure wakeup timer function type definition */
typedef void (*irqSrcConfigureWakeupTimer_t)(int ms);

/** Configure interrupt source type definition */
typedef void (*irqSrcInit_t)(bool init);

/** Wakeup vector structure */
typedef struct
{
    irqSrcInit_t initWakeUpSourceCb;
    irqSrcConfigureWakeupTimer_t configureWakeUpCb;
    uint64_t wakeupInterruptMask;
    uint32_t excludeFromPd;
} wakeup_vector_t;

/** Menu callback function type definition */
typedef status_t (*menuFunc_t)(wakeup_vector_index_t wakeupVectorIndex);

/** Menu item structure */
typedef struct
{
    /** The title of the menu item */
    const char *title;
    /** The function to call when the item is selected */
    menuFunc_t func;
    /** The parameter to pass to function */
    uint8_t param;
} menu_item_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static status_t Sleep(wakeup_vector_index_t wakeupVectorIndex);
static status_t DeepSleep(wakeup_vector_index_t wakeupVectorIndex);
static status_t PowerDown(wakeup_vector_index_t wakeupVectorIndex);
static status_t DeepPowerDown(wakeup_vector_index_t wakeupVectorIndex);
static status_t PowerOff(wakeup_vector_index_t wakeupVectorIndex);
static status_t Reset(wakeup_vector_index_t param);
static status_t GetPowerLibVersion(wakeup_vector_index_t param);

static void InitRTC(bool init);
static void InitOSTimer(bool init);
static void InitCTimer(bool init);
static void InitPint(bool init);
static void InitGint(bool init);

static void ConfigureRTCWakeUp(int ms);
static void ConfigureOSTimerWakeUp(int ms);
static void ConfigureCTimerWakeUp(int ms);
static void ConfigurePintWakeUp(int ms);
static void ConfigureGintWakeUp(int ms);
static void ConfigureWakeUpPin(int ms);

static void WaitPinStableHigh(int pin);
static void CTimer_IRQHandler(uint32_t flags);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/** Array of all menu items the user can select from */
static const menu_item_t s_menu[] = {
    {.title = "Get power library version", .func = GetPowerLibVersion, .param = 0},
    {.title = "Go to sleep. Wakeup from RTC timer", .func = Sleep, .param = kWAKEUP_VECTOR_RTC},
    {.title = "Go to sleep. Wakeup from OS timer", .func = Sleep, .param = kWAKEUP_VECTOR_OS_TIMER},
    {.title = "Go to sleep. Wakeup from Ctimer", .func = Sleep, .param = kWAKEUP_VECTOR_CTIMER},
    {.title = "Go to sleep. Wakeup from pin toggle (PINT)", .func = Sleep, .param = kWAKEUP_VECTOR_PINT},
    {.title = "Go to sleep. Wakeup from pin toggle (GINT)", .func = Sleep, .param = kWAKEUP_VECTOR_GINT},
    {.title = "Go to deep sleep. Wakeup from RTC timer", .func = DeepSleep, .param = kWAKEUP_VECTOR_RTC},
    {.title = "Go to deep sleep. Wakeup from OS timer", .func = DeepSleep, .param = kWAKEUP_VECTOR_OS_TIMER},
    {.title = "Go to deep sleep. Wakeup from Ctimer", .func = DeepSleep, .param = kWAKEUP_VECTOR_CTIMER},
    {.title = "Go to deep sleep. Wakeup from pin toggle (PINT)", .func = DeepSleep, .param = kWAKEUP_VECTOR_PINT},
    {.title = "Go to deep sleep. Wakeup from pin toggle (GINT)", .func = DeepSleep, .param = kWAKEUP_VECTOR_GINT},
    {.title = "Go to power down. Wakeup from RTC timer", .func = PowerDown, .param = kWAKEUP_VECTOR_RTC},
    {.title = "Go to power down. Wakeup from OS timer", .func = PowerDown, .param = kWAKEUP_VECTOR_OS_TIMER},
    {.title = "Go to deep power down. Wakeup from RTC timer", .func = DeepPowerDown, .param = kWAKEUP_VECTOR_RTC},
    {.title = "Go to deep power down. Wakeup from OS timer", .func = DeepPowerDown, .param = kWAKEUP_VECTOR_OS_TIMER},
    {.title = "Go to deep power down. Wakeup from pin toggle (always on wakeup pin 21)",
     .func  = DeepPowerDown,
     .param = kWAKEUP_VECTOR_WAKEUP_PIN},
    {.title = "Go to power off. Wakeup from pin toggle (always on wakeup pin 21)",
     .func  = PowerOff,
     .param = kWAKEUP_VECTOR_WAKEUP_PIN},
    {.title = "Reset device", .func = Reset, .param = 0},
    {.title = NULL, .func = NULL}};

/** Array of all possible wakeup vectors */
static wakeup_vector_t s_wakeupVector[] = {
    {InitRTC, ConfigureRTCWakeUp, kWAKEUP_RTC, kLOWPOWERCFG_FRO32K},
    {InitOSTimer, ConfigureOSTimerWakeUp, kWAKEUP_OS_EVENT, kLOWPOWERCFG_FRO32K},
    {InitCTimer, ConfigureCTimerWakeUp, kWAKEUP_CTIMER0, kLOWPOWERCFG_FRO192M | kLOWPOWERCFG_HFDSM},
    {InitPint, ConfigurePintWakeUp, kWAKEUP_PIN_INT0, kLOWPOWERCFG_NONE},
    {InitGint, ConfigureGintWakeUp, kWAKEUP_GINT0, kLOWPOWERCFG_NONE},
    {NULL, ConfigureWakeUpPin, kWAKEUP_WAKE_PAD, kLOWPOWERCFG_NONE}};

/** Variable to keep track which dc/dc mode is used. */
static dcdc_mode_t s_supplyMode;

/** Ctimer callback table. */
static ctimer_callback_t s_ctimerCallbackTable[] = {CTimer_IRQHandler};

/*******************************************************************************
 * Code
 ******************************************************************************/

void RTC_IRQHandler(void)
{
    RTC_ClearStatusFlags(RTC, kRTC_WakeupFlag);
    RTC_EnableTimer(RTC, false);
}

static void InitRTC(bool init)
{
    if (init)
    {
        PRINTF("Init RTC\n");
        RTC_Init(RTC);
        RTC_EnableWakeupTimer(RTC, true);
        EnableIRQ(RTC_IRQn);
    }
    else
    {
        PRINTF("De-Init RTC\n");
        RTC_EnableWakeupTimer(RTC, false);
        DisableIRQ(RTC_IRQn);
        RTC_Deinit(RTC);
    }
}

static void ConfigureRTCWakeUp(int ms)
{
    RTC_SetWakeupCount(RTC, ms);
    RTC_EnableTimer(RTC, true);

    /* Use FRO32K as deep sleep clock.
       No need to power on FRO32K, happens automatically when RTC or OS timer is used to wakeup the system.
     */
    PMC->RTCOSC32K |= PMC_RTCOSC32K_SEL(0u);

    PRINTF("Waking up from RTC after %d ms\n", ms);
}

void OSTimer_IRQHandler(void)
{
    OSTIMER_ClearStatusFlags(OSTIMER, kOSTIMER_MatchInterruptFlag);
}

static void InitOSTimer(bool init)
{
    if (init)
    {
        PRINTF("Init OS Timer\n");
        OSTIMER_Init(OSTIMER);
    }
    else
    {
        PRINTF("De-Init OS Timer\n");
        OSTIMER_Deinit(OSTIMER);
    }
}

static void ConfigureOSTimerWakeUp(int ms)
{
    uint64_t timerTicks = OSTIMER_GetCurrentTimerValue(OSTIMER);
    timerTicks += MSEC_TO_COUNT(ms, 32768);
    OSTIMER_SetMatchValue(OSTIMER, timerTicks, OSTimer_IRQHandler);

    /* Use FRO32K as deep sleep clock.
       No need to power on FRO32K, happens automatically when RTC or OS timer is used to wakeup the system.
     */
    PMC->RTCOSC32K |= PMC_RTCOSC32K_SEL(0u);
}

static void InitCTimer(bool init)
{
    if (init)
    {
        PRINTF("Init CTimer\n");
        ctimer_config_t config;
        CLOCK_AttachClk(kMAIN_CLK_to_CTIMER0);
        CTIMER_GetDefaultConfig(&config);
        CTIMER_Init(CTIMER0, &config);
        CTIMER_RegisterCallBack(CTIMER0, &s_ctimerCallbackTable[0], kCTIMER_SingleCallback);
    }
    else
    {
        PRINTF("De-Init Ctimer\n");
        CTIMER_Deinit(CTIMER0);
    }
}

static void ConfigureCTimerWakeUp(int ms)
{
    ctimer_match_config_t matchConfig;
    matchConfig.enableCounterReset = true;
    matchConfig.enableCounterStop  = false;
    matchConfig.matchValue         = MSEC_TO_COUNT(ms, CLOCK_GetCTimerClkFreq(0));
    matchConfig.outControl         = kCTIMER_Output_Toggle;
    matchConfig.outPinInitState    = false;
    matchConfig.enableInterrupt    = true;
    CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_0, &matchConfig);
    CTIMER_StartTimer(CTIMER0);
}

void CTimer_IRQHandler(uint32_t flags)
{
    (void)flags;
    CTIMER_ClearStatusFlags(CTIMER0, kCTIMER_Match0Flag);
    CTIMER_StopTimer(CTIMER0);
}

void Pint_IRQHandler(pint_pin_int_t pintr, uint32_t pmatchStatus)
{
}

static void InitPint(bool init)
{
    if (init)
    {
        PRINTF("Init PINT\n");
        INPUTMUX_Init(INPUTMUX);
        INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt0, PIN_INT_SRC);
        /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
        INPUTMUX_Deinit(INPUTMUX);

        PINT_Init(PINT);
        PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableFallEdge, Pint_IRQHandler);
        PINT_EnableCallbackByIndex(PINT, kPINT_PinInt0);
    }
    else
    {
        PRINTF("De-Init PINT\n");
        PINT_DisableCallbackByIndex(PINT, kPINT_PinInt0);
        PINT_Deinit(PINT);
    }
}

void Gint_IRQHandler(void)
{
    GINT_ClrStatus(GINT0);
}

static void ConfigurePintWakeUp(int ms)
{
    /* Make sure pin is debounced before using it (from potential previous usage)*/
    WaitPinStableHigh(BOARD_BUTTON1_GPIO_PIN);

    PRINTF("Push USER button (%s), to wake me up\n", BOARD_BUTTON1_PIN_NAME);
}

static void InitGint(bool init)
{
    if (init)
    {
        PRINTF("Init GINT\n");
        GINT_Init(GINT0);
        GINT_SetCtrl(GINT0, kGINT_CombineOr, kGINT_TrigLevel, Gint_IRQHandler);
        GINT_ConfigPins(GINT0, GINT0_PORT, GINT0_POL_MASK, GINT0_ENA_MASK);
        GINT_EnableCallback(GINT0);
    }
    else
    {
        PRINTF("De-Init GINT\n");
        GINT_DisableCallback(GINT0);
        GINT_Deinit(GINT0);
    }

    /* TODO: does not work for power down. Even when pin 18 is not present in GINT0_ENA_MASK, device wakes up*/
}

static void ConfigureGintWakeUp(int ms)
{
    /* Make sure pin is debounced before using it (from potential previous usage)*/
    WaitPinStableHigh(BOARD_BUTTON1_GPIO_PIN);

    PRINTF("Push USER button (%s), to wake me up\n", BOARD_BUTTON1_PIN_NAME);
}

static void ConfigureWakeUpPin(int ms)
{
    /* Make sure pin is debounced before using it (from potential previous usage)*/
    WaitPinStableHigh(BOARD_WAKEUP_GPIO_PIN);

    PRINTF("Push WAKEUP button (%s), to wake me up\n", BOARD_WAKEUP_PIN_NAME);
}

static void WaitPinStableHigh(int pin)
{
    int pinValue;
    int pinValueHighcount         = 0;
    int loopCounter               = 0;
    const int pinValueStableCount = 4 * 1024;
    const int maxLoopCounter      = 1024 * 1024;

    do
    {
        pinValue = GPIO_PinRead(GPIO, 0U, pin);
        if (pinValue == 1)
        {
            /* Wakeup pin has pull up, so expected value is 1 when pin is not being triggered.*/
            pinValueHighcount++;
        }
        else
        {
            pinValueHighcount = 0;
        }
        loopCounter++;
        if (loopCounter > maxLoopCounter)
        {
            /* After maxLoopCounter we consider debouncing as failed and we stop the loop. */
            PRINTF("Wakeup pin debouncing timeout\n");
            return;
        }

    } while (pinValueHighcount < pinValueStableCount);
    PRINTF("Wakeup pin debounced\n");
}

static void ConfigureWakeup(int wakeupVectorIndex, int sleepTimeMs)
{
    if (s_wakeupVector[wakeupVectorIndex].configureWakeUpCb)
    {
        s_wakeupVector[wakeupVectorIndex].configureWakeUpCb(sleepTimeMs);
    }
}

static void InitWakupSource(int wakeupVectorIndex)
{
    if (s_wakeupVector[wakeupVectorIndex].initWakeUpSourceCb)
    {
        s_wakeupVector[wakeupVectorIndex].initWakeUpSourceCb(true);
    }
}

static void DeInitWakupSource(int wakeupVectorIndex)
{
    if (s_wakeupVector[wakeupVectorIndex].initWakeUpSourceCb)
    {
        s_wakeupVector[wakeupVectorIndex].initWakeUpSourceCb(false);
    }
}

static status_t GetPowerLibVersion(wakeup_vector_index_t param)
{
    uint32_t libVersion = POWER_GetLibVersion();
    uint8_t major       = libVersion >> 16;
    uint8_t minor       = libVersion >> 8 & 0xFF;
    uint8_t bugfix      = libVersion & 0xFF;
    PRINTF("Power library version: %d.%d.%d\n", major, minor, bugfix);
    return kStatus_Success;
}

static status_t Reset(wakeup_vector_index_t param)
{
    PRINTF("Resetting device. Here we go!\n");
    POWER_Reset();
    return kStatus_Success;
}

static status_t Sleep(wakeup_vector_index_t wakeupVectorIndex)
{
    /* Init wake up source */
    InitWakupSource(wakeupVectorIndex);

    /* Configure the wake up source */
    ConfigureWakeup(wakeupVectorIndex, SLEEP_TIME_MS);

    /* Go to sleep */
    PRINTF("Going to sleep.\n");
    PRINTF("...zZZzzzZZzZZzzzZZ...\n");

    POWER_EnterSleep();
    PRINTF("Woken up from sleep.\n");

    /* De-init wake up source */
    DeInitWakupSource(wakeupVectorIndex);

    return kStatus_Success;
}

static status_t DeepSleep(wakeup_vector_index_t wakeupVectorIndex)
{
    status_t status;
    uint32_t excludeFromPd;

    /* Init wake up source */
    InitWakupSource(wakeupVectorIndex);

    /* Configure the wake up source */
    ConfigureWakeup(wakeupVectorIndex, SLEEP_TIME_MS);

    /* If the detected DC/DC mode is kDCDC_MODE_HV_SM, excludeFromPd mask has to be updated with
       kLOWPOWERCFG_DCDC_BYPASS to make sure DC/DC is put in bypass during the low power mode. */
    excludeFromPd = s_wakeupVector[wakeupVectorIndex].excludeFromPd;
    if (s_supplyMode == kDCDC_MODE_HV_SM)
    {
        excludeFromPd |= kLOWPOWERCFG_DCDC_BYPASS;
    }

    /* Go to deep sleep */
    PRINTF("Going to deep sleep.\n");
    PRINTF("...zZZzzzZZzZZzzzZZ...\n");

    status = POWER_EnterDeepSleep(excludeFromPd, s_wakeupVector[wakeupVectorIndex].wakeupInterruptMask);
    if (status != kStatus_Success)
    {
        PRINTF("POWER_EnterDeepSleep failed with error code 0x%x\n", status);
    }

    PRINTF("Woken up from deep sleep.\n");

    /* De-init wake up source */
    DeInitWakupSource(wakeupVectorIndex);

    return status;
}

static status_t PowerDown(wakeup_vector_index_t wakeupVectorIndex)
{
    status_t status;
    uint32_t excludeFromPd;

    /* Init wake up source */
    InitWakupSource(wakeupVectorIndex);

    /* Configure the wake up source */
    ConfigureWakeup(wakeupVectorIndex, SLEEP_TIME_MS);

    /* If the detected DC/DC mode is kDCDC_MODE_HV_SM, excludeFromPd mask has to be updated with
       kLOWPOWERCFG_DCDC_BYPASS to make sure DC/DC is put in bypass during the low power mode. */
    excludeFromPd = s_wakeupVector[wakeupVectorIndex].excludeFromPd;
    if (s_supplyMode == kDCDC_MODE_HV_SM)
    {
        excludeFromPd |= kLOWPOWERCFG_DCDC_BYPASS;
    }

    /* Go to power down */
    PRINTF("Going to power down, with CPU retention enabled.\n");
    PRINTF("...zZZzzzZZzZZzzzZZ...\n");

    status = POWER_EnterPowerDown(excludeFromPd, s_wakeupVector[wakeupVectorIndex].wakeupInterruptMask,
                                  kCPU_RETENTION_ENABLE);
    if (status != kStatus_Success)
    {
        PRINTF("POWER_EnterPowerDown failed with error code 0x%x\n", status);
    }
    else
    {
        /* When waking up from power down, with CPU retention, hardware has to be reinitialized. */
        BOARD_InitHardware();
    }

    PRINTF("Woken up from power down.\n");

    /* De-init wake up source */
    DeInitWakupSource(wakeupVectorIndex);

    return status;
}

static status_t DeepPowerDown(wakeup_vector_index_t wakeupVectorIndex)
{
    status_t status;
    uint32_t excludeFromPd;
    uint32_t wakeupIoCtrl;

    /* Init wake up source */
    InitWakupSource(wakeupVectorIndex);

    /* Configure the wake up source */
    ConfigureWakeup(wakeupVectorIndex, SLEEP_TIME_MS);

    /* If the detected DC/DC mode is kDCDC_MODE_HV_SM, excludeFromPd mask has to be updated with
       kLOWPOWERCFG_DCDC_BYPASS to make sure DC/DC is put in bypass during the low power mode. */
    excludeFromPd = s_wakeupVector[wakeupVectorIndex].excludeFromPd;
    if (s_supplyMode == kDCDC_MODE_HV_SM)
    {
        excludeFromPd |= kLOWPOWERCFG_DCDC_BYPASS;
    }

    if (s_wakeupVector[wakeupVectorIndex].wakeupInterruptMask == kWAKEUP_WAKE_PAD)
    {
        /* NHS52S04 EVK has pull up on wakeup pin, so configure wakeup pin for pull up and trigger wakeup
         * when it goes low*/
        wakeupIoCtrl = kWAKEUP_PIN_ENABLE | kWAKEUP_PIN_PUP_EN | kWAKEUP_PIN_WAKEUP_LOW_LVL;
    }
    else
    {
        wakeupIoCtrl = kWAKEUP_PIN_DISABLE;
    }

    /* Go to deep power down */
    PRINTF("Going to deep power down.\n");
    PRINTF("...zZZzzzZZzZZzzzZZ...\n");
    PRINTF("Expecting reboot after wakeup.\n");

    status =
        POWER_EnterDeepPowerDown(excludeFromPd, s_wakeupVector[wakeupVectorIndex].wakeupInterruptMask, wakeupIoCtrl);
    if (status != kStatus_Success)
    {
        PRINTF("POWER_EnterDeepPowerDown failed with error code 0x%x\n", status);
    }
    else
    {
        /* Should never come here, device will reboot after wakeup from deep power down.*/
    }

    return status;
}
static status_t PowerOff(wakeup_vector_index_t wakeupVectorIndex)
{
    status_t status;
    uint32_t excludeFromPd;
    uint32_t wakeupIoCtrl;

    /* Init wake up source */
    InitWakupSource(wakeupVectorIndex);

    /* Configure the wake up source */
    ConfigureWakeup(wakeupVectorIndex, SLEEP_TIME_MS);

    /* If the detected DC/DC mode is kDCDC_MODE_HV_SM, excludeFromPd mask has to be updated with
       kLOWPOWERCFG_DCDC_BYPASS to make sure DC/DC is put in bypass during the low power mode. */
    excludeFromPd = s_wakeupVector[wakeupVectorIndex].excludeFromPd;
    if (s_supplyMode == kDCDC_MODE_HV_SM)
    {
        excludeFromPd |= kLOWPOWERCFG_DCDC_BYPASS;
    }

    if (s_wakeupVector[wakeupVectorIndex].wakeupInterruptMask == kWAKEUP_WAKE_PAD)
    {
        /* NHS52S04 EVK has pull up on wakeup pin, so configure wakeup pin for pull up and trigger wakeup
         * when it goes low*/
        wakeupIoCtrl = kWAKEUP_PIN_ENABLE | kWAKEUP_PIN_PUP_EN | kWAKEUP_PIN_WAKEUP_LOW_LVL;
    }
    else
    {
        wakeupIoCtrl = kWAKEUP_PIN_DISABLE;
    }

    /* Go to power off */
    PRINTF("Going to power off.\n");
    PRINTF("...zZZzzzZZzZZzzzZZ...\n");
    PRINTF("Expecting reboot after wakeup.\n");

    status = POWER_EnterPowerOff(excludeFromPd, wakeupIoCtrl);
    if (status != kStatus_Success)
    {
        PRINTF("POWER_EnterPowerOff failed with error code 0x%x\n", status);
    }
    else
    {
        /* Should never come here, device will reboot after wakeup from power off.*/
    }

    return status;
}

static void ClearTerminal(void)
{
    /* Give reader of UART console the impression that console is cleared by printing a lot of end of lines.*/
    for (int i = 0; i < 1024; i++)
    {
        PRINTF("\n");
    }
}

/**
 * Reads a single line from UART. A line ends when a \r or \n is
 * detected. The function is safe in the sense that it assures that
 * the passed buffer is never overflown.
 *
 * @param buffer The buffer to store the line
 * @param bufferSize The size of the buffer in bytes
 */
static void ReadLine(char *buffer, uint32_t bufferSize)
{
    char *p = buffer;
    while (true)
    {
        *p = DbgConsole_Getchar();
        PRINTF("%c", *p);
        /* Is it a backspace? */
        if (*p == 8 || *p == 0x7f)
        {
            /* Yes, are we at the beginning of the buffer? */
            if (p != buffer)
            {
                /* No, then we can remove safely the previous character */
                p--;
            }
        }
        /* Is it a new-line or carriage-return? */
        else if (*p == '\n' || *p == '\r')
        {
            /* Yes, then we're done. Close the string with a terminating zero */
            *p = '\0';
            break;
        }
        /* Is the pointer pointing to the last element in the array? */
        else if (p != &buffer[bufferSize - 1])
        {
            /* No, then we can progress to the next character in the array */
            p++;
        }
    }
}

/**
 * Displays all menu items with a unique selector
 */
static void ShowMenu()
{
    const menu_item_t *item = s_menu;
    uint32_t idx            = 0;

    PRINTF("\n================= SELECT FROM MENU =================\n\n");
    while (item->title != NULL)
    {
        PRINTF("%c. %s\r\n", 'a' + idx, item->title);
        item++;
        idx++;
    }
}

/**
 * Calls the callback function of the given menu item and prints the result
 *
 * @param item The menu item to execute
 * @return The result of the test
 */
static status_t ExecuteMenuItem(const menu_item_t *item)
{
    ClearTerminal();

    PRINTF("\n----------------------------------------------------\n");
    PRINTF("SELECTED: %s\n", item->title);
    PRINTF("----------------------------------------------------\n");

    status_t status = item->func((wakeup_vector_index_t)item->param);
    return status;
}

/**
 * This function waits for user input and executes the selected test
 */
static void HandleMenu()
{
    const menu_item_t *item = NULL;
    static char input[256];

    ShowMenu();
    PRINTF("> ");
    ReadLine(input, sizeof(input));
    PRINTF("\r\n");

    /* Check whether the user has selected a valid menu item */
    if (input[0] >= 'a')
    {
        uint8_t idx = input[0] - 'a';
        item        = s_menu;
        while ((idx > 0) && (item->title != NULL))
        {
            idx--;
            item++;
        }
        if (item->title == NULL)
        {
            item = NULL;
        }
    }

    /* Is a valid item selected? */
    if (item)
    {
        /* Yes, then execute it */
        ExecuteMenuItem(item);
        PRINTF("\r\n");
    }
    else
    {
        /* No, inform the user */
        PRINTF("Invalid menu option\r\n");
    }
}
/*!
 * @brief Main function
 */
int main(void)
{
    reset_cause_t resetCause;

    BOARD_InitHardware();

    /* Get supply mode, this info is needed to determine correct parameters of low power modes.*/
    s_supplyMode = POWER_DCDC_GetSupplyMode();

    /* Get reset cause, this info is needed to know if the application was booted because of a wakeup or because of a
     * reset. */
    resetCause = POWER_GetResetCause();

    /* After waking up from deep power down from RTC, RTC interrupt is still fired,
   so we need to make sure RTC block is initialized (e.g. has clock), to be able to handle interrupt*/
    RTC_Init(RTC);

    if (resetCause == kRESET_CAUSE_PAD)
    {
        ClearTerminal();
    }

    PRINTF("\n\n====================================================\n");
    PRINTF("!!! MCXW345 BOOTED !!!\n");
    PRINTF("Boot reason: ");

    /* Inform the user why the application was rebooted */
    if (resetCause == kRESET_CAUSE_PAD)
    {
        PRINTF("Hardware reset");
    }
    else if (resetCause == kRESET_CAUSE_SWR)
    {
        PRINTF("Software reset");
    }
    else if (resetCause == kRESET_CAUSE_RTC)
    {
        PRINTF("RTC wakeup");
    }
    else if (resetCause == kRESET_CAUSE_OST)
    {
        PRINTF("OS timer wakeup");
    }
    else if (resetCause == kRESET_CAUSE_DPD_IO)
    {
        /* When waking up from deep sleep using wakup pin, kRESET_CAUSE_DPD_IO is reset cause. */
        PRINTF("Wake up from pin 21");
    }
    else if (resetCause == kRESET_CAUSE_POR)
    {
        /* When waking up from power off using wakup pin, kRESET_CAUSE_POR is reset cause. */
        PRINTF("Wake up from pin 21");
    }

    PRINTF("\nDC/DC mode: ");
    if (s_supplyMode == kDCDC_MODE_HV_SM)
    {
        PRINTF("Buck mode (HV)");
    }
    else if (s_supplyMode == kDCDC_MODE_LV_SM)
    {
        PRINTF("Boost mode (LV)");
    }
    else if (s_supplyMode == kDCDC_MODE_LV_SM)
    {
        PRINTF("Off (XR)");
    }
    PRINTF("\nDC/DC startup duration: %d us\n", POWER_DCDC_GetStartupDuration());

    PRINTF("====================================================\n");

    while (true)
    {
        HandleMenu();
    }
}
