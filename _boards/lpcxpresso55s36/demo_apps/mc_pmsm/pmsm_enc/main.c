/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"
#include "freemaster.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "fsl_gpio.h"
#include "fsl_usart.h"
#include "fsl_device_registers.h"
#include "fsl_ctimer.h"
#include "fsl_debug_console.h"
#include "m1_sm_snsless_enc.h"
#include "freemaster_serial_miniusart.h"
#include "clock_config.h"
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
#define FEATURE_ENC (1)               /* Encoder feature flag */
#define FEATURE_FIELD_WEAKENING (0)   /* Field weakening feature flag */
#define FEATURE_S_RAMP (0)            /* S-ramp feature flag */

#define FEATURE_SET (FEATURE_ENC << (0) | \
                     FEATURE_FIELD_WEAKENING << (1) | \
                     FEATURE_S_RAMP << (2))

/* Three instruction added after interrupt flag clearing as required */
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
static void DemoSpeedStimulator(void);
static void DemoPositionStimulator(void);
static void Application_Control_BL(void);
void BOARD_SW3_Callback(pint_pin_int_t pintr, uint32_t pmatch_status);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* CPU load measurement using Systick */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;

/* Demo mode enabled/disabled */
bool_t bDemoModeSpeed = FALSE;
bool_t bDemoModePosition = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt    = 0U;
static uint32_t ui32PositionStimulatorCnt = 0U;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "../../../examples/lpcxpresso55s36/demo_apps/mc_pmsm/pmsm_enc",      /* User Path 1- the highest priority */
    "",       /* User Path 2 */
    "lpcxpresso55s36",  /* board id */
    "pmsm_enc",         /* example id */
    MCRSP_VER,          /* sw version */
    FEATURE_SET,        /* example's feature-set */
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
    bDemoModeSpeed    = FALSE;
    bDemoModePosition = FALSE;
    ui32SpeedStimulatorCnt = 0U;

    /* Init board hardware. */
    BOARD_Init();

    /* Initialize peripheral motor control driver for motor M1 */
    MCDRV_Init_M1();

    /* Initialize RTCESL PQ */
    RTCESL_PQ_Init();

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();

    /* Turn off application */
    M1_SetAppSwitch(FALSE);

    /* Init application state - Spin or MID */
    g_sSpinMidSwitch.eAppState = kAppStateMID;

    if(g_sSpinMidSwitch.eAppState == kAppStateMID)
    {
      MID_Init_AR();
    }

    /* Spin state machine is default */
    g_sSpinMidSwitch.eAppState = kAppStateSpin;

    /* Enable interrupts  */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Infinite loop */
    while (1)
    {
        Application_Control_BL();

        /* FreeMASTER Polling function */
        FMSTR_Poll();
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
    BOARD_InitBootClocks();
    /* Init pins set in pin_mux file */
    BOARD_InitBootPins();
    /* Init peripherals set in peripherals file */
    BOARD_InitBootPeripherals();
}

/*!
 * @brief   Slow loop interrupt handler (1ms period)
 *           - motor M1 slow application machine function
 *
 * @param   void
 *
 * @return  none
 */
void CTIMER0_IRQHandler(void)
{
    static int16_t ui16i = 0;

    /* M1 Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);

    /* If in STOP state turn on RED */
    if (M1_GetAppState() == 2)
    {
        LED_RED_ON();
        LED_GREEN_OFF();
    }

    /* If in FAULT state RED blinking*/
    else if (M1_GetAppState() == 0)
    {
        if (ui16i-- < 0)
        {
            LED_RED_TOGGLE();
            bDemoModeSpeed = FALSE;
            bDemoModePosition = FALSE;
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

    /* Demo position stimulator */
    DemoPositionStimulator();

    /* Clear the match interrupt flag. */
    CTIMER0->IR |= CTIMER_IR_MR0INT(1U);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

#if (M1_CONNECTOR_ID == M1_CONNECTOR_ID_MC1)
/*!
 * @brief   ADC ISR called with 100us period processes
 *           - motor M1 fast application machine function
 *           - demo mode if enabled
 *
 * @param   void
 *
 * @return  none
 */
void ADC0_IRQHandler(void)
{
    /* Start CPU tick number couting */
    SYSTICK_START_COUNT();

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

    /* Stop CPU tick number couting and store actual and maximum ticks */
    SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
    g_ui32MaxNumberOfCycles =
        g_ui32NumberOfCycles > g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;

    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);

    /* Clear the TCOMP INT flag */
    ADC0->STAT |= ADC_STAT_TCOMP_INT_MASK;

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

#elif (M1_CONNECTOR_ID == M1_CONNECTOR_ID_MC2)

/*!
 * @brief   ADC ISR called with 100us period processes
 *           - motor M1 fast application machine function
 *           - demo mode if enabled
 *
 * @param   void
 *
 * @return  none
 */
void ADC1_IRQHandler(void)
{
    /* Start CPU tick number couting */
    SYSTICK_START_COUNT();

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

    /* Stop CPU tick number couting and store actual and maximum ticks */
    SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
    g_ui32MaxNumberOfCycles =
        g_ui32NumberOfCycles > g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;

    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);

    /* Clear the TCOMP INT flag */
    ADC1->STAT |= ADC_STAT_TCOMP_INT_MASK;

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}
#else
  #warning "No motor connector selected! Use correct define for M1_CONNECTOR_ID."
#endif

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
    if (bDemoModeSpeed)
    {
        ui32SpeedStimulatorCnt++;
        switch (ui32SpeedStimulatorCnt)
        {
            case 10:
                g_sM1Drive.eControl                  = kControlMode_SpeedFOC;
                g_sM1Drive.sMCATctrl.ui16PospeSensor = MCAT_SENSORLESS_CTRL;
                break;
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
    else
    {
        ui32SpeedStimulatorCnt = 0U;
    }
}

/*!
 * @brief   DemoPositionStimulator
 *           - When demo mode is enabled it changes the required position according
 *             to predefined profile
 *
 * @param   void
 *
 * @return  none
 */
static void DemoPositionStimulator(void)
{
    if (bDemoModePosition)
    {
        ui32PositionStimulatorCnt++;
        switch (ui32PositionStimulatorCnt)
        {
            case 2:
                g_sM1Drive.eControl                  = kControlMode_PositionFOC;
                g_sM1Drive.sMCATctrl.ui16PospeSensor = MCAT_ENC_CTRL;
                break;
            case 10:
                M1_SetPosition(ACC32(20));
                break;
            case 3000:
                M1_SetPosition(ACC32(10));
                break;
            case 6000:
                M1_SetPosition(ACC32(20));
                break;
            case 9000:
                M1_SetPosition(ACC32(0));
                break;
            case 12000:
                M1_SetPosition(ACC32(20));
                break;
            case 15000:
                M1_SetPosition(ACC32(0));
                break;
            case 18000:
                ui32PositionStimulatorCnt = 0;
                break;
            default:
			    ;
            	break;
        }
    }
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

/*!
 *@brief      Press SW3 button callback function
 *
 *@param      none
 *
 *@return     none
 */
void BOARD_SW3_Callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
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

  /* Add empty instructions for correct interrupt flag clearing */
  M1_END_OF_ISR;
}
