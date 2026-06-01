/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021, 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "mc_periph_init.h"
#include "freemaster.h"
#include "freemaster_serial_lpuart.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "fsl_rgpio.h"
#include "fsl_lpuart.h"
#include "m1_sm_servo.h"
#include "m2_sm_servo.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER "2.0.0" /* motor control package version */

/* Example's feature set in form of bits inside ui16featureSet.
   This feature set is expected to be growing over time.
   ... | FEATURE_S_RAMP | FEATURE_FIELD_WEAKENING | FEATURE_ENC
*/
#define FEATURE_ENC (1)               /* Encoder feature flag */
#define FEATURE_FIELD_WEAKENING (0)   /* Field weakening feature flag */
#define FEATURE_S_RAMP (0)            /* S-ramp feature flag */

#define FEATURE_SET (FEATURE_ENC << (0) | \
                     FEATURE_FIELD_WEAKENING << (1) | \
                     FEATURE_S_RAMP << (2))

/*! @brief The UART to use for FreeMASTER communication */
#define _FMSTR_LPUART_BASE_ADDR(x)   LPUART##x
#define _FMSTR_LPUART_IRQn(x)        LPUART##x##_IRQn
#define _FMSTR_LPUART_IRQ_HANDLER(x) LPUART##x##_IRQHandler

#define FMSTR_LPUART_BASE_ADDR(x)    _FMSTR_LPUART_BASE_ADDR(x)
#define FMSTR_LPUART_IRQn(x)         _FMSTR_LPUART_IRQn(x)
#define FMSTR_LPUART_IRQ_HANDLER(x)  _FMSTR_LPUART_IRQ_HANDLER(x)

#define BOARD_USER_BUTTON_PRIORITY 4

/* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT(par1)   \
    uint32_t val  = SysTick->VAL;  \
    uint32_t load = SysTick->LOAD; \
    par1          = load - val

/* Three instruction added after interrupt flag clearing as required */
#define M1_END_OF_ISR \
    {                 \
        __DSB();      \
        __ISB();      \
    }
        
/* Init SDK HW */
/* TMR1 reload ISR called with 1ms period */
RAM_FUNC_LIB
void TMR1_IRQHandler(void);
/* SINC conversation interrupt handler */
RAM_FUNC_LIB
void SINC1_CH0_IRQHandler(void);
RAM_FUNC_LIB
void SINC2_CH0_IRQHandler(void);

/* EnDat2.2 interrupt for motor connector 1 */
RAM_FUNC_LIB
void M1_ENDAT2P2_IRQHandler(void);

/* EnDat2.2 interrupt for motor connector 2 */
RAM_FUNC_LIB
void M2_ENDAT2P2_IRQHandler(void);

/* Initialization SysTick */
static void BOARD_InitSysTick(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* CPU load measurement using Systick */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;
uint32_t g_ui32M2NumberOfCycles    = 0U;
uint32_t g_ui32M2MaxNumberOfCycles = 0U;

/* Demo mode enabled/disabled */
bool_t bDemoModeSpeed    = FALSE;
bool_t bDemoModePosition = FALSE;
bool_t bM2DemoModeSpeed    = FALSE;
bool_t bM2DemoModePosition = FALSE;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "../../../examples/_boards/imx943evk/demo_apps/mc_pmsm/pmsm_servo_dual/cm7_core0",        /* User Path 1- the highest priority */
    "../../../boards/imx943evk/demo_apps/mc_pmsm/pmsm_servo_dual/cm7_core0",       /* User Path 2 */
    "imx943evk", /* board id */
    "pmsm_servo_dual",     /* example id */
    MCRSP_VER,      /* sw version */
    FEATURE_SET,    /* example's feature-set */
};

ctrl_m1_mid_t g_sSpinMidSwitch;           /* Control Spin/MID switching */

/* Encoder timeout fault counter */
uint8_t ui8M1EncISRCheck = 0U;
uint8_t ui8M2EncISRCheck = 0U;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void init_freemaster_lpuart(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
   
   
/*!
 * @brief Main function
 */
int main(void)
{  
    /*Accessing ID structure to prevent optimization*/
    g_sAppIdFM.ui16FeatureSet = FEATURE_SET;

    uint32_t ui32PrimaskReg;

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();

    SystemPlatformInit();
    BOARD_ConfigMPU();
    BOARD_InitDebugConsolePins();
    BOARD_BootClockRUN();

    /* Init motor control pins */
    BOARD_InitI2C6Pins();
    XBAR_Init(kXBAR_DSC1);
    BOARD_SelectFTUART();

    /* Motor controller 1 */
    BOARD_SelectM1PWM();
    BOARD_Init_M1_PWM();
    BOARD_Init_M1_ENDAT2P2();
    BOARD_SelectM1ENDAT();
    BOARD_SelectM1SINC();
    BOARD_Init_M1_SINC();
    BOARD_SelectM1Faults();
    BOARD_Init_M1_FAULTS();
    XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm2Mux0Trigger0, kXBAR1_OutputSinc1ExtTrigger0);

    /* Motor controller 2 */
    BOARD_SelectM2PWM();
    BOARD_Init_M2_PWM();
    BOARD_Init_M2_ENDAT2P2();
    BOARD_SelectM2ENDAT();
    BOARD_SelectM2SINC();
    BOARD_Init_M2_SINC();
    BOARD_Init_M2_FAULTS();
    XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Mux0Trigger0, kXBAR1_OutputSinc2ExtTrigger0);

    /* Route synchronization signal FlexPWM2_SM0_trig1 -> FlexPWM1_SM0_ExtSync0 */
    XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm2Mux1Trigger0, kXBAR1_OutputFlexpwm1ExtSync0);
    
    /* FreeMASTER communication layer initialization */
    init_freemaster_lpuart();
    
    FMSTR_Init();

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();

    /* Init peripheral motor control driver for motors M1 and M2 */
    MCDRV_Init();

    /* Turn off application */
    M1_SetAppSwitch(FALSE);

    /* Spin state machine is default */
    g_sSpinMidSwitch.eAppState = kAppStateSpin;
  
    /* Enable interrupts */
    EnableGlobalIRQ(ui32PrimaskReg);
    
    /* Enable PWM clock */
    g_sM1Pwm3ph.pui32PwmBaseAddress->MCTRL |= PWM_MCTRL_RUN(0xF);	/* Enable PWM for Motor 1 */
    g_sM2Pwm3ph.pui32PwmBaseAddress->MCTRL |= PWM_MCTRL_RUN(0xF);	/* Enable PWM for Motor 2 */
        
    /* Infinite loop */
    while (1)
    {      
        /* FreeMASTER Polling function */
        FMSTR_Poll();
    }
}

/* SINC1 conversation interrupt handler */
RAM_FUNC_LIB
void SINC1_CH0_IRQHandler(void)
{
  if(ui8M1EncISRCheck > 3U)
  {
      /* Set Encoder timeout fault */
      FAULT_SET(g_sM1Drive.sFaultIdPending, FAULT_ENC_TIMEOUT);
      
      /* Run M1 state machine */
      SM_StateMachineFast(&g_sM1Ctrl); 
      
      ui8M1EncISRCheck--;
  }
  
  /* Read SINC results and process data */
  M1_MCDRV_SINC_GET(&g_sM1Curr3phDcBus);
  
  ui8M1EncISRCheck++;
}

/* SINC2 conversation interrupt handler */
RAM_FUNC_LIB
void SINC2_CH0_IRQHandler(void)
{
  if(ui8M2EncISRCheck > 3U)
  {
      /* Set Encoder timeout fault */
      FAULT_SET(g_sM2Drive.sFaultIdPending, FAULT_ENC_TIMEOUT);
      
      /* Run M2 state machine */
      SM_StateMachineFast(&g_sM2Ctrl); 
      
      ui8M2EncISRCheck--;
  }
  
  /* Read SINC results and process data */
  M2_MCDRV_SINC_GET(&g_sM2Curr3phDcBus);
  
  ui8M2EncISRCheck++;
}

/* EnDat2.2 interrupt handler for motor connector 1 */
RAM_FUNC_LIB
void M1_ENDAT2P2_IRQHandler(void)
{
  /* Clear Encoder fault check flag */
  ui8M1EncISRCheck = 0U;  

  /* get position from EnDat2.2 */
  M1_MCDRV_ENCODER_GET(&g_sM1Enc);
  
  /* M1 State machine */
  SM_StateMachineFast(&g_sM1Ctrl);
  
  /* Call FreeMASTER recorder */
  FMSTR_Recorder(0);
  
  SDK_ISR_EXIT_BARRIER;
}

/* EnDat2.2 interrupt handler for motor connector 2 */
RAM_FUNC_LIB
void M2_ENDAT2P2_IRQHandler(void)
{
  /* Clear Encoder fault check flag */
  ui8M2EncISRCheck = 0U;  
  
  /* get position from EnDat2.2 */
  M2_MCDRV_ENCODER_GET(&g_sM2Enc);
  
  /* M2 State machine */
  SM_StateMachineFast(&g_sM2Ctrl);
  
  SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief   TMR1 reload ISR called with 1ms period and processes following functions:
 *           - motor M1 slow application machine function
 *
 * @param   void
 *
 * @return  none
 */
RAM_FUNC_LIB
void TMR1_IRQHandler(void)
{
    /* M1 Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);
    
    /* M2 Slow StateMachine call */
    SM_StateMachineSlow(&g_sM2Ctrl);    

    /* Clear the CSCTRL0[TCF1] flag */
    TMR1->CHANNEL[0].CSCTRL |= TMR_CSCTRL_TCF1(0x00);
    TMR1->CHANNEL[0].CSCTRL &= ~(TMR_CSCTRL_TCF1_MASK);

    /* Clear the CSCTRL0[TCF] flag */
    TMR1->CHANNEL[0].SCTRL &= ~(TMR_SCTRL_TCF_MASK);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}


/*!
 * @brief LPUART Module initialization (LPUART is a the standard block included e.g. in K66F)
 */
static void init_freemaster_lpuart(void)
{
    lpuart_config_t config;

    /* clang-format off */
    clk_t clk = {
        .clkId = kCLOCK_IpInvalid,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    /* clang-format on */

    clk.clkId = BOARD_GetUartClkId(BOARD_DEBUG_CONSOLE_PORT);

    CLOCK_SetRate(&clk);
    CLOCK_EnableClock(clk.clkId);

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200U;
    config.enableTx     = false;
    config.enableRx     = false;

    LPUART_Init(FMSTR_LPUART_BASE_ADDR(BOARD_DEBUG_UART_INSTANCE), &config, CLOCK_GetRate(clk.clkId));

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress(FMSTR_LPUART_BASE_ADDR(BOARD_DEBUG_UART_INSTANCE));

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(FMSTR_LPUART_IRQn(BOARD_DEBUG_UART_INSTANCE));
    EnableGlobalIRQ(0);
#endif
}

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
/*
 *   Application interrupt handler of communication peripheral used in interrupt modes
 *   of FreeMASTER communication.
 *
 *   NXP MCUXpresso SDK framework defines interrupt vector table as a part of "startup_XXXXXX.x"
 *   assembler/C file. The table points to weakly defined symbols, which may be overwritten by the
 *   application specific implementation. FreeMASTER overrides the original weak definition and
 *   redirects the call to its own handler.
 *
 */

void FMSTR_LPUART_IRQ_HANDLER(BOARD_DEBUG_UART_INSTANCE)(void)
{
    /* Call FreeMASTER Interrupt routine handler */
    FMSTR_SerialIsr();
}
#endif

/*!
 *@brief      SysTick initialization for CPU cycle measurement
 *
 *@param      none
 *
 *@return     none
 */
static void BOARD_InitSysTick(void)
{
    /* Initialize SysTick core timer to run free */
    /* Set period to maximum value 2^24*/
    SysTick->LOAD = 0xFFFFFF;

    /*Clock source - System Clock*/
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    /*Start Sys Timer*/
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}
