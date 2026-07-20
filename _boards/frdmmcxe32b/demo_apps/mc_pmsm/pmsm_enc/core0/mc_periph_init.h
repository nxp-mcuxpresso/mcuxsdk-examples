/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021, 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* This is a generic configuration file of the motor control driver. You need to edit the file.
 * Remove this warning statement after this file is edited manually or
 * re-generate this file using MC_PMSM Config Tool component.
 */

#ifndef _MC_PERIPH_INIT_H_
#define _MC_PERIPH_INIT_H_

#include "fsl_pit.h"
#include "fsl_sar_adc.h"
#include "fsl_trgmux.h"
#include "fsl_lcu.h"
#include "fsl_bctu.h"

#include "mcdrv_emios_mcxe.h"
#include "mcdrv_sar_adc_mcxe.h"
#include "mcdrv_lcu_mcxe.h"

#include "mcdrv_enc_emios.h"
#include "m1_pmsm_appconfig.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
  
/* Structure used during clocks and modulo calculations */
typedef struct _clock_setup
{
    uint32_t ui32FastPeripheralClock;
    uint32_t ui32CpuFrequency;
    uint32_t ui32BusClock;
    uint32_t ui32SysPllClock;
    uint16_t ui16M1SpeedLoopFreq;
    uint16_t ui16M1SpeedLoopModulo;
    uint16_t ui16M1PwmFreq;
    uint16_t ui16M1PwmModulo;
    uint16_t ui16M1PwmDeadTime;
} clock_setup_t;   
      
/******************************************************************************
 * Timing
 ******************************************************************************/
#define M1_PWM_FREQ (10000)         /* PWM frequency */
#define M1_FOC_FREQ_VS_PWM_FREQ (1) /* FOC calculation is called every n-th PWM reload */
#define M1_SPEED_LOOP_FREQ (1000)   /* Speed loop frequency */
#define M1_PWM_DEADTIME (500)       /* Output PWM deadtime value in nanoseconds */

#define M1_FAST_LOOP_TS ((float_t)1.0 / (float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define M1_SLOW_LOOP_TS ((float_t)1.0 / (float_t)(M1_SLOW_LOOP_FREQ))
#define M1_TIME_ONESEC_COUNT (M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)

/* Fast loop frequency in Hz */
#define M1_FAST_LOOP_FREQ       (M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)
/* Slow control loop frequency */
#define M1_SLOW_LOOP_FREQ       (1000U)

 /******************************************************************************
  * Output control
  ******************************************************************************/

/* Over-current Fault enable */
#define M1_FAULT_ENABLE         (0U)
/* Over-current Fault detection number */
#define M1_FAULT_NUM            (0U)
/* Over-current CMP input channel */   
#define CMP_INPUT_CHANNEL       (0U)
/* CMP DAC input channel */ 
#define CMP_DAC_CHANNEL         (7U)
/* CMP Overcurrent threshold */
#define CMP_THRESHOLD           (250U)

/* DC bus braking resistor control */
#define M1_BRAKE_SET()          
#define M1_BRAKE_CLEAR()        
/* DC bus braking threshold hysteresis */
#define M1_U_DCB_HYSTERESIS             (0.05F)
#define M1_U_DCB_UPPER_THRESHOLD        (frac16_t)((M1_U_DCB_TRIP) * (1.0F + (M1_U_DCB_HYSTERESIS)))
#define M1_U_DCB_LOWER_THRESHOLD        (frac16_t)((M1_U_DCB_TRIP) * (1.0F - (M1_U_DCB_HYSTERESIS)))

/******************************************************************************
 * ADC measurement definition
 ******************************************************************************/

/******************************************************************************
 * Define motor ADC control functions
 ******************************************************************************/
#define M1_MCDRV_CURR_3PH_VOLT_DCB_GET(par) MCDRV_CurrAndVoltDcBusGet(par)
#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par)
#define M1_MCDRV_CURR_3PH_CALIB_INIT(par) (MCDRV_Curr3Ph2ShCalibInit(par))
#define M1_MCDRV_CURR_3PH_CALIB(par) (MCDRV_Curr3Ph2ShCalib(par))
#define M1_MCDRV_CURR_3PH_CALIB_SET(par) (MCDRV_Curr3Ph2ShCalibSet(par))

/******************************************************************************
 * Define 3-ph PWM control functions for motor 1
 ******************************************************************************/
#define M1_MCDRV_PWM_PERIPH_INIT() (M1_InitPWM())
#define M1_MCDRV_PWM3PH_SET(par) (MCDRV_eMIOS_PhSet(par))
#define M1_MCDRV_PWM3PH_EN(void) (MCDRV_LCU_PhOutEn())
#define M1_MCDRV_PWM3PH_DIS(void) (MCDRV_LCU_PhOutDis())
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eMIOS_PhFltGet(par))

/******************************************************************************
 * Define position and speed sensor - quadrature encoder for motor 1
 ******************************************************************************/
#define M1_MCDRV_ENC_GET_DATA_FAST(par) (MCDRV_QdEncGet(par))
#define M1_MCDRV_ENC_GET_DATA_SLOW(par)
#define M1_MCDRV_ENC_SET_DIRECTION(par)
#define M1_MCDRV_ENC_SET_PULSES(par)
#define M1_MCDRV_ENC_CLEAR(par) (MCDRV_QdEncClear(par))
 
/* SENSORS constants moved from MCAT calculations */
#define PI (3.14159265358979323846)
#define M1_POSPE_ENC_PULSES (2000) // Number of pulses - Teknic 2311P
#define M1_POSPE_ENC_DIRECTION (0)
#define M1_POSPE_ENC_N_MIN (0.0F)  
#define M1_POSPE_MECH_POS_GAIN ACC32(65535.0/(float_t)(M1_POSPE_ENC_PULSES*4.0))
#define M1_POSPE_TO_F0 (100)       // Sensor TO bandwidth
#define M1_POSPE_TO_KP_GAIN (4 * PI * M1_POSPE_TO_F0)
#define M1_POSPE_TO_KI_GAIN ((2* PI * M1_POSPE_TO_F0) * (2* PI * M1_POSPE_TO_F0) * M1_FAST_LOOP_TS)
#define M1_POSPE_TO_THETA_GAIN (M1_FAST_LOOP_TS / PI )

/******************************************************************************
 * Global variable definitions
 ******************************************************************************/
        
extern mcdrv_pwm3ph_emios_t g_sM1Pwm3ph;
extern mcdrv_adc_t g_sM1Curr3phDcBus;
extern mcdrv_qd_enc_t g_sM1Enc;
extern clock_setup_t g_sClockSetup;

/*******************************************************************************
 * API
 ******************************************************************************/

void MCDRV_Init_M1(void);
void MCDRV_Calib_ADC(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _MC_PERIPH_INIT_H_  */
