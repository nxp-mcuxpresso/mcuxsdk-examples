/*
    * Copyright 2024 NXP
    *
    * SPDX-License-Identifier: BSD-3-Clause
*/

#include "mc_periph_init.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* configuration structure for 3-phase PWM driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

/* structure for current and voltage measurement*/
mcdrv_adc12_t g_sM1AdcSensor;

/* structure containing clock frequencies and modulos */
clock_setup_t g_sClockSetup;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief   void MCDRV_Init_M1(void)
 *           - Motor control driver main initialization
 *           - Calls initialization functions of peripherals required for motor
 *             control functionality
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_Init_M1(void)
{
    /* Init application clock dependent variables */
    M1_InitClock();
    
    /* Init PWM driver */
    M1_InitPWMA();
    
    /* Init ADC driver */
    M1_InitADC12();
}

/*!
 * @brief      Core, bus, flash clock setup
 *
 * @param      void
 *
 * @return     none
 */
void M1_InitClock(void)
{
    /* Calculate clock dependant variables for PMSM sensorless control algorithm */
    g_sClockSetup.ui32FastPeripheralClock = CLOCK_GetIpClkSrcFreq(kCLOCK_PWMACH0);
    g_sClockSetup.ui16M1PwmFreq           = M1_PWM_FREQ; /* 10kHz */
                                                         /* PWM module calculated as follows:
                                                          * PWM_MOD = PWM_CLOCK / PWM_FREQUNCY = 100 MHz / 10 kHz = 10000 */
    g_sClockSetup.ui16M1PwmModulo     = g_sClockSetup.ui32FastPeripheralClock / g_sClockSetup.ui16M1PwmFreq;
    g_sClockSetup.ui16M1PwmDeadTime   = g_sClockSetup.ui32FastPeripheralClock / (1000000000U / M1_PWM_DEADTIME);
    g_sClockSetup.ui16M1SpeedLoopFreq = M1_SLOW_LOOP_FREQ; /* 1kHz */
}

/*!
 * @brief   void M1_InitPWMA(void)
 *           - Initialization of the eFlexPWMA peripheral for motor M1
 *           - 3-phase center-aligned PWM
 *
 * @param   void
 *
 * @return  none
 */
void M1_InitPWMA(void)
{
	g_sM1Pwm3ph.pui32PwmBaseAddress = (PWM_Type *)PWMA;
	
	g_sM1Pwm3ph.ui16PhASubNum = M1_PWM_PAIR_PHA; /* PWMA phase A sub-module number */
	g_sM1Pwm3ph.ui16PhBSubNum = M1_PWM_PAIR_PHB; /* PWMA phase B sub-module number */
	g_sM1Pwm3ph.ui16PhCSubNum = M1_PWM_PAIR_PHC; /* PWMA phase C sub-module number */
	
    /* Enable the PWM counter */
    PWMA->MCTRL |= PWM_MCTRL_RUN(0x7);
}

/*!
 * @brief   void M1_InitADC12(void)
 *           - Initialization of the ADC12 peripheral
 *           - Initialization of the A/D converter for current and voltage sensing
 *
 * @param   void
 *
 * @return  none
 */
void M1_InitADC12(void)
{
    /**************************************/
    /* motor M1 ADC driver initialization */
    /**************************************/
    /* offset filter window */
    g_sM1AdcSensor.ui16OffsetFiltWindow = ADC_OFFSET_WINDOW;

    /* adc base address */
    g_sM1AdcSensor.pui32AdcBase = (ADC_Type *)ADC;

    /* Phase current measurement */
    /* Sector 1,6 - measured currents Ic & Ib */
    /* ADC0, channel Ic = M1_ADC0_PH_C, , SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaC = M1_ADC0_PH_C;
    g_sM1AdcSensor.sCurrSec16.ui16AdcNumPhaC  = ADC0;
    /* ADC1, channel Ib = M1_ADC1_PH_B, , SAMPLE & RESULT = 8 */
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaB = M1_ADC1_PH_B;
    g_sM1AdcSensor.sCurrSec16.ui16AdcNumPhaB  = ADC1;

    /* Sector 2,3 - measured currents Ic & Ia*/
    /* ADC0, channel Ic = M1_ADC1_PH_C, SAMPLE & RESULT = 8 */
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaC = M1_ADC1_PH_C;
    g_sM1AdcSensor.sCurrSec23.ui16AdcNumPhaC  = ADC1;
    /* ADC1, channel Ia = M1_ADC0_PH_A, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaA = M1_ADC0_PH_A;
    g_sM1AdcSensor.sCurrSec23.ui16AdcNumPhaA  = ADC0;

    /* Sector 4,5 - measured currents Ia & Ib */
    /* ADC0, channel Ia = M1_ADC0_PH_A, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaA = M1_ADC0_PH_A;
    g_sM1AdcSensor.sCurrSec45.ui16AdcNumPhaA  = ADC0;
    /* ADC1, channel Ib = M1_ADC1_PH_B, SAMPLE & RESULT = 8  */
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaB = M1_ADC1_PH_B;
    g_sM1AdcSensor.sCurrSec45.ui16AdcNumPhaB  = ADC1;

    /* UDCbus channel measurement */
    /*  channel Udcb = M1_ADC0_UDCB, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.ui16ChanNumVDcb = M1_ADC0_UDCB;
    g_sM1AdcSensor.ui16AdcNumVDcb  = ADC0;

    /* Assign channels and init all pointers */
    MCDRV_Curr3Ph2ShChanAssignInit(&g_sM1AdcSensor);
}
