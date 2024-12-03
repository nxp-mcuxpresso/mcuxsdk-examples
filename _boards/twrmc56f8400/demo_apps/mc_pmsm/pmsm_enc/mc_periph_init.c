/*
    * Copyright 2024 NXP
    *
    * SPDX-License-Identifier: BSD-3-Clause
*/

#include "mc_periph_init.h"
#include "fsl_common.h"
#include "fsl_xbara.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ENC_CTRL2_MOD_MASK                       (0x4U)
#define ENC_CTRL2_REVMOD_MASK                    (0x100U)

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* configuration structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

/* structure for current and voltage measurement*/
mcdrv_adc12_t g_sM1AdcSensor;

/* structure for mc33937 driver setting via QSPI*/
mcdrv_spi_drv3ph_t g_sM1Driver3ph;

/* structure containing clock frequencies and modulos */
clock_setup_t g_sClockSetup;

/* Structure for Encoder driver */
mcdrv_qd_enc_t g_sM1Enc;

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

void MCDRV_Init_M1(void) {
	/* Init application clock dependent variables */
	InitClock();

	/* Init PWM driver */
	InitPWMA();

	/* Init ADC driver */
	InitADC12();

	/* Init SPI driver */
	InitQSPI();

    /* Qudrature decoder peripheral init */
	M1_InitQD();

	/* init XBAR */
	InitXBARA();
}

/* function state variables */

void M1_InitQD(void){
	/* Enable clock to ENC modules */
	CLOCK_EnableClock(kCLOCK_QDC);

	/* Pass initialization data into encoder driver structure */
	/* encoder position and speed measurement */

	g_sM1Enc.pui32QdBase = (QDC_Type *) QDC;

	g_sM1Enc.sTo.f16PGain = M1_POSPE_TO_KP_GAIN;
	g_sM1Enc.sTo.i16PGainSh = M1_POSPE_TO_KP_SHIFT;
	g_sM1Enc.sTo.f16IGain = M1_POSPE_TO_KI_GAIN;
	g_sM1Enc.sTo.i16IGainSh = M1_POSPE_TO_KI_SHIFT;
	g_sM1Enc.sTo.f16ThGain = M1_POSPE_TO_THETA_GAIN;
	g_sM1Enc.sTo.i16ThGainSh = M1_POSPE_TO_THETA_SHIFT;

	g_sM1Enc.a32PosMeGain = M1_POSPE_MECH_POS_GAIN;
	g_sM1Enc.ui16Pp = M1_MOTOR_PP;
	g_sM1Enc.bDirection = M1_POSPE_ENC_DIRECTION;
	g_sM1Enc.sTo.f32Speed = M1_POSPE_ENC_N_MIN;
	g_sM1Enc.ui16PulseNumber = M1_POSPE_ENC_PULSES;
	MCDRV_QdEncSetDirection(&g_sM1Enc);

	/* Initialization modulo counter */
	M1_MCDRV_QD_SET_PULSES(&g_sM1Enc);

	/* quadrature pulses per one revolution */
	QDC->LMOD = QDC_LMOD_MOD((g_sM1Enc.ui16PulseNumber * 4U) - 1U);

	/* Enable modulo counting and revolution counter increment on roll-over */
	QDC->CTRL2 = (ENC_CTRL2_MOD_MASK | ENC_CTRL2_REVMOD_MASK);
}

/*!
 * @brief      Core, bus, flash clock setup
 *
 * @param      void
 *
 * @return     none
 */
void InitClock(void)
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
 * @brief   void InitPWMA(void)
 *           - Initialization of the eFlexPWMA peripheral for motor M1
 *           - 3-phase center-aligned PWM
 *
 * @param   void
 *
 * @return  none
 */
void InitPWMA(void)
{
	/* ---------------------------------------- */
	/* Initialize MC driver */
	g_sM1Pwm3ph.pui32PwmBaseAddress = (PWM_Type *) PWMA;

	g_sM1Pwm3ph.ui16PhASubNum = M1_PWM_PAIR_PHA; /* PWMA phase A sub-module number */
	g_sM1Pwm3ph.ui16PhBSubNum = M1_PWM_PAIR_PHB; /* PWMA phase B sub-module number */
	g_sM1Pwm3ph.ui16PhCSubNum = M1_PWM_PAIR_PHC; /* PWMA phase C sub-module number */

	g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM; /* PWMA fixed-value over-current fault number */
	g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM; /* PWMA adjustable over-current fault number */
}

void InitXBARA(void)
{
	/* Enable clock to XBAR */
	CLOCK_EnableClock(kCLOCK_XBARA);
}

/*!
 * @brief   void InitADC12(void)
 *           - Initialization of the ADC12 peripheral
 *           - Initialization of the A/D converter for current and voltage sensing
 *
 * @param   void
 *
 * @return  none
 */
void InitADC12(void)
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
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaC = M1_ADC1_PH_C;
    g_sM1AdcSensor.sCurrSec16.ui16AdcNumPhaC  = ADC1;
    /* ADC1, channel Ib = M1_ADC1_PH_B, , SAMPLE & RESULT = 8 */
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaB = M1_ADC0_PH_B;
    g_sM1AdcSensor.sCurrSec16.ui16AdcNumPhaB  = ADC0;

    /* Sector 2,3 - measured currents Ic & Ia*/
    /* ADC0, channel Ic = M1_ADC0_PH_C, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaC = M1_ADC1_PH_C;
    g_sM1AdcSensor.sCurrSec23.ui16AdcNumPhaC  = ADC1;
    /* ADC1, channel Ia = M1_ADC1_PH_A, SAMPLE & RESULT = 8 */
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaA = M1_ADC0_PH_A;
    g_sM1AdcSensor.sCurrSec23.ui16AdcNumPhaA  = ADC0;

    /* Sector 4,5 - measured currents Ia & Ib */
    /* ADC0, channel Ia = M1_ADC0_PH_A, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaA = M1_ADC1_PH_A;
    g_sM1AdcSensor.sCurrSec45.ui16AdcNumPhaA  = ADC1;
    /* ADC1, channel Ib = M1_ADC1_PH_B, SAMPLE & RESULT = 8  */
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaB = M1_ADC0_PH_B;
    g_sM1AdcSensor.sCurrSec45.ui16AdcNumPhaB  = ADC0;

    /* UDCbus channel measurement */
    /*  channel Udcb = M1_ADC1_UDCB, SAMPLE & RESULT = 9 */
    g_sM1AdcSensor.ui16ChanNumVDcb = M1_ADC1_UDCB;
    g_sM1AdcSensor.ui16AdcNumVDcb  = ADC1;

    /* Assign channels and init all pointers */
    MCDRV_Curr3Ph2ShChanAssignInit(&g_sM1AdcSensor);
}

/*!
 * @brief   void InitQSPI(void)
 *           - Initialization of the InitQSPI peripheral
 *           - Initialization of the QSPI for mc33937 moseft driver
 *
 * @param   void
 *
 * @return  none
 */
void InitQSPI(void)
{
    /**************************************/
    /* motor M1 ADC driver initialization */
    /**************************************/
	/* qspi base address */
	g_sM1Driver3ph.sSpiData.pSpiBase = (QSPI_Type *)QSPI0;
	g_sM1Driver3ph.sSpiData.ui16Pcs = (0);                 /* PCS number */

	/* Enable PIN & PORT */
	g_sM1Driver3ph.sSpiData.pGpioEnBase = (GPIO_Type *)(GPIOF);   /* GPIOx Base Address */
	g_sM1Driver3ph.sSpiData.ui16GpioEnPin = kGPIO_Pin0;                  /* pin number for driver enabled */

	/* Interrupt PIN & PORT */
	g_sM1Driver3ph.sSpiData.pGpioIntBase = (GPIO_Type *)(GPIOF);   /* GPIOx Base Address */
	g_sM1Driver3ph.sSpiData.ui16GpioIntPin = 8U;                 /* pin number for interrupt detection */

	/* Reset PIN & PORT */

	/* zero deadtime set in MC33937, dead time control from FTM*/
	MCDRV_Driver3PhConfig(&g_sM1Driver3ph);
	MCDRV_Driver3PhGetSr0(&g_sM1Driver3ph);
	MCDRV_Driver3PhGetSr1(&g_sM1Driver3ph);
	MCDRV_Driver3PhGetSr2(&g_sM1Driver3ph);
	MCDRV_Driver3PhGetSr3(&g_sM1Driver3ph);
	MCDRV_Driver3PhClearFlags(&g_sM1Driver3ph);
	MCDRV_Driver3PhSetEn(&g_sM1Driver3ph);
}
