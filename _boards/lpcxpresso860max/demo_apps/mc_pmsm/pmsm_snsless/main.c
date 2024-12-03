/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "freemaster.h"
#include "pin_mux.h"
#include "peripherals.h"

#include "board.h"
#include "fsl_pint.h"
#include "fsl_syscon.h"
#include "mid_sm_states.h"
#include "m1_mid_switch.h"

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

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* CPU load measurement using Systick */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;

/* Demo mode enabled/disabled */
bool_t bDemoMode = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt    = 0U;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0U;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "../../../examples/lpcxpresso860max/demo_apps/mc_pmsm/pmsm_snsless",     /* User Path 1- the highest priority */
    "",       /* User Path 2 */
    "lpcxpresso860max",    /* board id */
    "pmsm_snsless", /* example id */
    MCRSP_VER,      /* sw version */
    FEATURE_SET,    /* example's feature-set */
};

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
    uint32_t ui32PrimaskReg;

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();

    FLASH_CTRL->FLASHCFG = FLASH_CTRL_FLASHCFG_FLASHTIM(0x1);

    /* Disable demo mode after reset */
    bDemoMode              = FALSE;
    ui32SpeedStimulatorCnt = 0U;

    /*Accessing ID structure to prevent optimization*/
    g_sAppIdFM.ui16FeatureSet = FEATURE_SET;

    /* Init board hardware. */
    BOARD_Init();

    /* Init MC peripherals */
    MCDRV_Init_M1();

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();

    /* Turn off application */
    M1_SetAppSwitch(FALSE);

    /* Enable interrupts  */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Infinite loop */
    while (1)
    {
        /* FreeMASTER Polling function */
        FMSTR_Poll();
    }
}

/*!
 * @brief   Fast loop ISR
 *
 * @param   void
 *
 * @return  none
 */
void ADC0_SEQA_IRQHandler(void) {
    /* ADC conversion complete (fast loop period) interrupt.
    * Processes motor M1 fast application machine function.
    * Demo mode if enabled. */

    /* Start CPU tick number couting */
    SYSTICK_START_COUNT();

    switch(M1_MID_Get_State())
    {
    case kStateM1:
    /* Spin state machine */
    SM_StateMachineFast(&g_sM1Ctrl);
      break;
    case kStateMID:
      /* MID state machine */
      MID_ProcessFast_FL();
      break;
    default:
      ;
    }

    /* stop CPU tick number couting and store actual and maximum ticks */
    SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
    g_ui32MaxNumberOfCycles =
        g_ui32NumberOfCycles > g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;

    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);

    ADC_ClearStatusFlags(ADC0, kADC_ConvSeqAInterruptFlag);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;

}

/*!
 * @brief   Slow loop ISR
 *
 * @param   void
 *
 * @return  none
 */
void MRT0_IRQHANDLER(void) {
    /* Slow Loop periodic interrupt period.
    * Processes motor M1 slow application machine function. */

    static int16_t ui16i = 0U;

    /* Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);

    /* Check if transtion between M1/MID was requested. */
    M1_MID_Switch_BL();

    /* If in STOP state turn on red */
    if (M1_GetAppState() == 2U)
    {
        LED_R_ON();
        LED_G_OFF();
    }

    /* If in FAULT state red blinking*/
    else if (M1_GetAppState() == 0U)
    {
        if (ui16i-- < 0)
        {
            LED_R_TOGGLE();
            ui16i = 125U;
        }
        LED_G_OFF();
    }

    /* If in RUN or INIT state turn on green */
    else
    {
        LED_R_OFF();
        LED_G_ON();
    }

    /* Demo speed stimulator */
    DemoSpeedStimulator();

    /* Clear interrupt flag */
    MRT0->IRQ_FLAG = MRT_IRQ_FLAG_GFLAG0(1U);

    /* Add instructions for correct interrupt flag clearing */
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

    if (bDemoMode)
    {
        ui32SpeedStimulatorCnt++;
        switch (ui32SpeedStimulatorCnt)
        {
            case 100:
                M1_SetSpeed(FRAC16(1000.0 / M1_N_MAX));
                break;
            case 3000:
                M1_SetSpeed(FRAC16(2000.0 / M1_N_MAX));
                break;
            case 6000:
                M1_SetSpeed(FRAC16(4000.0 / M1_N_MAX));
                break;
            case 9000:
                M1_SetSpeed(FRAC16(900.0 / M1_N_MAX));
                break;
            case 12000:
                M1_SetSpeed(FRAC16(4000.0 / M1_N_MAX));
                break;
            case 15000:
                M1_SetSpeed(FRAC16(2000.0 / M1_N_MAX));
                break;
            case 18000:
                M1_SetSpeed(FRAC16(800.0 / M1_N_MAX));
                ui32SpeedStimulatorCnt = 0;
                break;
            default:
                break;
        }
    }
}

/*!
 * @brief   static void BOARD_Init(void)
 *           - Initialization of clocks, pins and GPIO
 *
 * @param   void
 *
 * @return  none
 */
static void BOARD_Init(void)
{
    /* Init pins set in pin_mux file */
    BOARD_InitBootPins();

    /* Initialize clock configuration */
    BOARD_BootClockFRO30M();

    /* ADC Calibration */
    MCDRV_Calib_ADC();

    /* Reinit system clock after ADC calibration */
    BOARD_BootClockFRO60M();

    /* Init peripherals set in peripherals file */
    BOARD_InitBootPeripherals();

    /* Init GPIO pins */
    BOARD_InitGPIO();

}


/*!
 * @brief   Port interrupt handler
 *
 * @param   void
 *
 * @return  none
 */
static void DemoButtonIsr(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    /* Proceed only if pressing longer than timeout */
    if (ui32ButtonFilter > 200)
    {
        ui32ButtonFilter = 0;
        if (bDemoMode)
        {
            M1_SetSpeed(0);
            M1_SetAppSwitch(FALSE);
            bDemoMode = FALSE;
        }
        else
        {
            M1_SetAppSwitch(TRUE);
            bDemoMode              = TRUE;
            ui32SpeedStimulatorCnt = 0;
        }
    }

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;

}


/*!
 *@brief      Initialization of the GPIO pins
 *
 *@param      none
 *
 *@return     none
 */
static void BOARD_InitGPIO(void)
{
    /* Connect trigger sources to PINT */
    SYSCON_AttachSignal(SYSCON, kPINT_PinInt1, kSYSCON_GpioPort0Pin4ToPintsel);

    /* Initialize PINT */
    PINT_Init(PINT);

    /* Setup Pin Interrupt 1 for falling edge */
    PINT_PinInterruptConfig(PINT, kPINT_PinInt1, kPINT_PinIntEnableFallEdge, DemoButtonIsr);

    /* Enable callbacks for PINT1 by Index */
    PINT_EnableCallbackByIndex(PINT, kPINT_PinInt1);

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
