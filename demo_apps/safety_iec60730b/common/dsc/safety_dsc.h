/*
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2015-2021 NXP.
 *
 * License: LA_OPT_NXP_Software_License
 *
 * This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms. If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.
 *
 * @version 4.0
 *
 * @brief safety handling functions
 * 
 */

#ifndef _IEC60730_SAFETY_H_
#define _IEC60730_SAFETY_H_

#include "safety_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CPU_TEST_ERROR                     0x1  /* CPU test fault flag */
#define FLASH_TEST_ERROR                   0x2  /* Flash test fault flag */
#define RAM_TEST_ERROR                     0x4  /* RAM test fault flag */
#define STACK_TEST_ERROR                   0x8  /* Stack test fault flag */
#define CLOCK_TEST_ERROR                  0x10  /* Clock test fault flag */
#define PC_TEST_ERROR                     0x20  /* Program counter test fault flag */
#define CPU_PRIMASK_ERROR                 0x40  /* PRIMASK test fault flag */
#define CPU_SP_MAIN_ERROR                 0x80  /* main SP test fault flag */
#define CPU_REGISTERS_ERROR              0x100  /* CPU registers test fault flag */
#define CPU_NONSTACKED_ERROR             0x200  /* non-stacked CPU test fault flag */
#define CPU_CONTROL_ERROR               0x1000  /* CONTROL register test fault flag */
#define CPU_SP_PROCESS_ERROR            0x2000  /* process SP test fault flag */
#define DIO_TEST_ERROR                  0x8000  /* DIO test fault flag */
#define AIO_TEST_ERROR                 0x10000  /* AIO test fault flag */
#define TSI_TEST_ERROR                0x200000  /* TSI test fault flag */

#define FS_FLASH_PASS         0x0        /* Flash test pass return */
#define FS_FLASH_FAIL         0x00000301 /* Flash test fail return */
#define FS_FLASH_PROGRESS     0x00000302



#define __SECTION_EXT(type, bank, name) __attribute__ ((section("." #type ".$" #bank "." #name)))
#define __SECTION(type, bank) __attribute__ ((section("." #type ".$" #bank)))
#define __SECTION_SIMPLE(type) __attribute__ ((section("." #type)))

#define __DATA_EXT(bank, name) __SECTION_EXT(data, bank, name)
#define __DATA(bank) __SECTION(data, bank)

#define __BSS_EXT(bank, name) __SECTION_EXT(bss, bank, name)
#define __BSS(bank) __SECTION(bss, bank)

// Macros for placing text (code), data, or bss into a section that is automatically
// placed after the vectors in the target image.
#define __AFTER_VECTORS_EXT(name) __attribute__ ((section(".after_vectors.$" #name)))
#define __AFTER_VECTORS __attribute__ ((section(".after_vectors")))

// Macros for causing functions to be relocated to RAM
#define __RAM_FUNC_EXT(name) __attribute__ ((section(".ramfunc.$" #name)))
#define __RAM_FUNC __attribute__ ((section(".ramfunc")))

// Macros to be used in preference to __RAM_FUNC to better match __DATA behaviour
#define __RAMFUNC_EXT(bank, name) __SECTION_EXT(ramfunc, bank, name)
#define __RAMFUNC(bank) __SECTION(ramfunc, bank)

// Macros for placing data or bss into a section that has the NOLOAD option set in the linker script
#define __NOINIT_DEF __SECTION_SIMPLE(noinit)
#define __NOINIT_EXT(bank, name) __SECTION_EXT(noinit, bank, name)
#define __NOINIT(bank) __SECTION(noinit, bank)

// Macros for placing text (code), or rodata into a different (flash) bank
#define __RODATA_EXT(bank,name) __SECTION_EXT(rodata, bank, name)
#define __RODATA(bank) __SECTION(rodata, bank)

#define __TEXT_EXT(bank,name) __SECTION_EXT(text, bank, name)
#define __TEXT(bank) __SECTION(text, bank)

typedef struct {
	unsigned long start_addr;
	unsigned long mem_length;
	unsigned short mem_type;
	unsigned short poly;
	unsigned short crc16;
} CRC16_record;  

/*! @brief Safety tests */
typedef struct _safety_common
{
    uint16_t fastIsrSafetySwitch;
    uint16_t safetyErrors;
    uint16_t CLOCK_test_result;
    uint16_t FLASH_test_result;
    uint16_t RAM_test_result;
    uint16_t PC_test_result;
    uint16_t CPU_DataRegisters_result;
    uint16_t CPU_PointerRegisters_result;
    uint16_t CPU_ShadowRegistersE_result;
    uint16_t CPU_ShadowRegistersEX_result;
    uint16_t CPU_eFPURegisters_result;
    uint16_t CPU_StatusRegister_result;
    uint16_t CPU_StackPointer_result;
    uint16_t CPU_sp_LoopRegisters_result;
    uint16_t STACK_test_result;
    uint16_t DIO_input_test_result;
    uint16_t DIO_output_test_result;
    uint16_t DIO_short_test_result;
    uint16_t AIO_test_result;
    uint16_t TSI_test_result;
    uint16_t cpuClkFreq;
    uint16_t mcgirclkFreq;
    uint16_t lpoFreq;
} safety_common_t;


typedef struct _ram_test
{
    uint32_t ui32RamTestStartAddress;
    uint32_t ui32RamTestEndAddress;    
    uint16_t ui16BlockSize;
    uint32_t ui32ActualAddress;
    uint16_t ui16DefaultBlockSize;
    uint32_t ui32BackupAddress;
    uint16_t ui16RamTestRange;
} ram_test_t;

/*! @brief Safety Flash test runtime */
typedef struct _flash_runtime_test_parameters
{
    uint32_t blockSize;         /* size of tested block */
    uint32_t actualAddress;     /* actual start address for crc module */
    uint32_t partCrc;           /* seed in begin, particular crc result in process, crc result in final*/
} flash_runtime_test_parameters_t;

/*! @brief Safety Flash test parameters */
typedef struct _flash_configuration_parameters
{
    uint32_t startConditionSeed;
    uint32_t startAddress;
    uint32_t endAddress;
    uint32_t size;
    uint32_t blockSize;
    uint16_t checksum_linker;
    uint16_t checksum_application;
} flash_configuration_parameters_t;

/*! @brief Safety Watchdog test */
typedef struct _cop_test
{
    uint32_t ui32CopTestExpected;
    uint32_t ui32CopTestTolerance;
    uint32_t ui32CopTestLimitHigh;
    uint32_t ui32CopTestLimitLow;
    uint32_t ui32CopTimeoutCheck;
    uint16_t ui16CopResets;
    uint16_t ui16CopRefreshRatio;
    uint16_t ui16CopErrorFlag;
} cop_test_t;



#ifdef __cplusplus
}
#endif

unsigned short SafetyWatchdogTest(cop_test_t *psSafetyWdTest);
void SafetyRamTestInit(ram_test_t *psSafetyRamTest, uint32_t *pui32SafetyRamStart, uint32_t *pui32SafetyRamEnd);
void SafetyRamAfterResetTest(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest);
void SafetyRamRuntimeTest(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest);

void SafetyCpuAfterResetTest(safety_common_t *psSafetyCommon);

void SafetyFlashTestInit(flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig, __pmem CRC16_record *psCrc_linker);
void SafetyFlashAfterResetTest(safety_common_t *psSafetyCommon, flash_configuration_parameters_t *psFlashConfig);
uint16_t SafetyFlashTestHandling(flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig);
void SafetyFlashRuntimeTest(safety_common_t *psSafetyCommon, flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig);

void SafetyStackTestInit(void);
void SafetyStackTest(safety_common_t *psSafetyCommon);

void SafetyDigitalOutputTest(safety_common_t *psSafetyCommon, fs_dio_test_t *pTestedPin);
void SafetyDigitalInputOutput_ShortAdjTest(safety_common_t *psSafetyCommon, fs_dio_test_t *pTestedPin, fs_dio_test_t *pAdjPin, uint16_t pinValue);
void SafetyDigitalInputOutput_ShortSupplyTest(safety_common_t *psSafetyCommon, fs_dio_test_t *pTestedPin, uint8_t polarity);

void SafetyClockTestInit(safety_common_t *psSafetyCommon, fs_clock_test_t *psSafetyClockTest);
void SafetyClockTestCheck(safety_common_t *psSafetyCommon, fs_clock_test_t *psSafetyClockTest);

void SafetyAnalogTestInitialization(void);
void SafetyAnalogTest(safety_common_t *psSafetyCommon);

void SafetyErrorHandling(safety_common_t *psSafetyCommon);
#endif /* _IEC60730_SAFETY_H_ */
