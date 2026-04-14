/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

#include "fsl_wuu.h"
#include "fsl_power.h"
#include "fsl_cmc.h"
#include "fsl_mu.h"
#include "fsl_smm.h"

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_reset.h"
#include "fsl_pmu.h"

#include "fsl_rtc.h"
/*${header:end}*/

/*${function:start}*/

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void)
{
    uint32_t image_size;
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    image_size = (uint32_t)&Image$$CORE1_REGION$$Length;
#elif defined(__ICCARM__)
#pragma section = "__core1_image"
    image_size = (uint32_t)__section_end("__core1_image") - (uint32_t)&core1_image_start;
#elif defined(__GNUC__)
    image_size = (uint32_t)core1_image_size;
#endif
    return image_size;
}
#endif

void APP_BootCore1(void)
{
    AON__CGU->RST_SUB_BLK |= CGU_RST_SUB_BLK_CM0P_RST_REL_MASK;
    for (uint8_t i = 0U; i < 100; i++)
    {
        __NOP();
    }
}

void BOARD_InitWakeupButtonAsWUUPin(void)
{
    CLOCK_EnableClock(kCLOCK_GatePORT1);
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);

    /* WakeUp Button */
    const port_pin_config_t port1_14_config = {/* Internal pull-up/down resistor is disabled */
                                               .pullSelect = kPORT_PullDisable,
                                               /* Low internal pull resistor value is selected. */
                                               .pullValueSelect = kPORT_LowPullResistor,
                                               /* Fast slew rate is configured */
                                               .slewRate = kPORT_FastSlewRate,
                                               /* Passive input filter is disabled */
                                               .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                               /* Open drain output is disabled */
                                               .openDrainEnable = kPORT_OpenDrainDisable,
                                               /* Low drive strength is configured */
                                               .driveStrength = kPORT_LowDriveStrength,
                                               /* Normal drive strength is configured */
                                               .driveStrength1 = kPORT_NormalDriveStrength,
                                               /* Pin is configured as WUU_P5 */
                                               .mux = kPORT_MuxAlt5,
                                               /* Digital input enabled */
                                               .inputBuffer = kPORT_InputBufferEnable,
                                               /* Digital input is not inverted */
                                               .invertInput = kPORT_InputNormal,
                                               /* Pin Control Register fields [15:0] are not locked */
                                               .lockRegister = kPORT_UnlockRegister};
    PORT_SetPinConfig(PORT1, 14U, &port1_14_config);
}

void BOARD_InitWakeupButtonAsGPIO(void)
{
    gpio_pin_config_t input_config = {.pinDirection = kGPIO_DigitalInput, .outputLogic = 1U};

    CLOCK_EnableClock(kCLOCK_GatePORT1);
    CLOCK_EnableClock(kCLOCK_GateGPIO1);
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);

    /* WakeUp Button */
    const port_pin_config_t port1_14_config = {/* Internal pull-up/down resistor is disabled */
                                               .pullSelect = kPORT_PullDisable,
                                               /* Low internal pull resistor value is selected. */
                                               .pullValueSelect = kPORT_LowPullResistor,
                                               /* Fast slew rate is configured */
                                               .slewRate = kPORT_FastSlewRate,
                                               /* Passive input filter is disabled */
                                               .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                               /* Open drain output is disabled */
                                               .openDrainEnable = kPORT_OpenDrainDisable,
                                               /* Low drive strength is configured */
                                               .driveStrength = kPORT_LowDriveStrength,
                                               /* Normal drive strength is configured */
                                               .driveStrength1 = kPORT_NormalDriveStrength,
                                               /* Pin is configured as GPIO */
                                               .mux = kPORT_MuxAlt0,
                                               /* Digital input enabled */
                                               .inputBuffer = kPORT_InputBufferEnable,
                                               /* Digital input is not inverted */
                                               .invertInput = kPORT_InputNormal,
                                               /* Pin Control Register fields [15:0] are not locked */
                                               .lockRegister = kPORT_UnlockRegister};
    PORT_SetPinConfig(PORT1, 14U, &port1_14_config);

    GPIO_PinInit(GPIO1, 14, &input_config);
}

void BOARD_InitExtIntButtonAsGPIO(void)
{
    /* GPIO0: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateAonGPIO);
    /* PORT0: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateAonPORT);

    gpio_pin_config_t SW5_config = {.pinDirection = kGPIO_DigitalInput, .outputLogic = 0U};
    /* Initialize GPIO functionality on pin PIO0_9   */
    GPIO_PinInit(AON__GPIO0, 9U, &SW5_config);

    const port_pin_config_t SW5 = {/* Internal pull-up/down resistor is disabled */
                                   .pullSelect = kPORT_PullDisable,
                                   /* Low internal pull resistor value is selected. */
                                   .pullValueSelect = kPORT_LowPullResistor,
                                   /* Fast slew rate is configured */
                                   .slewRate = kPORT_FastSlewRate,
                                   /* Passive input filter is disabled */
                                   .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                   /* Open drain output is disabled */
                                   .openDrainEnable = kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   .driveStrength = kPORT_LowDriveStrength,
                                   /* Normal drive strength is configured */
                                   .driveStrength1 = kPORT_NormalDriveStrength,
                                   /* Pin is configured as P0_9 */
                                   .mux = kPORT_MuxAlt0,
                                   /* Digital input enabled */
                                   .inputBuffer = kPORT_InputBufferEnable,
                                   /* Digital input is not inverted */
                                   .invertInput = kPORT_InputNormal,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   .lockRegister = kPORT_UnlockRegister};
    /* PORT0_9  is configured as P0_9 */
    PORT_SetPinConfig(AON__PORT0, 9U, &SW5);
}

void APP_InitROSC(void)
{
    uint32_t i = 0;

    for (i = 0; i < 3; i++)
    {
        AON__SMM->RTC_LDO_CNTRL = SMM_RTC_LDO_CNTRL_ISO_MASK | SMM_RTC_LDO_CNTRL_DGTL_RST_N_MASK;
        SDK_DelayAtLeastUs(1000, SystemCoreClock);
    }

    for (i = 0; i < 3; i++)
    {
        AON__SMM->RTC_LDO_CNTRL =
            SMM_RTC_LDO_CNTRL_ISO_MASK | SMM_RTC_LDO_CNTRL_DGTL_RST_N_MASK | SMM_RTC_LDO_CNTRL_ANA_RESET_N_MASK;
        SDK_DelayAtLeastUs(1000, SystemCoreClock);
    }

    AON__SMM->RTC_XTAL_CONFG1 = SMM_RTC_XTAL_CONFG1_CRNT_MROR_EN_MASK | SMM_RTC_XTAL_CONFG1_AMPSEL(2);
    SDK_DelayAtLeastUs(1000, SystemCoreClock);
    SDK_DelayAtLeastUs(1000, SystemCoreClock);
    AON__SMM->RTC_XTAL_CONFG1 |= SMM_RTC_XTAL_CONFG1_XTAL_EN_MASK;
    SDK_DelayAtLeastUs(1000, SystemCoreClock);
    AON__SMM->BIAS_CTRL = SMM_BIAS_CTRL_XTAL_SOX_4P_DIS_MASK;
    SDK_DelayAtLeastUs(1000, SystemCoreClock);
    SDK_DelayAtLeastUs(1000, SystemCoreClock);

    AON__SMM->RTC_XTAL_CONFG2 = SMM_RTC_XTAL_CONFG2_SUPDET_TM_SOX(2) | SMM_RTC_XTAL_CONFG2_CAP_BNK_EN_MASK;
    SDK_DelayAtLeastUs(1000, SystemCoreClock);
    AON__SMM->RTC_LDO_CNTRL = SMM_RTC_LDO_CNTRL_ISO_MASK | SMM_RTC_LDO_CNTRL_DGTL_RST_N_MASK |
                               SMM_RTC_LDO_CNTRL_ANA_RESET_N_MASK | SMM_RTC_LDO_CNTRL_LDO_EN_MASK |
                               SMM_RTC_LDO_CNTRL_LDO_CONFIG(0x30);
    SDK_DelayAtLeastUs(1000, SystemCoreClock);

    AON__SMM->RTC_LDO_CNTRL &= ~SMM_RTC_LDO_CNTRL_ISO_MASK;

    AON__RTC_AON->RTC_ALV_DTCT &= ~RTC_RTC_ALV_DTCT_BYPASS_MASK;
    AON__RTC_AON->RTC_ALV_DTCT |= RTC_RTC_ALV_DTCT_DTCT_EN_MASK;

    SDK_DelayAtLeastUs(6400, SystemCoreClock);
    /* wait until XTTAL FAIL goes away */
    do
    {
        AON__RTC_AON->INT = 0x001F; // clear all interrupts
        SDK_DelayAtLeastUs(6400, SystemCoreClock);
    } while (AON__RTC_AON->INT & RTC_INT_XTAL_FAIL_IF_MASK);

    AON__RTC_AON->CONFIG = RTC_CONFIG_K_EN_MASK | RTC_CONFIG_XTAL32_EN_MASK | RTC_CONFIG_FREE_RUNNING_MASK;

    AON__SMM->CNFG &= 0xff3f;
    SDK_DelayAtLeastUs(100000, SystemCoreClock);
}

void BOARD_Init96MClocksBoot(void)
{
    /* Config 32k Crystal Oscillator */
    /* Monitor is disabled */
    CLOCK_SetRoscMonitorMode(kSCG_RoscMonitorDisable);
    CLOCK_SetupFROHFClocking(96000000U, 0U); /* Setup FRO HF clock */

    CLOCK_SetupFRO12MClocking();             /* Setup FRO12M clock */

    CLOCK_EnableClock(kCLOCK_GateAonAPB);    /* Required for access to AON*/

    /*!< Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kFIRC_to_MAIN_CLK);            /* !< Switch MAIN_CLK to FIRC */
    CLOCK_AttachClk(kFIRC_to_FRO_HF);              /* !< Switch FRO_HF to FIRC */
    CLOCK_AttachClk(kCPU_CLK_to_SYSTICK);          /* !< Switch SYSTICK to CPU_CLK */
    CLOCK_AttachClk(kFRO16K_to_OSTIMER0);         /* !< Switch OSTIMER0 to FRO_16k */
    CLOCK_AttachClk(kSLOW_CLK_to_CLKOUT);          /* !< Switch CLKOUT to SLOW_CLK */
    CLOCK_AttachClk(kFRO_HF_DIV_to_ADC0);          /* !< Switch ADC0 to FRO_HF_DIV */
    CLOCK_AttachClk(kCLK_1M_to_UTICK0);            /* !< Switch UTICK0 to CLK_1M */
    CLOCK_AttachClk(kFRO12M_to_CMP0);              /* !< Switch CMP0 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO12M_to_PERIPH_GROUP0);     /* !< Switch PERIPH_GROUP0 to FRO_12M */
    CLOCK_AttachClk(kFRO_HF_DIV_to_PERIPH_GROUP1); /* !< Switch PERIPH_GROUP1 to FRO_HF_DIV */

    /*!< Set up dividers */
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);       /* !< Set AHBCLKDIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF_DIV, 1U);   /* !< Set MRCC_FRO_HF_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivWWDT0, 1U);        /* !< Set MRCC_WWDT0_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivCMP0_FUNC, 1U);    /* !< Set MRCC_CMP0_FUNC_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivTRACE, 1U);        /* !< Set MRCC_DBG_TRACE_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivADC0, 1U);         /* !< Set MRCC_ADC0_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivPeriphGroup0, 1U); /* !< Set MRCC_LPUART_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivPeriphGroup1, 1U); /* !< Set MRCC_LPUART_DIV divider to value 1 */

    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP0);
    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP1);

    /* Set SystemCoreClock variable */
    SystemCoreClock = 96000000U;

    FMU0->FCTRL = (FMU0->FCTRL & ~FMU_FCTRL_RWSC_MASK) | FMU_FCTRL_RWSC(2);
}

void BOARD_InitHardware(void)
{
    rosc_init_config_t roscInitConfig;
        
    BOARD_InitDEBUG_UARTPins();
    BOARD_Init96MClocksBoot();
    BOARD_InitDebugConsole();

    Power_ClearLpPowerSettings();
    
    PMU_DoHandshakeBetweenPMUAndPAC(AON__PMU);

    CMC_EnableDebugOperation(CMC, false);

    EnableIRQ(MU_A_RX_IRQn);
    MU_EnableInterrupts(APP_MU, (kMU_Rx0FullInterruptEnable));
    /* Initialize Rosc if not already initialized */
    if (!CLOCK_IsRoscInitialized())
    {
        AON__CGU->CLK_CONFIG |= CGU_CLK_CONFIG_XTAL32_OUT_EN_MASK;
        /* Get default Rosc initialization configuration */
        CLOCK_GetDefaultInitRoscConfig(&roscInitConfig);
        
        /* Configure Rosc initialization for FRDM-MCXL255 for faster init*/
        roscInitConfig.detectionDelay = 50U;
        roscInitConfig.detectionTimeout = 0U;
        roscInitConfig.detectionDelaySwitchedMode = 50U;
        roscInitConfig.detectionTimeoutSwitchedMode = 50U;
        
        CLOCK_InitRosc(&roscInitConfig);
    }
    
    rtc_alive_detector_config_t rtcAliveDetConfig = {
        .enableAliveDetector = true,
        .bypassAnalogAliveMechanism = false,
        .mechanismPeriod = 0,
    };
    RTC_ConfigureAliveDetector(APP_RTC_BASE, &rtcAliveDetConfig);
    
    CLOCK_DisableADVCControl();
}

/*${function:end}*/
