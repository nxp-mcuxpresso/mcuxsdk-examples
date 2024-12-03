/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
//#include "mcdrv_flash_lpc55s36.h"
#include "m1_pmsm_appconfig.h"


/* Switch between motor 1 and motor 2 connector */
#define M1_CONNECTOR_ID_MC1		(1U)                    /* Motor connector 1 */
#define M1_CONNECTOR_ID_MC2		(2U)                    /* Motor connector 2 */
#define M1_CONNECTOR_ID			(M1_CONNECTOR_ID_MC2)

/* Enable/disable online update flash parameters */
#define ENABLE_FLASH_PARAM_UPDATE (false)

/******************************************************************************
 * Timing
 ******************************************************************************/
/* MCU core clock */
#define MCU_CLOCK_FREQ          (150000000U) /* 150 MHz */
/* PWM frequency in Hz*/
#define M1_PWM_FREQ             (10000U)
/* PWM modulo = FTM_input_clock / M1_PWM_FREQ */
#define M1_PWM_MODULO           (MCU_CLOCK_FREQ / M1_PWM_FREQ)
/* Output PWM deadtime value in nanoseconds */
#define M1_PWM_DEADTIME (500)
/* PWM vs. Fast control loop ratio */
#define M1_FOC_FREQ_VS_PWM_FREQ (1U)
/* Slow control loop frequency */
#define M1_SLOW_LOOP_FREQ       (1000U)

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
/* Configuration table of ADC channels according to the input pin signals:
 * Valid for lpc55s36 together with FRDM-MC-LVPMSM
 *
 * Motor 1
 * M1_I_A       | ADC0_CH3A
 * M1_I_B       | ADC0_CH8B
 * M1_I_C       | ADC0_CH3B
 * M1_U_DCB     | ADC0_CH1A
 *
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
 * Define motor ADC control functions
 ******************************************************************************/
#define M1_MCDRV_ADC_GET(par)	(MCDRV_CurrAndVoltDcBusGet(par))
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
#define M1_MCDRV_P2M3PH_FLT_TRY_CLR(par) (MCDRV_eFlexPwm3PhFltTryClr(par))

/******************************************************************************
 * Define position and speed sensor - quadrature encoder for motor 1
 ******************************************************************************/
#define M1_MCDRV_QD_GET(par) (MCDRV_QdEncGet(par))
#define M1_MCDRV_QD_SET_DIRECTION(par) (MCDRV_QdEncSetDirection(par))
#define M1_MCDRV_QD_SET_PULSES(par) (MCDRV_QdEncSetPulses(par))
#define M1_MCDRV_QD_CLEAR(par) (MCDRV_QdEncClear(par))

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
extern mcdrv_eflexpwm_t g_sM1Pwm3ph;
extern mcdrv_adc_t g_sM1AdcSensor;
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
