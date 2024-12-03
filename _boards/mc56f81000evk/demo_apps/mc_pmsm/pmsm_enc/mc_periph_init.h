/*
    * Copyright 2024 NXP
    *
    * SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _MC_PERIPH_INIT_H_
#define _MC_PERIPH_INIT_H_

#include "fsl_device_registers.h"
#include "mcdrv_pwm3ph_pwma.h"
#include "mcdrv_adc_adc12.h"
#include "mcdrv_enc_qd.h"
#include "m1_pmsm_appconfig.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Structure used during clocks and modulo calculations */
typedef struct _clock_setup
{
    uint32_t ui32FastPeripheralClock;
    uint32_t ui32BusClock;
    uint16_t ui16M1SpeedLoopFreq;
    uint16_t ui16M1SpeedLoopModulo;
    uint16_t ui16M1PwmFreq;
    uint16_t ui16M1PwmModulo;
    uint16_t ui16M1PwmDeadTime;

} clock_setup_t;

/******************************************************************************
 * Clock & PWM definition
 ******************************************************************************/
/* NOTE: Following timing defines must correspond to actual timing set by MCUX Config Tools. */
/* PWM frequency */
#define M1_PWM_FREQ (10000)
/* FOC calculation is called every n-th PWM reload */
#define M1_FOC_FREQ_VS_PWM_FREQ (1)
/* Output PWM deadtime value in nanoseconds */
#define M1_PWM_DEADTIME (500)
/* Fast loop frequency in Hz */
#define M1_FAST_LOOP_FREQ (M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)
/* Slow control loop frequency */
#define M1_SLOW_LOOP_FREQ (1000U)

#define M1_FAST_LOOP_TS ((float_t)1.0 / (float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define M1_SLOW_LOOP_TS ((float_t)1.0 / (float_t)(M1_SLOW_LOOP_FREQ))
#define M1_TIME_ONESEC_COUNT (M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)

/* Assignment of PWM Submodules to motor phases
 * 0 - PWM Submodule 0
 * 1 - PWM Submodule 1
 * 2 - PWM Submodule 2
 * 3 - PWM Submodule 3
 */
#define M1_PWM_PAIR_PHA (0)
#define M1_PWM_PAIR_PHB (1)
#define M1_PWM_PAIR_PHC (2)

/* Braking resistor macros */
#define M1_BRAKE_SET() 
#define M1_BRAKE_CLEAR() 
/* DC bus braking threshold hysteresis */
#define M1_U_DCB_HYSTERESIS         (0.05F)
#define M1_U_DCB_UPPER_THRESHOLD    (frac16_t)((M1_U_DCB_TRIP) * (1.0F + (M1_U_DCB_HYSTERESIS)))
#define M1_U_DCB_LOWER_THRESHOLD    (frac16_t)((M1_U_DCB_TRIP) * (1.0F - (M1_U_DCB_HYSTERESIS)))

/* Over-current Fault detection */
#define M1_FAULT_NUM (0)

/* CPU load measurement START / STOP macros */
#define CPU_LOAD_START_COUNT() (TMRA->CHANNEL[0].CNTR = 0)
#define CPU_LOAD_STOP_COUNT(par1)   \
    par1 = TMRA->CHANNEL[0].CNTR

/******************************************************************************
 * ADC measurement definition
 ******************************************************************************/
/* Configuration table of ADC channels according to the input pin signals:
 * Valid for 56f8100-evk board together with FRDM-MC-LVPMSM
 *
 * Proper ADC channel assignment needs to follow these rules:
 *   - at least one phase current must be assigned to both ADC modules
 *   - two other phase current channels must be assigned to different ADC modules
 *   - Udcb and auxiliary channels must be assigned to different ADC modules

 Quantity     | ADC module 0                  | ADC module 1
--------------------------------------------------------------------------
 I_phA        | ADCA_CH0 (GA0)                | -
 I_phB        | -                             | ADCB_CH0 (PTB0)
 I_phC        | ADCA_CH3 (GA3)                | ADCB_CH3 (GB3)
 U_dcbus      | ADCA_CH2 (GA2)                | -
 Aux		  | -                             | ADCB_CH2 (GB2)
*/

/* Phase current A assingned to ADC0 only */
#define M1_ADC0_PH_A (0)
#define M1_ADC1_PH_A (MCDRV_CHAN_OFF)
/* Phase current B assingned to ADC1 only */
#define M1_ADC0_PH_B (MCDRV_CHAN_OFF)
#define M1_ADC1_PH_B (0)
/* Phase current C assingned to ADC0 and ADC1 */
#define M1_ADC0_PH_C (3)
#define M1_ADC1_PH_C (3)
/* Udc bus voltage is assigned to ADC0 only */
#define M1_ADC0_UDCB (2)
#define M1_ADC1_UDCB (MCDRV_CHAN_OFF)
/* Aux channel is assigned to ADC1 only */
#define M1_ADC0_AUX (MCDRV_CHAN_OFF)
#define M1_ADC1_AUX (2)

/* offset measurement filter window */
#define ADC_OFFSET_WINDOW (3)
/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * Define motor 1 ADC control functions
 ******************************************************************************/
#define M1_MCDRV_ADC_GET(par) \
    MCDRV_Curr3Ph2ShGet(par); \
    MCDRV_VoltDcBusGet(par);  \
    MCDRV_AuxValGet(par);
#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par) (MCDRV_Curr3Ph2ShChanAssign(par))
#define M1_MCDRV_CURR_3PH_CALIB_INIT(par) (MCDRV_Curr3Ph2ShCalibInit(par))
#define M1_MCDRV_CURR_3PH_CALIB(par) (MCDRV_Curr3Ph2ShCalib(par))
#define M1_MCDRV_CURR_3PH_CALIB_SET(par) (MCDRV_Curr3Ph2ShCalibSet(par))

/******************************************************************************
 * Define motor 1 3-ph PWM control functions
 ******************************************************************************/
#define M1_MCDRV_PWM3PH_SET(par) (MCDRV_eFlexPwm3PhSet(par))
#define M1_MCDRV_PWM3PH_EN(par) (MCDRV_eFlexPwm3PhOutEn(par))
#define M1_MCDRV_PWM3PH_DIS(par) (MCDRV_eFlexPwm3PhOutDis(par))
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eFlexPwm3PhFltGet(par))

/******************************************************************************
 * Define motor 1 Quadrature Decoder control functions
 ******************************************************************************/
#define M1_MCDRV_QD_PERIPH_INIT() M1_InitQD()
#define M1_MCDRV_QD_GET(par) (MCDRV_QdEncGet(par))
#define M1_MCDRV_QD_SET_DIRECTION(par) (MCDRV_QdEncSetDirection(par))
#define M1_MCDRV_QD_SET_PULSES(par) (MCDRV_QdEncSetPulses(par))
#define M1_MCDRV_QD_CLEAR(par) (MCDRV_QdEncClear(par))

/******************************************************************************
 * global variable definitions
 ******************************************************************************/
extern mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;
extern mcdrv_adc12_t g_sM1AdcSensor;
extern clock_setup_t g_sClockSetup;
extern mcdrv_qd_enc_t g_sM1Enc;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void InitClock(void);
void MCDRV_Init_M1(void);
void InitPWMA(void);
void InitXBARA(void);
void InitADC12(void);
void M1_InitQD(void);

#ifdef __cplusplus
}
#endif
#endif /* _MC_PERIPH_INIT_H_ */
