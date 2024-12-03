/*
    * Copyright 2024 NXP
    *
    * SPDX-License-Identifier: BSD-3-Clause
*/

#include "fsl_device_registers.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "peripherals.h"
#include "board.h"

#include "m1_sm_snsless_enc.h"
#include "mc_periph_init.h"
#include "mid_sm_states.h"
#include "m1_mid_switch.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MCRSP_VER "2.0.0"

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

void ADC_A_IRQHANDLER(void);
void PIT0_IRQHANDLER(void);
void INT_0_IRQHANDLER(void);
static void DemoSpeedStimulator(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* CPU load measurement variables */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;

/* Demo mode enabled/disabled */
bool_t bDemoMode = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt = 0U;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0U;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "../boards/mc56f81000evk/mc_pmsm/pmsm_enc",                       /* User Path 1- the highest priority */
    "../../../boards/mc56f81000evk/demo_apps/mc_pmsm/pmsm_enc",       /* User Path 2 */
    "mc56f81000evk",    /* board id */
    "pmsm_enc", 	/* example id */
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
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();

	/* Disable demo mode after reset */
    bDemoMode              = FALSE;
    ui32SpeedStimulatorCnt = 0U;

	/*Accessing ID structure to prevent optimization*/
	g_sAppIdFM.ui16FeatureSet = FEATURE_SET;

    /* Initialize peripheral motor control driver for motor M1*/
    MCDRV_Init_M1();

    /* Turn off application */
    M1_SetAppSwitch(0U);

    /* Enable interrupts. */
    __EI(0);

    /* Enable the PWM counter */
    PWMA->MCTRL |= PWM_MCTRL_RUN(0x7);

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
/* kADC12_CC0_VECTORn interrupt handler */
#pragma interrupt alignsp saveall
void ADC_A_IRQHANDLER(void) {

	/* Clear the EOS flag */
	ADC->STAT = ADC_STAT_EOSI0_MASK;

	/* Reset CPU load timer */
	CPU_LOAD_START_COUNT();

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
	static int16_t i16i = 0;

	/* Clear PIT roll-over bit. */
	PIT0->CTRL &= ~PIT_CTRL_PRF_MASK;

	/* Slow StateMachine call */
	SM_StateMachineSlow(&g_sM1Ctrl);

    /* Check if transtion between M1/MID was requested. */
    M1_MID_Switch_BL();

	/* If in STOP state turn on red */
	if (M1_GetAppState() == 2U)
	{
		/* RED LED - ON, GREEN LED OFF */
		GPIO_PinClear(BOARD_INITPINS_LED_R_GPIO, BOARD_INITPINS_LED_R_PIN_MASK);
		GPIO_PinSet(BOARD_INITPINS_LED_G_GPIO, BOARD_INITPINS_LED_G_PIN_MASK);
	}

	/* If in FAULT state red blinking*/
	else if (M1_GetAppState() == 0U)
	{
		if (i16i-- < 0)
		{
			/* toggle red LED */
			GPIO_PinToggle(BOARD_INITPINS_LED_R_GPIO, BOARD_INITPINS_LED_R_PIN_MASK);
			i16i = 125;
		}
		/* LED GREEN OFF */
		GPIO_PinSet(BOARD_INITPINS_LED_G_GPIO, BOARD_INITPINS_LED_G_PIN_MASK);
	}

	/* If in RUN or INIT state turn on green */
	else
	{
		/* RED LED - OFF, GREEN LED ON */
		GPIO_PinClear(BOARD_INITPINS_LED_G_GPIO, BOARD_INITPINS_LED_G_PIN_MASK);
	    GPIO_PinSet(BOARD_INITPINS_LED_R_GPIO, BOARD_INITPINS_LED_R_PIN_MASK);
	}

	/* Demo speed stimulator */
	DemoSpeedStimulator();
}
#pragma interrupt off

/* kGPIOC_VECTORn interrupt handler */
#pragma interrupt alignsp saveall
void INT_0_IRQHANDLER(void) {
    /* clear interrupt flags - SW5 GC13*/
	GPIO_PinClearEdgeDetectedStatusFlags(BOARD_INITPINS_SW5_GPIO, BOARD_INITPINS_SW5_PIN_MASK);

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
