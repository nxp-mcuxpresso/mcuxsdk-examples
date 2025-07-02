/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "power_mode_switch.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_rtc.h"
#include "fsl_pit.h"
#include "fsl_swt.h"
#include "fsl_wkpu.h"
#include "fsl_lpuart.h"
#include "fsl_power.h"
#include "fsl_mc_rgm.h"
#include "fsl_lpcmp.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Check if application starts from normal boot
 * @return true if normal boot, false if not
 */
static bool APP_NormalBoot(void);

/*!
 * @brief Check if RTC is enabled
 * @return true if RTC is enabled, false if not
 */
static bool APP_RTCIsEnabled(void);

/*!
 * @brief Check if SWT is enabled
 * @return true if SWT is enabled, false if not
 */
static bool APP_SWTIsEnabled(void);

/*!
 * @brief Check if PIT RTI is enabled
 * @return true if PIT RTI is enabled, false if not
 */
static bool APP_RTIIsEnabled(void);

/*!
 * @brief Check if LPCMP is enabled
 * @return true if LPCMP is enabled, false if not
 */
static bool APP_LPCMPIsEnabled(void);

/*!
 * @brief Deinit wakeup peripherals.
 */
static void APP_DeinitWakeupPeripherals(void);

/*!
 * @brief Get selected wakeup source
 * @return Selected wakeup source, enum of app_wakeup_source_t
 */
static app_wakeup_source_t APP_SelectWakeupSource(void);

/*!
 * @brief Get wakeup timeout from user input
 */
static uint8_t APP_GetWakeupTimeout(void);

/*!
 * @brief Get selected wake up source and configure wakup source module.
 */
static void APP_GetWakeupConfiguration(void);

/*!
 * @brief Get target power mode from user input
 * @return Target power mode, enum of app_power_mode_t
 */
static app_power_mode_t APP_GetTargetPowerMode(void);

/*!
 * @brief Configure SWT as wake up source
 * @param timeOut Wake up time out value
 */
static void APP_SetWakeupSWTConfiguration(uint8_t timeOut);

/*!
 * @brief Configure RTC-API as wake up source
 * @param timeOut Wake up time out value
 */
static void APP_SetWakeupRTCAPIConfiguration(uint8_t timeOut);

/*!
 * @brief Configure RTC as wake up source
 * @param timeOut Wake up time out value
 */
static void APP_SetWakeupRTCConfiguration(uint8_t timeOut);

/*!
 * @brief Configure LPCMP round robin as wake up source
 * @param timeOut Wake up time out value
 */
static void APP_SetWakeupLPCMPRoundRobinWakeupInterruptConfiguration(uint8_t timeOut);

/*!
 * @brief Configure RTI as wake up source
 * @param timeOut Wake up time out value
 */
static void APP_SetWakeupRTIConfiguration(uint8_t timeOut);

/*!
 * @brief Configure pin as wake up source
 */
static void APP_SetWakeupPinConfiguration(void);

/*!
 * @brief Delay for PIT RTI control register stable after configuration is written
 */
static void APP_DelayForPITRTIUpdate(void);

/*!
 * @brief STANDBY IO configuration standby exit: Must be written as 1 after IO configurations are done on standby exit
 */
static void APP_SetStandbyIoConfiguration(void);

/*!
 * @brief STANDBY IO configuration standby entry: Must be written as 0 before IO configurations are done in standby
 * entry sequence
 */
static void APP_ClearStandbyIoConfiguration(void);

/*!
 * @brief Set standby mode clock configuration
 */
static void APP_SetStandbyClockConfiguration(void);

/*!
 * @brief Pre switch hook function, mainly close debug console
 */
static void APP_PowerPreSwitchHook(void);

/*!
 * @brief Pull down JTAG TDI pin to prevent small-scale voltage fluctuation in JTAG TDI pin
 */
static void APP_PullDownJTAGTDI(void);

/*!
 * @brief Set standby mode power PMC voltage regulator configuration
 */
static void APP_SetStandbyPowerPMCVoltageRegulatorConfiguration(void);

/*!
 * @brief Disable unused peripheral clock
 */
static void APP_DisableUnusedPeripheralClock(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

char *const g_modeNameArray[] = APP_POWER_MODE_NAME;
char *const g_modeDescArray[] = APP_POWER_MODE_DESC;

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    uint32_t freq;
    app_power_mode_t targetPowerMode;
    bool needSetWakeup = false;

    BOARD_InitHardware();

    if (POWER_ExitFromStandbyMode())
    {
        /* STANDBY IO configuration standby exit: Must be written as 1 after
        * IO configurations are done on standby exit. */
        APP_SetStandbyIoConfiguration();
        WKPU_ClearExternalWakeupSourceConfig(APP_WKPU, kWKPU_Source0 | kWKPU_Source1 | kWKPU_Source2 | kWKPU_Source3 | kWKPU_Source44);
        APP_DeinitWakeupPeripherals();
        PRINTF("Recover from standby mode.\r\n");
    }
    else
    {
        if (APP_NormalBoot())
        {
            PRINTF("\r\nNormal Boot.\r\n");
        }
        else
        {
            PRINTF("Start from other reset.\r\n");
        }
    }

    freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    PRINTF("\r\n###########################    Power Mode Switch Demo    ###########################\r\n");
    PRINTF("    Core Clock = %dHz \r\n", freq);
    PRINTF("    Power mode: Run in FPM\r\n");
    while (!needSetWakeup)
    {
        targetPowerMode = APP_GetTargetPowerMode();

        if ((targetPowerMode > kAPP_PowerModeMin) && (targetPowerMode < kAPP_PowerModeMax))
        {
            /* If target mode is Active mode, don't need to set wakeup source. */
            if (targetPowerMode == kAPP_PowerModeRun)
            {
                needSetWakeup = false;
            }
            else
            {
                needSetWakeup = true;
            }
        }
    }

    APP_GetWakeupConfiguration();
    APP_PowerPreSwitchHook();
    APP_PullDownJTAGTDI();
    APP_SetStandbyClockConfiguration();
    /* Adjust voltage: disable Last Mile and V25 regulators */
    APP_SetStandbyPowerPMCVoltageRegulatorConfiguration();
    /* STANDBY IO configuration standby entry: Must be written as 0
     * before IO configurations are done in standby entry sequence. */
    APP_ClearStandbyIoConfiguration();
    APP_DisableUnusedPeripheralClock();
    POWER_EnterStandbyMode();
    for (;;)
    {
        __ASM("nop");
    }
}

static bool APP_NormalBoot(void)
{
    bool isNormalBoot = true;

    if (MC_RGM_GetFunctionalResetSourcesStatus(APP_MC_RGM) &
        (kMC_RGM_ExternalDestructiveResetFLag | kMC_RGM_SoftwareFunctionalResetFlag))
    {
        isNormalBoot = true;
        MC_RGM_ClearFunctionalResetSourcesStatus(APP_MC_RGM, kMC_RGM_AllFunctionalResetFlags);
    }
    else
    {
        isNormalBoot = false;
    }

    return isNormalBoot;
}

static bool APP_RTCIsEnabled(void)
{
    if ((MC_ME_COFB_STAT_REG(APP_RTC_CLOCK) & (1UL << MC_ME_COFB_CLKEN_BIT(APP_RTC_CLOCK))) == 0U)
    {
        return false;
    }
    else
    {
        return true;
    }
}

static bool APP_SWTIsEnabled(void)
{
    if ((MC_ME_COFB_STAT_REG(APP_SWT_CLOCK) & (1UL << MC_ME_COFB_CLKEN_BIT(APP_SWT_CLOCK))) == 0U)
    {
        return false;
    }
    else
    {
        return true;
    }
}

static bool APP_RTIIsEnabled(void)
{
    if ((MC_ME_COFB_STAT_REG(APP_PIT_RTI_CLOCK) & (1UL << MC_ME_COFB_CLKEN_BIT(APP_PIT_RTI_CLOCK))) == 0U)
    {
        return false;
    }
    else
    {
        return true;
    }
}

static bool APP_LPCMPIsEnabled(void)
{
    if ((MC_ME_COFB_STAT_REG(APP_LPCMP_CLOCK) & (1UL << MC_ME_COFB_CLKEN_BIT(APP_LPCMP_CLOCK))) == 0U)
    {
        return false;
    }
    else
    {
        return true;
    }
}

static void APP_DeinitWakeupPeripherals(void)
{
    /* Disable RTC if exits from RTC wake up last time */
    if (APP_RTCIsEnabled())
    {
        RTC_Deinit(APP_RTC);
    }

    /* Disable SWT if exits from SWT wake up last time */
    if (APP_SWTIsEnabled())
    {
        SWT_Deinit(APP_SWT);
    }

    /* Disable PIT if exits from PIT wake up last time */
    if (APP_RTIIsEnabled())
    {
        PIT_RTI_Deinit(APP_PIT_RTI);
    }

    /* Disable LPCMP if exits from LPCMP wake up last time */
    if (APP_LPCMPIsEnabled())
    {
        LPCMP_Deinit(APP_LPCMP);
    }
}

static app_wakeup_source_t APP_SelectWakeupSource(void)
{
    char ch;
    PRINTF("Please select wakeup source:\r\n");

    PRINTF("\tPress %c to select SWT as wakeup source;\r\n", kAPP_WakeupSourceSWT);
    PRINTF("\tPress %c to select RTC API as wakeup source;\r\n", kAPP_WakeupSourceRTCAPI);
    PRINTF("\tPress %c to select RTC as wakeup source;\r\n", kAPP_WakeupSourceRTC);
    PRINTF("\tPress %c to select LPCMP round robin wake-up interrupt as wakeup source;\r\n",
           kAPP_WakeupSourceLPCMPRoundRobinWakeupInterrupt);
    PRINTF("\tPress %c to select RTI as wakeup source;\r\n", kAPP_WakeupSourceRTI);
    PRINTF("\tPress %c to select Button as wakeup source;\r\n", kAPP_WakeupSourceButton);

    PRINTF("Waiting for wakeup source select...\r\n");
    ch = GETCHAR();

    if ((ch >= 'a') && (ch <= 'z'))
    {
        ch -= 'a' - 'A';
    }

    return (app_wakeup_source_t)ch;
}

static uint8_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;

    while (1)
    {
        PRINTF("Select the wake up timeout in seconds.\r\n");
        PRINTF("The allowed range is 1s ~ 9s.\r\n");
        PRINTF("Eg. enter 5 to wake up in 5 seconds.\r\n");
        PRINTF("\r\nWaiting for input timeout value...\r\n\r\n");

        timeout = GETCHAR();
        PRINTF("%c\r\n", timeout);
        if ((timeout > '0') && (timeout <= '9'))
        {
            return timeout - '0';
        }
        PRINTF("Wrong value!\r\n");
    }
}

static void APP_GetWakeupConfiguration(void)
{
    wkpu_external_wakeup_source_config_t wkpu_config;
    app_wakeup_source_t wakeupSource;
    uint8_t timeOutValue;

    /*
     * config->event               = kWKPU_WakeUp;
     * config->edge                = kWKPU_PinRisingEdge;
     * config->enableFilter        = true;
     */
    WKPU_GetDefaultExternalWakeUpSourceConfig(&wkpu_config);

    wakeupSource = APP_SelectWakeupSource();

    switch (wakeupSource)
    {
        case kAPP_WakeupSourceSWT:
        {
            PRINTF("SWT is selected as wakeup source!\r\n");
            WKPU_SetExternalWakeUpSourceConfig(APP_WKPU, kWKPU_Source0, &wkpu_config);
            timeOutValue = APP_GetWakeupTimeout();
            APP_SetWakeupSWTConfiguration(timeOutValue);
            PRINTF("Will wakeup in %d seconds.\r\n", timeOutValue);
            break;
        }
        case kAPP_WakeupSourceRTCAPI:
        {
            PRINTF("RTC API is selected as wakeup source!\r\n");
            WKPU_SetExternalWakeUpSourceConfig(APP_WKPU, kWKPU_Source0, &wkpu_config);
            timeOutValue = APP_GetWakeupTimeout();
            APP_SetWakeupRTCAPIConfiguration(timeOutValue);
            PRINTF("Will wakeup in %d seconds.\r\n", timeOutValue);
            break;
        }
        case kAPP_WakeupSourceRTC:
        {
            PRINTF("RTC is selected as wakeup source.\r\n");
            WKPU_SetExternalWakeUpSourceConfig(APP_WKPU, kWKPU_Source1, &wkpu_config);
            timeOutValue = APP_GetWakeupTimeout();
            APP_SetWakeupRTCConfiguration(timeOutValue);
            PRINTF("Will wakeup in %d seconds.\r\n", timeOutValue);
            break;
        }
        case kAPP_WakeupSourceLPCMPRoundRobinWakeupInterrupt:
        {
            PRINTF(
                "LPCMP round robin wake-up interrupt is selected as wakeup source. Please connect PTA0 with voltage \
                input and make sure it is lower than 1.6V.\r\n");
            WKPU_SetExternalWakeUpSourceConfig(APP_WKPU, kWKPU_Source2, &wkpu_config);
            timeOutValue = APP_GetWakeupTimeout();
            APP_SetWakeupLPCMPRoundRobinWakeupInterruptConfiguration(timeOutValue);
            PRINTF("Will wakeup in %d seconds.\r\n", timeOutValue);
            break;
        }
        case kAPP_WakeupSourceRTI:
        {
            PRINTF("RTI is selected as wakeup source.\r\n");
            WKPU_SetExternalWakeUpSourceConfig(APP_WKPU, kWKPU_Source3, &wkpu_config);
            timeOutValue = APP_GetWakeupTimeout();
            APP_SetWakeupRTIConfiguration(timeOutValue);
            PRINTF("Will wakeup in %d seconds.\r\n", timeOutValue);
            break;
        }
        case kAPP_WakeupSourceButton:
        {
            PRINTF("Wakeup Button is selected as wakeup source.\r\n");
            APP_SetWakeupPinConfiguration();
            WKPU_SetExternalWakeUpSourceConfig(APP_WKPU, kWKPU_Source44, &wkpu_config);
            PRINTF("Please press %s to wakeup.\r\n", APP_WAKEUP_BUTTON_NAME);
            break;
        }
        default:
            assert(false);
            break;
    }
}

static app_power_mode_t APP_GetTargetPowerMode(void)
{
    uint8_t ch;

    app_power_mode_t inputPowerMode;

    do
    {
        PRINTF("\r\nSelect the desired operation \n\r\n");
        for (app_power_mode_t modeIndex = kAPP_PowerModeRun; modeIndex <= kAPP_PowerModeStandby; modeIndex++)
        {
            PRINTF("\tPress %c to enter: %s mode\r\n", modeIndex,
                   g_modeNameArray[(uint8_t)(modeIndex - kAPP_PowerModeRun)]);
        }

        PRINTF("\r\nWaiting for power mode select...\r\n\r\n");

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        inputPowerMode = (app_power_mode_t)ch;

        if ((inputPowerMode > kAPP_PowerModeStandby) || (inputPowerMode < kAPP_PowerModeRun))
        {
            PRINTF("Wrong Input!");
        }
    } while (inputPowerMode > kAPP_PowerModeStandby);

    PRINTF("\t%s\r\n", g_modeDescArray[(uint8_t)(inputPowerMode - kAPP_PowerModeRun)]);

    return inputPowerMode;
}

static void APP_SetWakeupSWTConfiguration(uint8_t timeOut)
{
    swt_config_t config;

    SWT_GetDefaultConfig(&config);
    config.timeoutValue       = timeOut * APP_TIMER_CLOCK_FFEQUENCY;
    config.interruptThenReset = true;
    SWT_Init(APP_SWT, &config);

    /* External reset pin is not asserted on a 'functional' reset SWT0_RST event */
    MC_RGM_DisableBidirectionalReset(APP_MC_RGM, kMC_RGM_BidirectionalSwt0Reset);
    /* Functional reset event SWT0_RST generates an interrupt request */
    MC_RGM_DemoteFunctionalResetToInterrupt(APP_MC_RGM, kMC_RGM_Swt0Reset);

    SWT_ClearTimeoutResetFlag(APP_SWT);
}

static void APP_SetWakeupRTCAPIConfiguration(uint8_t timeOut)
{
    rtc_config_t rtcConfig;
    /*
     * RTC default configuration
     *     config->clockSource                       = (rtc_clock_source_t)0U;
     *     config->clockDivide                       = kRTC_ClockDivide1;
     *     config->registerAcessibleInSupervisorOnly = false;
     *     config->freezeCounterInDebugMode          = true;
     *     config->enableAnalogComparatorTrigger     = false;
     */
    RTC_GetDefaultConfig(&rtcConfig);
    rtcConfig.clockSource = APP_TIMER_CLOCK_SOURCE;
    RTC_Init(APP_RTC, &rtcConfig);
    RTC_SetAPIValue(APP_RTC, timeOut * APP_TIMER_CLOCK_FFEQUENCY);
    RTC_EnableRTC(APP_RTC);
    RTC_EnableAPI(APP_RTC);
}

static void APP_SetWakeupRTCConfiguration(uint8_t timeOut)
{
    rtc_config_t rtcConfig;
    /*
     * RTC default configuration
     *     config->clockSource                       = (rtc_clock_source_t)0U;
     *     config->clockDivide                       = kRTC_ClockDivide1;
     *     config->registerAcessibleInSupervisorOnly = false;
     *     config->freezeCounterInDebugMode          = true;
     *     config->enableAnalogComparatorTrigger     = false;
     */
    RTC_GetDefaultConfig(&rtcConfig);
    rtcConfig.clockSource = APP_TIMER_CLOCK_SOURCE;
    RTC_Init(APP_RTC, &rtcConfig);
    RTC_SetRTCValue(APP_RTC, timeOut * APP_TIMER_CLOCK_FFEQUENCY);
    RTC_EnableRTC(APP_RTC);
}

static void APP_SetWakeupLPCMPRoundRobinWakeupInterruptConfiguration(uint8_t timeOut)
{
    rtc_config_t rtcConfig;
    lpcmp_config_t mLpcmpConfigStruct;
    lpcmp_dac_config_t mLpcmpDacConfigStruct;
    lpcmp_roundrobin_config_t mLpcmpRoundRobinConfigStruct;
    const siul2_pin_settings_t pin_pta0_config = {
        .base         = SIUL2,
        .pinPortIdx   = 0u,
        .mux          = kPORT_MUX_ALT1,
        .safeMode     = kPORT_SAFE_MODE_DISABLED,
        .inputFilter  = kPORT_INPUT_FILTER_NOT_AVAILABLE,
        .pullConfig   = kPORT_INTERNAL_PULL_NOT_ENABLED,
        .pullKeep     = kPORT_PULL_KEEP_DISABLED,
        .invert       = kPORT_INVERT_DISABLED,
        .inputBuffer  = kPORT_INPUT_BUFFER_DISABLED,
        .outputBuffer = kPORT_OUTPUT_BUFFER_DISABLED,

        .initValue = 2u,
    };

    SIUL2_PinInit(&pin_pta0_config);

    /*
     * RTC default configuration
     *     config->clockSource                       = (rtc_clock_source_t)0U;
     *     config->clockDivide                       = kRTC_ClockDivide1;
     *     config->registerAcessibleInSupervisorOnly = false;
     *     config->freezeCounterInDebugMode          = true;
     *     config->enableAnalogComparatorTrigger     = false;
     */
    RTC_GetDefaultConfig(&rtcConfig);
    rtcConfig.clockSource = APP_TIMER_CLOCK_SOURCE;
    RTC_Init(APP_RTC, &rtcConfig);
    RTC_EnableAnalogComparatorTrigger(APP_RTC);
    RTC_SetAPIValue(APP_RTC, timeOut * APP_TIMER_CLOCK_FFEQUENCY);
    RTC_EnableRTC(APP_RTC);
    RTC_EnableAPI(APP_RTC);
    /*
     *   config->enableStopMode        = false;
     *   config->enableOutputPin       = false;
     *   config->useUnfilteredOutput   = false;
     *   config->enableInvertOutput    = false;
     *   config->enableNanoPowerMode   = false;
     *   config->enableHighSpeedMode   = false;
     *   config->hysteresisMode        = kLPCMP_HysteresisLevel0;
     *   config->functionalSourceClock = kLPCMP_FunctionalClockSource0;
     */
    LPCMP_GetDefaultConfig(&mLpcmpConfigStruct);

    /* Configure LPCMP input channels. */
    mLpcmpConfigStruct.plusInputSrc  = kLPCMP_PlusInputSrcDac;
    mLpcmpConfigStruct.minusInputSrc = kLPCMP_MinusInputSrcMux;

    /* Init the LPCMP module. */
    LPCMP_Init(APP_LPCMP, &mLpcmpConfigStruct);

    mLpcmpDacConfigStruct.referenceVoltageSource = kLPCMP_VrefSourceVin1;
    mLpcmpDacConfigStruct.DACValue =
        ((LPCMP_DCR_DAC_DATA_MASK >> LPCMP_DCR_DAC_DATA_SHIFT) >> 1U); /* Half of reference voltage. */
    LPCMP_SetDACConfig(APP_LPCMP, &mLpcmpDacConfigStruct);

    /* Configure the roundrobin mode. */
    mLpcmpRoundRobinConfigStruct.sampleClockNumbers = APP_LPCMP_ROUND_ROBIN_SAMPLE_CLOCK_NUMBERS;
    mLpcmpRoundRobinConfigStruct.initDelayModules   = APP_LPCMP_ROUND_ROBIN_INIT_DELAY_MODULES;

    /* The sampleTimeThreshhold can't bigger than channelSampleNumbers. */
    mLpcmpRoundRobinConfigStruct.fixedMuxPort       = APP_LPCMP_ROUND_ROBIN_FIXED_MUX_PORT;
    mLpcmpRoundRobinConfigStruct.fixedChannel       = APP_LPCMP_ROUND_ROBIN_FIXED_CHANNEL;
    mLpcmpRoundRobinConfigStruct.checkerChannelMask = APP_LPCMP_ROUND_ROBIN_CHANNELS_CHECKER_MASK;

    /* Disable roundrobin mode before configure related registers. */
    LPCMP_EnableRoundRobinMode(APP_LPCMP, false);

    LPCMP_SetRoundRobinConfig(APP_LPCMP, &mLpcmpRoundRobinConfigStruct);
    LPCMP_SetPreSetValue(APP_LPCMP, APP_LPCMP_ROUND_ROBIN_CHANNELS_PRE_STATE_MASK);

    LPCMP_EnableRoundRobinMode(APP_LPCMP, true);

    /* Enable the interrupt. */
    LPCMP_EnableInterrupts(APP_LPCMP, kLPCMP_RoundRobinInterruptEnable);
}

static void APP_SetWakeupRTIConfiguration(uint8_t timeOut)
{
    pit_config_t pitConfig;
    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);
    /* Init pit rti timer */
    PIT_RTI_Init(APP_PIT_RTI, &pitConfig);
    /* Set timer period for RTI */
    PIT_ClearRtiSyncStatus(APP_PIT_RTI);
    PIT_SetRtiTimerPeriod(APP_PIT_RTI, USEC_TO_COUNT(timeOut * 1000000U, APP_TIMER_CLOCK_FFEQUENCY));
    while(kPIT_RtiLoadValueSyncFlag != (PIT_GetRtiSyncStatus(APP_PIT_RTI)))
    {
    }
    /* Enable timer interrupts for RTI */
    PIT_ClearRtiStatusFlags(APP_PIT_RTI, kPIT_RtiTimerFlag);
    PIT_EnableRtiInterrupts(APP_PIT_RTI, kPIT_RtiTimerInterruptEnable);
    /* Enable timer interrupts for RTI */
    PIT_StartRtiTimer(APP_PIT_RTI);
    APP_DelayForPITRTIUpdate();
}

static void APP_SetWakeupPinConfiguration(void)
{
    SIUL2_SetPinInputBuffer(SIUL2, APP_WAKEUP_PIN_INDEX, true, 1, kPORT_INPUT_MUX_NO_INIT);
}

static void APP_DelayForPITRTIUpdate(void)
{
    /* The RTI requires several RTI clock cycles to get enabled or updated. */
    uint32_t rtiWait = 10 * CLOCK_GetFreq(kCLOCK_CoreSysClk) / APP_TIMER_CLOCK_FFEQUENCY;
    for (uint32_t i = 0; i < rtiWait; i++)
    {
        __ASM("NOP");
    }
}

static void APP_SetStandbyIoConfiguration(void)
{
    APP_DCMGPR->DCMRWF1 |= (uint32_t)DCM_GPR_DCMRWF1_STANDBY_IO_CONFIG_MASK;
}

static void APP_ClearStandbyIoConfiguration(void)
{
    APP_DCMGPR->DCMRWF1 &= ~((uint32_t)DCM_GPR_DCMRWF1_STANDBY_IO_CONFIG_MASK);
}

static void APP_SetStandbyClockConfiguration(void)
{
    /* Switch to FIRC as CPU clock. */
    CLOCK_AttachClk(kFIRC_CLK_to_MUX0);

    /* Adjust the divide */
    MC_CGM->MUX_0_DIV_TRIG_CTRL = MC_CGM_MUX_0_DIV_TRIG_CTRL_TCTL_MASK | MC_CGM_MUX_0_DIV_TRIG_CTRL_HHEN_MASK;
    MC_CGM->MUX_0_DC_0          = MC_CGM_MUX_0_DC_0_DIV(0U);
    MC_CGM->MUX_0_DC_1          = MC_CGM_MUX_0_DC_1_DIV(0U);
    MC_CGM->MUX_0_DC_2          = MC_CGM_MUX_0_DC_2_DIV(1U);
    MC_CGM->MUX_0_DC_3          = MC_CGM_MUX_0_DC_3_DIV(0U);
    MC_CGM->MUX_0_DC_4          = MC_CGM_MUX_0_DC_4_DIV(0U);
    MC_CGM->MUX_0_DC_5          = MC_CGM_MUX_0_DC_6_DIV(0U);
    MC_CGM->MUX_0_DIV_TRIG      = MC_CGM_MUX_0_DIV_TRIG_TRIGGER(1u);
    while ((MC_CGM->MUX_0_DIV_UPD_STAT & MC_CGM_MUX_0_DIV_UPD_STAT_DIV_STAT_MASK) != 0)
    {
    }
    while ((MC_CGM->MUX_0_CSS & MC_CGM_MUX_0_CSS_SWIP_MASK) != 0)
    {
    }

    /* Disable PLL */
    PLL->PLLCR = PLL_PLLCR_PLLPD_MASK;
    CLOCK_DisableClock(kCLOCK_Pll);
    /* Disbale Fxosc */
    CLOCK_DinitFxosc();
    CLOCK_DisableClock(kCLOCK_Fxosc);
    /* Disable Sxosc */
    SXOSC->SXOSC_CTRL &= ~SXOSC_SXOSC_CTRL_OSCON_MASK;
    CLOCK_DisableClock(kCLOCK_Sxosc);

    /* Configure flash wait states */
    FLASH->CTL = FLASH_CTL_RWSC(1u);
    /* Configure SRAM read wait states */
    PRAMC_0->PRCR1 = 0U;
    PRAMC_1->PRCR1 = 0U;

    /* Disable FIRC and enable SIRC in standby mode*/
    CLOCK_DisableFircInStandbyMode();
    CLOCK_EnableSircInStandbyMode();

    SystemCoreClockUpdate();
}

static void APP_PowerPreSwitchHook(void)
{
    /* Wait for debug console output finished. */
    while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
    {
    }

    APP_DeinitDebugConsole();
}

static void APP_PullDownJTAGTDI()
{
    SIUL2_SetPinPullSel(SIUL2, 69, kPORT_INTERNAL_PULL_DOWN_ENABLED);
}

static void APP_SetStandbyPowerPMCVoltageRegulatorConfiguration(void)
{
#if defined(FSL_FEATURE_PMC_HAS_LAST_MILE_REGULATOR) && (FSL_FEATURE_PMC_HAS_LAST_MILE_REGULATOR)
    POWER_DisableLastMileRegulator();
#endif
    POWER_DisableLPMV25Regulator();
}

static void APP_DisableUnusedPeripheralClock(void)
{
    /* Disable unused peripheral clock. */
    /* PRTN0_COFB1 peripherals */
    CLOCK_DisableClock(kCLOCK_Trgmux);
    CLOCK_DisableClock(kCLOCK_Bctu);
    CLOCK_DisableClock(kCLOCK_Emios0);
    CLOCK_DisableClock(kCLOCK_Emios1);
    CLOCK_DisableClock(kCLOCK_Emios2);
    CLOCK_DisableClock(kCLOCK_Lcu0);
    CLOCK_DisableClock(kCLOCK_Lcu1);
    CLOCK_DisableClock(kCLOCK_Adc0);
    CLOCK_DisableClock(kCLOCK_Adc1);
    CLOCK_DisableClock(kCLOCK_Adc2);
    CLOCK_DisableClock(kCLOCK_Pit1);

    /* PRTN1_COFB0 peripherals  */
    CLOCK_DisableClock(kCLOCK_Edma);
    /* TCD 0-11 are controlled through EDMA */
    CLOCK_DisableClock(kCLOCK_Eim);
    CLOCK_DisableClock(kCLOCK_Erm);
    CLOCK_DisableClock(kCLOCK_Stm0);

    /* PRTN1_COFB1 peripherals */
    CLOCK_DisableClock(kCLOCK_Dmamux0);
    CLOCK_DisableClock(kCLOCK_Dmamux1);
    CLOCK_DisableClock(kCLOCK_Vwrap);
    CLOCK_DisableClock(kCLOCK_Cmu05);
    CLOCK_DisableClock(kCLOCK_Tspc);
    CLOCK_DisableClock(kCLOCK_Pit2);

    /* PRTN1_COFB2 peripherals */
    CLOCK_DisableClock(kCLOCK_Flexcan0);
    CLOCK_DisableClock(kCLOCK_Flexcan1);
    CLOCK_DisableClock(kCLOCK_Flexcan2);
    CLOCK_DisableClock(kCLOCK_Flexcan3);
    CLOCK_DisableClock(kCLOCK_Flexcan4);
    CLOCK_DisableClock(kCLOCK_Flexcan5);
    CLOCK_DisableClock(kCLOCK_Flexio);
    CLOCK_DisableClock(kCLOCK_Lpuart0);
    CLOCK_DisableClock(kCLOCK_Lpuart1);
    CLOCK_DisableClock(kCLOCK_Lpuart2);
    CLOCK_DisableClock(kCLOCK_Lpuart3);
#if defined(FSL_FEATURE_SOC_LPUART_COUNT) && (FSL_FEATURE_SOC_LPUART_COUNT > 4U)
    CLOCK_DisableClock(kCLOCK_Lpuart4);
    CLOCK_DisableClock(kCLOCK_Lpuart6);
    CLOCK_DisableClock(kCLOCK_Lpuart7);
#endif
    CLOCK_DisableClock(kCLOCK_Lpi2c0);
    CLOCK_DisableClock(kCLOCK_Lpi2c1);
    CLOCK_DisableClock(kCLOCK_Lpspi0);
    CLOCK_DisableClock(kCLOCK_Lpspi1);
    CLOCK_DisableClock(kCLOCK_Lpspi2);
    CLOCK_DisableClock(kCLOCK_Lpspi3);
    CLOCK_DisableClock(kCLOCK_Sai0);
    CLOCK_DisableClock(kCLOCK_Lpcmp0);
    CLOCK_DisableClock(kCLOCK_TempSensor);

    /* PRTN1_COFB3 peripherals */
    CLOCK_DisableClock(kCLOCK_Crc0);

    /* PRTN2_COFB0 peripherals */
    CLOCK_DisableClock(kCLOCK_Tcd12);    // Maps to MCME_TCD12
    CLOCK_DisableClock(kCLOCK_Tcd13);    // Maps to MCME_TCD13
    CLOCK_DisableClock(kCLOCK_Tcd14);    // Maps to MCME_TCD14
    CLOCK_DisableClock(kCLOCK_Tcd15);    // Maps to MCME_TCD15
    CLOCK_DisableClock(kCLOCK_Tcd16);    // Maps to MCME_TCD16
    CLOCK_DisableClock(kCLOCK_Tcd17);    // Maps to MCME_TCD17
    CLOCK_DisableClock(kCLOCK_Tcd18);    // Maps to MCME_TCD18
    CLOCK_DisableClock(kCLOCK_Tcd19);    // Maps to MCME_TCD19
    CLOCK_DisableClock(kCLOCK_Tcd20);    // Maps to MCME_TCD20
    CLOCK_DisableClock(kCLOCK_Tcd21);    // Maps to MCME_TCD21
    CLOCK_DisableClock(kCLOCK_Tcd22);    // Maps to MCME_TCD22
    CLOCK_DisableClock(kCLOCK_Tcd23);    // Maps to MCME_TCD23
    CLOCK_DisableClock(kCLOCK_Tcd24);    // Maps to MCME_TCD24
    CLOCK_DisableClock(kCLOCK_Tcd25);    // Maps to MCME_TCD25
    CLOCK_DisableClock(kCLOCK_Tcd26);    // Maps to MCME_TCD26
    CLOCK_DisableClock(kCLOCK_Tcd27);    // Maps to MCME_TCD27
    CLOCK_DisableClock(kCLOCK_Tcd28);    // Maps to MCME_TCD28
    CLOCK_DisableClock(kCLOCK_Tcd29);    // Maps to MCME_TCD29
    CLOCK_DisableClock(kCLOCK_Tcd30);    // Maps to MCME_TCD30
    CLOCK_DisableClock(kCLOCK_Tcd31);    // Maps to MCME_TCD31
    CLOCK_DisableClock(kCLOCK_Sema42);   // Maps to MCME_SEMA42
    CLOCK_DisableClock(kCLOCK_Stm1);     // Maps to MCME_STM1

    /* PRTN2_COFB1 peripherals */
    CLOCK_DisableClock(kCLOCK_Emac);
#if defined(FSL_FEATURE_SOC_LPUART_COUNT) && (FSL_FEATURE_SOC_LPUART_COUNT > 8U)
    CLOCK_DisableClock(kCLOCK_Lpuart8);
    CLOCK_DisableClock(kCLOCK_Lpuart9);
    CLOCK_DisableClock(kCLOCK_Lpuart10);
    CLOCK_DisableClock(kCLOCK_Lpuart11);
    CLOCK_DisableClock(kCLOCK_Lpuart12);
    CLOCK_DisableClock(kCLOCK_Lpuart13);
    CLOCK_DisableClock(kCLOCK_Lpuart14);
    CLOCK_DisableClock(kCLOCK_Lpuart15);
#endif
#if defined(FSL_FEATURE_SOC_LPSPI_COUNT) && (FSL_FEATURE_SOC_LPUART_COUNT == 6U)
    CLOCK_DisableClock(kCLOCK_Lpspi4);
    CLOCK_DisableClock(kCLOCK_Lpspi5);
#endif
    CLOCK_DisableClock(kCLOCK_Qspi);
    CLOCK_DisableClock(kCLOCK_Sai1);
    CLOCK_DisableClock(kCLOCK_Lpcmp2);
}