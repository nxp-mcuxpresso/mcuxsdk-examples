/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_cm33_RW61x.h"

#if FMSTR_SERIAL_ENABLE
	#include "freemaster.h"
#endif

#if (defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050)) /* KEIL */
    #include "linker_config.h"
#endif

#if defined(__IAR_SYSTEMS_ICC__) /* IAR */
    #pragma section =  ".safety_ram"
    #pragma section =  ".checksum"
    #pragma location = ".checksum"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef CRC_BASE
    #define CRC_BASE  0x0u /*Function API for HW and SW CRC is the same, due to this reason, we must define this symbol, even though it is not used  */
#endif

/*******************************************************************************
* Variables
******************************************************************************/
#if (defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050)) /* KEIL */
    const uint32_t programCounterTestFlag = (uint32_t)m_pc_test_flag;
    #define PC_TEST_FLAG ((uint32_t *) programCounterTestFlag)

    const uint32_t safetyErrorCodeAddress = (uint32_t)m_safety_error_code;
    #define SAFETY_ERROR_CODE ((uint32_t *) safetyErrorCodeAddress)

    const uint32_t c_backupAddress = (uint32_t)m_ram_test_backup;

    /* put values from extern symbols to const variables */
    const uint32_t c_stackTestFirstAddress = (uint32_t)m_stack_test_p_2;
    const uint32_t c_stackTestSecondAddress = (uint32_t)m_stack_test_p_3;

    /* The safety-related FLASH end border marker. */
    extern uint32_t Load$$ER_IROM3$$Limit;

    /* The safety-related FLASH CRC value. */
    fs_crc_t c_sfsCRC __attribute__((used, section(".flshcrc"))) =
    {
        .ui16Start      = 0xA55AU,
        .ui32FlashStart = (uint32_t)__PC_test_start__,
        .ui32FlashEnd   = (uint32_t)&Load$$ER_IROM3$$Limit,
        .ui32CRC        = (uint32_t)FS_CFG_FLASH_TST_CRC,
        .ui16End        = 0x5AA5U
    };

#else /* IAR + MCUXpresso */
    extern uint32_t m_pc_test_flag;   /* from Linker command file */
    const uint32_t programCounterTestFlag = (uint32_t)&m_pc_test_flag;
    #define PC_TEST_FLAG ((uint32_t *) programCounterTestFlag)

    extern uint32_t m_safety_error_code; /* from Linker command file */
    const uint32_t safetyErrorCodeAddress = (uint32_t)&m_safety_error_code;
    #define SAFETY_ERROR_CODE ((uint32_t *) safetyErrorCodeAddress)

    extern uint32_t m_ram_test_backup; /* symbol from Linker command file */
    const uint32_t c_backupAddress = (uint32_t)&m_ram_test_backup;

    extern uint32_t m_stack_test_p_2;   /* symbol from Linker command file */
    extern uint32_t m_stack_test_p_3;   /* symbol from Linker command file */

    /* put values from extern symbols to const variables */
    const uint32_t c_stackTestFirstAddress = (uint32_t)&m_stack_test_p_2;
    const uint32_t c_stackTestSecondAddress = (uint32_t)&m_stack_test_p_3;

    #if defined(__IAR_SYSTEMS_ICC__) /* IAR */
        extern const uint32_t __checksum;  /* calculated by Linker */
        const uint32_t        c_checksumStart @ "checksum_start_mark";
        const uint32_t        c_checksumEnd   @ "checksum_end_mark";

    #else /* MCUXpresso */
        extern uint32_t stack_test_block_size; /* from Linker command file */
        #define STACK_TEST_BLOCK_SIZE (uint32_t)&stack_test_block_size
        extern uint32_t ram_test_backup_size; /* from Linker command file */
        #define RAM_TEST_BACKUP_SIZE (uint32_t)&ram_test_backup_size

        extern uint32_t __PC_test_start__; /* symbol from Linker command file */
        extern uint32_t m_safety_flash_end; /* symbol from Linker command file */

        /* The safety-related FLASH CRC value. */
        fs_crc_t c_sfsCRC __attribute__((used, section(".flshcrc"))) =
        {
            .ui16Start      = 0xA55AU,
            .ui32FlashStart = (uint32_t) &__PC_test_start__,
            .ui32FlashEnd   = (uint32_t) &m_safety_flash_end,
            .ui32CRC        = (uint32_t)FS_CFG_FLASH_TST_CRC,
            .ui16End        = 0x5AA5U
        };
    #endif
#endif

/** This variable is used as flag for automatic "run" test during example development,
these test have a 3 return value:
1. All tests pass - wait until all after/reset and runtime test passed
2. Safety errror - return from SafetyErrorHandling() function
3. timeout - if case of that all tests are not finished until setted timeout */
#define CLOCK_PASSED              (1 << 0)
#define INVARIABLE_RUNTIME_PASSED (1 << 1)
#define VARIABLE_RUNTIME_PASSED   (1 << 2)
#define ALL_RUNTIME_PASSED        0x07

/***************************************************
 *                 BIT MASK
 * b.0  - Clock test
 * b.1  - Invariable memory RunTime
 * b.2  - Variable memory Runtime
 ***************************************************/
uint32_t runtime_test_finish_flag = 0; /* no test performed */

/* This function is only for development validation -
 * indicate that runtime test run at leaset one. */
static void test_end(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief   Initialization of Safety clock test.
 *
 *          Complete Initialization of the clock test.
 *          Function calculates limit values.
 *          Cals clock test init function from the IEC60730B library.
 *
 * @param   psSafetyCommon    - The pointer of the Common Safety structure
 * @param   psSafetyClockTest - The pointer of the Safety Clock test structure
 * @param   peClockFreq       - The pointer of the clock name enumeration
 *
 * @return  None
 */
void SafetyClockTestInit(safety_common_t *psSafetyCommon, fs_clock_test_t *psSafetyClockTest)
{
    psSafetyCommon->refClkFreq           = REF_TIMER_CLOCK_FREQUENCY;
    psSafetyClockTest->clockTestExpected = psSafetyCommon->refClkFreq / (uint32_t)ISR_FREQUENCY;
    psSafetyClockTest->clockTestTolerance =
        (psSafetyClockTest->clockTestExpected * (uint32_t)CLOCK_TEST_TOLERANCE) / (uint32_t)100;
    psSafetyClockTest->clockTestLimitHigh =
        psSafetyClockTest->clockTestExpected + psSafetyClockTest->clockTestTolerance;
    psSafetyClockTest->clockTestLimitLow = psSafetyClockTest->clockTestExpected - psSafetyClockTest->clockTestTolerance;
    psSafetyClockTest->clockTestStart    = 0; /* clock test result will be processed after the first interrupt occurs */

    FS_CLK_Init((uint32_t *)&psSafetyClockTest->clockTestContext);

    /* Initialization of timer */
    CTIMER0_Init();   
}

/*!
 * @brief   Clock test function, called from interrupt.
 *
 *          This function calls clock test function from the IEC60730B library and enable the test evaluation.
 *          It must be called in the Systick interrupt to catch the value of LPTMR counter.
 *
 * @param   psSafetyClockTest - The pointer of the Safety Clock test structure
 *
 * @return  None
 */
void SafetyClockTestIsr(fs_clock_test_t *psSafetyClockTest)
{
    FS_CLK_CTIMER((fs_ctimer_t *)CTIMER0, (uint32_t *)&psSafetyClockTest->clockTestContext);

    if (psSafetyClockTest->clockTestStart != 2)
    {
        psSafetyClockTest->clockTestStart += 1; /* to prevent checking of result before execution */
    }
}

/*!
 * @brief   Clock test check function.
 *
 *          This function can be called from any place of application.
 *          It calls the FS_CLK_Check function from the IEC60730 library
 *          In case of incorrect clock test result, it updates the safetyErrors variable accordingly.
 *          A node of program flow check is placed here.
 *
 * @param   psSafetyCommon          - The pointer of the Common Safety structure
 * @param   psSafetyClockTest       - The pointer of the Safety Clock test structure
 * @param   psSafetyProgramFlowTest - The pointer of the Program flow test structure
 *
 * @return  None
 */
void SafetyClockTestCheck(safety_common_t *psSafetyCommon, fs_clock_test_t *psSafetyClockTest)
{
    if (psSafetyClockTest->clockTestStart) /* condition is valid after the first Systick interrupt */
    {
        psSafetyCommon->CLOCK_test_result =
            FS_CLK_Check(psSafetyClockTest->clockTestContext, psSafetyClockTest->clockTestLimitLow,
                         psSafetyClockTest->clockTestLimitHigh);
        if (psSafetyCommon->CLOCK_test_result == FS_FAIL_CLK)
        {
            psSafetyCommon->safetyErrors |= CLOCK_TEST_ERROR;
            SafetyErrorHandling(psSafetyCommon);
        }

        runtime_test_finish_flag |= CLOCK_PASSED; /*Variable for development run test, useless for final application */
    }
}

/*!
 * @brief   Initialization of Safety Flash test.
 *
 *          Enable clock for HW CRC module.
 *          Inits the Flash test variables
 *
 * @param   psFlashCrc    - The pointer of the Flash CRC structure.
 * @param   psFlashConfig - The pointer of the Safety Flash test configuration structure.
 *
 * @return  None
 */
void SafetyFlashTestInit(fs_flash_runtime_test_parameters_t *psFlashCrc,
                         fs_flash_configuration_parameters_t *psFlashConfig)
{
#if HW_FLASH_TEST
    /* Enable clock to CRC module */
    RESET_PeripheralReset(kCRC_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Crc);
#endif

#if defined(__IAR_SYSTEMS_ICC__) /* IAR */
    psFlashConfig->startAddress = (uint32_t)&c_checksumStart;
    psFlashConfig->endAddress =
        4 + (uint32_t)&c_checksumEnd; /* We must test also last adress, due to this reason +4 in IAR */
    psFlashConfig->checksum = __checksum;

#else /* KEIL + MCUXpresso */
    psFlashConfig->startAddress       = c_sfsCRC.ui32FlashStart;
    psFlashConfig->endAddress         = c_sfsCRC.ui32FlashEnd;
    psFlashConfig->checksum           = c_sfsCRC.ui32CRC;
#endif

    psFlashConfig->size               = psFlashConfig->endAddress - psFlashConfig->startAddress ;
    psFlashConfig->blockSize          = FLASH_TEST_BLOCK_SIZE;
    psFlashConfig->startConditionSeed = (uint32_t)FLASH_TEST_CONDITION_SEED;

    psFlashCrc->actualAddress = psFlashConfig->startAddress;       /* start address */
    psFlashCrc->partCrc       = psFlashConfig->startConditionSeed; /* initial seed */
    psFlashCrc->blockSize =
        (psFlashConfig->size < psFlashConfig->blockSize) ? psFlashConfig->size : psFlashConfig->blockSize;
}

/*!
 * @brief   After-reset Flash test.
 *
 *          This function calls the flash test function from IEC60730 library.
 *          Safety-related part of the flash is tested at once.
 *          In case of incorrect flash test result, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 * @param   psFlashConfig  - The pointer of the Safety Flash test configuration structure.
 *
 * @return  None
 */
void SafetyFlashAfterResetTest(safety_common_t *psSafetyCommon, fs_flash_configuration_parameters_t *psFlashConfig)
{
#if HW_FLASH_TEST
    psSafetyCommon->FLASH_test_result = FS_CM33_FLASH_HW32(psFlashConfig->startAddress, psFlashConfig->size, CRC_BASE,
                                                             psFlashConfig->startConditionSeed);

    if ((uint32_t)psSafetyCommon->FLASH_test_result != psFlashConfig->checksum)
    {
        psSafetyCommon->safetyErrors |= FLASH_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#else
    psSafetyCommon->FLASH_test_result = FS_CM33_FLASH_SW32(psFlashConfig->startAddress, psFlashConfig->size, (uint32_t)CRC_BASE,
                                                           psFlashConfig->startConditionSeed);

    if ((uint32_t)psSafetyCommon->FLASH_test_result != psFlashConfig->checksum)
    {
        psSafetyCommon->safetyErrors |= FLASH_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif
}

/*!
 * @brief   Runtime Flash test.
 *
 *          This function calls the flash test function from IEC60730 library.
 *          Safety-related part of the flash is tested in sequence.
 *          Calls SafetyFlashTestHandling function.
 *          In case of incorrect flash test result, it updates the safetyErrors variable accordingly.
 *          A node of program flow check is placed here.
 *
 * @param   psSafetyCommon          - The pointer of the Common Safety structure
 * @param   psFlashCrc              - The pointer of the Flash CRC structure.
 * @param   psFlashConfig           - The pointer of the Safety Flash test configuration structure.
 * @param   psSafetyProgramFlowTest - The pointer of the Program flow test structure
 *
 * @return  None
 */
void SafetyFlashRuntimeTest(safety_common_t *psSafetyCommon,
                            fs_flash_runtime_test_parameters_t *psFlashCrc,
                            fs_flash_configuration_parameters_t *psFlashConfig)
{
/* CRC calculation for a given block of Flash memory */
#if HW_FLASH_TEST
    psFlashCrc->partCrc = FS_CM33_FLASH_HW32(psFlashCrc->actualAddress, psFlashCrc->blockSize, CRC_BASE, psFlashCrc->partCrc);
#else
    psFlashCrc->partCrc = FS_CM33_FLASH_SW32(psFlashCrc->actualAddress, psFlashCrc->blockSize, (uint32_t)CRC_BASE, psFlashCrc->partCrc);
#endif

    if (FS_FLASH_FAIL == SafetyFlashTestHandling(psFlashCrc, psFlashConfig))
    {
        psSafetyCommon->safetyErrors |= FLASH_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

/*!
 * @brief   Handling of flash test when used in runtime.
 *
 *          The function updates the flash test variables, when flash is tested in sequence.
 *
 * @param   __checksum    - Constant that is calculated by Linker and stored in Flash.
 * @param   psFlashCrc    - The pointer of the Flash CRC structure.
 * @param   psFlashConfig - The pointer of the Safety Flash test configuration structure.
 *
 * @return  Result of the flash test: FS_ST_FLASH_FAIL or FS_ST_FLASH_PASS
 */
uint32_t SafetyFlashTestHandling(fs_flash_runtime_test_parameters_t *psFlashCrc,
                                 fs_flash_configuration_parameters_t *psFlashConfig)
{
    psFlashCrc->actualAddress += psFlashCrc->blockSize;         /* set the actual address for testing */
    if (psFlashCrc->actualAddress == psFlashConfig->endAddress) /* if all the addresses were tested... */
    {
        if ((uint32_t)psFlashCrc->partCrc ==
            psFlashConfig->checksum) /* checksum must be same as calculated in linker */
        {
            psFlashCrc->finalCrc = (uint32_t)psFlashCrc->partCrc; /* Store final CRC value */

            psFlashCrc->partCrc = psFlashConfig->startConditionSeed; /* set start seed as input for CRC calculation */
            psFlashCrc->actualAddress = psFlashConfig->startAddress; /* set start address */
            psFlashCrc->blockSize     = psFlashConfig->blockSize;    /* size of block for CRC testing */

            runtime_test_finish_flag |=
                INVARIABLE_RUNTIME_PASSED; /*Variable for development run test, useless for final application */

            return FS_FLASH_PASS;
        }
        else
        {
            return FS_FLASH_FAIL;
        }
    }
    else
    {
        if (psFlashConfig->endAddress - psFlashCrc->actualAddress <
            psFlashConfig->blockSize) /* set size of last block */
        {
            psFlashCrc->blockSize =
                psFlashConfig->endAddress - psFlashCrc->actualAddress; /* arrange the block size for remaining memory */
        }
        return FS_FLASH_PASS;
    }
}

/*!
 * @brief   Initialization of Safety RAM test.
 *
 *          Inits the RAM test variables
 *
 * @param   psSafetyRamTest - The pointer of the RAM test structure.
 * @param   pSafetyRamStart - The pointer of the RAM test start address.
 * @param   pSafetyRamEnd   - The pointer of the RAM test end address.
 *
 * @return  None
 */
void SafetyRamTestInit(fs_ram_test_t *psSafetyRamTest, uint32_t *pSafetyRamStart, uint32_t *pSafetyRamEnd)
{
    psSafetyRamTest->ramTestStartAddress = (uint32_t)pSafetyRamStart;
    psSafetyRamTest->ramTestEndAddress   = (uint32_t)pSafetyRamEnd;
    psSafetyRamTest->defaultBlockSize    = RAM_TEST_BACKUP_SIZE;
    psSafetyRamTest->blockSize           = RAM_TEST_BLOCK_SIZE;
    psSafetyRamTest->actualAddress       = psSafetyRamTest->ramTestStartAddress;

#if (defined(__GNUC__) && (__ARMCC_VERSION >= 6010050)) /* KEIL */
    psSafetyRamTest->backupAddress = (uint32_t)m_ram_test_backup;
#else /* IAR + MCUXpresso */
    psSafetyRamTest->backupAddress          = (uint32_t)&m_ram_test_backup;
#endif
}

/*!
 * @brief   After-reset RAM test.
 *
 *          This function calls the RAM test function from IEC60730 library.
 *          Safety-related part of the RAM is tested at once.
 *          In case of incorrect RAM test result, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon  - The pointer of the Common Safety structure
 * @param   psSafetyRamTest - The pointer of the Safety RAM test structure.
 *
 * @return  None
 */
void SafetyRamAfterResetTest(safety_common_t *psSafetyCommon, fs_ram_test_t *psSafetyRamTest)
{
    psSafetyCommon->RAM_test_result = FS_CM33_RAM_AfterReset(
        psSafetyRamTest->ramTestStartAddress, psSafetyRamTest->ramTestEndAddress, psSafetyRamTest->defaultBlockSize,
        psSafetyRamTest->backupAddress, FS_CM33_RAM_SegmentMarchC);
    
    if (psSafetyCommon->RAM_test_result == FS_FAIL_RAM)
    {
        psSafetyCommon->safetyErrors |= RAM_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

/*!
 * @brief   Runtime RAM test.
 *
 *          This function calls the RAM test function from IEC60730 library.
 *          Safety-related part of the RAM is tested in sequence.
 *          Calls SafetyFlashTestHandling function.
 *          In case of incorrect RAM test result, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon  - The pointer of the Common Safety structure
 * @param   psSafetyRamTest - The pointer of the Safety RAM test structure.
 *
 * @return  None
 */
void SafetyRamRuntimeTest(safety_common_t *psSafetyCommon, fs_ram_test_t *psSafetyRamTest)
{
    psSafetyCommon->RAM_test_result =
        FS_CM33_RAM_Runtime(psSafetyRamTest->ramTestStartAddress, psSafetyRamTest->ramTestEndAddress,
                               (uint32_t *)&psSafetyRamTest->actualAddress, psSafetyRamTest->blockSize,
                               psSafetyRamTest->backupAddress, FS_CM33_RAM_SegmentMarchX);

    if (psSafetyCommon->RAM_test_result == FS_FAIL_RAM)
    {
        psSafetyCommon->safetyErrors |= RAM_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    /* Test if whole RAM was tested, only for example validation */
    if (psSafetyRamTest->ramTestStartAddress == psSafetyRamTest->actualAddress)
        runtime_test_finish_flag |=
            VARIABLE_RUNTIME_PASSED; /*Variable for development run test, useless for final application */
}

/*!
 * @brief   Program counter test.
 *
 *          This function uses two addresses: first is defined in linker file (iec60730b_cm33_pc_object.o),
 *          second address comes as function argument (must be RAM address).
 *          Both addresses must be defined by the developer and suitable to test all of the possible PC bits.
 *          This test cannot be interrupted.
 *          In case of incorrect PC test result, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 * @param   pattern        - RAM address, it can vary with multiple function calls
 *
 * @return  None
 */
void SafetyPcTest(safety_common_t *psSafetyCommon, uint32_t pattern)
{
    psSafetyCommon->PC_test_result = FS_CM33_PC_Test(pattern, FS_PC_Object, (uint32_t *)PC_TEST_FLAG);
    
    if (psSafetyCommon->PC_test_result == FS_FAIL_PC)
    {
        psSafetyCommon->safetyErrors |= PC_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

/*!
 * @brief   After-reset CPU registers test.
 *
 *          This function calls the CPU test functions from IEC60730 library.
 *          All the registers are tested at once.
 *          In case of incorrect flash test result, it updates the safetyErrors variable accordingly.
 *          See IEC60730 library documentation for CPU errors handling !
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
void SafetyCpuAfterResetTest(safety_common_t *psSafetyCommon)
{
#if DSP_SUPPORT
    /* stacked CPU registers */
    psSafetyCommon->CPU_reg_test_result = FS_CM33_CPU_Register();
    if (psSafetyCommon->CPU_reg_test_result == FS_FAIL_CPU_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_REGISTERS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#else   
    /* stacked CPU registers */
    psSafetyCommon->CPU_reg_test_result = FS_CM33_CPU_Register_NDSP();
    if (psSafetyCommon->CPU_reg_test_result == FS_FAIL_CPU_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_REGISTERS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif
    
    /* non-stacked CPU registers */
    psSafetyCommon->CPU_non_stacked_test_result = FS_CM33_CPU_NonStackedRegister();
    if (psSafetyCommon->CPU_non_stacked_test_result == FS_FAIL_CPU_NONSTACKED_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_NONSTACKED_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    
    /* PRIMASK Secure */
    psSafetyCommon->CPU_primask_s_test_result = FS_CM33_CPU_Primask_S();
    if (psSafetyCommon->CPU_primask_s_test_result == FS_FAIL_CPU_PRIMASK)
    {
        psSafetyCommon->safetyErrors |= CPU_PRIMASK_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    
#if TZ_SUPPORT
    /* PRIMASK Non-Secure */
    psSafetyCommon->CPU_primask_ns_test_result = FS_CM33_CPU_Primask_NS();
    if (psSafetyCommon->CPU_primask_ns_test_result == FS_FAIL_CPU_PRIMASK)
    {
        psSafetyCommon->safetyErrors |= CPU_PRIMASK_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif
    
    /* SP main Secure */
    FS_CM33_CPU_SPmain_S();
    
#if TZ_SUPPORT
    /* SP main Non-Secure */
    FS_CM33_CPU_SPmain_NS();
#endif

    /* SP main limit Secure */
    FS_CM33_CPU_SPmain_Limit_S();
    
#if TZ_SUPPORT
    /* SP main limit Non-Secure */
    FS_CM33_CPU_SPmain_Limit_NS();
#endif

    /* SP process Secure */
    FS_CM33_CPU_SPprocess_S();
    
#if TZ_SUPPORT
    /* SP process Non-Secure */
    FS_CM33_CPU_SPprocess_NS();
#endif

    /* SP process limit Secure */
    FS_CM33_CPU_SPprocess_Limit_S();
    
#if TZ_SUPPORT
    /* SP process limit Non-Secure */
    FS_CM33_CPU_SPprocess_Limit_NS();
#endif

    psSafetyCommon->CPU_control_s_test_result = FS_CM33_CPU_Control_S();
    if (psSafetyCommon->CPU_control_s_test_result == FS_FAIL_CPU_CONTROL)
    {
        psSafetyCommon->safetyErrors |= CPU_CONTROL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    
#if TZ_SUPPORT
    /* CONTROL Non-Secure */
    psSafetyCommon->CPU_control_ns_test_result = FS_CM33_CPU_Control_NS();
    if (psSafetyCommon->CPU_control_ns_test_result == FS_FAIL_CPU_CONTROL)
    {
        psSafetyCommon->safetyErrors |= CPU_CONTROL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif

    /* Special Secure */
    psSafetyCommon->CPU_special_s_test_result = FS_CM33_CPU_Special8PriorityLevels_S();
    if (psSafetyCommon->CPU_special_s_test_result == FS_FAIL_CPU_SPECIAL)
    {
        psSafetyCommon->safetyErrors |= CPU_SPECIAL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    
#if TZ_SUPPORT
    /* Special Non-Secure */
    psSafetyCommon->CPU_special_ns_test_result = FS_CM33_CPU_Special8PriorityLevels_NS();
    if (psSafetyCommon->CPU_special_ns_test_result == FS_FAIL_CPU_SPECIAL)
    {
        psSafetyCommon->safetyErrors |= CPU_SPECIAL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif

#if FPU_SUPPORT
    psSafetyCommon->CPU_fpu_test_result = FS_CM33_CPU_Float1();
    if (psSafetyCommon->CPU_fpu_test_result == FS_FAIL_CPU_FLOAT_1)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_1_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    psSafetyCommon->CPU_fpu_test_result = FS_CM33_CPU_Float2();
    if (psSafetyCommon->CPU_fpu_test_result == FS_FAIL_CPU_FLOAT_2)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_2_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif
}

/*!
 * @brief   Uninterruptible test of CPU registers.
 *
 *          This function calls the CPU test functions from IEC60730 library.
 *          The function must be called from an interrupt with highest priority.
 *          In case of incorrect flash test result, it updates the safetyErrors variable accordingly.
 *          See IEC60730 library documentation for CPU errors handling !
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
void SafetyCpuIsrTest(safety_common_t *psSafetyCommon)
{
    /* PRIMASK Secure */
    psSafetyCommon->CPU_primask_s_test_result = FS_CM33_CPU_Primask_S();
    if (psSafetyCommon->CPU_primask_s_test_result == FS_FAIL_CPU_PRIMASK)
    {
        psSafetyCommon->safetyErrors |= CPU_PRIMASK_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    /* Special Secure */
    psSafetyCommon->CPU_special_s_test_result = FS_CM33_CPU_Special8PriorityLevels_S();
    if (psSafetyCommon->CPU_special_s_test_result == FS_FAIL_CPU_SPECIAL)
    {
        psSafetyCommon->safetyErrors |= CPU_SPECIAL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    /* SP main Secure */
    FS_CM33_CPU_SPmain_S();

    /* SP main limit Secure */
    FS_CM33_CPU_SPmain_Limit_S();

#if TZ_SUPPORT /* If device supports TrustZone */
    /* PRIMASK Non-Secure */
    psSafetyCommon->CPU_primask_ns_test_result = FS_CM33_CPU_Primask_NS();
    if (psSafetyCommon->CPU_primask_ns_test_result == FS_FAIL_CPU_PRIMASK)
    {
        psSafetyCommon->safetyErrors |= CPU_PRIMASK_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
	
    /* Special Non-Secure */
    psSafetyCommon->CPU_special_ns_test_result = FS_CM33_CPU_Special8PriorityLevels_NS();
    if (psSafetyCommon->CPU_special_ns_test_result == FS_FAIL_CPU_SPECIAL)
    {
        psSafetyCommon->safetyErrors |= CPU_SPECIAL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif
}

/*!
 * @brief   Interruptible test of CPU registers.
 *
 *          This function calls the CPU test functions from IEC60730 library.
 *          The function can be called from the background loop.
 *          In case of incorrect flash test result, it updates the safetyErrors variable accordingly.
 *          See IEC60730 library documentation for CPU errors handling !
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
void SafetyCpuBackgroundTest(safety_common_t *psSafetyCommon)
{
#if DSP_SUPPORT
    /* stacked CPU registers */
    psSafetyCommon->CPU_reg_test_result = FS_CM33_CPU_Register();
    if (psSafetyCommon->CPU_reg_test_result == FS_FAIL_CPU_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_REGISTERS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#else   
    /* stacked CPU registers */
    psSafetyCommon->CPU_reg_test_result = FS_CM33_CPU_Register_NDSP();
    if (psSafetyCommon->CPU_reg_test_result == FS_FAIL_CPU_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_REGISTERS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif

    psSafetyCommon->CPU_non_stacked_test_result = FS_CM33_CPU_NonStackedRegister();
    if (psSafetyCommon->CPU_non_stacked_test_result == FS_FAIL_CPU_NONSTACKED_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_NONSTACKED_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    
#if FPU_SUPPORT
    psSafetyCommon->CPU_fpu_test_result = FS_CM33_CPU_Float1();
    if (psSafetyCommon->CPU_fpu_test_result == FS_FAIL_CPU_FLOAT_1)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_1_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    psSafetyCommon->CPU_fpu_test_result = FS_CM33_CPU_Float2();
    if (psSafetyCommon->CPU_fpu_test_result == FS_FAIL_CPU_FLOAT_2)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_2_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif
}

/*!
 * @brief   Initialization of Stack test.
 *
 *          This function calls FS_CM33_STACK_Init function from IEC60730 library.
 *
 * @param   void - macros from header files define the parameters
 *
 * @return  None
 */
void SafetyStackTestInit(void)
{
    FS_CM33_STACK_Init(STACK_TEST_PATTERN, c_stackTestFirstAddress, c_stackTestSecondAddress, STACK_TEST_BLOCK_SIZE);
}

/*!
 * @brief   Stack test.
 *
 *          This function calls the STACK test function from IEC60730 library
 *          Stack is tested for underflow and overflow condition.
 *          In case of incorrect Stack test result, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
void SafetyStackTest(safety_common_t *psSafetyCommon)
{
    psSafetyCommon->STACK_test_result = FS_CM33_STACK_Test(STACK_TEST_PATTERN, c_stackTestFirstAddress,
                                                           c_stackTestSecondAddress, STACK_TEST_BLOCK_SIZE);
    if (psSafetyCommon->STACK_test_result == FS_FAIL_STACK)
    {
        psSafetyCommon->safetyErrors |= STACK_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

/*!
 * @brief   Handling of the safety functions that must be called in interrupt routine.
 *
 *          This function switches between safety functions that are called in interrupt
 *
 * @param   psSafetyCommon       - The pointer of the Common Safety structure
 * @param   psSafetyRamTest      - The pointer of the Safety RAM test structure.
 * @param   psSafetyRamStackTest - The pointer of the Safety RAM test structure for Stack area.
 *
 * @return  None
 */
void SafetyIsrFunction(safety_common_t *psSafetyCommon, fs_ram_test_t *psSafetyRamTest, fs_ram_test_t *psSafetyRamStackTest)
{
    switch (psSafetyCommon->fastIsrSafetySwitch)
    {
        case 0: /* CPU registers test that cannot be interrupted */
            SafetyCpuIsrTest(psSafetyCommon);
            break;
        case 1: /* Program counter test */
#if PC_TEST_ENABLED
            SafetyPcTest(psSafetyCommon, PC_TEST_PATTERN);
#endif
            break;
        case 2: /* RAM March test for safety related RAM space */
#if RAM_TEST_ENABLED
            SafetyRamRuntimeTest(psSafetyCommon, psSafetyRamTest);
#endif
            break;
        case 3: /* RAM March test for memory occupied by the Stack */
#if RAM_TEST_ENABLED
            SafetyRamRuntimeTest(psSafetyCommon, psSafetyRamStackTest);
#endif
            break;
        default:
            __asm("nop");
            break;
    }

    psSafetyCommon->fastIsrSafetySwitch++;
    if (psSafetyCommon->fastIsrSafetySwitch == 4)
        psSafetyCommon->fastIsrSafetySwitch = 0;
}

/* Function for example validation during  development */
void development_test_terminate(void)
{
    static uint8_t runtime_counter = 0;
    if (runtime_test_finish_flag == ALL_RUNTIME_PASSED)
    {
        runtime_test_finish_flag = 0;
        runtime_counter++;
        if (runtime_counter == 10)
        {
            test_end(); /* Mark that runtime test run */
        }
    }
}

/* This function is only for example validation during development */
static void test_end(void)
{
    __asm("nop");
}

/*!
 * @brief   Handling with a safety error.
 *
 *          This function stores the code of recognized safety error into the dedicated RAM memory that is deleted only
 * after POR. If SAFETY_ERROR_ACTION macro is defined, interrupts are disabled and function waits for watchdog reset.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
void SafetyErrorHandling(safety_common_t *psSafetyCommon)
{
  *SAFETY_ERROR_CODE = psSafetyCommon->safetyErrors;
  test_end(); /* Only for example validation. Indicate that some error occured and test can be stoped */
#if SAFETY_ERROR_ACTION
  __asm("CPSID i"); /* disable interrupts */
  while (1){
    #if FMSTR_SERIAL_ENABLE
        FMSTR_Poll(); /* Freemaster cummunication */
    #endif
  }
#endif
}
