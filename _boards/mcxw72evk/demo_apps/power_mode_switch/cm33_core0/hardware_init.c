/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_spc.h"
#include "peripherals.h"
#include "board.h"
#include "app.h"
#include "fsl_port.h"
#include "fsl_spc.h"
#include "fsl_cmc.h"
#include "fsl_vbat.h"
/*${header:end}*/

/*${function:start}*/
void APP_InitDebugConsole(void)
{
    /*
     * Debug console RX pin is set to disable for current leakage, need to re-configure pinmux.
     * Debug console TX pin: Don't need to change.
     */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

void APP_DeinitDebugConsole(void)
{
    DbgConsole_Deinit();
    /*
     * Set pin for current leakage.
     * Debug console RX pin: Set to pinmux to disable.
     * Debug console TX pin: Don't need to change.
     */
    PORT_SetPinMux(DEBUG_CONSOLE_RX_PORT, DEBUG_CONSOLE_RX_PIN, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(DEBUG_CONSOLE_TX_PORT, DEBUG_CONSOLE_TX_PIN, kPORT_PinDisabledOrAnalog);
}

void APP_DeinitWakeupButton(void)
{
    port_pin_config_t wakeupButtonDefaultConfig = {kPORT_PullDisable, (uint16_t)kPORT_LowPullResistor,
                                                   /* Slow slew rate is configured */
                                                   (uint16_t)kPORT_SlowSlewRate,
                                                   /* Passive input filter is disabled */
                                                   (uint16_t)kPORT_PassiveFilterDisable,
                                                   /* Open drain output is enabled */
                                                   (uint16_t)kPORT_OpenDrainEnable,
                                                   /* Low drive strength is configured */
                                                   (uint16_t)kPORT_LowDriveStrength,
                                                   /* Normal drive strength is configured */
                                                   (uint16_t)kPORT_NormalDriveStrength,
                                                   /* Pin is configured as default value */
                                                   (uint16_t)kPORT_PinDisabledOrAnalog,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   (uint16_t)kPORT_UnlockRegister};
    PORT_SetPinConfig(PORTC, 6U, &wakeupButtonDefaultConfig);
}

void APP_LPTMR_IRQ_HANDLER(void)
{
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(APP_LPTMR))
    {
        LPTMR_DisableInterrupts(APP_LPTMR, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(APP_LPTMR, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(APP_LPTMR);
    }
}

void APP_VBAT_IRQ_HANDLER(void)
{
    if (VBAT_GetStatusFlags(APP_VBAT) & kVBAT_StatusFlagWakeupPin)
    {
        VBAT_ClearStatusFlags(APP_VBAT, kVBAT_StatusFlagWakeupPin);
    }
}

void WUU0_IRQHandler(void)
{
    if ((WUU_GetExternalWakeUpPinsFlag(APP_WUU) & 0x800UL) != 0UL)
    {
        WUU_ClearExternalWakeUpPinsFlag(APP_WUU, 0x800UL);
    }
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitBootPeripherals();

    CLOCK_DeinitSysOsc();
    CLOCK_DeinitSirc();

    if ((CMC_GetSystemResetStatus(APP_CMC) & kCMC_WakeUpReset) != 0UL)
    {
        /* Close ISO. */
        SPC_ClearPeriphIOIsolationFlag(APP_SPC);

        if (SPC_CheckPowerDomainLowPowerRequest(APP_SPC, kSPC_PowerDomain0))
        {
            SPC_ClearPowerDomainLowPowerRequestFlag(APP_SPC, kSPC_PowerDomain0);
        }
        if (SPC_CheckPowerDomainLowPowerRequest(APP_SPC, kSPC_PowerDomain1))
        {
            SPC_ClearPowerDomainLowPowerRequestFlag(APP_SPC, kSPC_PowerDomain1);
        }
        if (SPC_CheckPowerDomainLowPowerRequest(APP_SPC, kSPC_PowerDomain2))
        {
            RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK));
            RFMC->RF2P4GHZ_CTRL &= ~RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;
            SPC_ClearPowerDomainLowPowerRequestFlag(APP_SPC, kSPC_PowerDomain2);
        }
        SPC_ClearLowPowerRequest(APP_SPC);
    }

    RFMC->RF2P4GHZ_CFG |= RFMC_RF2P4GHZ_CFG_FORCE_DBG_PWRUP_ACK_MASK;
}
/*${function:end}*/
