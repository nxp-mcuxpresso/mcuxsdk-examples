/*
    * Copyright 2024 NXP
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
#include "mcdrv_cmt_eflexpwm_lpc55s36.h"

/******************************************************************************
 * Timing
 ******************************************************************************/
#define M1_PWM_FREQ (20000U)         /* PWM frequency - 20kHz */
#define M1_FOC_FREQ_VS_PWM_FREQ (1U) /* FOC calculation is called every n-th PWM reload */
#define M1_SPEED_LOOP_FREQ (1000U)   /* Speed loop frequency */
#define M1_PWM_DEADTIME (500U)       /* Output PWM deadtime value in nanoseconds */
#define M1_SLOW_LOOP_FREQ (M1_SPEED_LOOP_FREQ)

/* Over Current Fault detection */
#define M1_FAULT_NUM (0)

/* Array with swap and mask constants */
extern const uint16_t bldcCommutationTableComp[16];

/******************************************************************************
 * ADC measurement definition
 ******************************************************************************/
/* Configuration table of ADC channels according to the input pin signals:
 * Valid for lpc55s36 together with FRDM-MC-LVBLDC
 *
 * Motor 2
 * M2_BEMF_A    | ADC1_CH3A
 * M2_BEMF_B    | ADC1_CH2A/2B
 * M2_BEMF_C    | ADC1_CH3B
 * M2_U_DCB     | ADC1_CH1A
 * M2_I_DCB     | ADC1_CH1B
 */

/* Phase BEMF A assigned to ADC1 */   
#define M1_ADC1_PH_A_CHNL (3U)
#define M1_ADC1_PH_A_SIDE (0U)

/* Phase BEMF B assigned to ADC1 */
#define M1_ADC1_PH_B_CHNL (2U)
#define M1_ADC1_PH_B_SIDE (0U)
//#define M1_ADC2_PH_B_SIDE (1)

/* Phase BEMF C assigned to ADC1 */
#define M1_ADC1_PH_C_CHNL (3U)
#define M1_ADC1_PH_C_SIDE (1U)

/* Phase voltage UDCB assigned to ADC1 */
#define M1_ADC1_UDCB_CHNL (1U)
#define M1_ADC1_UDCB_SIDE (0U)

/* Phase current IDCB assigned to ADC1 */
#define M1_ADC1_IDCB_CHNL (1U)
#define M1_ADC1_IDCB_SIDE (1U)
   
/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * Define motor ADC control functions
 ******************************************************************************/
#define M1_MCDRV_ADC_PERIPH_INIT() (InitADC())
#define M1_MCDRV_ADC_GET(par) MCDRV_CurrAndVoltDcBusGet(par)
        
#define M1_MCDRV_ADC_ASSIGN_BEMF(par)       
#define M1_MCDRV_CURR_CALIB_INIT(par) MCDRV_CurrOffsetCalibInit(par)
#define M1_MCDRV_CURR_CALIB(par) MCDRV_CurrOffsetCalib(par)
#define M1_MCDRV_CURR_CALIB_SET(par) MCDRV_CurrOffsetCalibSet(par)

/******************************************************************************
 * Define motor 3-ph PWM control functions
 ******************************************************************************/
#define M1_MCDRV_PWM_PERIPH_INIT() (M1_InitPWM())
#define M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(par1, par2) MCDRV_eFlexSetPwmOutput(par1, par2)
#define M1_MCDRV_PWM3PH_SET_DUTY(par1, par2) MCDRV_eFlexSetDutyCycle(par1, par2)
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eFlexPwm3PhFltGet(par))

/******************************************************************************
 * Define motor 1 asynchronous time event functions
 ******************************************************************************/
#define M1_MCDRV_TMR_CMT_PERIPH_INIT() InitCMT()
#define M1_MCDRV_TMR_CMT_SET(par1, par2) MCDRV_CmtSet(par1, par2)
#define M1_MCDRV_TMR_CMT_GET(par) MCDRV_CmtGet(par)

/******************************************************************************
 * Global typedefs
 ******************************************************************************/
/* Structure used during clocks and modulo calculations */
typedef struct _clock_setup
{
    uint32_t ui32FastPeripheralClock;
    uint16_t ui16M1SpeedLoopFreq;
    uint16_t ui16M1PwmFreq;
    uint16_t ui16M1PwmModulo;
    uint16_t ui16M1PwmDeadTime;
    uint32_t ui32CmtTimerFreq;
} clock_setup_t;

/******************************************************************************
 * Global variable definitions
 ******************************************************************************/
extern mcdrv_eflexpwm_t g_sM1Pwm3ph;
extern mcdrv_adc_t g_sM1AdcSensor;
extern mcdrv_cmt_pwma_t g_sM1CmtTmr;
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
