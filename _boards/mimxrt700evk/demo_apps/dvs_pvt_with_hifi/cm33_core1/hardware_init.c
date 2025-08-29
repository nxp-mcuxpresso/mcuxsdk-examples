/*
 * Copyright 2024-2025 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_mu.h"
#include "pvt_app_config.h"
#include "pmic_support.h"
#include "fsl_iopctl.h"
#include "dsp_support.h"
#include "fsl_utick.h"
#include "fsl_dsp.h"
/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/

/*${function:start}*/
static void BOARD_BootClockRUN_InitFRO2(void)
{
    const clock_fro_config_t g_fro2Config_BOARD_BootClockRUN = {
#if (CPU1_DEMO_MAINCLK_FREQ == CPU1_DEMO_MAINCLK_FREQ_SP0)
        .targetFreq = 200000000, /* FRO TUNER output clock frequency: 200000000Hz */
#elif (CPU1_DEMO_MAINCLK_FREQ == CPU1_DEMO_MAINCLK_FREQ_SP1)
        .targetFreq = 160000000, /* FRO TUNER output clock frequency: 160000000Hz */
#else
        .targetFreq = 250000000, /* FRO TUNER output clock frequency: 250000000Hz */
#endif
        .range         = 100,   /* FRO range value: 100counts */
        .trim1DelayUs  = 15,    /* FRO Trim1 delay: 15us */
        .trim2DelayUs  = 150,   /* FRO Trim2 delay: 150us */
        .refDiv        = 1,     /* The FRO reference divider is 1 */
        .enableInt     = 0,     /* The FRO interrupts are disabled */
        .coarseTrimEn  = false, /* The coarse value autotrimming is disabled */
        .fastStartupEn = true,  /* The fast startup is enabled */
    };

    /* Enable power and ungate the FRO0. */
    POWER_DisablePD(kPDRUNCFG_GATE_FRO2);
    POWER_DisablePD(kPDRUNCFG_PD_FRO2);
    /* Configure FRO clock module in closed loop (autotrimming) mode */
    if (kStatus_Success !=
        CLOCK_EnableFroClkFreqCloseLoop(FRO2, &g_fro2Config_BOARD_BootClockRUN,
                                        kCLOCK_FroDiv1OutEn | kCLOCK_FroDiv3OutEn | kCLOCK_FroDiv6OutEn))
    {
        CLOCK_FroFineTune(FRO2, g_fro2Config_BOARD_BootClockRUN.targetFreq,
                          (uint16_t)(FRO2->AUTOTRIM.RW & FRO_AUTOTRIM_AUTOTRIM_MASK));
        CLOCK_EnableFroClkOutput(FRO2, kCLOCK_FroDiv1OutEn | kCLOCK_FroDiv3OutEn | kCLOCK_FroDiv6OutEn);
    }
    /* Setup domain specific clock gates */
    CLOCK_EnableFro2ClkForDomain(kCLOCK_Vdd1SenseDomainEnable);
}

void BOARD_ClockLPPreConfig(void)
{
    /* BOARD_BootClockRUN_InitClockModule(kClockModule_XTAL_OSC); // SOSC already enabled from CPU0 . */
    BOARD_BootClockRUN_InitFRO2();
#if (CPU1_DEMO_MAINCLK_FREQ == CPU1_DEMO_MAINCLK_FREQ_SP0)
    CLOCK_SetClkDiv(kCLOCK_DivSenseMainClk, 2U);
#else
    CLOCK_SetClkDiv(kCLOCK_DivSenseMainClk, 1U);
#endif

    /* Switch SENSE_MAIN_CLK selector to FRO2.FRO_MAX_VDD1_SENSE_CLK */
    CLOCK_AttachClk(kFRO2_DIV1_to_SENSE_MAIN);
}

void BOARD_ClockLPPostConfig(void)
{
}

void BOARD_NotifyBoot(void)
{
    RESET_ClearPeripheralReset(kMU1_RST_SHIFT_RSTn);
    MU_Init(MU1_MUB);
    MU_SetFlags(MU1_MUB, BOOT_FLAG);
}

static inline void BOARD_ConfigSupplySetpoints(void)
{
    /* Set the four LDO setpoints LVD */
    power_lvd_voltage_t vdd1Lvd = {
        .VDD12.lvl3 = 700000U,
        .VDD12.lvl2 = 600000U,
        .VDD12.lvl1 = 550000U,
        .VDD12.lvl0 = 500000U,
    };

    (void)POWER_ConfigLvdSetpoints(kRegulator_Vdd2LDO, &vdd1Lvd);

    POWER_SelectRunSetpoint(kRegulator_Vdd1LDO, 1U);
    POWER_SelectSleepSetpoint(kRegulator_Vdd1LDO, 0U);
    POWER_SelectRunSetpoint(kRegulator_Vdd2LDO, 0U);
    POWER_SelectSleepSetpoint(kRegulator_Vdd2LDO, 0U);
    POWER_SelectRunSetpoint(kRegulator_DCDC, 1U);
    POWER_SelectSleepSetpoint(kRegulator_DCDC, 0U);

    POWER_ApplyPD();
}
void BOARD_InitPowerConfig(void)
{
    /* Enable the used modules in sense side. */
    POWER_DisablePD(kPDRUNCFG_SHUT_SENSEP_MAINCLK);
    POWER_DisablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK);
    POWER_DisablePD(kPDRUNCFG_GATE_FRO2);
    POWER_DisablePD(kPDRUNCFG_PD_FRO2);
    POWER_DisablePD(kPDRUNCFG_PD_LPOSC);

    SYSCON3->SENSE_AUTOGATE_EN = 0x3U;
    CLOCK_EnableClock(kCLOCK_Cpu1); /*Let CPU1 control it's clock. */

    /* Disable unused clock. */
    CLOCK_DisableClock(kCLOCK_Glikey1);
    CLOCK_DisableClock(kCLOCK_Glikey2);
    CLOCK_DisableClock(kCLOCK_Glikey4);
    CLOCK_DisableClock(kCLOCK_Glikey5);
    CLOCK_DisableClock(kCLOCK_SenseAccessRamArbiter0);
    CLOCK_DisableClock(kCLOCK_MediaAccessRamArbiter1);
    CLOCK_AttachClk(kNONE_to_SYSTICK);
    CLOCK_AttachClk(kNONE_to_MICFIL0);

    /* Disable unused modules. */
    POWER_EnablePD(kPDRUNCFG_SHUT_RAM0_CLK);
    POWER_EnablePD(kPDRUNCFG_SHUT_RAM1_CLK); /* Compute access RAM arbiter1 clock. */
    POWER_EnablePD(kPDRUNCFG_SHUT_COMNN_MAINCLK);
    POWER_EnablePD(kPDRUNCFG_SHUT_MEDIA_MAINCLK);
    POWER_EnablePD(kPDRUNCFG_PD_SYSXTAL);
    POWER_EnablePD(kPDRUNCFG_PD_PLLANA);
    POWER_EnablePD(kPDRUNCFG_PD_PLLLDO);
    POWER_EnablePD(kPDRUNCFG_PD_AUDPLLANA);
    POWER_EnablePD(kPDRUNCFG_PD_AUDPLLLDO);
    POWER_EnablePD(kPDRUNCFG_PD_ADC0);
    PMC1->PDRUNCFG1 = 0x7FFFFFFFU;
    PMC1->PDRUNCFG2 &= ~(0x3FFC0000U); /* Power up all the SRAM partitions in Sense domain. */
    PMC1->PDRUNCFG3 &= ~(0x3FFC0000U);
    POWER_EnablePD(kPDRUNCFG_PPD_OCOTP);

    POWER_ApplyPD();

    /* Request the domains out of sense into RBB mode. */
    POWER_EnableRunAFBB(kPower_BodyBiasVdd1);
    POWER_EnableRunNBB(kPower_BodyBiasVdd1Sram);
    POWER_EnableRunRBB(kPower_BodyBiasVdd2 | kPower_BodyBiasVddn | kPower_BodyBiasVdd2Sram);
    POWER_EnableSleepRBB(kPower_BodyBiasVdd2 | kPower_BodyBiasVddn | kPower_BodyBiasVdd2Sram | kPower_BodyBiasVdd1 |
                         kPower_BodyBiasVdd1Sram);

    POWER_ApplyPD();

    /* Configure LVD first, then drop supply voltage. Note, the LVD setpoint is selected by aggregated value from PMC0 &
     * PMC1. */
    BOARD_ConfigSupplySetpoints();
    BOARD_SetPmicVdd1Voltage(MAX_VDD1);
}

/* Set IO pads to default. */
void BOARD_DisableIoPads(void)
{
    uint8_t port, pin;

    RESET_ClearPeripheralReset(kIOPCTL1_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Iopctl1);

    port = 8;
    pin  = 5U; /* Keep JTAG pin unchanged. */

    for (; pin <= 31U; pin++)
    {
        IOPCTL_PinMuxSet(port, pin, 0U);
    }

    port = 9U;
    for (pin = 0U; pin <= 2U; pin++)
    {
        IOPCTL_PinMuxSet(port, pin, 0U);
    }

    port = 10U;
    for (pin = 0U; pin <= 17U; pin++)
    {
        IOPCTL_PinMuxSet(port, pin, 0U);
    }
}

/*!
 * @brief API to perform a dummy read to the selected SRAM partitions. SRAM auto clock gating can save power if
 * partitions are parked on the core for their domain (CPU0 for RAM arbiter0 and CPU1 for RAM
 * arbiter1). This function will perform a dummy read to the specified RAM partitions to force the clock to
 * park on that core until another master accesses each partition.
 *
 * NOTE, make sure the caller is allowed to access the given SRAM partition, otherwise the system may hang!
 *
 * @param pt SRAM Partition, 0-17 for RAM arbiter0, 18-29 for RAM arbiter1.
 *
 */
void POWER_SramDummyRead(uint32_t pt)
{
    uint32_t dummy;
    const uint32_t sram_addr[30] = {0x20000000, 0x20008000, 0x20010000, 0x20018000, 0x20020000, 0x20030000,
                                    0x20040000, 0x20060000, 0x20080000, 0x200C0000, 0x20100000, 0x20180000,
                                    0x20200000, 0x20300000, 0x20400000, 0x20480000, 0x20500000, 0x20540000,
                                    0x20580000, 0x20588000, 0x20590000, 0x20598000, 0x205A0000, 0x205B0000,
                                    0x205C0000, 0x205E0000, 0x20600000, 0x20680000, 0x20700000, 0x20740000};

    dummy = *((volatile uint32_t *)(sram_addr[pt]));
    dummy++; /* suppress warning. */
}

void BOARD_BootDSP(void)
{
#if (CPU1_DEMO_PVT_ON_CPU_DSP != 0U)
#if (CPU1_DEMO_MAINCLK_FREQ == CPU1_DEMO_MAINCLK_FREQ_SP0)
    BOARD_DSP_Init(1U, 2U, true); /* Select FRO2_MAX divider by 3 as HIFI clock. */
#else
    BOARD_DSP_Init(1U, 1U, true); /* Select FRO2_MAX divider by 1 for HIFI clock. */
#endif
#endif /* CPU1_DEMO_PVT_ON_CPU_DSP */
}

void BOARD_InitHardware(void)
{
    uint32_t i;

    BOARD_DisableIoPads();
    POWER_DisablePD(kPDRUNCFG_PD_FRO2); /* Sense uses FRO2. */
    BOARD_InitPins();

    /* PMIC is used. */
    POWER_DisableLPRequestMask(kPower_MaskLpi2c15);
    POWER_ApplyPD();
    BOARD_InitPmic();

#if (MAX_VDD1 > 1000000U)
    /* Increase power supply first. */
    BOARD_SetPmicVdd1Voltage(MAX_VDD1);
#endif

    BOARD_BootClockRUN();
    SystemCoreClockUpdate();

    BOARD_InitDebugConsole();

    /* Initialze power/clock configuration. */
    BOARD_InitPowerConfig();

    /* Dummy read the SRAM partition to let SRAM partition's clock park on Sense domain,
     * incase the it was on CPU0's RAM1 clock and the clock is disabled after CPU1 boot. */
    for (i = DEMO_HIFI1_SRAM_PT_START; i <= DEMO_HIFI1_SRAM_PT_END; i++)
    {
        POWER_SramDummyRead(i);
    }

    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };
    /* DSP toggles led to show it working normally. */
    GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, &led_config);
    GPIO_EnablePinControlNonPrivilege(BOARD_LED_RED_GPIO, (1 << BOARD_LED_RED_GPIO_PIN));
    GPIO_EnablePinControlNonSecure(BOARD_LED_RED_GPIO, (1 << BOARD_LED_RED_GPIO_PIN));
}

void BOARD_EnterSleep(void)
{
    uint32_t irqMask;
    /* Disable clock for unused modules. */
    DbgConsole_Deinit();
    CLOCK_DisableClock(kCLOCK_Syscon1);
    CLOCK_DisableClock(kCLOCK_Iopctl1);
    CLOCK_DisableClock(kCLOCK_Mu1);
    CLOCK_DisableClock(kCLOCK_Syscon3);
    CLOCK_DisableClock(kCLOCK_Sema420);
    CLOCK_DisableClock(kCLOCK_LPI2c15);

    irqMask = DisableGlobalIRQ();
    /* POWER_EnablePD(kPDRUNCFG_PD_LPOSC); */
    POWER_EnablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK);

    POWER_EnterSleep();
    /* POWER_DisablePD(kPDRUNCFG_PD_LPOSC); */
    POWER_DisablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK);

    EnableGlobalIRQ(irqMask);
    __ISB();

    /* Re-enable clock for modules. */
    CLOCK_EnableClock(kCLOCK_Syscon1);
    CLOCK_EnableClock(kCLOCK_Iopctl1);
    CLOCK_AttachClk(kFRO2_DIV3_to_SENSE_BASE);
    CLOCK_EnableClock(kCLOCK_Mu1);
    CLOCK_EnableClock(kCLOCK_Syscon3);
    CLOCK_EnableClock(kCLOCK_Sema420);
    CLOCK_EnableClock(kCLOCK_LPI2c15);

    BOARD_InitDebugConsole();
}

void BOARD_EnterDeepSleep(const uint32_t exclude_from_pd[7])
{
    BOARD_SetDeepSleepPinConfig();

#if defined(APP_SWTICH_CLOCK_FOR_DS) && (APP_SWTICH_CLOCK_FOR_DS != 0U)
    CLOCK_AttachClk(kLPOSC_to_SENSE_BASE);
    CLOCK_AttachClk(kSENSE_BASE_to_SENSE_MAIN);
#if (CPU1_DEMO_MAINCLK_FREQ == CPU1_DEMO_MAINCLK_FREQ_SP0)
    CLOCK_SetClkDiv(kCLOCK_DivSenseMainClk, 1U);
#endif
    CLOCK_EnableFroClkOutput(FRO2, kCLOCK_FroDiv6OutEn); /* Need Keep DIV6. */ /* Only run PVT on CPU. */
#endif

    POWER_EnterDeepSleep(exclude_from_pd);

#if defined(APP_SWTICH_CLOCK_FOR_DS) && (APP_SWTICH_CLOCK_FOR_DS != 0U)
    CLOCK_EnableFroClkOutput(FRO2, kCLOCK_FroDiv1OutEn | kCLOCK_FroDiv3OutEn | kCLOCK_FroDiv6OutEn);
#if (CPU1_DEMO_MAINCLK_FREQ == CPU1_DEMO_MAINCLK_FREQ_SP0)
    CLOCK_SetClkDiv(kCLOCK_DivSenseMainClk, 2U);
#endif
    CLOCK_AttachClk(kFRO2_DIV1_to_SENSE_MAIN);
    CLOCK_AttachClk(kFRO2_DIV3_to_SENSE_BASE);
#endif

    BOARD_RestoreDeepSleepPinConfig();
}

/* Init the wait timer used for PVT task to periodic check whether the supply can go lower. */
void pvt_wait_timer_init(void)
{
    CLOCK_AttachClk(kLPOSC_to_UTICK);
    CLOCK_SetClkDiv(kCLOCK_DivUtick1Clk, 1);
    UTICK_Init(UTICK1);

    NVIC_ClearPendingIRQ((IRQn_Type)UTICK1_IRQn);
    NVIC_SetPriority((IRQn_Type)UTICK1_IRQn, 4U);
}

void pvt_wait_timer_start(uint32_t ms, utick_callback_t cb)
{
    assert(ms != 0U);
    UTICK_SetTick(UTICK1, kUTICK_Onetime, (ms * 1000U) - 1U, cb);
}
/*${function:end}*/
