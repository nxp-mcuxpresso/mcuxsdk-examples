/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"

#include "app.h"
#include "pin_mux.h"
#include "board.h"
#include "app_srtm.h"
#include "fsl_gpc.h"
#include "sai_low_power_audio.h"
#include "lpm.h"
#include "fsl_debug_console.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
static LPM_POWER_STATUS_M4 m4_lpm_state = LPM_M4_STATE_RUN;
/* Using SRC_GPR9 register as the communication address with A core. */
#define ServiceFlagAddr SRC->GPR9
#define ServiceBusy     (0x5555U)
#define ServiceIdle     (0x0U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*
 * Give readable string of current M4 lpm state
 */
const char *LPM_MCORE_GetPowerStatusString(void)
{
    switch (m4_lpm_state)
    {
        case LPM_M4_STATE_RUN:
            return "RUN";
        case LPM_M4_STATE_WAIT:
            return "WAIT";
        case LPM_M4_STATE_STOP:
            return "STOP";
        default:
            return "UNKNOWN";
    }
}

void LPM_MCORE_ChangeM4Clock(LPM_M4_CLOCK_SPEED target)
{
    /* Change CCM Root to change M4 clock*/
    switch (target)
    {
        case LPM_M4_LOW_FREQ:
            if (CLOCK_GetRootMux(kCLOCK_RootM4) != kCLOCK_M4RootmuxOsc24M)
            {
                CLOCK_SetRootMux(kCLOCK_RootM4, kCLOCK_M4RootmuxOsc24M);
                CLOCK_SetRootDivider(kCLOCK_RootM4, 1U, 1U);
            }
            break;
        case LPM_M4_HIGH_FREQ:
            if (CLOCK_GetRootMux(kCLOCK_RootM4) != kCLOCK_M4RootmuxSysPll1)
            {
                CLOCK_SetRootDivider(kCLOCK_RootM4, 1U, 2U);
                CLOCK_SetRootMux(kCLOCK_RootM4, kCLOCK_M4RootmuxSysPll1); /* switch cortex-m4 to SYSTEM PLL1 */
            }
            break;
        default:
            break;
    }
}

void LPM_MCORE_SetPowerStatus(GPC_Type *base, LPM_POWER_STATUS_M4 targetPowerMode)
{
    gpc_lpm_config_t config;
    config.enCpuClk              = false;
    config.enFastWakeUp          = false;
    config.enDsmMask             = false;
    config.enWfiMask             = false;
    config.enVirtualPGCPowerdown = true;
    config.enVirtualPGCPowerup   = true;
    switch (targetPowerMode)
    {
        case LPM_M4_STATE_RUN:
            GPC->LPCR_M4 = GPC->LPCR_M4 & (~GPC_LPCR_M4_LPM0_MASK);
            break;
        case LPM_M4_STATE_WAIT:
            GPC_EnterWaitMode(GPC, &config);
            break;
        case LPM_M4_STATE_STOP:
            GPC_EnterStopMode(GPC, &config);
            break;
        default:
            break;
    }

    m4_lpm_state = targetPowerMode;
}
void PreSleepProcessing(void)
{
    APP_SRTM_Suspend();
    DbgConsole_Deinit();
}

void PostSleepProcessing(void)
{
    APP_SRTM_Resume();
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE,
                    BOARD_DEBUG_UART_CLK_FREQ);
}

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    uint32_t irqMask;
    uint64_t counter = 0;
    uint32_t timeoutTicks;
    uint32_t timeoutMilliSec = (uint64_t)1000 * xExpectedIdleTime / configTICK_RATE_HZ;

    irqMask = DisableGlobalIRQ();

    /* Only when no context switch is pending and no task is waiting for the scheduler
     * to be unsuspended then enter low power entry.
     */
    if (eTaskConfirmSleepModeStatus() != eAbortSleep)
    {
        timeoutTicks = LPM_EnterTicklessIdle(timeoutMilliSec, &counter);
        if (timeoutTicks)
        {
            if (APP_SRTM_ServiceIdle() && LPM_AllowSleep())
            {
                LPM_MCORE_ChangeM4Clock(LPM_M4_LOW_FREQ);
                LPM_MCORE_SetPowerStatus(BOARD_GPC_BASEADDR, LPM_M4_STATE_STOP);
                PRINTF("\r\nMode:%s\r\n", LPM_MCORE_GetPowerStatusString());
                PreSleepProcessing();
                ServiceFlagAddr = ServiceIdle;
                __DSB();
                __ISB();
                __WFI();
                ServiceFlagAddr = ServiceBusy;
                PostSleepProcessing();
                LPM_MCORE_ChangeM4Clock(LPM_M4_HIGH_FREQ);
                LPM_MCORE_SetPowerStatus(BOARD_GPC_BASEADDR, LPM_M4_STATE_RUN);
                PRINTF("\r\nMode:%s\r\n", LPM_MCORE_GetPowerStatusString());
            }
            else
            {
                __DSB();
                __ISB();
                __WFI();
            }
        }
        LPM_ExitTicklessIdle(timeoutTicks, counter);
    }

    EnableGlobalIRQ(irqMask);
}
void MainTask(void *pvParameters)
{
    uint8_t control_char;

    /* Treat M4 as busy status by default.*/
    ServiceFlagAddr = ServiceBusy;

    /*
     * Wait For A53 Side Become Ready
     */
    PRINTF("********************************\r\n");
    PRINTF("Please:\r\n");
    PRINTF("  1) Boot A53 kernel first to create the link between M core and A core;\r\n");
    PRINTF("  2) Then press \"s\" or \"S\" to start the demo.\r\n");
    PRINTF("********************************\r\n");

    for (;;)
    {
        control_char = GETCHAR();
        PRINTF("%c", control_char);
        if ((control_char == 's') || (control_char == 'S'))
        {
            break;
        }
    }
    PRINTF("\r\n");

    /* Configure GPC */
    GPC_Init(BOARD_GPC_BASEADDR, APP_PowerUpSlot, APP_PowerDnSlot);
    GPC_EnableIRQ(BOARD_GPC_BASEADDR, BOARD_MU_IRQ_NUM);
    GPC_EnableIRQ(BOARD_GPC_BASEADDR, SYSTICK_IRQn);
    GPC_EnableIRQ(BOARD_GPC_BASEADDR, I2C3_IRQn);
    while (true)
    {
        /* Use App task logic to replace vTaskDelay */
        PRINTF("\r\nTask %s is working now.\r\n", (char *)pvParameters);
        vTaskDelay(portMAX_DELAY);
    }
}
/*!
 * @brief Main function
 */
int main(void)
{
    char *taskID = "A";
    BOARD_InitHardware();
#if APP_SRTM_CODEC_USED_I2C
    APP_SRTM_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
#endif
    PRINTF("\r\n####################  LOW POWER AUDIO TASK ####################\n\r\n");
    PRINTF("    Build Time: %s--%s \r\n", __DATE__, __TIME__);

    APP_SRTM_Init();

    xTaskCreate(MainTask, "Main Task", 256U, (void *)taskID, tskIDLE_PRIORITY + 1U, NULL);

    /* Start FreeRTOS scheduler. */
    vTaskStartScheduler();

    /* Application should never reach this point. */
    for (;;)
    {
    }
}
void vApplicationMallocFailedHook(void)
{
    PRINTF("Malloc Failed!!!\r\n");
}
