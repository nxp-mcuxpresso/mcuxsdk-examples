/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_llwu.h"
#include "fsl_lptmr.h"
#include "fsl_msmc.h"
#include "fsl_pmc0.h"
#include "fsl_mu.h"
#include "fsl_debug_console.h"

#include "board.h"
#include "app.h"
#include "lpm.h"
#include "app_srtm.h"
#include "power_mode_switch.h"

/*******************************************************************************
 * Struct Definitions
 ******************************************************************************/
#define LLWU_WAKEUP_PIN_IDX    (1U) /* LLWU_P1 used for VOL+ button */
#define LLWU_WAKEUP_PIN_TYPE   kLLWU_ExternalPinFallingEdge
#define APP_WAKEUP_BUTTON_NAME "VOL+"

typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceLptmr, /*!< Wakeup by LPTMR.        */
    kAPP_WakeupSourcePin    /*!< Wakeup by external pin. */
} app_wakeup_source_t;

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
extern void APP_InitPMC0(void);
extern void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, lpm_power_mode_t targetMode);
extern void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, lpm_power_mode_t targetMode, bool result);
extern void APP_UpdateSimDgo(uint32_t gpIdx, uint32_t mask, uint32_t value);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_wakeupTimeout;            /* Wakeup timeout. (Unit: Second) */
static app_wakeup_source_t s_wakeupSource; /* Wakeup source.                 */
static SemaphoreHandle_t s_wakeupSig;
static const char *s_modeNames[] = {"RUN", "WAIT", "STOP", "VLPR", "VLPW", "VLPS", "HSRUN", "LLS", "VLLS"};
bool disableWirelessPinsInVLLS;
bool disableJtagPinsInVLLS;

/*******************************************************************************
 * Function Code
 ******************************************************************************/

/* LLWU interrupt handler. */
void APP_LLWU0_IRQHandler(void)
{
    bool wakeup = false;

    if (LLWU_GetInternalWakeupModuleFlag(LLWU, LLWU_MODULE_LPTMR1))
    {
        /* Woken up by LPTMR, then clear LPTMR flag. */
        LPTMR_ClearStatusFlags(LPTMR1, kLPTMR_TimerCompareFlag);
        LPTMR_DisableInterrupts(LPTMR1, kLPTMR_TimerInterruptEnable);
        LPTMR_StopTimer(LPTMR1);
        wakeup = true;
    }

    if (LLWU_GetExternalWakeupPinFlag(LLWU, LLWU_WAKEUP_PIN_IDX))
    {
        /* Woken up by external pin. */
        LLWU_ClearExternalWakeupPinFlag(LLWU, LLWU_WAKEUP_PIN_IDX);
        wakeup = true;
    }

    if (LLWU_GetInternalWakeupModuleFlag(LLWU, SYSTICK_LLWU_MODULE))
    {
        /* Woken up by Systick LPTMR, then clear LPTMR flag. */
        LPTMR_ClearStatusFlags(SYSTICK_BASE, kLPTMR_TimerCompareFlag);
    }

    if (LLWU_GetInternalWakeupModuleFlag(LLWU, LLWU_MODULE_USBPHY))
    {
        /* Woken up by USB PHY, then need to wakeup. */
        wakeup = true;
    }

    if (wakeup)
    {
        xSemaphoreGiveFromISR(s_wakeupSig, NULL);
        portYIELD_FROM_ISR(pdTRUE);
    }
}

/* LPTMR1 interrupt handler. */
void LPTMR1_IRQHandler(void)
{
    bool wakeup = false;

    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(LPTMR1))
    {
        LPTMR_ClearStatusFlags(LPTMR1, kLPTMR_TimerCompareFlag);
        LPTMR_DisableInterrupts(LPTMR1, kLPTMR_TimerInterruptEnable);
        LPTMR_StopTimer(LPTMR1);
        wakeup = true;
    }

    if (wakeup)
    {
        xSemaphoreGiveFromISR(s_wakeupSig, NULL);
        portYIELD_FROM_ISR(pdTRUE);
    }

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
    exception return operation might vector to incorrect interrupt */
    __DSB();
}

static void APP_IRQDispatcher(IRQn_Type irq, void *param)
{
    switch (irq)
    {
        case LLWU0_IRQn:
            APP_LLWU0_IRQHandler();
            break;
        case SNVS_IRQn:
            break;
        case PCTLA_IRQn:
            if ((1U << APP_PIN_IDX(APP_PIN_VOL_PLUS)) & PORT_GetPinsInterruptFlags(PORTA))
            {
                /* Flag will be cleared by app_srtm.c */
                xSemaphoreGiveFromISR(s_wakeupSig, NULL);
                portYIELD_FROM_ISR(pdTRUE);
            }
            break;
        case PCTLB_IRQn:
            break;
        default:
            break;
    }
}

static void APP_ShowPowerMode(smc_power_state_t powerMode)
{
    switch (powerMode)
    {
        case kSMC_PowerStateRun:
            PRINTF("    Power mode: RUN\r\n");
            break;
        case kSMC_PowerStateVlpr:
            PRINTF("    Power mode: VLPR\r\n");
            break;
        case kSMC_PowerStateHsrun:
            PRINTF("    Power mode: HSRUN\r\n");
            break;
        default:
            PRINTF("    Power mode wrong\r\n");
            break;
    }
}

/* Get input from user about wakeup timeout. */
static uint8_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout = 0U;
    uint8_t c;

    while (1)
    {
        PRINTF("Select the wake up timeout in seconds.\r\n");
        PRINTF("The allowed range is 1s ~ 999s.\r\n");
        PRINTF("Eg. enter 5 to wake up in 5 seconds.\r\n");
        PRINTF("\r\nWaiting for input timeout value...\r\n\r\n");

        do
        {
            c = GETCHAR();
            if ((c >= '0') && (c <= '9'))
            {
                PRINTF("%c", c);
                timeout = timeout * 10U + c - '0';
            }
            else if ((c == '\r') || (c == '\n'))
            {
                break;
            }
            else
            {
                PRINTF("%c\r\nWrong value!\r\n", c);
                timeout = 0U;
            }
        } while (timeout != 0U && timeout < 100U);

        if (timeout > 0U)
        {
            PRINTF("\r\n");
            break;
        }
    }

    return timeout;
}

/* Get wakeup source by user input. */
static app_wakeup_source_t APP_GetWakeupSource(void)
{
    uint8_t ch;

    while (1)
    {
        PRINTF("Select the wake up source:\r\n");
        PRINTF("Press T for LPTMR - Low Power Timer\r\n");
        PRINTF("Press S for switch/button %s. \r\n", APP_WAKEUP_BUTTON_NAME);

        PRINTF("\r\nWaiting for key press..\r\n\r\n");

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        if (ch == 'T')
        {
            return kAPP_WakeupSourceLptmr;
        }
        else if (ch == 'S')
        {
            return kAPP_WakeupSourcePin;
        }
        else
        {
            PRINTF("Wrong value!\r\n");
        }
    }
}

/* Get wakeup timeout and wakeup source. */
static void APP_GetWakeupConfig(void)
{
    /* Get wakeup source by user input. */
    s_wakeupSource = APP_GetWakeupSource();

    if (kAPP_WakeupSourceLptmr == s_wakeupSource)
    {
        /* Wakeup source is LPTMR, user should input wakeup timeout value. */
        s_wakeupTimeout = APP_GetWakeupTimeout();
        PRINTF("Will wakeup in %d seconds.\r\n", s_wakeupTimeout);
    }
    else
    {
        PRINTF("Press %s to wake up.\r\n", APP_WAKEUP_BUTTON_NAME);
    }
}

static void APP_SetWakeupConfig(lpm_power_mode_t targetMode)
{
    if (kAPP_WakeupSourceLptmr == s_wakeupSource)
    {
        LPTMR_SetTimerPeriod(LPTMR1, (1000UL * s_wakeupTimeout / 16U));
        LPTMR_StartTimer(LPTMR1);
        LPTMR_EnableInterrupts(LPTMR1, kLPTMR_TimerInterruptEnable);
    }

    /* To avoid conflicting access of LLWU with SRTM dispatcher, we put the LLWU setting into SRTM dispatcher context.*/
    /* If targetMode is VLLS/LLS, setup LLWU. */
    if ((LPM_PowerModeLls == targetMode) || (LPM_PowerModeVlls == targetMode))
    {
        if (kAPP_WakeupSourceLptmr == s_wakeupSource)
        {
            /* Set LLWU LPTMR1 module wakeup source. */
            APP_SRTM_SetWakeupModule(LLWU_MODULE_LPTMR1, true);
        }
        else
        {
            /* Set PORT and LLWU wakeup pin. */
            APP_SRTM_SetWakeupPin(APP_PIN_VOL_PLUS, (uint16_t)LLWU_WAKEUP_PIN_TYPE | 0x100);
        }
    }
    else
    {
        /* Set PORT pin. */
        if (kAPP_WakeupSourcePin == s_wakeupSource)
        {
            APP_SRTM_SetWakeupPin(APP_PIN_VOL_PLUS, (uint16_t)LLWU_WAKEUP_PIN_TYPE);
        }
    }
}

static void APP_ClearWakeupConfig(lpm_power_mode_t targetMode)
{
    if (kAPP_WakeupSourcePin == s_wakeupSource)
    {
        APP_SRTM_SetWakeupPin(APP_PIN_VOL_PLUS, (uint16_t)kLLWU_ExternalPinDisable);
    }
    else if ((LPM_PowerModeLls == targetMode) || (LPM_PowerModeVlls == targetMode))
    {
        APP_SRTM_SetWakeupModule(LLWU_MODULE_LPTMR1, false);
    }
}

static status_t APP_PowerModeSwitch(smc_power_state_t curPowerState, lpm_power_mode_t targetPowerMode)
{
    status_t status = kStatus_Success;

    switch (targetPowerMode)
    {
        case LPM_PowerModeVlpr:
            APP_SetClockVlpr();
            status = SMC_SetPowerModeVlpr(MSMC0);
            while (kSMC_PowerStateVlpr != SMC_GetPowerModeState(MSMC0))
            {
            }
            break;

        case LPM_PowerModeRun:
            /* If enter RUN from HSRUN, fisrt change clock. */
            if (kSMC_PowerStateHsrun == curPowerState)
            {
                APP_SetClockRunFromHsrun();
            }

            /* Power mode change. */
            status = SMC_SetPowerModeRun(MSMC0);
            while (kSMC_PowerStateRun != SMC_GetPowerModeState(MSMC0))
            {
            }

            /* If enter RUN from VLPR, change clock after the power mode change. */
            if (kSMC_PowerStateVlpr == curPowerState)
            {
                APP_SetClockRunFromVlpr();
            }
            break;

        case LPM_PowerModeHsrun:
            status = SMC_SetPowerModeHsrun(MSMC0);
            while (kSMC_PowerStateHsrun != SMC_GetPowerModeState(MSMC0))
            {
            }

            APP_SetClockHsrun(); /* Change clock setting after power mode change. */
            break;

        default:
            PRINTF("Wrong value\r\n");
            break;
    }

    if (status != kStatus_Success)
    {
        PRINTF("!!!! Power switch failed !!!!!\r\n");
    }

    return status;
}

static uint32_t APP_GetInputNumWithEcho(uint32_t length, bool allowZero)
{
    uint8_t ch;
    uint8_t digBuffer[8U] = {0U};
    uint8_t i, j;
    uint8_t digCount = 0U;
    uint32_t temp1, temp2 = 0U;
    uint32_t result  = 0U;
    bool getFirstDig = false;

    assert(length <= (sizeof(digBuffer) / sizeof(digBuffer[0U])));

    /* Get user input and echo it back to terminal. */
    for (;;)
    {
        ch = GETCHAR();

        if (('a' <= ch) && ('f' >= ch))
        {
            ch = ch - ('a' - 'A');
        }

        if (((('0' <= ch) && ('9' >= ch)) || (('A' <= ch) && ('F' >= ch))) && (digCount < length))
        {
            if (false == getFirstDig)
            {
                if (allowZero || ('0' < ch))
                {
                    getFirstDig = true;
                }
                else
                {
                    continue;
                }
            }

            PUTCHAR(ch);
            digBuffer[digCount] = ch;
            digCount++;
        }
        else if ((0x7FU == ch) && (0U != digCount))
        {
            digBuffer[digCount] = 0x0U;
            digCount--;
            PUTCHAR(0x7FU);
        }
        else
        {
            if ('\r' == ch)
            {
                break;
            }
        }
    }

    /* Reconstruct user input number. */
    temp1 = digCount - 1;
    for (i = 0; i < digCount; i++)
    {
        if (('0' <= digBuffer[i]) && ('9' >= digBuffer[i]))
        {
            temp2 = digBuffer[i] - '0';
        }
        else if (('A' <= digBuffer[i]) && ('F' >= digBuffer[i]))
        {
            temp2 = digBuffer[i] - 'A' + 10U;
        }
        else
        {
        }

        for (j = 0U; j < temp1; j++)
        {
            temp2 *= 16U;
        }
        temp1--;
        result += temp2;
    }

    return result;
}

static void APP_ReadPmicRegister(void)
{
    uint32_t reg;
    uint32_t value;

    for (;;)
    {
        /* Scroll to a new page. */
        PRINTF("Please select the register address to dump(0~FF):");
        reg = APP_GetInputNumWithEcho(2U, true);
        if (0xFFU >= reg)
        {
            break;
        }
    }
    PRINTF("\r\n");

    /* Read register from PF1550 in SRTM dispatcher context */
    value = APP_SRTM_GetPmicReg(reg);

    PRINTF("\r\nDump Format: [Register Address] = Register Content:\r\n");
    PRINTF("[0x%x] = 0x%x\r\n", reg, value);
    PRINTF("\r\n");
    PRINTF("Press Any Key to Home Page...");
    GETCHAR();
}

static void APP_SetPmicRegister(void)
{
    uint32_t reg;
    uint32_t value;
    uint32_t temp;

    for (;;)
    {
        /* Scroll to a new page. */
        PRINTF("Please input the register address to set(0~FF):");
        reg = APP_GetInputNumWithEcho(2U, true);
        if (255U >= reg)
        {
            break;
        }
    }
    PRINTF("\r\n");

    for (;;)
    {
        /* Scroll to a new page. */
        PRINTF("Please input the register content to set(0~FF):");
        value = APP_GetInputNumWithEcho(2U, true);
        if (0xFFU >= value)
        {
            break;
        }
    }
    PRINTF("\r\n");

    /* Set register content to PF1550 in SRTM dispatcher context */
    APP_SRTM_SetPmicReg(reg, value);

    /* Read register from PF1550 in SRTM dispatcher context */
    temp = APP_SRTM_GetPmicReg(reg);

    PRINTF("\r\nDump Format: [Register Address] = Register Content:\r\n");
    PRINTF("[0x%x] = 0x%x\r\n", reg, temp);
    PRINTF("\r\n");
    PRINTF("Press Any Key to Home Page...");
    GETCHAR();
}

static void APP_PowerTestMode(void)
{
    uint32_t cmd;
    uint32_t value;

    for (;;)
    {
        PRINTF("[Power Test Mode]\r\n");
        PRINTF("Commands:\r\n");
        /* Should not disable SW3, just for trial */
        PRINTF("0: Toggle PMIC SW3 regulator");
        PRINTF(" - Just for trial. Should not do it on EVK board.\r\n");
        /* Should not disable DDR_SW_EN, just for trial */
        PRINTF("1: Toggle DDR_SW_EN load switch");
        PRINTF(" - Just for trial. Should not do it on EVK board.\r\n");
        /* Have impact on Linux WIFI feature resume, but can reduce leakage. */
        PRINTF("2: %s WL_REG_ON/BT_REG_ON pins in VLLS\r\n", disableWirelessPinsInVLLS ? "Enable" : "Disable");
        PRINTF("   - Disable these pins leads to failure in Linux WIFI driver resume.\r\n");
        PRINTF("   - Just for showing minimum board leakage. \r\n");
        /* Have impact on debug feature during suspend/resume. */
        PRINTF("3: %s JTAG pins in VLLS\r\n", disableJtagPinsInVLLS ? "Enable" : "Disable");
        /* May have risk in M4 running during PMIC standby mode. */
        PRINTF("4: Toggle PMIC LDO3 low power mode in VLLS.\r\n");
        PRINTF("Selection:");
        cmd = APP_GetInputNumWithEcho(1U, true);
        if (4 >= cmd)
        {
            break;
        }
    }
    PRINTF("\r\n");

    if (cmd == 0)
    {
        APP_SRTM_ToggleSW3();
    }
    else if (cmd == 1)
    {
        GPIO_PortToggle(GPIOB, 1U << 6);
    }
    else if (cmd == 2)
    {
        disableWirelessPinsInVLLS = !disableWirelessPinsInVLLS;
    }
    else if (cmd == 3)
    {
        disableJtagPinsInVLLS = !disableJtagPinsInVLLS;
    }
    else
    {
        value = APP_SRTM_GetPmicReg(0x53U);
        if ((value & 0x8U) == 0)
        {
            value |= 0x8U; /* Enable low power mode */
        }
        else
        {
            value &= ~0x8U; /* Disable low power mode */
        }
        APP_SRTM_SetPmicReg(0x53U, value);
    }

    PRINTF("Press Any Key to Home Page...");
    GETCHAR();
}

void APP_SetPowerMode(smc_power_state_t powerMode)
{
    switch (powerMode)
    {
        case kSMC_PowerStateRun:
            LPM_SetPowerMode(LPM_PowerModeRun);
            break;
        case kSMC_PowerStateVlpr:
            LPM_SetPowerMode(LPM_PowerModeVlpr);
            break;
        case kSMC_PowerStateHsrun:
            LPM_SetPowerMode(LPM_PowerModeHsrun);
            break;
        default:
            break;
    }
}

/* Power Mode Switch task */
void PowerModeSwitchTask(void *pvParameters)
{
    status_t status;
    lptmr_config_t lptmrConfig;
    smc_power_state_t curPowerState;
    mu_power_mode_t powerMode;
    lpm_power_mode_t targetPowerMode;
    uint32_t resetSrc;
    uint32_t freq = 0U;
    uint8_t ch;
    const char *errorMsg;

    /* As IRQ handler main entry locates in app_srtm.c to support services, here need an entry to handle application
     * IRQ events.
     */
    APP_SRTM_SetIRQHandler(APP_IRQDispatcher, NULL);
    /* Add Systick as LLS/VLLS wakeup source, depending on SYSTICK_LLWU_WAKEUP value. */
    APP_SRTM_SetWakeupModule(SYSTICK_LLWU_MODULE, SYSTICK_LLWU_WAKEUP);

    /* Setup LPTMR. */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;  /* Use LPO 1KHz as clock source. */
    lptmrConfig.bypassPrescaler      = false;
    lptmrConfig.value                = kLPTMR_Prescale_Glitch_3; /* Divide clock source by 16. */
    LPTMR_Init(LPTMR1, &lptmrConfig);
    NVIC_SetPriority(LPTMR1_IRQn, APP_LPTMR1_IRQ_PRIO);

    EnableIRQ(LPTMR1_IRQn);

    resetSrc = SMC_GetPreviousResetSources(MSMC0);
    PRINTF("\r\nMCU wakeup source 0x%x...\r\n", resetSrc);

    APP_SRTM_BootCA7();

    for (;;)
    {
        freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
        PRINTF("\r\n####################  Power Mode Switch Task ####################\n\r\n");
        PRINTF("    Build Time: %s--%s \r\n", __DATE__, __TIME__);
        PRINTF("    Core Clock: %dHz \r\n", freq);
        curPowerState = SMC_GetPowerModeState(MSMC0);
        APP_ShowPowerMode(curPowerState);
        PRINTF("\r\nSelect the desired operation \n\r\n");
        PRINTF("Press  %c for enter: RUN      - Normal RUN mode\r\n", kAPP_PowerModeRun);
        PRINTF("Press  %c for enter: WAIT     - Wait mode\r\n", kAPP_PowerModeWait);
        PRINTF("Press  %c for enter: STOP     - Stop mode\r\n", kAPP_PowerModeStop);
        PRINTF("Press  %c for enter: VLPR     - Very Low Power Run mode\r\n", kAPP_PowerModeVlpr);
        PRINTF("Press  %c for enter: VLPW     - Very Low Power Wait mode\r\n", kAPP_PowerModeVlpw);
        PRINTF("Press  %c for enter: VLPS     - Very Low Power Stop mode\r\n", kAPP_PowerModeVlps);
        PRINTF("Press  %c for enter: HSRUN    - High Speed RUN mode\r\n", kAPP_PowerModeHsrun);
        PRINTF("Press  %c for enter: LLS      - Low Leakage Stop mode\r\n", kAPP_PowerModeLls);
        PRINTF("Press  %c for enter: VLLS     - Very Low Leakage Stop mode\r\n", kAPP_PowerModeVlls);
        PRINTF("Press  Q for query CA7 core power status.\r\n");
        PRINTF("Press  W for wake up CA7 core in VLLS/VLPS.\r\n");
        PRINTF("Press  T for reboot CA7 core.\r\n");
        PRINTF("Press  U for shutdown CA7 core.\r\n");
        PRINTF("Press  V for boot CA7 core.\r\n");
        PRINTF("Press  R for read PF1550 Register.\r\n");
        PRINTF("Press  S for set PF1550 Register.\r\n");
        PRINTF("Press  Z for enhanced power configuration.\r\n");
        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");

        /* Wait for user response */
        do
        {
            ch = GETCHAR();
        } while ((ch == '\r') || (ch == '\n'));

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        targetPowerMode = (lpm_power_mode_t)(ch - 'A');

        if (targetPowerMode <= LPM_PowerModeVlls)
        {
            if (!LPM_IsTargetModeValid(targetPowerMode, &errorMsg))
            {
                assert(errorMsg);
                PRINTF(errorMsg);
            }
            else if (!LPM_SetPowerMode(targetPowerMode))
            {
                PRINTF("Some task doesn't allow to enter mode %s\r\n", s_modeNames[targetPowerMode]);
            }
            else if ((LPM_PowerModeRun == targetPowerMode) || (LPM_PowerModeHsrun == targetPowerMode) ||
                     (LPM_PowerModeVlpr == targetPowerMode))
            {
                /* If target mode is RUN/VLPR/HSRUN, switch directly. */
                APP_PowerPreSwitchHook(curPowerState, targetPowerMode);
                status = APP_PowerModeSwitch(curPowerState, targetPowerMode);
                APP_PowerPostSwitchHook(curPowerState, targetPowerMode, status == kStatus_Success);
            }
            else /* Idle task will handle the low power state. */
            {
                APP_GetWakeupConfig();
                APP_SetWakeupConfig(targetPowerMode);
                xSemaphoreTake(s_wakeupSig, portMAX_DELAY);
                if (targetPowerMode == LPM_PowerModeVlls)
                {
                    /* PMC0 need to be reconfigured after VLLS. */
                    APP_InitPMC0();
                }
                /* Need to reset power mode to avoid unintentional WFI. */
                curPowerState = SMC_GetPowerModeState(MSMC0);
                APP_SetPowerMode(curPowerState);
                /* The call might be blocked by SRTM dispatcher task. Must be called after power mode reset. */
                APP_ClearWakeupConfig(targetPowerMode);
            }
        }
        else if ('Q' == ch)
        {
            powerMode = MU_GetOtherCorePowerMode(MUA);
            switch (powerMode)
            {
                case kMU_PowerModeRun:
                    PRINTF("CA7 power mode RUN!\r\n");
                    break;

                case kMU_PowerModeWait:
                    PRINTF("CA7 power mode WAIT!\r\n");
                    break;

                case kMU_PowerModeStop:
                    PRINTF("CA7 power mode STOP/VLPS!\r\n");
                    break;

                case kMU_PowerModeDsm:
                    PRINTF("CA7 power mode LLS/VLLS!\r\n");
                    break;

                default:
                    PRINTF("Wrong power mode value %d!\r\n", (int32_t)powerMode);
                    break;
            }
        }
        else if ('W' == ch)
        {
            APP_SRTM_WakeupCA7();
        }
        else if ('T' == ch)
        {
            APP_SRTM_RebootCA7();
        }
        else if ('U' == ch)
        {
            APP_SRTM_ShutdownCA7();
        }
        else if ('V' == ch)
        {
            APP_SRTM_BootCA7();
        }
        else if ('R' == ch)
        {
            APP_ReadPmicRegister();
        }
        else if ('S' == ch)
        {
            APP_SetPmicRegister();
        }
        else if ('Z' == ch)
        {
            APP_PowerTestMode();
        }
        else
        {
            PRINTF("Invalid command %c[0x%x]\r\n", ch, ch);
        }

        PRINTF("\r\nNext loop\r\n");
    }
}

void vApplicationMallocFailedHook(void)
{
    PRINTF("Malloc Failed!!!\r\n");
}

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    uint32_t irqMask;
    smc_power_state_t curPowerState;
    lpm_power_mode_t targetPowerMode;
    bool result;

    irqMask = DisableGlobalIRQ();

    /* Only when no context switch is pending and no task is waiting for the scheduler
     * to be unsuspended then enter low power entry.
     */
    if (eTaskConfirmSleepModeStatus() != eAbortSleep)
    {
        targetPowerMode = LPM_GetPowerMode();
        if (targetPowerMode != LPM_PowerModeRun && targetPowerMode != LPM_PowerModeVlpr &&
            targetPowerMode != LPM_PowerModeHsrun)
        {
            /* Only wait when target power mode is not running */
            curPowerState = SMC_GetPowerModeState(MSMC0);
            APP_PowerPreSwitchHook(curPowerState, targetPowerMode);
            result = LPM_WaitForInterrupt((uint64_t)1000 * xExpectedIdleTime / configTICK_RATE_HZ);
            APP_PowerPostSwitchHook(curPowerState, targetPowerMode, result);
        }
    }

    EnableGlobalIRQ(irqMask);
}

/* Called in PowerModeSwitchTask */
static bool APP_LpmListener(lpm_power_mode_t curMode, lpm_power_mode_t newMode, void *data)
{
    PRINTF("WorkingTask %d: Transfer from %s to %s\r\n", (uint32_t)data, s_modeNames[curMode], s_modeNames[newMode]);

    /* Do necessary preparation for this mode change */

    return true; /* allow this switch */
}

/*!
 * @brief simulating working task.
 */
static void WorkingTask(void *pvParameters)
{
    LPM_RegisterPowerListener(APP_LpmListener, pvParameters);

    for (;;)
    {
        /* Use App task logic to replace vTaskDelay */
        PRINTF("Task %d is working now\r\n", (uint32_t)pvParameters);
        vTaskDelay(portMAX_DELAY);
    }
}

/*! @brief Main function */
int main(void)
{
    BOARD_InitHardware();

    APP_SRTM_Init();
    LPM_Init();

#if APP_ENABLE_GPIO_PAD_LOW_POWER
    /* NOTE: Please see the definition of APP_ENABLE_GPIO_PAD_LOW_POWER before using the DGO update here. */
    /* Set PTB/PTC/PTE to low range, PTA/PTF to high range to save power. Need to align with board design. */
    APP_UpdateSimDgo(11,
                     SIM_SIM_DGO_GP11_PTA_RANGE_CTRL_MASK | SIM_SIM_DGO_GP11_PTB_RANGE_CTRL_MASK |
                         SIM_SIM_DGO_GP11_PTC_RANGE_CTRL_MASK | SIM_SIM_DGO_GP11_PTE_RANGE_CTRL_MASK |
                         SIM_SIM_DGO_GP11_PTF_RANGE_CTRL_MASK,
                     SIM_SIM_DGO_GP11_PTA_RANGE_CTRL(2) | SIM_SIM_DGO_GP11_PTB_RANGE_CTRL(1) |
                         SIM_SIM_DGO_GP11_PTC_RANGE_CTRL(1) | SIM_SIM_DGO_GP11_PTE_RANGE_CTRL(1) |
                         SIM_SIM_DGO_GP11_PTF_RANGE_CTRL(2));
#endif

    s_wakeupSig = xSemaphoreCreateBinary();

    xTaskCreate(PowerModeSwitchTask, "Main Task", 512U, NULL, tskIDLE_PRIORITY + 1U, NULL);
    xTaskCreate(WorkingTask, "Working Task", configMINIMAL_STACK_SIZE, (void *)1, tskIDLE_PRIORITY + 2U, NULL);

    /* Start FreeRTOS scheduler. */
    vTaskStartScheduler();

    /* Application should never reach this point. */
    for (;;)
    {
    }
}
