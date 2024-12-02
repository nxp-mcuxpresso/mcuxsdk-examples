/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_clock.h"
#include "fsl_power.h"
#include "fsl_ipmq.h"
#include "fsl_aon_common.h"
#include "power_mode_switch.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_PrintBootInfo(void);
static app_power_mode_select_t DEMO_GetTargetPowerMode(void);
static void DEMO_EnterDesiredPowerMode(app_power_mode_select_t powerMode);

/*******************************************************************************
 * Variables
 ******************************************************************************/
char *const g_modeNameArray[] = DEMO_POWER_MODE_NAME;
char *const g_modeDescArray[] = DEMO_POWER_MODE_DESC;

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    uint32_t freq;
    app_power_mode_select_t selectedPowerMode;
    aon_config_t aonConfig;
    status_t ret;

    BOARD_InitHardware();

    PRINTF("\r\n###########################    Power Mode Switch Demo    ###########################\r\n");

    Aon_GetDefaultConfig(&aonConfig);
    ret = Aon_Init(&aonConfig);

    if (ret == kStatus_Fail)
    {
        PRINTF("\r\n###########################    AON initialization failed    ###########################\r\n");
        return 0;
    }

    DEMO_PrintBootInfo();
    freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    PRINTF("\t\tCM33 Clock = %dHz\t\r\n", freq);

    while (1)
    {
        /* Get target power mode to enter. */
        selectedPowerMode = DEMO_GetTargetPowerMode();

        if (selectedPowerMode != kAPP_PowerModeActive)
        {
            DEMO_EnterDesiredPowerMode(selectedPowerMode);
        }

        PRINTF("\r\n----------Next Loop ----------\r\n");
    }
}

void DEMO_MU_ErrorReport(void)
{
    PRINTF("MU message Eorror Report!!!\r\n");
}

static void DEMO_PrintBootInfo(void)
{
    power_boot_info_t bootInfo;
    status_t status;
    char *const wakeupReason[] = DEMO_WAKEUP_REASON;

    /* Get Boot Reason */
    do
    {
        status = Power_GetBootInfo(&bootInfo, DEMO_USER_DATA);
    } while (status != kStatus_Success);

    PRINTF("\t\tRetained RAM Block Mask: %x\t\r\n", bootInfo.retentionBlocks);
    if (bootInfo.rtcAlive == true)
    {
        PRINTF("\t\tRTC Alive: true\t\r\n");
    }
    else
    {
        PRINTF("\t\tRTC Alive: false\t\r\n");
    }
    PRINTF("\t\tWakeup Reason: %s\r\n", wakeupReason[(uint8_t)bootInfo.wakeupReason]);
}

static app_power_mode_select_t DEMO_GetTargetPowerMode(void)
{
    uint8_t ch;

    app_power_mode_select_t inputPowerMode;

    do
    {
        PRINTF("\r\nSelect the desired operation...\r\n");
        for (app_power_mode_select_t modeIndex = kAPP_PowerModeActive; modeIndex < kAPP_PowerModeMax; modeIndex++)
        {
            PRINTF("\tPress %c to enter: %s mode\r\n", modeIndex,
                   g_modeNameArray[(uint8_t)(modeIndex - kAPP_PowerModeActive)]);
        }

        PRINTF("\r\nWaiting for power mode select...\r\n\r\n");

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }
        inputPowerMode = (app_power_mode_select_t)ch;

        if ((inputPowerMode >= kAPP_PowerModeMax) || (inputPowerMode <= kAPP_PowerModeMin))
        {
            PRINTF("Wrong Input!");
        }
    } while (inputPowerMode >= kAPP_PowerModeMax);

    PRINTF("\t%s\r\n", g_modeDescArray[(uint8_t)(inputPowerMode - kAPP_PowerModeActive)]);

    return inputPowerMode;
}

static void DEMO_EnterDesiredPowerMode(app_power_mode_select_t powerMode)
{
    power_wakeup_source_t selectedWs;

    char ch;

    PRINTF("Selected desired wakeup source...\r\n");

    if (powerMode < kAPP_PowerModeDeepPowerDown1)
    {
        PRINTF("\r\nPress A to select %s as wakeup source.\r\n", DEMO_WAKEUP_PIN_NAME);
    }

    PRINTF("\r\nPress B to select %s as wakeup source.\r\n", DEMO_EXT_INT_PIN_NAME);

    ch = GETCHAR();

    if ((ch >= 'a') && (ch <= 'z'))
    {
        ch -= 'a' - 'A';
    }

    if (ch == 'A')
    {
        PRINTF("Please press %s on board to wakeup device.\r\n", DEMO_WAKEUP_PIN_NAME);
        selectedWs = DEMO_WAKEUP_PIN_WS_ID;
    }
    else if (ch == 'B')
    {
        PRINTF("Please press %s on board to wakeup device.\r\n", DEMO_EXT_INT_PIN_NAME);
        selectedWs = DEMO_EXT_INT_PIN_WS_ID;
    }

    Power_EnableWakeupSource(selectedWs);
    switch (powerMode)
    {
        case kAPP_PowerModeActive:
        {
            /* Nothing to do. */
            break;
        }
        case kAPP_PowerModeSleep:
        {
            Power_EnterSleep();
            break;
        }
        case kAPP_PowerModeDeepSleep:
        {
            (void)Power_EnterDeepSleep(DEMO_USER_DATA);
            break;
        }
        case kAPP_PowerModeDeepPowerDown1:
        {
            (void)Power_EnterDeepPowerDown1(DEMO_DPD1_RETAINED_RAM_BLOCKS, kPower_AonClkFreq0P75MHz,
                                            kPower_wakeupPolicy_CM0PFirst, -1, true, DEMO_USER_DATA);
            break;
        }
        case kAPP_PowerModeDeepPowerDown2:
        {
            (void)Power_EnterDeepPowerDown2(DEMO_DPD2_RETAINED_RAM_BLOCKS, kPower_AonClkFreq0P75MHz,
                                            kPower_wakeupPolicy_CM0PFirst, -1, true, 0U, DEMO_USER_DATA);
            break;
        }
        case kAPP_PowerModeDeepPowerDown3:
        {
            (void)Power_EnterDeepPowerDown3(-1, DEMO_USER_DATA);
            break;
        }
        case kAPP_PowerModeShutDown:
        {
            (void)Power_EnterShutDown(-1, DEMO_USER_DATA);
            break;
        }
        default:
        {
            break;
        }
    }
}
