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
#include "mcdrv_eflexpwm_lpc.h"
#include "mcdrv_adc_lpc55s36.h"
#include "mcdrv_enc_qd.h"
#include "m1_pmsm_appconfig.h"
#include "m2_pmsm_appconfig.h"
#include "mcdrv_flash_lpc55s36.h"

/* LPC55S36's internal oamps are connected to motor connector 1 only. */
#define USE_INTERNAL_OPAMPS     (true)

/* Enable/disable online update flash parameters */
#define ENABLE_FLASH_PARAM_UPDATE (true)

/******************************************************************************
 * Timing - common for motor 1 and motor 2
 ******************************************************************************/
#define MCU_CLOCK_FREQ          (150000000U)                    /* MCU core clock 150 MHz */

/******************************************************************************
 * Timing for motor 1
 ******************************************************************************/
#define M1_PWM_FREQ             (10000U)                        /* PWM frequency in Hz */
#define M1_PWM_MODULO           (MCU_CLOCK_FREQ / M1_PWM_FREQ)  /* PWM modulo = FTM_input_clock / M1_PWM_FREQ */
#define M1_PWM_DEADTIME         (1000)                           /* Output PWM deadtime value in nanoseconds */
#define M1_FOC_FREQ_VS_PWM_FREQ (1U)                            /* PWM vs. Fast control loop ratio */
#define M1_SLOW_LOOP_FREQ       (1000U)                         /* Slow control loop frequency in Hz */

/******************************************************************************
 * Timing for motor 2
 ******************************************************************************/
#define M2_PWM_FREQ             (10000U)                        /* PWM frequency in Hz */
#define M2_PWM_MODULO           (MCU_CLOCK_FREQ / M2_PWM_FREQ)  /* PWM modulo = FTM_input_clock / M2_PWM_FREQ */
#define M2_PWM_DEADTIME         (1000)                           /* Output PWM deadtime value in nanoseconds */
#define M2_FOC_FREQ_VS_PWM_FREQ (1U)                            /* PWM vs. Fast control loop ratio */
#define M2_SLOW_LOOP_FREQ       (1000U)                         /* Slow control loop frequency in Hz */

 /******************************************************************************
  * Output control
  ******************************************************************************/
/* DC bus braking resistor control */
#define M1_BRAKE_SET()
#define M2_BRAKE_SET()
#define M1_BRAKE_CLEAR()
#define M2_BRAKE_CLEAR()
/* DC bus braking threshold hysteresis */
#define M1_U_DCB_HYSTERESIS (0.05F)
#define M2_U_DCB_HYSTERESIS (0.05F)

/******************************************************************************
 * ADC measurement definition
 ******************************************************************************/
/* Configuration table of ADC channels according to the input pin signals:
 * Valid for lpc55s36 together with FRDM-MC-LVPMSM
 *
 * Motor 1 - external OpAmp
 * M1_I_A       | ADC0_CH3A
 * M1_I_B       | ADC0_CH8B
 * M1_I_C       | ADC0_CH3B
 * M1_U_DCB     | ADC0_CH1A
 *
 * Motor 1 - internal OpAmp
 * M1_I_A       | ADC0_CH0A
 * M1_I_B       | ADC0_CH2A
 * M1_I_C       | ADC0_CH0B
 * M1_U_DCB     | ADC0_CH1A

 * Motor 2
 * M2_I_A       | ADC1_CH3A
 * M2_I_B       | ADC1_CH2A
 * M2_I_C       | ADC1_CH3B
 * M2_U_DCB     | ADC1_CH1B
 */

/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * Define motor 1 ADC control functions
 ******************************************************************************/
#define M1_MCDRV_CURR_3PH_VOLT_DCB_GET(par)	(MCDRV_CurrAndVoltDcBusGet(par))
#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par)
#define M1_MCDRV_CURR_3PH_CALIB_INIT(par) (MCDRV_Curr3Ph2ShCalibInit(par))
#define M1_MCDRV_CURR_3PH_CALIB(par) (MCDRV_Curr3Ph2ShCalib(par))
#define M1_MCDRV_CURR_3PH_CALIB_SET(par) (MCDRV_Curr3Ph2ShCalibSet(par))

/******************************************************************************
 * Define motor 2 ADC control functions
 ******************************************************************************/
#define M2_MCDRV_CURR_3PH_VOLT_DCB_GET(par)	(MCDRV_CurrAndVoltDcBusGet(par))
#define M2_MCDRV_CURR_3PH_CHAN_ASSIGN(par)
#define M2_MCDRV_CURR_3PH_CALIB_INIT(par) (MCDRV_Curr3Ph2ShCalibInit(par))
#define M2_MCDRV_CURR_3PH_CALIB(par) (MCDRV_Curr3Ph2ShCalib(par))
#define M2_MCDRV_CURR_3PH_CALIB_SET(par) (MCDRV_Curr3Ph2ShCalibSet(par))

/******************************************************************************
 * Define motor 1 3-ph PWM control functions
 ******************************************************************************/
#define M1_MCDRV_PWM3PH_SET(par) (MCDRV_eFlexPwm3PhSet(par))
#define M1_MCDRV_PWM3PH_EN(par) (MCDRV_eFlexPwm3PhOutEn(par))
#define M1_MCDRV_PWM3PH_DIS(par) (MCDRV_eFlexPwm3PhOutDis(par))
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eFlexPwm3PhFltGet(par))
#define M1_MCDRV_P2M3PH_FLT_TRY_CLR(par) (MCDRV_eFlexPwm3PhFltTryClr(par))

/******************************************************************************
 * Define motor 2 3-ph PWM control functions
 ******************************************************************************/
#define M2_MCDRV_PWM3PH_SET(par) (MCDRV_eFlexPwm3PhSet(par))
#define M2_MCDRV_PWM3PH_EN(par) (MCDRV_eFlexPwm3PhOutEn(par))
#define M2_MCDRV_PWM3PH_DIS(par) (MCDRV_eFlexPwm3PhOutDis(par))
#define M2_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eFlexPwm3PhFltGet(par))
#define M2_MCDRV_P2M3PH_FLT_TRY_CLR(par) (MCDRV_eFlexPwm3PhFltTryClr(par))

/******************************************************************************
 * Define position and speed sensor - quadrature encoder for motor 1
 ******************************************************************************/
#define M1_MCDRV_ENC_GET(par) (MCDRV_QdEncGet(par))
#define M1_MCDRV_ENC_SET_DIRECTION(par) (MCDRV_QdEncSetDirection(par))
#define M1_MCDRV_ENC_SET_PULSES(par) (MCDRV_QdEncSetPulses(par))
#define M1_MCDRV_ENC_CLEAR(par) (MCDRV_QdEncClear(par))

/******************************************************************************
 * Define position and speed sensor - quadrature encoder for motor 2
 ******************************************************************************/
#define M2_MCDRV_ENC_GET(par) (MCDRV_QdEncGet(par))
#define M2_MCDRV_ENC_SET_DIRECTION(par) (MCDRV_QdEncSetDirection(par))
#define M2_MCDRV_ENC_SET_PULSES(par) (MCDRV_QdEncSetPulses(par))
#define M2_MCDRV_ENC_CLEAR(par) (MCDRV_QdEncClear(par))

/******************************************************************************
 * SENSORS constants moved from MCAT calculation
 ******************************************************************************/
#define M1_POSPE_ENC_PULSES (2000)
#define M1_POSPE_ENC_DIRECTION (0)
#define M1_POSPE_ENC_N_MIN (0.0F)
#define M1_POSPE_MECH_POS_GAIN ACC32(32768.0/((M1_POSPE_ENC_PULSES*4.0)/2.0))
#define M1_POSPE_TO_KP_GAIN (1256.64F)
#define M1_POSPE_TO_KI_GAIN (39.4784F)
#define M1_POSPE_TO_THETA_GAIN (0.0000318310F)

#define M2_POSPE_ENC_PULSES (1000)
#define M2_POSPE_ENC_DIRECTION (0)
#define M2_POSPE_ENC_N_MIN (0.0F)
#define M2_POSPE_MECH_POS_GAIN ACC32(16.384)
#define M2_POSPE_TO_KP_GAIN (1256.64F)
#define M2_POSPE_TO_KI_GAIN (39.4784F)
#define M2_POSPE_TO_THETA_GAIN (0.0000318310F)

/******************************************************************************
 * Define online update flash parameters functions
 ******************************************************************************/
#define M1_MCDRV_FLASH_CFG_INIT() (Drv_Flash_Init())
#define M1_MCDRV_FLASH_CFG_READ(par) (Drv_Flash_Cfg_Read(par))
#define M1_MCDRV_FLASH_CFG_CLEAR(par) (Drv_Flash_Cfg_Clear(par))
#define M1_MCDRV_FLASH_CFG_WRITE(par) (Drv_Flash_Cfg_Write(par))
#define M1_MCDRV_FLASH_CFG_SWAP(par) (Drv_ParamsSwap(par))
#define M1_MCDRV_FLASH_CFG_BACKGROUND() (Drv_Flash_Cfg_Background())

/******************************************************************************
 * Global typedefs
 ******************************************************************************/
/* Structure used during clocks and modulo calculations */
typedef struct _clock_setup
{
    /* Common variables for motor 1 and motor 2 */
    uint32_t ui32FastPeripheralClock;
    uint32_t ui32CpuFrequency;
    uint32_t ui32BusClock;
    uint32_t ui32SysPllClock;

    /* Variables for motor 1 */
    uint16_t ui16M1SpeedLoopFreq;
    uint16_t ui16M1SpeedLoopModulo;
    uint16_t ui16M1PwmFreq;
    uint16_t ui16M1PwmModulo;
    uint16_t ui16M1PwmDeadTime;

    /* Variables for motor 2 */
    uint16_t ui16M2SpeedLoopFreq;
    uint16_t ui16M2SpeedLoopModulo;
    uint16_t ui16M2PwmFreq;
    uint16_t ui16M2PwmModulo;
    uint16_t ui16M2PwmDeadTime;
} clock_setup_t;

/******************************************************************************
 * Global variable definitions
 ******************************************************************************/
extern mcdrv_eflexpwm_t g_sM1Pwm3ph;
extern mcdrv_eflexpwm_t g_sM2Pwm3ph;
extern mcdrv_adc_t g_sM1Curr3phDcBus;
extern mcdrv_adc_t g_sM2Curr3phDcBus;
extern mcdrv_qd_enc_t g_sM1Enc;
extern mcdrv_qd_enc_t g_sM2Enc;

extern clock_setup_t g_sClockSetup;

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
void MCDRV_Init(void);

#ifdef __cplusplus
}
#endif
#endif /* _MC_PERIPH_INIT_H_  */
