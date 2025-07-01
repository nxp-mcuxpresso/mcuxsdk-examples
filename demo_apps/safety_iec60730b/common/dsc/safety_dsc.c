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

#include "fsl_pit.h"
#include "fsl_qtmr.h"
#include "fsl_cadc.h"
#include "fsl_sim.h"
#include "safety_dsc.h"
#include "freemaster.h"
#include "freemaster_serial_qsci.h"

#include "safety_config.h"

extern uint32_t _wd_test_backup;   /* from Linker configuration file */

#define WATCHDOG_TEST_VARIABLES ((fs_wdog_test  *)  &_wd_test_backup)
//#define WATCHDOG_TEST_VARIABLES ((fs_wdog_test  *)  &SIM->SCR0)

extern uint32_t _ram_test_backup_size; /* from Linker command file */
#define RAM_TEST_BACKUP_SIZE (uint32_t)&_ram_test_backup_size

extern uint32_t _ram_test_backup; /* symbol from Linker command file */

extern uint32_t _stack_test_block_size; /* from Linker command file */
#define STACK_TEST_BLOCK_SIZE (uint32_t)&_stack_test_block_size

extern uint16_t _stack_test_p_2;   /* symbol from Linker command file */
extern uint16_t _stack_test_p_3;   /* symbol from Linker command file */

extern qtmr_config_t sTMRA0Config;
extern qtmr_channel_config_t sChannel0Config;
extern pit_config_t sPit1Config;

/* aio test structure */
static fs_aio_test_t aio_Str;
/* limit values */
const fs_aio_limits_t adc_limits[IEC60730B_CFG_AIO_CHANNELS_CNT] = IEC60730B_CFG_AIO_CHANNELS_LIMITS_INIT;
/* channels to scan */
const uint8_t IEC60730B_ADC_inputs[IEC60730B_CFG_AIO_CHANNELS_CNT] = IEC60730B_CFG_AIO_CHANNELS_INIT;
/* samples definition */
const uint8_t IEC60730B_ADC_samples[IEC60730B_CFG_AIO_CHANNELS_CNT] = IEC60730B_CFG_AIO_SAMPLES_INIT;


unsigned short SafetyWatchdogTest(cop_test_t *psSafetyWdTest)
{       
	uint16_t ui16ResetStatus;
	//ui16ResetStatus = SIM->RSTAT && (kSIM_PowerONResetFlag | kSIM_ExternalResetFlag | kSIM_COPLossOfReferenceResetFlag | kSIM_COPCPUTimeOutResetFlag | kSIM_SofwareResetFlag);
	
	psSafetyWdTest->ui32CopTestExpected = ((uint32_t)COP_TIMEOUT_IN_MILI_SECONDS * PIT1_FREQ)/1000;
	psSafetyWdTest->ui32CopTestTolerance = (psSafetyWdTest->ui32CopTestExpected * (uint32_t)WD_TEST_TOLERANCE)/(uint32_t)100;
	psSafetyWdTest->ui32CopTestLimitHigh = psSafetyWdTest->ui32CopTestExpected + psSafetyWdTest->ui32CopTestTolerance;
	psSafetyWdTest->ui32CopTestLimitLow = psSafetyWdTest->ui32CopTestExpected - psSafetyWdTest->ui32CopTestTolerance;
	
	ui16ResetStatus = SIM_GetResetStatusFlags(SIM);
    if(ui16ResetStatus == SIM_RSTAT_COP_CPU_MASK)
    {
    	FS_watchdog_check(psSafetyWdTest->ui32CopTestLimitHigh, psSafetyWdTest->ui32CopTestLimitLow, WATCHDOG_RESETS_LIMIT, ENDLESS_LOOP_ENABLE, WATCHDOG_TEST_VARIABLES);
    }
    else
    {    	
#ifdef WATCHDOG_TEST_ON
    	PIT1_initialization(PIT1_RELOAD, PIT1_CLOCK_DIV);
        FS_watchdog_setup(WATCHDOG_TEST_VARIABLES, PIT1_BASE, PIT_32BIT_COUNTER);
#else
        WATCHDOG_TEST_VARIABLES->resets = 0;           
#endif    
    }

	psSafetyWdTest->ui16CopResets       = WATCHDOG_TEST_VARIABLES->resets;
	psSafetyWdTest->ui32CopTimeoutCheck = WATCHDOG_TEST_VARIABLES->counter;
    psSafetyWdTest->ui16CopErrorFlag    = WATCHDOG_TEST_VARIABLES->wd_test_uncomplete_flag;
    return (uint16_t)ui16ResetStatus;    	
}
/*!
 * @brief   Digital Input/Output test.
 *
 *          This function calls output test functions from IEC60730 library
 *          In case of incorrect test result, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 * @param   pTestedPin     - The pointer to the DIO test item structure (pin definition)
 *
 * @return  None
 */
void SafetyDigitalOutputTest(safety_common_t *psSafetyCommon, fs_dio_test_t *pTestedPin)
{      
	PortInit((GPIO_Type*)pTestedPin->gpio, PIN_DIRECTION_OUT, pTestedPin->pinNum, PIN_PULL_UP);
    
    psSafetyCommon->DIO_output_test_result = FS_DIO_Output(pTestedPin, DIO_WAIT_CYCLE);
    
    if ((psSafetyCommon->DIO_output_test_result) == FS_DIO_FAIL)
    {
    	psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    PortInit((GPIO_Type*)pTestedPin->gpio, PIN_DIRECTION_OUT, pTestedPin->pinNum, PIN_PULL_UP);
}
/*!
 * @brief   Digital Input/Output Short to Adjacent pins test
 *
 *          This function calls digital io short test SET and GET functions from IEC60730 library
 *          In case of incorrect test conditions, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 * @param   *pTestedPin    - The pointer to the DIO test item structure (pin definition)
 * @param   *pAdjPin       - The pointer of the DIO test item structure for Adjacent pin (pin definition)
 * @param   pinValue       - logical 1 or logical 0 will be set on the tested pin  
 *
 * @return  None
 */
void SafetyDigitalInputOutput_ShortAdjTest(safety_common_t *psSafetyCommon, fs_dio_test_t *pTestedPin, fs_dio_test_t *pAdjPin, uint16_t pinValue)
{
	PortInit((GPIO_Type*)pTestedPin->gpio, PIN_DIRECTION_IN, pTestedPin->pinNum, PIN_PULL_UP);
	PortInit((GPIO_Type*)pAdjPin->gpio, PIN_DIRECTION_OUT, pAdjPin->pinNum, PIN_PULL_UP);


    psSafetyCommon->DIO_short_test_result = FS_DIO_ShortToAdjSet(pTestedPin, pAdjPin, pinValue, DIO_BACKUP);
    if ((psSafetyCommon->DIO_short_test_result) == FS_DIO_FAIL)
    {
        psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    /* if needed, place some delay loop here */

    psSafetyCommon->DIO_input_test_result = FS_DIO_InputExt(pTestedPin, pAdjPin, pinValue, DIO_BACKUP);    
    if ((psSafetyCommon->DIO_input_test_result) == FS_DIO_FAIL)
    {
        psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    PortInit((GPIO_Type*)pTestedPin->gpio, PIN_DIRECTION_IN, pTestedPin->pinNum, PIN_PULL_UP);
   	PortInit((GPIO_Type*)pAdjPin->gpio, PIN_DIRECTION_OUT, pAdjPin->pinNum, PIN_PULL_UP);
}

/*!
 * @brief   Digital Input/Output Short to Supply test.
 *
 *          This function calls digital io short test SET and GET functions from IEC60730 library
 *          In case of incorrect test conditions, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon   - The pointer of the Common Safety structure
 * @param   *pTestedPin      - The pointer to the DIO test item structure (pin definition)
 * @param   polarity         - macro DIO_SHORT_TO_VDD_TEST or DIO_SHORT_TO_GND_TEST
 *
 * @return  None
 */
void SafetyDigitalInputOutput_ShortSupplyTest(safety_common_t *psSafetyCommon, fs_dio_test_t *pTestedPin, uint8_t polarity)
{
    uint8_t i; 
	PortInit((GPIO_Type*)pTestedPin->gpio, PIN_DIRECTION_IN, pTestedPin->pinNum, PIN_PULL_UP);
    psSafetyCommon->DIO_short_test_result = FS_DIO_ShortToSupplySet(pTestedPin, polarity, DIO_BACKUP);
    if ((psSafetyCommon->DIO_short_test_result) == FS_DIO_FAIL)
    {
        SafetyErrorHandling(psSafetyCommon);
    }

    /* if needed, place some delay loop here */
    for (i = 0;i< 50; i++)
    {
        asm(NOP);
    }

    psSafetyCommon->DIO_input_test_result = FS_DIO_InputExt(pTestedPin, pTestedPin, polarity, DIO_BACKUP);
    if ((psSafetyCommon->DIO_input_test_result) == FS_DIO_FAIL)
    {
        psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    PortInit((GPIO_Type*)pTestedPin->gpio, PIN_DIRECTION_IN, pTestedPin->pinNum, PIN_PULL_UP);
}


/*!
 * @brief   Initialization of Safety clock test.
 *
 *          Complete Initialization of the clock test.
 *          Function calculates limit values.
 
 * @param   psSafetyCommon    - The pointer of the Common Safety structure
 * @param   psSafetyClockTest - The pointer of the Safety Clock test structure
 *
 * @return  None
 */
void SafetyClockTestInit(safety_common_t *psSafetyCommon, fs_clock_test_t *psSafetyClockTest)
{
  
#if defined(FSL_FEATURE_PIT_32BIT_COUNTER) && FSL_FEATURE_PIT_32BIT_COUNTER
    uint32_t u32PeriodCount = sPit1Config.u32PeriodCount;
#else
    uint32_t u32PeriodCount = (uint32_t)sPit1Config.u16PeriodCount; 
#endif
  
  
  uint32_t SecondaryClockTemp;
  uint32_t PrimaryClockTemp;
  psSafetyClockTest->qtimer_channel_base = (FS_CHAN_QTIMER_Type *)TMRA_BASE;
  SecondaryClockTemp =  (1 << sPit1Config.ePrescaler)*u32PeriodCount;
  SecondaryClockTemp = (uint32_t)QTIMER_SECONDARY_CLK / SecondaryClockTemp;
  PrimaryClockTemp = QTIMER_PRIMARY_CLK / (1 << (sChannel0Config.sInputConfig.ePrimarySource - 8));
  
  psSafetyClockTest->clockTestExpected = PrimaryClockTemp / SecondaryClockTemp;		
  psSafetyClockTest->clockTestTolerance = (psSafetyClockTest->clockTestExpected * (uint32_t)CLOCK_TEST_TOLERANCE) /(uint32_t)100 ;
  psSafetyClockTest->clockTestLimitHigh =  psSafetyClockTest->clockTestExpected + psSafetyClockTest->clockTestTolerance;
  psSafetyClockTest->clockTestLimitLow  =  psSafetyClockTest->clockTestExpected - psSafetyClockTest->clockTestTolerance;
  
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
    psSafetyCommon->CLOCK_test_result = FS_CLK_QTIMER_Check(psSafetyClockTest);
    if (psSafetyCommon->CLOCK_test_result == FS_CLK_FAIL)
     {
         psSafetyCommon->safetyErrors |= CLOCK_TEST_ERROR;
         SafetyErrorHandling(psSafetyCommon);
     }
}


/*!
* @brief   Initialization of Safety RAM test.
*
*          Inits the RAM test variables
*
* @param   psSafetyRamTest     - The pointer of the RAM test structure.
* @param   pui32SafetyRamStart - The pointer of the RAM test start address.
* @param   pui32SafetyRamEnd   - The pointer of the RAM test end address.
*
* @return  None
*/
void SafetyRamTestInit(ram_test_t *psSafetyRamTest, uint32_t *pui32SafetyRamStart, uint32_t *pui32SafetyRamEnd)
{
    psSafetyRamTest->ui32RamTestStartAddress = (uint32_t)pui32SafetyRamStart;
    psSafetyRamTest->ui32RamTestEndAddress =   (uint32_t)pui32SafetyRamEnd;
    psSafetyRamTest->ui16DefaultBlockSize =  RAM_TEST_BACKUP_SIZE;
    psSafetyRamTest->ui16BlockSize = RAM_TEST_BLOCK_SIZE;
    psSafetyRamTest->ui32ActualAddress = psSafetyRamTest->ui32RamTestStartAddress;

    psSafetyRamTest->ui32BackupAddress = (uint32_t)&_ram_test_backup;
    psSafetyRamTest->ui16RamTestRange = psSafetyRamTest->ui32RamTestEndAddress - psSafetyRamTest->ui32RamTestStartAddress;
}


void SafetyRamAfterResetTest(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest)
{
    psSafetyCommon->RAM_test_result = FS_RAM_AfterReset((unsigned long *)psSafetyRamTest->ui32RamTestStartAddress, \
        		                                        (unsigned long *)psSafetyRamTest->ui32BackupAddress,       \
        		                                         FS_RAM_SegmentMarchX,                                 \
        		                                        (unsigned short)psSafetyRamTest->ui16BlockSize,            \
        		                                        (unsigned short)psSafetyRamTest->ui16RamTestRange
                                                         );

    if(psSafetyCommon->RAM_test_result == FS_RAM_FAIL)
    {
        psSafetyCommon->safetyErrors |= RAM_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}


void SafetyRamRuntimeTest(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest)
{
    psSafetyCommon->RAM_test_result = FS_RAM_Runtime((unsigned long *)psSafetyRamTest->ui32RamTestStartAddress, \
                                                     (unsigned long *)psSafetyRamTest->ui32BackupAddress,       \
                                                      FS_RAM_SegmentMarchC,                                 \
                                                     (unsigned long *)&psSafetyRamTest->ui32ActualAddress,      \
                                                     (unsigned short)psSafetyRamTest->ui16BlockSize,            \
                                                     (unsigned short)psSafetyRamTest->ui16RamTestRange
                                                      );
    if(psSafetyCommon->RAM_test_result == FS_RAM_FAIL)
    {
        psSafetyCommon->safetyErrors |= RAM_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}


/*!
 * @brief   After-reset CPU registers test.
 *
 *          This function calls the CPU test functions from IEC60730 library.
 *          All the registers are tested at once.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
void SafetyCpuAfterResetTest(safety_common_t *psSafetyCommon)
{
	psSafetyCommon->CPU_DataRegisters_result = FS_CPU_DataRegisters();
	psSafetyCommon->safetyErrors |= psSafetyCommon->CPU_DataRegisters_result;
	psSafetyCommon->CPU_PointerRegisters_result = FS_CPU_PointerRegisters();
	psSafetyCommon->safetyErrors |= psSafetyCommon->CPU_PointerRegisters_result; 
	psSafetyCommon->CPU_ShadowRegistersE_result = FS_CPU_ShadowRegistersE();
	psSafetyCommon->safetyErrors |= psSafetyCommon->CPU_ShadowRegistersE_result;
	psSafetyCommon->CPU_ShadowRegistersEX_result = FS_CPU_ShadowRegistersEX();
	psSafetyCommon->safetyErrors |= psSafetyCommon->CPU_ShadowRegistersEX_result;
	
	psSafetyCommon->CPU_eFPURegisters_result = FS_CPU_eFPURegisters();
	psSafetyCommon->safetyErrors |= psSafetyCommon->CPU_eFPURegisters_result;
	
	psSafetyCommon->CPU_eFPURegisters_result = FS_CPU_FPCSRregister();
	psSafetyCommon->safetyErrors |= psSafetyCommon->CPU_eFPURegisters_result;	

	psSafetyCommon->CPU_StatusRegister_result = FS_CPU_StatusRegister();
	psSafetyCommon->safetyErrors |= psSafetyCommon->CPU_StatusRegister_result;
	psSafetyCommon->CPU_StackPointer_result = FS_CPU_StackPointer();
	psSafetyCommon->safetyErrors |= psSafetyCommon->CPU_StackPointer_result;
	psSafetyCommon->CPU_sp_LoopRegisters_result = FS_CPU_LoopRegisters();
	psSafetyCommon->safetyErrors |= psSafetyCommon->CPU_sp_LoopRegisters_result;
   
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
void SafetyFlashTestInit(flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig, __pmem CRC16_record *psCrc_linker)
{
	psFlashConfig->checksum_linker = psCrc_linker->crc16;
	psFlashConfig->startAddress = (psCrc_linker->start_addr)>>1;
	psFlashConfig->size = (psCrc_linker->mem_length)>>1;
	psFlashConfig->endAddress = psFlashConfig->startAddress + psFlashConfig->size;
	psFlashConfig->startConditionSeed = 0;
    
	psFlashCrc->actualAddress  = psFlashConfig->startAddress;    /* start address */
	psFlashCrc->partCrc = psFlashConfig->startConditionSeed;    /* initial seed */
	psFlashCrc->blockSize = (psFlashConfig->size < FLASH_TEST_BLOCK_SIZE) ? psFlashConfig->size : FLASH_TEST_BLOCK_SIZE;
	psFlashConfig->blockSize = psFlashCrc->blockSize;
}


void SafetyFlashAfterResetTest(safety_common_t *psSafetyCommon, flash_configuration_parameters_t *psFlashConfig)
{

#if HW_FLASH_TEST
	psFlashConfig->checksum_application = FS_flash_HW16_program((unsigned long*)psFlashConfig->startAddress, (unsigned long)psFlashConfig->size, (unsigned long*)CRC_BASE, (unsigned short)psFlashConfig->startConditionSeed);
#else
	psFlashConfig->checksum_application = FS_flash_SW16_program((unsigned long*)psFlashConfig->startAddress, (unsigned long)psFlashConfig->size, (unsigned long*)CRC_BASE, (unsigned short)psFlashConfig->startConditionSeed);
#endif
			
	if(psFlashConfig->checksum_linker != psFlashConfig->checksum_application)
	{
		psSafetyCommon->FLASH_test_result = FS_FLASH_FAIL;
	}
	else
	{		
		psSafetyCommon->FLASH_test_result = FS_FLASH_PASS;
	}	
}

/*!
 * @brief   Runtime Flash test.
 *
 *          This function calls the flash test function from IEC60730 library.
 *          Safety-related part of the flash is tested in sequence.
 *          Calls SafetyFlashTestHandling function.
 *          In case of incorrect flash test result, it updates the ui32SafetyErrors variable accordingly.
 *          A node of program flow check is placed here.
 *
 * @param   psSafetyCommon          - The pointer of the Common Safety structure
 * @param   psFlashCrc              - The pointer of the Flash CRC structure.
 * @param   psFlashConfig           - The pointer of the Safety Flash test configuration structure.
 * @param   psSafetyProgramFlowTest - The pointer of the Program flow test structure
 *
 * @return  None
 */
void SafetyFlashRuntimeTest(safety_common_t *psSafetyCommon, flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig)
{
#if HW_FLASH_TEST
            psFlashCrc->partCrc = FS_flash_HW16_program((unsigned long*)psFlashCrc->actualAddress, (unsigned long)psFlashCrc->blockSize, (unsigned long*)CRC_BASE, (unsigned short)psFlashCrc->partCrc);
#else
            psFlashCrc->partCrc = FS_flash_SW16_program((unsigned long*)psFlashCrc->actualAddress, (unsigned long)psFlashCrc->blockSize, (unsigned long*)CRC_BASE, (unsigned short)psFlashCrc->partCrc);
#endif

    psSafetyCommon->FLASH_test_result = SafetyFlashTestHandling( psFlashCrc, psFlashConfig);
    if(FS_FLASH_FAIL == psSafetyCommon->FLASH_test_result)
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
 * @return  Result of the flash test: IEC60730B_ST_FLASH_FAIL or IEC60730B_ST_FLASH_PASS
 */
uint16_t SafetyFlashTestHandling( flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig)
{
	uint16_t st = FS_FLASH_FAIL;
    psFlashCrc->actualAddress += psFlashCrc->blockSize;    /* set the actual address for testing */
    if(psFlashCrc->actualAddress == psFlashConfig->endAddress)   /* if all the addresses were tested... */
    {
        psFlashCrc->actualAddress = psFlashConfig->startAddress;         /* set start address */
        psFlashCrc->blockSize = psFlashConfig->blockSize;                /* size of block for CRC testing */
        if((uint16_t)psFlashCrc->partCrc == psFlashConfig->checksum_linker)      /* checksum must be same as calculated in linker */
        {
            psFlashCrc->partCrc = psFlashConfig->startConditionSeed; /* set start seed as input for CRC calculation */
            st = FS_FLASH_PASS;         
        }
        else
        {
        	st = FS_FLASH_FAIL;
        }
    }
    else
    {
        if (psFlashConfig->endAddress - psFlashCrc->actualAddress < psFlashCrc->blockSize) /* set size of last block */
        {
            psFlashCrc->blockSize = psFlashConfig->endAddress  - psFlashCrc->actualAddress;  /* arrange the block size for remaining memory */
        }
        st = FS_FLASH_PROGRESS;
        
    }
    return (st);
}


void SafetyStackTestInit(void)
{
    FS_StackInit(STACK_TEST_PATTERN, (unsigned short *)&_stack_test_p_2, (unsigned short *)&_stack_test_p_3, STACK_TEST_BLOCK_SIZE);
}


void SafetyStackTest(safety_common_t *psSafetyCommon)
{
    psSafetyCommon->STACK_test_result = FS_StackTest(STACK_TEST_PATTERN, (unsigned short *)&_stack_test_p_2, (unsigned short *)&_stack_test_p_3, STACK_TEST_BLOCK_SIZE);
    if(psSafetyCommon->STACK_test_result == FS_STACK_FAIL)
    {
        psSafetyCommon->safetyErrors |= STACK_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}


void SafetyAnalogTestInitialization(void)
{
#if(0)	
	adc_limits[0].high = 4100;
	adc_limits[0].low  = 3900; 
	adc_limits[1].high = 30;
	adc_limits[1].low  = 0;
	adc_limits[2].high = 1200;
	adc_limits[2].low  = 900;
#endif
	
	FS_AIO_InputInit_CYCLIC(&aio_Str, (fs_aio_limits_t*)adc_limits, (uint8_t *)IEC60730B_ADC_inputs, (uint8_t *)IEC60730B_ADC_samples, IEC60730B_CFG_AIO_CHANNELS_CNT); 
	FS_AIO_InputTrigger(&aio_Str);
}


void SafetyAnalogTest(safety_common_t *psSafetyCommon)
{
psSafetyCommon->AIO_test_result = FS_AIO_InputCheck_CYCLIC(&aio_Str, (unsigned long *)((FS_ADC_CYCLIC_Type*) 0xE500));    

switch(psSafetyCommon->AIO_test_result)
{
case FS_AIO_START:  /* state START means that everything is ready to trigger the conversion */
	FS_AIO_InputSet_CYCLIC(&aio_Str, (unsigned long *)((FS_ADC_CYCLIC_Type*) 0xE500));
	CADC_DoSoftwareTriggerConverter(ADC, kCADC_ConverterA);
    break;
case FS_AIO_FAIL:
    psSafetyCommon->safetyErrors |= AIO_TEST_ERROR;
    SafetyErrorHandling(psSafetyCommon);
    break;
case FS_AIO_INIT:
	FS_AIO_InputTrigger(&aio_Str);
    break;
case FS_AIO_PASS:   /* successfull execution of test, call the trigger function again */
	FS_AIO_InputTrigger(&aio_Str);
    break;   
default:
   asm(NOP);

    break;   
}  
}


/*!
 * @brief   Handling with a safety error.
 *
 *          This function stores the code of recognized safety error into the dedicated RAM memory that is deleted only after POR.
 *          If SAFETY_ERROR_ACTION macro is defined, interrupts are disabled and function waits for watchdog reset.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
void SafetyErrorHandling(safety_common_t *psSafetyCommon)
{
 	//*SAFETY_ERROR_CODE = psSafetyCommon->safetyErrors;
#if SAFETY_ERROR_ACTION
 //   __asm("CPSID i"); /* disable interrupts */
    while(1){
    	FMSTR_Poll(); /* To run FMSTR in ERROR */
    };
#endif
}
