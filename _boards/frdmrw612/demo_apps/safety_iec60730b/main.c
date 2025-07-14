/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_config.h"
#if FMSTR_SERIAL_ENABLE
#include "freemaster.h"
#endif


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void SYSTICK_Isr(void);

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

    safety_common_t g_sSafetyCommon @ ".safety_ram";
    fs_flash_runtime_test_parameters_t g_sFlashCrc @ ".safety_ram";
    fs_flash_configuration_parameters_t g_sFlashConfig @ ".safety_ram";
    fs_ram_test_t g_sSafetyRamTest @ ".safety_ram";
    fs_ram_test_t g_sSafetyRamStackTest @ ".safety_ram";
    fs_clock_test_t g_sSafetyClockTest @ ".safety_ram";

#elif (defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050)) /* KEIL */
    #include "linker_config.h"

    /* The safety-related RAM border marker. */
    extern uint32_t Image$$SafetyRam_region$$Limit;

    uint32_t stack_pointer_addr = (uint32_t)__BOOT_STACK_ADDRESS;

    uint16_t crcPostbuild; /* Checksum result calculated by srec_cat.exe in post-build phase */

    safety_common_t g_sSafetyCommon __attribute__((section(".safety_ram")));
    fs_clock_test_t g_sSafetyClockTest __attribute__((section(".safety_ram")));
    fs_ram_test_t g_sSafetyRamTest __attribute__((section(".safety_ram")));
    fs_ram_test_t g_sSafetyRamStackTest __attribute__((section(".safety_ram")));
    fs_flash_runtime_test_parameters_t g_sFlashCrc __attribute__((section(".safety_ram")));
    fs_flash_configuration_parameters_t g_sFlashConfig __attribute__((section(".safety_ram")));

#else /* MCUXpresso */

    uint16_t crcPostbuild; /* Checksum result calculated by srec_cat.exe in post-build phase */

    extern uint32_t __BOOT_STACK_ADDRESS; /* from Linker command file */
    uint32_t stack_pointer_addr = (uint32_t)&__BOOT_STACK_ADDRESS;

    extern uint32_t m_sec_fs_ram_start; /* from Linker command file */
    uint32_t pui32SafetyRamSectionStart = (uint32_t)&m_sec_fs_ram_start;

    extern uint32_t m_sec_fs_ram_end; /* from Linker command file */
    uint32_t pui32SafetyRamSectionEnd = (uint32_t)&m_sec_fs_ram_end;

    safety_common_t g_sSafetyCommon __attribute__((section(".safety_ram")));
    fs_clock_test_t g_sSafetyClockTest __attribute__((section(".safety_ram")));
    fs_ram_test_t g_sSafetyRamTest __attribute__((section(".safety_ram")));
    fs_ram_test_t g_sSafetyRamStackTest __attribute__((section(".safety_ram")));
    fs_flash_runtime_test_parameters_t g_sFlashCrc __attribute__((section(".safety_ram")));
    fs_flash_configuration_parameters_t g_sFlashConfig __attribute__((section(".safety_ram")));
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
    __asm("CPSID i"); /* disable interrupts */

    /* Clock initialization */
    ClockInit();

    /* Pin initialization */
    BOARD_InitBootPins();

    g_sSafetyCommon.safetyErrors        = 0; /* clear the variable that records safety error codes */
    g_sSafetyCommon.fastIsrSafetySwitch = 0;

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

#if FMSTR_SERIAL_ENABLE
    SerialInit();
    FMSTR_Init();
#endif /* FMSTR_SERIAL_ENABLE */

    /* Flash test init */
    SafetyFlashTestInit(&g_sFlashCrc, &g_sFlashConfig);

#if FLASH_TEST_ENABLED
    /* After-reset flash test */
    SafetyFlashAfterResetTest(&g_sSafetyCommon, &g_sFlashConfig);
#endif /* FLASH_TEST_ENABLED */

    /* Ram test init for Safety related RAM space */
    SafetyRamTestInit(&g_sSafetyRamTest, safetyRamStart, safetyRamEnd);

    /* Ram test init for Stack memory */
    SafetyRamTestInit(&g_sSafetyRamStackTest, (uint32_t *)c_stackTestFirstAddress,
                      (uint32_t *)c_stackTestSecondAddress);

    /* Ram after-reset test for safety related memory*/
    SafetyRamAfterResetTest(&g_sSafetyCommon, &g_sSafetyRamTest);

    /* Ram after-reset test for Stack area */
    SafetyRamAfterResetTest(&g_sSafetyCommon, &g_sSafetyRamStackTest);

#if PC_TEST_ENABLED
    /* Program Counter test */
    SafetyPcTest(&g_sSafetyCommon, PC_TEST_PATTERN);
#endif

    /* CPU test after */
    SafetyCpuAfterResetTest(&g_sSafetyCommon);

    /* initialize Stack test */
    SafetyStackTestInit();

    /* Stack overflow and underflow test */
    SafetyStackTest(&g_sSafetyCommon);

#if CLOCK_TEST_ENABLED
    /* Initialize Clock test */
    SafetyClockTestInit(&g_sSafetyCommon, &g_sSafetyClockTest);
#endif

    /* Initialize SysTick */
    SystickInit(SYSTICK_RELOAD_VALUE);

    /* Enable interrupts */
    __asm("CPSIE i");

    while (1)
    {
        /* Interruptable CPU registers test */
        SafetyCpuBackgroundTest(&g_sSafetyCommon);

        /* safety test of CPU CONTROL register, it cannot be placed in interrupt, thus interrupts must be disabled for a
         * while */
        /* - see IEC60730 library documentation for CPU errors handling ! */
        __asm("CPSID i");
        g_sSafetyCommon.CPU_control_s_test_result = FS_CM33_CPU_Control_S();       
        __asm("CPSIE i");
        if (g_sSafetyCommon.CPU_control_s_test_result == FS_FAIL_CPU_CONTROL)
        {
            g_sSafetyCommon.safetyErrors |= CPU_CONTROL_ERROR;
            SafetyErrorHandling(&g_sSafetyCommon);
        }

        /* safety test of CPU SP_PROCESS register, it cannot be placed in interrupt, thus interrupts must be disabled
         * for a while */
        /* - see IEC60730 library documentation for CPU errors handling ! */
        __asm("CPSID i");
        FS_CM33_CPU_SPprocess_S();
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

#if FMSTR_SERIAL_ENABLE
        /* Freemaster cummunication */
        FMSTR_Poll(); 
#endif /* FMSTR_SERIAL_ENABLE */

        development_test_terminate(); /* For example validation during development */
    }
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

#if CLOCK_TEST_ENABLED
    /* Clock test function */
    SafetyClockTestIsr(&g_sSafetyClockTest);
#endif

    /* Safety tests which cannot be interrupted */
    SafetyIsrFunction(&g_sSafetyCommon, &g_sSafetyRamTest, &g_sSafetyRamStackTest);
}
