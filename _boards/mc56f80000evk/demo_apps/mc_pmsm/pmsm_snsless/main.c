/*
    * Copyright 2024 NXP
    *
    * SPDX-License-Identifier: BSD-3-Clause
*/

//#define MID_ENABLE

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "peripherals.h"
#include "board.h"

#ifndef MID_ENABLE
#include "m1_sm_snsless.h"
#else
#include "mid_sm_states.h"
#include "m1_mid_switch.h"
#endif

#include "mc_periph_init.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MCRSP_VER "2.0.0"

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

void ADC_A_IRQHANDLER(void);			/* Fast loop interrupt handler */
void PIT0_IRQHANDLER(void);				/* Slow loop interrupt handler */
void GPIOE_INT_VECT_IRQHANDLER(void);	/* User button interrupt handler */
static void DemoSpeedStimulator(void);	/* Demo Speed Stimulator */

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* CPU load measurement variables */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;

/* Demo mode enabled/disabled */
bool_t bDemoMode    = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt = 0U;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0U;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "../boards/mc56f80000evk/mc_pmsm/pmsm_snsless",                       /* User Path 1- the highest priority */
    "../../../boards/mc56f80000evk/demo_apps/mc_pmsm/pmsm_snsless",       /* User Path 2 */
    "mc56f80000evk",    /* board id */
    "pmsm_snsless", /* example id */
    MCRSP_VER,      /* sw version */
    FEATURE_SET,    /* example's feature-set */
};
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

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();

	/* Disable demo modes after reset */
    bDemoMode    = FALSE;
    ui32SpeedStimulatorCnt = 0U;

    /* Initialize peripheral motor control driver for motor M1*/
    MCDRV_Init_M1();

#ifndef MID_ENABLE
    /* Turn off application */
    M1_SetAppSwitch(0U);
#else
    /* Call MID init routine. */
    MID_Init();
#endif

    /* Enable interrupts. */
    __EI(0);

    /* Infinite loop */
    while (1)
    {
        /* FreeMASTER Polling function */
        FMSTR_Poll();
    }
}

/*!
 * @brief   ADC conversion complete ISR called with 100us period processes
 *           - motor M1 fast application machine function
 *           - demo mode if enabled
 *
 * @param   void
 *
 * @return  none
 */
/* kADC_CC0_VECTORn interrupt handler */
#pragma interrupt alignsp saveall
void ADC_A_IRQHANDLER(void) {

	/* Clear the EOS flag */
	ADC->STAT = ADC_STAT_EOSI0_MASK;

	/* Reset CPU load timer */
	CPU_LOAD_START_COUNT();

#ifndef MID_ENABLE
	/* Spin state machine */
    SM_StateMachineFast(&g_sM1Ctrl);
#else
    /* MID state machine */
    MID_ProcessFast_FL();
#endif

	/* Read actual CPU load */
	CPU_LOAD_STOP_COUNT(g_ui32NumberOfCycles);
	g_ui32MaxNumberOfCycles =
		g_ui32NumberOfCycles > g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;

	/* Call FreeMASTER recorder */
	FMSTR_Recorder(0);
}
#pragma interrupt off

/*!
 * @brief   PIT0 rollover ISR called with 1ms period and processes following functions:
 *           - motor M1 slow application machine function
 *
 * @param   void
 *
 * @return  none
 */
/* kPIT0_ROLLOVR_VECTORn interrupt handler */
#pragma interrupt alignsp saveall
void PIT0_IRQHANDLER(void)
{
	static int16_t i16i = 0U;

	/* Clear PIT roll-over bit. */
	PIT0->CTRL &= ~PIT_CTRL_PRF_MASK;

#ifndef MID_ENABLE
	/* Slow StateMachine call */
	SM_StateMachineSlow(&g_sM1Ctrl);

	/* If in STOP state turn on red */
	if (M1_GetAppState() == 2U)
	{
		/* RED LED - ON, BLUE LED OFF */
		LED_RED_ON();
		LED_BLUE_OFF();
	}

	/* If in FAULT state red blinking*/
	else if (M1_GetAppState() == 0U)
	{
		if (i16i-- < 0)
		{
			/* toggle red LED */
			LED_RED_TOGGLE();
			i16i = 125U;
		}
		/* LED BLUE OFF */
		LED_BLUE_OFF();
	}

	/* If in RUN or INIT state turn on green */
	else
	{
		/* RED LED - OFF, BLUE LED ON */
		LED_RED_OFF();
		LED_BLUE_ON();
	}

	/* Demo speed stimulator */
	DemoSpeedStimulator();
#else
    /* No action in the slow loop is required when identification is enabled */
#endif

}
#pragma interrupt off

/*!
 * @brief   User button SW2 interrupt handler
 *
 * @param   void
 *
 * @return  none
 */
/* kGPIOE_VECTORn interrupt handler */
#pragma interrupt alignsp saveall
void GPIOE_INT_VECT_IRQHANDLER(void) {
    /* clear interrupt flags - SW2 GE6*/
	GPIO_PortClearEdgeDetectedStatusFlags(BOARD_BUTTON_SW2_GPIO, BOARD_BUTTON_SW2_GPIO_PIN);

#ifndef MID_ENABLE
    /* Proceed only if pressing longer than timeout */
    if (ui32ButtonFilter > 200U)
    {
        ui32ButtonFilter = 0U;
        if (bDemoMode)
        {
            M1_SetSpeed(0);
            M1_SetAppSwitch(FALSE);
            bDemoMode = FALSE;
        }
        else
        {
            M1_SetAppSwitch(TRUE);
            bDemoMode = TRUE;
            ui32SpeedStimulatorCnt = 0U;
        }
    }
#endif

}
#pragma interrupt off

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
    if (ui32ButtonFilter < 1000U)
        ui32ButtonFilter++;

    if (bDemoMode)
    {
    	g_sM1Drive.eControl = kControlMode_SpeedFOC;
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
            ui32SpeedStimulatorCnt = 0U;
            break;
        default:
            break;
        }
    }
}
