/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "safety_config.h"

#include "freemaster.h"
#include "freemaster_serial_qsci.h"

////////////////////////////////////////////////////////////////////////////////
// Local macros define what QSCI port is used
////////////////////////////////////////////////////////////////////////////////
	/* Use SCI_1 at Boot_SCI USB-to-serial port */
	#define BOARD_FMSTR_UART_INSTANCE   0
	#define BOARD_FMSTR_UART_CLOCK_NAME kCLOCK_QSCI0

#define BOARD_FMSTR_UART_BASE_ADDR  qsciBases[BOARD_FMSTR_UART_INSTANCE]

/* Macro to access the QSCI IRQn interrupt vector number */
#define QSCIx_IRQn(x, irq)  QSCI##x##_##irq##_IRQn
#define QSCIx_RERR_IRQn(x)  QSCIx_IRQn(x, RERR)
#define QSCIx_RCV_IRQn(x)   QSCIx_IRQn(x, RCV)
#define QSCIx_TDRE_IRQn(x)  QSCIx_IRQn(x, TDRE)
#define QSCIx_TRIDLE_IRQn(x) QSCIx_IRQn(x, TRIDLE)

/* Macro to declare IRQ handler for QSCI module number defined by a macro.
 * Refer to vector table and associated IRQ handlers. */
#define QSCI_IRQ_HANDER2(x, vec) ivINT_QSCI##x##_##vec
#define QSCI_IRQ_HANDER(x, vec) QSCI_IRQ_HANDER2(x, vec) /* two-phase to enable x macro to expand */

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

// array of all QSCI base addresses
const QSCI_Type *qsciBases[] = QSCI_BASE_PTRS;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static void init_freemaster_qsci(void);

#define PIT_IRQ_PRIO 1 /* Priority level 0*/
volatile uint16_t exception_counter;
volatile uint16_t refresh_on;
volatile uint16_t ui16reset_source;
volatile bool pitIsrFlag = false;
ram_test_t g_sSafetyRamTest; 
#pragma define_section safety_ram ".safety_ram.data" RW
#pragma section safety_ram begin
cop_test_t g_sSafetyCopTest;
crc_config_t sCrcConfig;
safety_common_t g_sSafetyCommon;
fs_clock_test_t g_sSafetyClockTest;
#pragma section safety_ram end
extern uint16_t _pc_test_flag;   /* from Linker configuration file */
extern uint32_t _safety_ram;     /* start of section for ram test ,from Linker command file */
extern uint32_t _end_safety_ram; /* end of section for ram test, from Linker command file */
extern __pmem CRC16_record g_crc_linker;
extern uint32_t start_text;
extern uint32_t end_text;
flash_runtime_test_parameters_t g_sFlashCrc;
flash_configuration_parameters_t g_sFlashConfig;
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	uint16_t clock_test_condition = 0;   
	uint16_t delay;
	uint8_t i; 
	refresh_on = 1;
    /* Board initialization */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    
   // LED_GREEN_INIT(kGPIO_OutputLow);

#ifdef WATCHDOG_ENABLED
    watchdogEnable(COP_TIMEOUT_IN_MICRO_SECONDS);
#endif 
    ui16reset_source = SafetyWatchdogTest(&g_sSafetyCopTest);
    watchdogRefresh(); //**********COP*****WATCHDOG***********
    /*****************/
    /* CPU test      */
    /*****************/ 
    SafetyCpuAfterResetTest(&g_sSafetyCommon);   
    watchdogRefresh(); //**********COP*****WATCHDOG***********
	/*****************/
    /* Stack test    */
	/*****************/
    SafetyStackTestInit();
    watchdogRefresh(); //**********COP*****WATCHDOG***********
    SafetyStackTest(&g_sSafetyCommon);
    /*****************/
    /* Flash test    */
    /*****************/ 
#if(HW_FLASH_TEST)    
    CLOCK_EnableClock(kCLOCK_CRC);    
#endif
    
    watchdogRefresh(); //**********COP*****WATCHDOG***********
    SafetyFlashTestInit(&g_sFlashCrc, &g_sFlashConfig, &g_crc_linker);
    watchdogRefresh(); //**********COP*****WATCHDOG***********
    SafetyFlashAfterResetTest(&g_sSafetyCommon, &g_sFlashConfig);
    watchdogRefresh(); //**********COP*****WATCHDOG***********
    
#if PC_TEST_ENABLED
	/* Program Counter test */
    g_sSafetyCommon.PC_test_result =  FS_PC_Test(FS_PC_object_1, FS_PC_subroutine, (unsigned short *)&_pc_test_flag); //TODO: set address of object
    g_sSafetyCommon.PC_test_result |= FS_PC_Test(FS_PC_object_2, FS_PC_subroutine, (unsigned short *)&_pc_test_flag);
    watchdogRefresh(); //**********COP*****WATCHDOG***********
#endif
#if RAM_TEST_ENABLED
    /* Ram test init for Safety related RAM space */
    SafetyRamTestInit(&g_sSafetyRamTest, &_safety_ram, &_end_safety_ram);
    /* Ram after-reset test for safety related memory*/
    SafetyRamAfterResetTest(&g_sSafetyCommon, &g_sSafetyRamTest);  
    watchdogRefresh(); //**********COP*****WATCHDOG***********
#endif
    watchdogRefresh(); //**********COP*****WATCHDOG***********
    
    /*****************/
    /* DIO test      */
    /*****************/
#if DIO_TEST_ENABLED
    for (i=0; dio_safety_test_items[i]!= 0; i++)
    {
		SafetyDigitalOutputTest(&g_sSafetyCommon,dio_safety_test_items[i]);
		SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon,dio_safety_test_items[i], DIO_SHORT_TO_GND_TEST);
		SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon,dio_safety_test_items[i], DIO_SHORT_TO_VDD_TEST);
    }
    watchdogRefresh(); //**********COP*****WATCHDOG***********
    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, dio_safety_test_items[0],dio_safety_test_items[1], LOGICAL_ONE); 
    watchdogRefresh(); //**********COP*****WATCHDOG***********
    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, dio_safety_test_items[0],dio_safety_test_items[1], LOGICAL_ZERO); 
    watchdogRefresh(); //**********COP*****WATCHDOG***********
#endif
    /*****************/
    /* ADC test      */
    /*****************/ 
#if AIO_TEST_ENABLED
    /* ADC init */
    /* GPIO A4 set as ANA4,  GPIO A5 set as ANA5 inputs */
    ADC_Init();    
    watchdogRefresh(); //**********COP*****WATCHDOG***********   
    SafetyAnalogTestInitialization(); /* initialisation of ADC test */     
    for (i=0;i<6;i++)
    {
        watchdogRefresh(); //**********COP*****WATCHDOG***********	
        SafetyAnalogTest(&g_sSafetyCommon);
        for(delay=0;delay<1000;delay++)
        {
        	asm(NOP);
        }
    }
#endif
   
    CLOCK_EnableClock(BOARD_FMSTR_UART_CLOCK_NAME);

    watchdogRefresh(); //**********COP*****WATCHDOG***********
    /* FreeMASTER communication layer initialization */
    init_freemaster_qsci();

    watchdogRefresh(); //**********COP*****WATCHDOG***********
    /* This example uses shared code from FreeMASTER generic example application */
    FMSTR_Init();
    watchdogRefresh(); //**********COP*****WATCHDOG***********

#if CLOCK_TEST_ENABLED   
    watchdogRefresh(); //**********COP*****WATCHDOG***********
    XBARA_Init(XBARA); 
    watchdogRefresh(); //**********COP*****WATCHDOG***********
    XBARA_SetSignalsConnection(XBARA,kXBARA_InputPit1SyncOut,kXBARA_OutputTa0In);   
    watchdogRefresh(); //**********COP*****WATCHDOG***********
    SIM_SetInternalPeriInput(SIM, kSIM_TMRA0InputIndex, kSIM_TMRA0Input_XB_OUT38); 
    watchdogRefresh(); //**********COP*****WATCHDOG***********
    TMRA0_initialization();
    watchdogRefresh(); //**********COP*****WATCHDOG***********
    PIT1_initialization(3052U, kPIT_PrescalerDivBy64);
    watchdogRefresh(); //**********COP*****WATCHDOG***********  
    SafetyClockTestInit(&g_sSafetyCommon, &g_sSafetyClockTest);
#endif   /*CLOCK_TEST_ENABLED */
    PIT0_initialization(PIT0_RELOAD,PIT0_CLOCK_DIV);
    EnableIRQWithPriority(PIT0_ROLLOVR_IRQn, PIT_IRQ_PRIO); 
    __EI0();
    while(1)
    {
        /*****************/
        /* Clock test    */
        /*****************/ 
    	if (exception_counter >10)
    		clock_test_condition |=1;
    	if(clock_test_condition)
    	{
    	g_sSafetyCommon.CLOCK_test_result = FS_CLK_QTIMER_Check(&g_sSafetyClockTest);
 //   	QTMR_ClearStatusFlags(TMRA, kQTMR_Channel0, TMR_SCTRL_IEF_MASK);
    	}
#if FLASH_TEST_ENABLED
    	SafetyFlashRuntimeTest(&g_sSafetyCommon, &g_sFlashCrc, &g_sFlashConfig);
#endif
    	
#if AIO_TEST_ENABLED    	
    	SafetyAnalogTest(&g_sSafetyCommon);
#endif    	
    	SafetyStackTest(&g_sSafetyCommon);
        FMSTR_Poll();
    }
}
#pragma interrupt saveall
void ivINT_PIT0_ROLLOVR(void);
void ivINT_PIT0_ROLLOVR(void)
{
	GPIO_PinWrite(GPIOC, kGPIO_Pin6, kGPIO_OutputHigh);
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT0);
    exception_counter ++;
    if(exception_counter ==20)
    {
    	//LED_GREEN_TOGGLE();
    	exception_counter = 0;  	
    }
#if RAM_TEST_ENABLED
    SafetyRamRuntimeTest(&g_sSafetyCommon, &g_sSafetyRamTest);
#endif    
    if(refresh_on)
    {
        watchdogRefresh(); //**********COP*****WATCHDOG***********
    }   
}
#pragma interrupt off

/*!
 * @brief QSCI Module initialization
 */
static void init_freemaster_qsci(void)
{
	qsci_config_t config;
	uint32_t uartClkSrcFreq;
	
	uartClkSrcFreq = CLOCK_GetIpClkSrcFreq(BOARD_FMSTR_UART_CLOCK_NAME);
	/*
	* config.baudRate_Bps = 115200U;
	* config.parityMode = kUART_ParityDisabled;
	* config.stopBitCount = kUART_OneStopBit;
	* config.txFifoWatermark = 0;
	* config.rxFifoWatermark = 1;
	* config.enableTx = false;
	* config.enableRx = false;
	*/
	QSCI_GetDefaultConfig(&config, 115200U, uartClkSrcFreq);
	config.bEnableTx = false;
	config.bEnableRx = false;
	
	QSCI_Init((QSCI_Type*)BOARD_FMSTR_UART_BASE_ADDR, &config);
	
	/* Register communication module used by FreeMASTER driver. */
	FMSTR_SerialSetBaseAddress((QSCI_Type*)BOARD_FMSTR_UART_BASE_ADDR);
}
