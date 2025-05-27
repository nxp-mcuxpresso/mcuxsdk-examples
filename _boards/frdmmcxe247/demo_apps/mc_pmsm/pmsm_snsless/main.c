/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"
#include "freemaster.h"
#include "freemaster_serial_lpuart.h"
#include "fsl_lpuart.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "m1_sm_snsless.h"
#include "board.h"
#include "mid_sm_states.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER "2.0.0" /* motor control package version */

/* Example's feature set in form of bits inside ui16featureSet.
   This feature set is expected to be growing over time.
   ... | FEATURE_S_RAMP | FEATURE_FIELD_WEAKENING | FEATURE_ENC
*/
#define FEATURE_ENC (0)               /* Encoder feature flag */
#define FEATURE_FIELD_WEAKENING (0)   /* Field weakening feature flag */
#define FEATURE_S_RAMP (0)            /* S-ramp feature flag */

#define FEATURE_SET (FEATURE_ENC << (0) | \
                     FEATURE_FIELD_WEAKENING << (1) | \
                     FEATURE_S_RAMP << (2))

/* Macro for correct Cortex CM0 / CM4 end of interrupt */
#define M1_END_OF_ISR \
    {                 \
        __DSB();      \
        __ISB();      \
    }

/* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT(par1)   \
    uint32_t val  = SysTick->VAL;  \
    uint32_t load = SysTick->LOAD; \
    par1          = load - val

static void BOARD_Init(void);
static void BOARD_InitSysTick(void);
static void BOARD_InitGPIO(void);
static void DemoSpeedStimulator(void);
static void Application_Control_BL(void);
static void init_freemaster_lpuart(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* CPU load measurement using Systick */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;

/* Demo mode enabled/disabled */
bool_t bDemoModeSpeed = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt    = 0U;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0U;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "../../../examples/frdmmcxe247/demo_apps/mc_pmsm/pmsm_snsless", /* User Path 1- the highest priority */
    "",       /* User Path 2 */
    "frdmmcxe247",   /* board id */
    "pmsm_snsless",  /* example id */
    MCRSP_VER,       /* sw version */
    FEATURE_SET,     /* example's feature-set */
};

mid_app_cmd_t g_eMidCmd;                  /* Start/Stop MID command */
ctrl_m1_mid_t g_sSpinMidSwitch;           /* Control Spin/MID switching */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief   Application main function processing peripheral function calling and
 *          infinite loop
 *
 * @param   void
 *
 * @return  none
 */
int main(void)
{
    /*Accessing ID structure to prevent optimization*/
    g_sAppIdFM.ui16FeatureSet = FEATURE_SET;

    uint32_t ui32PrimaskReg;

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();

    /* Disable demo mode after reset */
    bDemoModeSpeed         = FALSE;
    ui32SpeedStimulatorCnt = 0U;

    /* Init board hardware. */
    BOARD_Init();

    /* Initialize peripheral motor control driver for motor M1 */
    MCDRV_Init_M1();
    
    /* FreeMASTER communication layer initialization */
    init_freemaster_lpuart();

    /* FreeMASTER initialization */
    FMSTR_Init();

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();

    /* Turn off application */
    M1_SetAppSwitch(FALSE);

    /* Init MID state machine - call before the spin state machine */
    g_sSpinMidSwitch.eAppState = kAppStateMID;

    if(g_sSpinMidSwitch.eAppState == kAppStateMID)
    {
      MID_Init_AR();
    }

    /* Spin state machine is default */
    g_sSpinMidSwitch.eAppState = kAppStateSpin;

    /* Enable interrupts  */
    EnableGlobalIRQ(ui32PrimaskReg);
    
    PDB0->SC = (PDB0->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0x0); /* PDB Trigger off (software trigger) */
    PDB0->SC |= PDB_SC_LDOK_MASK;  /* LDOK */
    
    PDB1->SC = (PDB1->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0x0); /* PDB Trigger off (software trigger) */
    PDB1->SC |= PDB_SC_LDOK_MASK;  /* LDOK */

    /* Infinite loop */
    while (1)
    {
        Application_Control_BL();

        /* FreeMASTER Polling function */
        FMSTR_Poll();
    }
}

/* ADC0_IRQn interrupt handler */
void ADC0_IRQHandler(void) {
    /* ADC conversion complete (fast loop period) interrupt.
     * Processes motor M1 fast application machine function. */

    /* Start CPU tick number couting */
    SYSTICK_START_COUNT();
    
    /* TP - ON */
    GPIO_PortSet(GPIOA, 1U << 14);

    switch(g_sSpinMidSwitch.eAppState)
    {
    case kAppStateSpin:
        /* M1 state machine */
        SM_StateMachineFast(&g_sM1Ctrl);
      break;
    default:
        /* MID state machine */
        MID_ProcessFast_FL();
      break;
    }

    /* TP - OFF */
    GPIO_PortClear(GPIOA, 1U << 14);
 
    /* Stop CPU tick number couting and store actual and maximum ticks */
    SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
    g_ui32MaxNumberOfCycles =
      g_ui32NumberOfCycles > g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;

    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
       Store immediate overlapping exception return operation might vector to incorrect interrupt. */
    #if defined __CORTEX_M && (__CORTEX_M == 4U)
      __DSB();
    #endif
}

/* FTM2_IRQn interrupt handler */
void FTM2_Ovf_Reload_IRQHandler(void) {
    static int16_t ui16i = 0;

    /* Slow Loop periodic interrupt period.
    * Processes motor M1 slow application machine function. */
    /* Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);
    
    /* If in STOP state turn on red */
    if (M1_GetAppState() == 2)
    {
        LED_RED_ON();
        LED_GREEN_OFF();
    }

    /* If in FAULT state red blinking*/
    else if (M1_GetAppState() == 0)
    {
        if (ui16i-- < 0)
        {
            LED_RED_TOGGLE();
            ui16i = 125;
        }
        LED_GREEN_OFF();
    }

    /* If in RUN or INIT state turn on green */
    else
    {
        LED_RED_OFF();
        LED_GREEN_ON();
    }

    /* Demo speed stimulator */
    DemoSpeedStimulator();

    /* Clear the TOF flag */
    FTM2->SC &= ~(FTM_SC_TOF_MASK);

    /* Add instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
       Store immediate overlapping exception return operation might vector to incorrect interrupt. */
    #if defined __CORTEX_M && (__CORTEX_M == 4U)
      __DSB();
    #endif
}

/* PDB0_IRQn interrupt handler */
void PDB0_IRQHandler(void) {
    /* Handling the PDB error interrupt.
    * Reinitializes the PDB module */

    PDB0->SC = (PDB0->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0x0); /* PDB Trigger off (software trigger) */
    PDB0->SC |= PDB_SC_LDOK_MASK;  /* LDOK */
  
    PDB_ClearStatusFlags(PDB0, kPDB_DelayEventFlag);

    /* Add instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
       Store immediate overlapping exception return operation might vector to incorrect interrupt. */
    #if defined __CORTEX_M && (__CORTEX_M == 4U)
      __DSB();
    #endif
}

void PDB1_IRQHandler(void) {
    /* Handling the PDB error interrupt.
    * Reinitializes the PDB module */

    PDB1->SC = (PDB1->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0x0); /* PDB Trigger off (software trigger) */
    PDB1->SC |= PDB_SC_LDOK_MASK;  /* LDOK */
  
    PDB_ClearStatusFlags(PDB1, kPDB_DelayEventFlag);

    /* Add instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
       Store immediate overlapping exception return operation might vector to incorrect interrupt. */
    #if defined __CORTEX_M && (__CORTEX_M == 4U)
      __DSB();
    #endif
}


/*!
 * @brief   Port interrupt handler
 *
 * @param   void
 *
 * @return  none
 */
void PORTA_IRQHandler(void)
{
    if (PORTA->PCR[9] & PORT_PCR_ISF_MASK)
    {
        /* Clear the flag */
        PORTA->PCR[9] |= PORT_PCR_ISF_MASK;

        /* Proceed only if pressing longer than timeout */
        if (ui32ButtonFilter > 200)
        {
            ui32ButtonFilter = 0;
            if (bDemoModeSpeed)
            {
                /* Stop application */
                M1_SetSpeed(0);
                M1_SetAppSwitch(FALSE);
                bDemoModeSpeed = FALSE;
            }
            else
            {
                /* Start application */
                M1_SetAppSwitch(TRUE);
                bDemoModeSpeed         = TRUE;
                ui32SpeedStimulatorCnt = 0;
            }
        }
    }

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
 * @brief   DemoSpeedStimulator
 *           - When demo mode is enabled it changes the required speed according
 *             to predefined profile
 *
 * @param   void
 *
 * @return  none
 */
static void DemoSpeedStimulator(void)
{
    /* Increment push button pressing counter  */
    if (ui32ButtonFilter < 1000)
        ui32ButtonFilter++;

    if (bDemoModeSpeed)
    {
        ui32SpeedStimulatorCnt++;
        switch (ui32SpeedStimulatorCnt)
        {
            case 100:
                M1_SetSpeed(1000.0F);
                break;
            case 3000:
                M1_SetSpeed(2000.0F);
                break;
            case 6000:
                M1_SetSpeed(4000.0F);
                break;
            case 9000:
                M1_SetSpeed(-1000.0F);
                break;
            case 12000:
                M1_SetSpeed(-2000.0F);
                break;
            case 15000:
                M1_SetSpeed(-4000.0F);
                break;
            case 18000:
                M1_SetSpeed(800.0F);
                ui32SpeedStimulatorCnt = 0;
                break;
            default:
            	;
                break;
        }
    }
}



/*!
 * @brief LPUART Module initialization (LPUART is a the standard block included e.g. in K66F)
 */
static void init_freemaster_lpuart(void)
{
    lpuart_config_t config;

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

    LPUART_Init((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR, &config, CLOCK_GetIpFreq(kCLOCK_Lpuart2));

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR);

}

/*!
 * @brief   Application_Control_BL
 *           - Control switching between Spin and MID
 *
 * @param   void
 *
 * @return  none
 */
static void Application_Control_BL(void)
{
  switch(g_sSpinMidSwitch.eAppState)
  {
    case kAppStateSpin:
        /* M1 state machine */
        if(g_sSpinMidSwitch.bCmdRunMid == TRUE)
        {
          if((kSM_AppStop == M1_GetAppState()) && (FALSE == M1_GetAppSwitch()) )
          {
            MID_Init_AR();
            g_sSpinMidSwitch.sFaultCtrlM1_Mid &= ~(FAULT_APP_SPIN);
            g_eMidCmd = kMID_Cmd_Stop;                          /* Reset MID control command */
            g_sSpinMidSwitch.eAppState = kAppStateMID;          /* MID routines will be processed */
          }
          else
            g_sSpinMidSwitch.sFaultCtrlM1_Mid |= FAULT_APP_SPIN;

          g_sSpinMidSwitch.bCmdRunMid = FALSE;                  /* Always clear request */
        }

        g_sSpinMidSwitch.bCmdRunM1 = FALSE;
        break;
    default:
        /* MID state machine */
        if(g_sSpinMidSwitch.bCmdRunM1 == TRUE)
        {
          if((g_eMidCmd == kMID_Cmd_Stop) && (kMID_Stop == MID_GetActualState()))
          {
            g_sSpinMidSwitch.sFaultCtrlM1_Mid &= ~(FAULT_APP_MID);
            g_sM1Ctrl.eState = kSM_AppInit;                      /* Set Init state for M1 state machine */
            g_sSpinMidSwitch.eAppState = kAppStateSpin;          /* Switch application state to Spin */
          }
          else
            g_sSpinMidSwitch.sFaultCtrlM1_Mid |= FAULT_APP_MID;

           /* Always clear request */
          g_sSpinMidSwitch.bCmdRunM1 = FALSE;
          g_sSpinMidSwitch.bCmdRunMid = FALSE;
          break;
        }

        g_sSpinMidSwitch.bCmdRunMid = FALSE;
        MID_Process_BL(&g_eMidCmd);
        break;
  }
}

/*!
 *@brief      Initialization of the Clocks and Pins
 *
 *@param      none
 *
 *@return     none
 */
static void BOARD_Init(void)
{
    /* Initialize clock configuration */
    BOARD_BootClockHSRUN();
    
    CLOCK_SetIpSrc(kCLOCK_Lpuart2, BOARD_DEBUG_UART_CLKSRC);
    CLOCK_SetIpSrc(kCLOCK_Adc0, kCLOCK_IpSrcSircAsync);  
    CLOCK_EnableClock(kCLOCK_Adc0);
    CLOCK_SetIpSrc(kCLOCK_Adc1, kCLOCK_IpSrcSircAsync);
    CLOCK_EnableClock(kCLOCK_Adc1);
    CLOCK_EnableClock(kCLOCK_Ftm0);
    CLOCK_EnableClock(kCLOCK_Ftm2);  
    CLOCK_EnableClock(kCLOCK_Pdb0);
    CLOCK_EnableClock(kCLOCK_Pdb1);
    CLOCK_EnableClock(kCLOCK_Cmp0);

    /* Init pins set in pin_mux file */
    BOARD_InitBootPins();

    /* Init GPIO pins */
    BOARD_InitGPIO();

}

/*!
 *@brief      Initialization of the GPIO pins
 *
 *@param      none
 *
 *@return     none
 */
static  void BOARD_InitGPIO(void)
{
    /* LED pin configuration */
    const gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput, /* Set current pin as digital output */
        (uint8_t)1U          /* Set default logic high */
    };

    /* Enable port for RED LED */
    GPIO_PinInit(GPIOC, 13U, &led_config);

    /* Enable port for GREEN LED */
    GPIO_PinInit(GPIOB, 11U, &led_config);
    
    /* Enable port for BLUE LED */
    GPIO_PinInit(GPIOC, 12U, &led_config);

    /* SW2 pin configuration */
    PORT_SetPinInterruptConfig(PORTA, 9U, kPORT_InterruptRisingEdge); /* Enable interrupt */

    /* Enable & setup interrupts */
    EnableIRQ(PORTA_IRQn);
    NVIC_SetPriority(PORTA_IRQn, 4);
}

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
