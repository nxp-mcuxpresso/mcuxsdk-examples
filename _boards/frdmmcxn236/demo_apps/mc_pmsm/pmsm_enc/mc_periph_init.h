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

#include "fsl_device_registers.h"
#include "mcdrv_pwm3ph_pwma.h"
#include "mcdrv_adc_mcxn.h"
#include "mcdrv_enc_qdc.h"
#include "m1_pmsm_appconfig.h"
#include "peripherals.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_inputmux.h"
#include "fsl_port.h"
#include "fsl_lpcmp.h"
#include "fsl_spc.h"

/* Switch between motor 1 and motor 2 connector */
#define M1_CONNECTOR_ID_MC1		(1U)                    /* Motor connector 1 */
#define M1_CONNECTOR_ID_MC2		(2U)                    /* Motor connector 2 */
#define M1_CONNECTOR_ID			(M1_CONNECTOR_ID_MC2)

/******************************************************************************
 * Timing
 ******************************************************************************/
#define M1_PWM_FREQ (16000)         /* PWM frequency - 16kHz */
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

/* Over Current Fault detection */
#define M1_FAULT_NUM (0)

 /******************************************************************************
  * Output control
  ******************************************************************************/
/* DC bus braking resistor control */
#define M1_BRAKE_SET()
#define M1_BRAKE_CLEAR()
/* DC bus braking threshold hysteresis */
#define M1_U_DCB_HYSTERESIS (0.05F)

/******************************************************************************
 * ADC measurement definition
 ******************************************************************************/
/* Configuration of ADC channels according to the input pin signal */
#define CUR_A_CHANNEL_NUMBER    21U
#define CUR_B_CHANNEL_NUMBER    20U
#define CUR_C_CHANNEL_NUMBER    19U
#define VOLT_DCB_CHANNEL_NUMBER 18U   

/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * Define motor ADC control functions
 ******************************************************************************/
#define M1_MCDRV_CURR_3PH_VOLT_DCB_GET(par)	(MCDRV_CurrAndVoltDcBusGet(par))
#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par)
#define M1_MCDRV_CURR_3PH_CALIB_INIT(par) (MCDRV_Curr3Ph2ShCalibInit(par))
#define M1_MCDRV_CURR_3PH_CALIB(par) (MCDRV_Curr3Ph2ShCalib(par))
#define M1_MCDRV_CURR_3PH_CALIB_SET(par) (MCDRV_Curr3Ph2ShCalibSet(par))

/******************************************************************************
 * Define motor 3-ph PWM control functions
 ******************************************************************************/
#define M1_MCDRV_PWM3PH_SET(par) (MCDRV_eFlexPwm3PhSet(par))
#define M1_MCDRV_PWM3PH_EN(par) (MCDRV_eFlexPwm3PhOutEn(par))
#define M1_MCDRV_PWM3PH_DIS(par) (MCDRV_eFlexPwm3PhOutDis(par))
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eFlexPwm3PhFltGet(par))

/******************************************************************************
 * Define position and speed sensor - quadrature encoder for motor 1
 ******************************************************************************/
#define M1_MCDRV_ENC_GET(par) (MCDRV_QdEncGet(par))
#define M1_MCDRV_ENC_SET_DIRECTION(par) (MCDRV_QdEncSetDirection(par))
#define M1_MCDRV_ENC_SET_PULSES(par) (MCDRV_QdEncSetPulses(par))
#define M1_MCDRV_ENC_CLEAR(par) (MCDRV_QdEncClear(par))

/* SENSORS constans moved from MCAT */
#define M1_POSPE_ENC_PULSES (2000)
#define M1_POSPE_ENC_DIRECTION (0)
#define M1_POSPE_ENC_N_MIN (0.0F)
#define M1_POSPE_MECH_POS_GAIN ACC32(32768.0/((M1_POSPE_ENC_PULSES*4.0)/2.0))
#define M1_POSPE_TO_KP_GAIN (1256.64F)
#define M1_POSPE_TO_KI_GAIN (24.6740F)
#define M1_POSPE_TO_THETA_GAIN (0.0000198944F)

/******************************************************************************
 * Global typedefs
 ******************************************************************************/
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
 * Global variable definitions
 ******************************************************************************/
extern mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;
extern mcdrv_adc_t g_sM1Curr3phDcBus;
extern mcdrv_qd_enc_t g_sM1Enc;

extern clock_setup_t g_sClockSetup;

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
void MCDRV_Init_M1(void);

#ifdef __cplusplus
}
#endif
#endif /* _MC_PERIPH_INIT_H_  */
