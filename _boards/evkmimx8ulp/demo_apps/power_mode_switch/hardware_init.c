/*
 * Copyright 2021-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_rgpio.h"
#include "fsl_iomuxc.h"
#include "fsl_lpuart.h"
#include "fsl_lptmr.h"
#include "power_mode_switch.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "app_srtm.h"
#include "fsl_reset.h"
#include "fsl_upower.h"
#include "lpm.h"
#include "fsl_mu.h"
#include "fsl_fusion.h"
/*${header:end}*/

/*${function:start}*/
#define SKIP_JTAG_PINS (0)
extern lpm_ad_power_mode_e AD_CurrentMode;
extern pca9460_buck3ctrl_t buck3_ctrl;
extern pca9460_ldo1_cfg_t ldo1_cfg;
static uint32_t iomuxBackup[25 + 16 + 24]; /* Backup 25 PTA, 16 PTB and 24 PTC IOMUX registers */
static uint32_t gpioICRBackup[25 + 16 + 24];

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    UPOWER_PowerOnMemPart(0U, (uint32_t)kUPOWER_MP1_DMA0);

    CLOCK_SetIpSrcDiv(kCLOCK_Tpm0, kCLOCK_Pcc1BusIpSrcCm33Bus, 1U, 0U);
    CLOCK_SetIpSrcDiv(kCLOCK_Lpi2c0, kCLOCK_Pcc1BusIpSrcCm33Bus, 0U, 0U);
    CLOCK_SetIpSrcDiv(kCLOCK_Lpi2c1, kCLOCK_Pcc1BusIpSrcCm33Bus, 0U, 0U);
    /* Use Pll1Pfd2Div clock source 12.288MHz. */
    CLOCK_SetIpSrc(kCLOCK_Sai0, kCLOCK_Cm33SaiClkSrcPll1Pfd2Div);

    CLOCK_EnableClock(kCLOCK_Dma0Ch0);
    CLOCK_EnableClock(kCLOCK_Dma0Ch16);
    CLOCK_EnableClock(kCLOCK_Dma0Ch17);
    CLOCK_EnableClock(kCLOCK_RgpioA);
    CLOCK_EnableClock(kCLOCK_RgpioB);
    CLOCK_EnableClock(kCLOCK_Wuu0);
    CLOCK_EnableClock(kCLOCK_Bbnsm);

    RESET_PeripheralReset(kRESET_Sai0);
    RESET_PeripheralReset(kRESET_Lpi2c0);
    RESET_PeripheralReset(kRESET_Lpi2c1);
    RESET_PeripheralReset(kRESET_Tpm0);

    /* In order to enable the PDM record service located in FUSION_AO, FUSION needs to be initial. */
    Fusion_Init();

    APP_SRTM_Init();

    /* If RTD reset is due to DPD exit, should go different flow here */
    if (CMC_RTD->SSRS & CMC_SSRS_WAKEUP_MASK)
    {
        CMC_RTD->SSRS = CMC_SSRS_WAKEUP_MASK;
        /* Assume that Application Domain is entered Deep Power Down Mode */
        AD_CurrentMode = AD_DPD;
        /*
         * AD is also in Deep Power Down mode when RTD is in Deep Power Down Mode.
         * AD/RTD exiting from Deep Power Down Mode is same with cold boot flow.
         * So don't need setup TRDC when RTD exit from Deep Power Down mode.
         *
         */
        // BOARD_SetTrdcAfterApdReset();
        MU_Init(MU0_MUA);
        MU_BootOtherCore(MU0_MUA, (mu_core_boot_mode_t)0);
    }
    else
    {
        APP_SRTM_StartCommunication();
    }
}

static void APP_Suspend(void)
{
    uint32_t i;
    uint32_t setting;
    uint32_t backupIndex;
    lpm_rtd_power_mode_e targetPowerMode = LPM_GetPowerMode();
    SIM_SEC_Type *base = SIM_SEC;

    backupIndex = 0;

    /* Backup PTA IOMUXC and GPIOA ICR registers then disable */
    for (i = 0; i <= 24; i++)
    {
        iomuxBackup[backupIndex] = IOMUXC0->PCR0_IOMUXCARRAY0[i];

        gpioICRBackup[backupIndex] = GPIOA->ICR[i];

        GPIOA->ICR[i] = 0; /* Disable interrupts */

                           /*
                            * Skip PTA20 ~ 23(JTAG pins)[define SKIP_JTAG_PINS as 1] if want to debug code with JTAG before entering power
                            * down/deep sleep mode
                            */
#if SKIP_JTAG_PINS
        if ((i >= 20) && (i <= 23))
#endif
        {
#if 0
            if (targetPowerMode == LPM_PowerModeDeepSleep)
            {
                /*
                 * Failed to wakeup cortex-M33 by SW8 button after set any pads of PTA/PTB to Analog/HiZ state when RTD in
                 * deep sleep mode, so do nothing here for Deep Sleep Mode
                 */
            }
            else
#endif
            {
                IOMUXC0->PCR0_IOMUXCARRAY0[i] = 0;
            }
        }
        backupIndex++;
    }

    /* Backup PTB IOMUXC and GPIOB ICR registers then disable */
    for (i = 0; i <= 15; i++)
    {
        iomuxBackup[backupIndex] = IOMUXC0->PCR0_IOMUXCARRAY1[i];

        gpioICRBackup[backupIndex] = GPIOB->ICR[i];

#if 0
        if (targetPowerMode == LPM_PowerModeDeepSleep)
        {
            /*
             * Failed to wakeup cortext-M33 by SW8 button after disable any gpios's interrupt when RTD in Deep Sleep
             * Mode, so do nothing here for Deep Sleep Mode
             */
        }
        else
#endif
        {
            GPIOB->ICR[i] = 0; /* disable interrupts */
        }

        /*
         * If it's wakeup source, need to set as WUU0_P24
         * Power Down/Deep Power Down wakeup through WUU and NMI pin
         * Sleep/Deep Sleep wakeup via interrupt from M33 peripherals or external GPIO pins. WIC detects wakeup source.
         */
        if ((i == 12) && (WUU0->PE2 & WUU_PE2_WUPE24_MASK))
        {
            if (targetPowerMode == LPM_PowerModeDeepSleep)
            {
                /*
                 * Deep Sleep wakeup via interrupt not WUU,
                 * so do nothing in here for Deep Sleep Mode
                 */
                /* enable interrupts for PTB12 */
                GPIOB->ICR[i] = gpioICRBackup[backupIndex];
            }
            else
            {
                /*
                 * Disable interrupt temperarily to prevent glitch
                 * interrupt during switching IOMUXC pin selection
                 */
                setting = WUU0->PE2 & WUU_PE2_WUPE24_MASK;
                WUU0->PE2 &= !WUU_PE2_WUPE24_MASK;

                /* Change PTB12's function as WUU0_P24(IOMUXC_PTB12_WUU0_P24) */
                IOMUXC0->PCR0_IOMUXCARRAY1[i] = IOMUXC0_PCR0_IOMUXCARRAY1_MUX(13);

                WUU0->PE2 |= setting;
            }
        }
        else if ((i != 10) && (i != 11)) /* PTB10 and PTB11 is used as i2c function by upower */
        {
#if 0
            if (targetPowerMode == LPM_PowerModeDeepSleep)
            {
                /*
                 * Failed to wakeup cortex-M33 by sw8 button after setup any pads of PTA/PTB to Analog/HiZ state when
                 * RTD in Deep Sleep Mode, so do nothing here for Deep Sleep Mode
                 */
            }
            else
#endif
            {
                IOMUXC0->PCR0_IOMUXCARRAY1[i] = 0;
            }
        }
        backupIndex++;
    }

    /* Backup PTC IOMUXC and GPIOC ICR registers then disable */
    for (i = 0; i <= 23; i++)
    {
        iomuxBackup[backupIndex] = IOMUXC0->PCR0_IOMUXCARRAY2[i];

        gpioICRBackup[backupIndex] = GPIOC->ICR[i];

        GPIOC->ICR[i] = 0; /* disable interrupts */

        /* Skip PTC0 ~ 10(FlexSPI0 pins) if run on flash(XiP) */
        if ((i > 10) || !BOARD_IS_XIP_FLEXSPI0())
        {
            IOMUXC0->PCR0_IOMUXCARRAY2[i] = 0;
        }

        backupIndex++;
    }

    /* Cleare any potential interrupts before enter Power Down */
    WUU0->PF = WUU0->PF;

    /* Save SRTM context */
    APP_SRTM_Suspend();

    /* M33 will gate all clk in DSL/PD, disable FUSION platform clk before M33 enter DSL/PD mode to fix PD+DSL combination corner case in EVK9. */
    base->SYSCTRL0 &= ~(SIM_SEC_SYSCTRL0_FUSION_CLK_EN_MASK | SIM_SEC_SYSCTRL0_FUSION_PLAT_HCLK_EN_MASK);
    base->SYSCTRL0 &= ~(SIM_SEC_SYSCTRL0_FUSION_BCLK_EN_MASK | SIM_SEC_SYSCTRL0_FUSION_PBCLK_EN_MASK);
}

static void APP_Resume(bool resume)
{
    uint32_t i;
    uint32_t backupIndex;
    SIM_SEC_Type *base = SIM_SEC;

    backupIndex = 0;

    /* Restore PTA IOMUXC and GPIOA ICR registers */
    for (i = 0; i <= 24; i++)
    {
        IOMUXC0->PCR0_IOMUXCARRAY0[i] = iomuxBackup[backupIndex];
        GPIOA->ICR[i]                 = gpioICRBackup[backupIndex];
        backupIndex++;
    }

    /* Restore PTB IOMUXC and GPIOB ICR registers */
    for (i = 0; i <= 15; i++)
    {
        IOMUXC0->PCR0_IOMUXCARRAY1[i] = iomuxBackup[backupIndex];
        GPIOB->ICR[i]                 = gpioICRBackup[backupIndex];
        backupIndex++;
    }

    /* Restore PTC IOMUXC and GPIOC ICR registers */
    for (i = 0; i <= 23; i++)
    {
        IOMUXC0->PCR0_IOMUXCARRAY2[i] = iomuxBackup[backupIndex];
        GPIOC->ICR[i]                 = gpioICRBackup[backupIndex];
        backupIndex++;
    }

    EnableIRQ(WUU0_IRQn);

    /* Enable FUSION platform clk. */
    base->SYSCTRL0 |= SIM_SEC_SYSCTRL0_FUSION_CLK_EN_MASK | SIM_SEC_SYSCTRL0_FUSION_PLAT_HCLK_EN_MASK;
    base->SYSCTRL0 |= SIM_SEC_SYSCTRL0_FUSION_BCLK_EN_MASK | SIM_SEC_SYSCTRL0_FUSION_PBCLK_EN_MASK;

    APP_SRTM_Resume(resume);
}

/* Disable gpio to save power */
void APP_DisableGPIO(void)
{
    int i = 0;

    /* Disable PTA and set PTA to Analog/HiZ state to save power */
    for (i = 0; i <= 24; i++)
    {
        GPIOA->ICR[i] = 0; /* Disable interrupts */

                           /*
                            * Skip PTA20 ~ 23(JTAG pins)[define SKIP_JTAG_PINS as 1] if want to debug code with JTAG before entering deep
                            * power down mode
                            */
#if SKIP_JTAG_PINS
        if ((i >= 20) && (i <= 23))
#endif
        {
            IOMUXC0->PCR0_IOMUXCARRAY0[i] = 0; /* Set to Analog/HiZ state */
        }
    }

    /* Disable PTB and set PTB to Analog/HiZ state to save power */
    for (i = 0; i <= 15; i++)
    {
        if ((i != 10) && (i != 11))            /* PTB10 and PTB11 is used as i2c function by upower */
        {
            GPIOB->ICR[i]                 = 0; /* Disable interrupts */
            IOMUXC0->PCR0_IOMUXCARRAY1[i] = 0; /* Set to Analog/HiZ state */
        }
    }

    /* Disable PTC and set PTC to Analog/HiZ state to save power */
    for (i = 0; i <= 23; i++)
    {
        GPIOC->ICR[i] = 0; /* Disable interrupts */

        /* Skip PTC0 ~ 10(FlexSPI0 pins) if run on flash(XiP) */
        if ((i > 10) || !BOARD_IS_XIP_FLEXSPI0())
        {
            IOMUXC0->PCR0_IOMUXCARRAY2[i] = 0; /* Set to Analog/HiZ state */
        }
    }
}

void APP_PowerPreSwitchHook(lpm_rtd_power_mode_e targetMode)
{
    uint32_t setting;

    if ((LPM_PowerModeActive != targetMode))
    {
        /* Wait for debug console output finished. */
        while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
        {
        }
        DbgConsole_Deinit();
        /*
         * Set pin for current leakage.
         * Debug console RX pin: Set to pinmux to analog.
         * Debug console TX pin: Set to pinmux to analog.
         */
        IOMUXC_SetPinMux(IOMUXC_PTA10_LPUART1_TX, 0);
        IOMUXC_SetPinConfig(IOMUXC_PTA10_LPUART1_TX, 0);
        IOMUXC_SetPinMux(IOMUXC_PTA11_LPUART1_RX, 0);
        IOMUXC_SetPinConfig(IOMUXC_PTA11_LPUART1_RX, 0);

        if (LPM_PowerModePowerDown == targetMode || LPM_PowerModeDeepSleep == targetMode)
        {
            APP_Suspend();
        }
        else if (LPM_PowerModeDeepPowerDown == targetMode)
        {
            APP_DisableGPIO();
            /* If PTB12 is wakeup source, set to WUU0_P24 */
            if ((WUU0->PE2 & WUU_PE2_WUPE24_MASK) != 0)
            {
                /* Disable interrupt temperarily to prevent glitch
                 * interrupt during switching IOMUXC pin selection
                 */
                setting = WUU0->PE2 & WUU_PE2_WUPE24_MASK;
                WUU0->PE2 &= !WUU_PE2_WUPE24_MASK;

                IOMUXC0->PCR0_IOMUXCARRAY1[12] = IOMUXC0_PCR0_IOMUXCARRAY0_MUX(13);

                WUU0->PE2 |= setting;
            }

            /* Cleare any potential interrupts before enter Deep Power Down */
            WUU0->PF = WUU0->PF;
        }
    }
}

void APP_PowerPostSwitchHook(lpm_rtd_power_mode_e targetMode, bool result)
{
    if (LPM_PowerModeActive != targetMode)
    {
        if (LPM_PowerModePowerDown == targetMode || LPM_PowerModeDeepSleep == targetMode)
        {
            APP_Resume(result);
        }

        /*
         * Debug console RX pin was set to disable for current leakage, need to re-configure pinmux.
         * Debug console TX pin was set to disable for current leakage, need to re-configure pinmux.
         */
        IOMUXC_SetPinMux(IOMUXC_PTA10_LPUART1_TX, 0U);
        IOMUXC_SetPinConfig(IOMUXC_PTA10_LPUART1_TX, IOMUXC_PCR_PE_MASK | IOMUXC_PCR_PS_MASK);
        IOMUXC_SetPinMux(IOMUXC_PTA11_LPUART1_RX, 0U);
        IOMUXC_SetPinConfig(IOMUXC_PTA11_LPUART1_RX, IOMUXC_PCR_PE_MASK | IOMUXC_PCR_PS_MASK);

        BOARD_InitClock(); /* initialize system osc for uart(using osc as clock source) */
        BOARD_InitDebugConsole();
    }
    PRINTF("== Power switch %s ==\r\n", result ? "OK" : "FAIL");
    /* Reinitialize TRDC */
    if (AD_CurrentMode == AD_PD)
    {
        BOARD_SetTrdcGlobalConfig();
    }
    else if (AD_CurrentMode == AD_DPD)
    {
        /*
         * For oem_open lifecycle and closed lifecycle part,
         * there are different default setting,
         * Sentinel will apply the default settings after sentinel exit from power down mode.
         * If the sillicon is in oem_open lifecycle, sentinel apply default settings A.
         * If the sillicon is in closed lifecycle, sentinel apply default settings B.
         * A and B is different.
         * So whatever sillicon is in which lifecycle,
         * change default settings directly.
         */
        BOARD_SetTrdcGlobalConfig();
        BOARD_SetTrdcAfterApdReset();
    }
}
/*${function:end}*/
