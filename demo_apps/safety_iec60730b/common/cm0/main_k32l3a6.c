/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_config.h"
#include "safety_test_items.h"
#ifdef K32L3A60_CM0PLUS_H_
#include "fsl_mu.h"
#endif /* K32L3A60_CM0PLUS_H_ */
#include "board.h"
#include "pin_mux.h"


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void SYSTICK_Isr(void);
void safety_dio_runtime(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Start and end addresses for March test applied to Stack area */
extern const uint32_t c_stackTestFirstAddress;  /* defined in safety_*.c */
extern const uint32_t c_stackTestSecondAddress; /* defined in safety_*.c */

/* Test variable */
volatile uint32_t counter = 0;

#if defined(__IAR_SYSTEMS_ICC__) /* IAR */
    #pragma section =  ".safety_ram"
    #pragma section = ".pctest"

    wd_test_t g_sSafetyWdTest @ ".safety_ram";
    safety_common_t g_sSafetyCommon @ ".safety_ram";
    fs_flash_runtime_test_parameters_t g_sFlashCrc @ ".safety_ram";
    fs_flash_configuration_parameters_t g_sFlashConfig @ ".safety_ram";
    fs_ram_test_t g_sSafetyRamTest @ ".safety_ram";
    fs_ram_test_t g_sSafetyRamStackTest @ ".safety_ram";
    fs_clock_test_t g_sSafetyClockTest @ ".safety_ram";

  #ifdef K32L3A60_CM0PLUS_H_
    /* variables from CM0+ core, placed in shared memory */
    volatile unsigned long ui32shTempResult @ "rpmsg_sh_mem_section";
    volatile safety_common_t g_sCM0Shared @ "rpmsg_sh_mem_section";
    volatile uint32_t ui32shCM0ClockTest @ "rpmsg_sh_mem_section";
    volatile unsigned short ui16CM0Checksum @ "rpmsg_sh_mem_section";
    volatile fs_flash_runtime_test_parameters_t g_sCM0FlashTest @ "rpmsg_sh_mem_section";
    volatile wd_test_t g_sCM0WatchDogTest @ "rpmsg_sh_mem_section";
  #endif /* K32L3A60_CM0PLUS_H_ */

#elif (defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050)) /* KEIL */
    #include "linker_config.h"

    /* The safety-related RAM border marker. */
    extern uint32_t Image$$SafetyRam_region$$Limit;

		uint32_t stack_pointer_addr = (uint32_t)__BOOT_STACK_ADDRESS;

    uint16_t crcPostbuild; /* Checksum result calculated by srec_cat.exe in post-build phase */

    wd_test_t g_sSafetyWdTest __attribute__((section(".safety_ram")));
    safety_common_t g_sSafetyCommon __attribute__((section(".safety_ram")));
    fs_clock_test_t g_sSafetyClockTest __attribute__((section(".safety_ram")));
    fs_ram_test_t g_sSafetyRamTest __attribute__((section(".safety_ram")));
    fs_ram_test_t g_sSafetyRamStackTest __attribute__((section(".safety_ram")));
    fs_flash_runtime_test_parameters_t g_sFlashCrc __attribute__((section(".safety_ram")));
    fs_flash_configuration_parameters_t g_sFlashConfig __attribute__((section(".safety_ram")));

  #ifdef K32L3A60_CM0PLUS_H_
    /* variables from CM0+ core, placed in shared memory */
    volatile unsigned long ui32shTempResult __attribute__((section(".rpmsg_sh_mem_section")));
    volatile safety_common_t g_sCM0Shared __attribute__((section(".rpmsg_sh_mem_section")));
    volatile uint32_t ui32shCM0ClockTest __attribute__((section(".rpmsg_sh_mem_section")));
    volatile unsigned short ui16CM0Checksum __attribute__((section(".rpmsg_sh_mem_section")));
    volatile fs_flash_runtime_test_parameters_t g_sCM0FlashTest __attribute__((section(".rpmsg_sh_mem_section")));
    volatile wd_test_t g_sCM0WatchDogTest __attribute__((section(".rpmsg_sh_mem_section")));
  #endif /* K32L3A60_CM0PLUS_H_ */

#else /* MCUXpresso */
//    #include <cr_section_macros.h>

    uint16_t crcPostbuild; /* Checksum result calculated by srec_cat.exe in post-build phase */

    extern uint32_t __BOOT_STACK_ADDRESS; /* from Linker command file */
    uint32_t stack_pointer_addr = (uint32_t)&__BOOT_STACK_ADDRESS;

    extern uint32_t m_sec_fs_ram_start; /* from Linker command file */
    uint32_t pui32SafetyRamSectionStart = (uint32_t)&m_sec_fs_ram_start;

    extern uint32_t m_sec_fs_ram_end; /* from Linker command file */
    uint32_t pui32SafetyRamSectionEnd = (uint32_t)&m_sec_fs_ram_end;

    wd_test_t g_sSafetyWdTest __attribute__((section(".safety_ram")));
    safety_common_t g_sSafetyCommon __attribute__((section(".safety_ram")));
    fs_clock_test_t g_sSafetyClockTest __attribute__((section(".safety_ram")));
    fs_ram_test_t g_sSafetyRamTest __attribute__((section(".safety_ram")));
    fs_ram_test_t g_sSafetyRamStackTest __attribute__((section(".safety_ram")));
    fs_flash_runtime_test_parameters_t g_sFlashCrc __attribute__((section(".safety_ram")));
    fs_flash_configuration_parameters_t g_sFlashConfig __attribute__((section(".safety_ram")));

    #ifdef K32L3A60_CM0PLUS_H_
    /* variables from CM0+ core, placed in shared memory */
    volatile unsigned long ui32shTempResult __attribute__((section(".rpmsg_sh_mem_section")));
    volatile safety_common_t g_sCM0Shared __attribute__((section(".rpmsg_sh_mem_section")));
    volatile uint32_t ui32shCM0ClockTest __attribute__((section(".rpmsg_sh_mem_section")));
    volatile unsigned short ui16CM0Checksum __attribute__((section(".rpmsg_sh_mem_section")));
    volatile fs_flash_runtime_test_parameters_t g_sCM0FlashTest __attribute__((section(".rpmsg_sh_mem_section")));
    volatile wd_test_t g_sCM0WatchDogTest __attribute__((section(".rpmsg_sh_mem_section")));
  #endif /* K32L3A60_CM0PLUS_H_ */

#endif


/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief main function
 *
 * @param void
 *
 * @return None
 */
int32_t main(void)
{
#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif
      /* Clock initialization */
    ClockInit();

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif

    /* Watchdog test */
    SafetyWatchdogTest(&g_sSafetyCommon, &g_sSafetyWdTest);

#ifdef K32L3A60_CM0PLUS_H_
    g_sCM0WatchDogTest = g_sSafetyWdTest; /* pass the variables into the shared memory */

    g_sSafetyCommon.AIO_test_result = 0;
    g_sSafetyCommon.CLOCK_test_result = 0;
    g_sSafetyCommon.DIO_input_test_result = 0;
    g_sSafetyCommon.DIO_output_test_result = 0;
#endif /* K32L3A60_CM0PLUS_H_ */



#ifdef K32L3A60_CM0PLUS_H_
    BOARD_InitBootPins();
#endif /* K32L3A60_CM0PLUS_H_ */

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif

#ifdef K32L3A60_CM0PLUS_H_
    /* Multicore initialization */
    PCC0->CLKCFG[PCC_MSCM_INDEX] |= PCC_CLKCFG_CGC_MASK; /* enable clock to MSCM */
    PCC1->CLKCFG[PCC_MUB_INDEX] |= PCC_CLKCFG_CGC_MASK; /* enable clock to MUB */
    MU_MaskHardwareReset(MUB, false);  /* MUA can reset the CM0+ core */
    SMC1->MR &= ~SMC_MR_BOOTCFG_MASK;

    while (!(MUB->SR & MU_SR_TEn_MASK)) /* while transmit register not ready */
    {
    }
    MUB->TR[0] = 1234;   /* send the message to the CM4 */
#endif /* K32L3A60_CM0PLUS_H_ */

#if defined(__IAR_SYSTEMS_ICC__) /* IAR */
    uint32_t *safetyRamStart = __section_begin(".safety_ram");
    uint32_t *safetyRamEnd   = __section_end(".safety_ram");

#elif (defined(__GNUC__) && (__ARMCC_VERSION >= 6010050)) /* KEIL */
    uint32_t *safetyRamStart = (uint32_t *)m_safety_ram_start;
    uint32_t *safetyRamEnd   = (uint32_t *)&Image$$SafetyRam_region$$Limit;

#else /* MCUXpresso */
    uint32_t *safetyRamStart = (uint32_t *)pui32SafetyRamSectionStart;
    uint32_t *safetyRamEnd   = (uint32_t *)pui32SafetyRamSectionEnd;
#endif

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif

    g_sSafetyCommon.safetyErrors        = 0; /* clear the variable that records safety error codes */
    g_sSafetyCommon.fastIsrSafetySwitch = 0;

    /* Flash test init */
    SafetyFlashTestInit(&g_sFlashCrc, &g_sFlashConfig);

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif

#if FLASH_TEST_ENABLED
    /* After-reset flash test */
    SafetyFlashAfterResetTest(&g_sSafetyCommon, &g_sFlashConfig);

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif
#endif /* FLASH_TEST_ENABLED */

    /* Ram test init for Safety related RAM space */
       SafetyRamTestInit(&g_sSafetyRamTest, safetyRamStart, safetyRamEnd);

    /* Ram test init for Stack memory */
    SafetyRamTestInit(&g_sSafetyRamStackTest, (uint32_t *)c_stackTestFirstAddress,
                      (uint32_t *)c_stackTestSecondAddress);

    /* Ram after-reset test for safety related memory*/
    SafetyRamAfterResetTest(&g_sSafetyCommon, &g_sSafetyRamTest);

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif

    /* Ram after-reset test for Stack area */
    SafetyRamAfterResetTest(&g_sSafetyCommon, &g_sSafetyRamStackTest);

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif

#if PC_TEST_ENABLED
    /* Program Counter test */
    SafetyPcTest(&g_sSafetyCommon, PC_TEST_PATTERN);
#endif

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif

    /* CPU test after */
    SafetyCpuAfterResetTest(&g_sSafetyCommon);

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif

    /* initialize Stack test */
    SafetyStackTestInit();

    /* Stack overflow and underflow test */
    SafetyStackTest(&g_sSafetyCommon);

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif

#if ADC_TEST_ENABLED
#ifndef K32L3A60_CM0PLUS_H_
    AdcInit();

    #if WATCHDOG_ENABLED
      Watchdog_refresh; /* refreshing the watchdog */
    #endif

    /* initialize Analog test */
     SafetyAnalogTestInitialization();

    /* After-reset ADC test */
    for (uint8_t i = 0; i < 4; i++) /* first iteration is init phase */
    {
        for (uint8_t y = 0; y < 40; y++)
            __asm("nop"); /* delay because of conversion time */
        SafetyAnalogTest(&g_sSafetyCommon);

        #if WATCHDOG_ENABLED
          Watchdog_refresh; /* refreshing the watchdog */
        #endif
    }
#endif /* K32L3A60_CM0PLUS_H_ */
#endif /* ADC_TEST_ENABLED */

#if DIO_TEST_ENABLED
    /* Digital I/O test */
    SafetyDIOTestInit(&g_sSafetyCommon, g_dio_safety_test_items);

  #ifdef _MKE02Z4_H_
    SafetyDigitalOutputTest(&g_sSafetyCommon,g_dio_safety_test_items[0]);
    SafetyDigitalInputTest(&g_sSafetyCommon,g_dio_safety_test_items[1]);
  #else

    for (int i = 0; g_dio_safety_test_items[i] != 0; i++)
    {
        SafetyDigitalOutputTest(&g_sSafetyCommon, g_dio_safety_test_items[i]);
        SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon, g_dio_safety_test_items[i], DIO_SHORT_TO_GND_TEST);
        SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon, g_dio_safety_test_items[i], DIO_SHORT_TO_VDD_TEST);
    }

    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, g_dio_safety_test_items[0], g_dio_safety_test_items[1],
                                          LOGICAL_ONE);
    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, g_dio_safety_test_items[0], g_dio_safety_test_items[1],
                                          LOGICAL_ZERO);

#if defined(_MKE15Z7_H_) | defined(_MKE17Z7_H_)
    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, g_dio_safety_test_items[2], g_dio_safety_test_items[3],
                                          LOGICAL_ZERO);
    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, g_dio_safety_test_items[6], g_dio_safety_test_items[7],
                                          LOGICAL_ONE);
#endif

#endif /* _MKE02Z4_H_ */
#endif /* DIO_TEST_ENABLED */

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif

#if CLOCK_TEST_ENABLED
    /* Initialize Clock test */
    SafetyClockTestInit(&g_sSafetyCommon, &g_sSafetyClockTest);
#endif

    /* Initialize SysTick */
    SystickInit(SYSTICK_RELOAD_VALUE);

    /* Enable interrupts */
    __asm("CPSIE i");

#ifdef K32L3A60_CM0PLUS_H_
    LED2_INIT(0);
#endif /* K32L3A60_CM0PLUS_H_ */

    while (TRUE)
    {
        /* Interruptable CPU registers test */
        SafetyCpuBackgroundTest(&g_sSafetyCommon);

        /* safety test of CPU CONTROL register, it cannot be placed in interrupt, thus interrupts must be disabled for a
         * while */
        /* - see IEC60730 library documentation for CPU errors handling ! */
        __asm("CPSID i");
        g_sSafetyCommon.CPU_control_test_result = FS_CM0_CPU_Control();
        __asm("CPSIE i");
        if (g_sSafetyCommon.CPU_control_test_result == FS_FAIL_CPU_CONTROL)
        {
            g_sSafetyCommon.safetyErrors |= CPU_CONTROL_ERROR;
            SafetyErrorHandling(&g_sSafetyCommon);
        }

        /* safety test of CPU SP_PROCESS register, it cannot be placed in interrupt, thus interrupts must be disabled
         * for a while */
        /* - see IEC60730 library documentation for CPU errors handling ! */
        __asm("CPSID i");
        FS_CM0_CPU_SPprocess();
        __asm("CPSIE i");

#if FLASH_TEST_ENABLED
        /* Runtime Flash test */
        SafetyFlashRuntimeTest(&g_sSafetyCommon, &g_sFlashCrc, &g_sFlashConfig);
#endif

#if CLOCK_TEST_ENABLED
        /* Runtime Clock test */
        SafetyClockTestCheck(&g_sSafetyCommon, &g_sSafetyClockTest);
#endif

        /* Stack overflow and underflow test */
        SafetyStackTest(&g_sSafetyCommon);

#if ADC_TEST_ENABLED
#ifndef K32L3A60_CM0PLUS_H_
        /* Runtime ADC test */
        SafetyAnalogTest(&g_sSafetyCommon);
#endif /* K32L3A60_CM0PLUS_H_ */
#endif

#if DIO_TEST_ENABLED
        /* Digital I/O test */
        safety_dio_runtime();
#endif /* DIO_TEST_ENABLED */

        development_test_terminate(); /* For example validation during development */

#ifdef K32L3A60_CM0PLUS_H_
        ui32shTempResult ++;
        /* move results into the shared memory */
        g_sCM0Shared = g_sSafetyCommon;
        ui32shCM0ClockTest = g_sSafetyClockTest.clockTestContext;
        g_sCM0FlashTest = g_sFlashCrc;
 #endif /* K32L3A60_CM0PLUS_H_ */
    }
}

void safety_dio_runtime(void)
{
#ifdef _MKE02Z4_H_
    SafetyDigitalOutputTest(&g_sSafetyCommon, g_dio_safety_test_items[0]);
    SafetyDigitalInputTest(&g_sSafetyCommon, g_dio_safety_test_items[1]);
#else

    /* Static variable for indexing in items array */
    static uint8_t dio_cnt_number = 0;

    if (g_dio_safety_test_items[dio_cnt_number] != NULL)
    {
        SafetyDigitalOutputTest(&g_sSafetyCommon, g_dio_safety_test_items[dio_cnt_number]);
        SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon, g_dio_safety_test_items[dio_cnt_number],
                                                 DIO_SHORT_TO_GND_TEST);
        SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon, g_dio_safety_test_items[dio_cnt_number],
                                                 DIO_SHORT_TO_VDD_TEST);

        /* In next call, test next DIO channel */
        dio_cnt_number++;
    }
    else
    {
        dio_cnt_number = 0;
    }
#endif
}


/*!
 * @brief Systick interrupt function
 *
 * @param void
 *
 * @return None
 */
void SYSTICK_Isr(void)
{
    counter++;

    if((counter % 500) == 0)
    {
      LED2_TOGGLE(); //Core CM0+ toggles GREEN LED
    }

#if CLOCK_TEST_ENABLED
    /* Clock test function */
    SafetyClockTestIsr(&g_sSafetyClockTest);
#endif

    /* Safety tests which cannot be interrupted */
    SafetyIsrFunction(&g_sSafetyCommon, &g_sSafetyRamTest, &g_sSafetyRamStackTest);

    /* Refreshing the watchdog. For short period of interrupts, choose higher refresh ratio parameter */
    SafetyWatchdogRuntimeRefresh(&g_sSafetyWdTest);
}
