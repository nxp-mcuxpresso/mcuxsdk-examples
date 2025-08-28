/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MC_PERIPH_INIT_H_
#define _MC_PERIPH_INIT_H_


#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_xbar.h"
#include "fsl_biss.h"

#include "board.h"

#include "mcdrv_pwm3ph_pwma.h"
#include "mcdrv_bissc.h"
#include "mcdrv_endat2p2.h"
#include "mcdrv_sinc.h"
#include "m1_pmsm_appconfig.h"
#include "m2_pmsm_appconfig.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Structure used during clocks and modulo calculations */
typedef struct _clock_setup
{
    uint32_t ui32FastPeripheralClock;
    uint32_t ui32CpuFrequency;
    uint32_t ui32BusClock;
    uint32_t ui32SysPllClock;
    uint16_t ui16M1SpeedLoopFreq;   
    uint16_t ui16M1PwmFreq;
    uint16_t ui16M1PwmModulo; 
    uint16_t ui16M1PwmDeadTime;  
    uint16_t ui16M2SpeedLoopFreq;
    uint16_t ui16M2PwmFreq;
    uint16_t ui16M2PwmModulo;
    uint16_t ui16M2PwmDeadTime;
} clock_setup_t;



/******************************************************************************
 * Clock & PWM definition for motor 1
 ******************************************************************************/
#define M1_PWM_FREQ (32000)         /* PWM frequency - 32kHz */
#define M1_FOC_FREQ_VS_PWM_FREQ (1) /* FOC calculation is called every n-th PWM reload */
#define M1_SPEED_LOOP_FREQ (4000)   /* Speed loop frequency */
#define M1_PWM_DEADTIME (500)       /* Output PWM deadtime value in nanoseconds */

#define M1_FAST_LOOP_TS ((float_t)1.0 / (float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define M1_SLOW_LOOP_TS ((float_t)1.0 / (float_t)(M1_SPEED_LOOP_FREQ))
#define M1_TIME_ONESEC_COUNT (M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)

/* Fast loop frequency in Hz */
#define M1_FAST_LOOP_FREQ       (M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)

/* Over Current Fault detection */
#define M1_FAULT_NUM (0)
#define M1_FAULT_OV_NUM (1)

/* Braking resistor macros */
#define M1_BRAKE_SET()
#define M1_BRAKE_CLEAR()
/* DC bus braking threshold hysteresis */
#define M1_U_DCB_HYSTERESIS (0.05F)

/* Measure phase currents using ADC. 0 - false, 1 - true */   
#define PHASE_CUR_ADC_MEASUREMENT (0)
 
/* DC bus voltage not measured using ADC. 0 - false, 1 - true */ 
#define DCBUS_NO_MEASUREMENT (1)

#define DIG_ENCODER_MUX_HIPERFACE_DSL   0x0
#define DIG_ENCODER_MUX_ENDAT2P2        0x1
#define DIG_ENCODER_MUX_ENDAT3          0x2
#define DIG_ENCODER_MUX_BISS            0x3
   
/* Interrupt number and interrupt handler for the TPM instance used */

#define M1_ENDAT2P2_BASE           ENDAT2P2_2
#define M1_ENDAT2P2_IRQn           Reserved165_IRQn
#define M1_ENDAT2P2_IRQHandler     Reserved165_IRQHandler

#define M2_ENDAT2P2_BASE           ENDAT2P2_1
#define M2_ENDAT2P2_IRQn           Reserved163_IRQn
#define M2_ENDAT2P2_IRQHandler     Reserved163_IRQHandler


/******************************************************************************
 * Define common phase currents and DC bus measurement functions for motor 1
 ******************************************************************************/
#define M1_MCDRV_CURR_3PH_VOLT_DCB_GET(par)    /* Need to be empty - SINC data are read and processed in SINC IRQ. */

/* Keep these macros to avoid project build error. */               
#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par)      
#define M1_MCDRV_CURR_3PH_CALIB_INIT(par) 
#define M1_MCDRV_CURR_3PH_CALIB(par)
#define M1_MCDRV_CURR_3PH_CALIB_SET(par)

/******************************************************************************
 * Define slow control loop timer - common for motor 1 and motor 2
 ******************************************************************************/
#define M1_MCDRV_TMR_SLOWLOOP_INIT() InitTMR1()


/******************************************************************************
 * Define 3-ph PWM control functions for motor 1 (PWM2 instance)
 ******************************************************************************/
#define M1_MCDRV_PWM_PERIPH_INIT() (M1_InitPWM())
#define M1_MCDRV_PWM3PH_SET(par) (MCDRV_eFlexPwm3PhSet(par))
#define M1_MCDRV_PWM3PH_EN(par) (MCDRV_eFlexPwm3PhOutEn(par))
//#define M1_MCDRV_PWM3PH_DIS(par) (MCDRV_eFlexPwm3PhOutDis(par))
#define M1_MCDRV_PWM3PH_DIS(par) (MCDRV_eFlexPwm3PhOutDis_Optim(par))
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eFlexPwm3PhFltGet(par))
#define M1_MCDRV_PWM3PH_FLT_OV_GET(par) (MCDRV_eFlexPwm3PhFlt2Get(par))

/******************************************************************************
 * Define position and speed sensor for motor 1 - EnDat2.2
 ******************************************************************************/

/* Example specific position/speed sensor defines */
#define M1_MCDRV_ENDAT2P2_PERIPH_INIT() (InitEndat2p2_2())
#define M1_MCDRV_ENDAT2P2_GET(par) (MCDRV_Endat2p2DataRead(par))

/* Common position/speed sensor defines */
#define M1_MCDRV_ENC_GET_DATA_FAST(par) (MCDRV_EnDatGetPositionFoc(par))
#define M1_MCDRV_ENC_GET_DATA_SLOW(par) (MCDRV_EnDatGetPositionFullAndSpeed(par))
#define M1_MCDRV_ENC_CLEAR(par) (MCDRV_Endat2p2Clear(par))
#define M1_MCDRV_ENC_SET_OFFSET(par) (MCDRV_Endat2p2SetOffset(par))
#define M1_MCDRV_ENC_SET_DIRECTION(par)
#define M1_MCDRV_ENC_SET_PULSES(par)
#define M1_MCDRV_ENC_GET_POSITION(par)
#define M1_MCDRV_ENC_GET_SPEED(par)

/******************************************************************************
 * Define SINC filter for motor 1
 ******************************************************************************/
#define PWM_CLK_MC                      (133333333U)
#define SINC_CLK_MC                     (PWM_CLK_MC / 8U)

#define SINC1_ORD		        (3U)	// Third-order filter
#define SINC1_OSR			(63U)   // 64x oversampling (PFOSR + 1)
#define SINC1_IRQ_PRIORITY		(0U)
#define SINC1_NUM_CHANNELS_USED		(4U)
#define SINC1_PRETRIGGER_VAL            ((SINC1_ORD * (SINC1_OSR + 1U)) + ((SINC1_OSR + 1U) / 2U))      // 3*OSR + OSR/2 ... first sample (OSR*ORD)/MCLK, next samples OSR/MCLK
#define SINC1_PWM_CLK_RATION            (PWM_CLK_MC / SINC_CLK_MC)
#define M1_MCDRV_SINC_INIT()            Sinc1_Init()
#define M1_MCDRV_SINC_GET(par)          (MCDRV_SincReadPhCurrDcBusVolt(par))

/* Init sensors pointers */
#define M1_SET_PTR_U_DC_BUS(par)       (g_sM1Curr3phDcBus.pfltUDcBus = &(par))
#define M1_SET_PTR_I_ABC(par)          (g_sM1Curr3phDcBus.psIABC = &(par))
#define M1_SET_PTR_SECTOR(par)         (g_sM1Curr3phDcBus.pui16SVMSector = &(par))
#define M1_SET_PTR_AUX_CHAN(par)


/******************************************************************************
 * Define SINC filter for motor 2
 ******************************************************************************/

#define SINC2_ORD		        (3U)	// Third-order filter
#define SINC2_OSR			(63U)   // 64x oversampling (PFOSR + 1)
#define SINC2_IRQ_PRIORITY		(0U)
#define SINC2_NUM_CHANNELS_USED		(4U)
#define SINC2_PRETRIGGER_VAL            ((SINC2_ORD * (SINC2_OSR + 1U)) + ((SINC2_OSR + 1U) / 2U))      // 3*OSR + OSR/2
#define SINC2_PWM_CLK_RATION            (PWM_CLK_MC / SINC_CLK_MC)
#define M2_MCDRV_SINC_INIT()            Sinc2_Init()
#define M2_MCDRV_SINC_GET(par)          (MCDRV_SincReadPhCurrDcBusVolt(par))

/* Init sensors pointers */
#define M2_SET_PTR_U_DC_BUS(par)       (g_sM2Curr3phDcBus.pfltUDcBus = &(par))
#define M2_SET_PTR_I_ABC(par)          (g_sM2Curr3phDcBus.psIABC = &(par))
#define M2_SET_PTR_SECTOR(par)         (g_sM2Curr3phDcBus.pui16SVMSector = &(par))
#define M2_SET_PTR_AUX_CHAN(par)
   
/******************************************************************************
 * Clock & PWM definition for motor 2
 ******************************************************************************/
#define M2_PWM_FREQ (32000)         /* PWM frequency - 32kHz */
#define M2_FOC_FREQ_VS_PWM_FREQ (1) /* FOC calculation is called every n-th PWM reload */
#define M2_SPEED_LOOP_FREQ (4000)   /* Speed loop frequency */
#define M2_PWM_DEADTIME (500)       /* Output PWM deadtime value in nanoseconds */

#define M2_FAST_LOOP_TS ((float_t)1.0 / (float_t)(M2_PWM_FREQ / M2_FOC_FREQ_VS_PWM_FREQ))
#define M2_SLOW_LOOP_TS ((float_t)1.0 / (float_t)(M2_SPEED_LOOP_FREQ))
#define M2_TIME_ONESEC_COUNT (M2_PWM_FREQ / M2_FOC_FREQ_VS_PWM_FREQ)

/* Fast loop frequency in Hz */
#define M2_FAST_LOOP_FREQ       (M2_PWM_FREQ / M2_FOC_FREQ_VS_PWM_FREQ)

/* Over Current Fault detection */
#define M2_FAULT_NUM (0)
#define M2_FAULT_OV_NUM (1)

/* Braking resistor macros */
#define M2_BRAKE_SET()
#define M2_BRAKE_CLEAR()
/* DC bus braking threshold hysteresis */
#define M2_U_DCB_HYSTERESIS (0.05F)
   
/******************************************************************************
 * Define common phase currents and DC bus measurement functions for motor 1
 ******************************************************************************/
#define M2_MCDRV_CURR_3PH_VOLT_DCB_GET(par)    /* Need to be empty - SINC data are read and processed in SINC IRQ. */

/* Keep these macros to avoid project build error. */
#define M2_MCDRV_CURR_3PH_CHAN_ASSIGN(par)      
#define M2_MCDRV_CURR_3PH_CALIB_INIT(par) 
#define M2_MCDRV_CURR_3PH_CALIB(par)
#define M2_MCDRV_CURR_3PH_CALIB_SET(par)

/******************************************************************************
 * Define motor 2 3-ph PWM control functions (PWM1 instance)
 ******************************************************************************/
#define M2_MCDRV_PWM_PERIPH_INIT() (M2_InitPWM())
#define M2_MCDRV_PWM3PH_SET(par) (MCDRV_eFlexPwm3PhSet(par))
#define M2_MCDRV_PWM3PH_EN(par) (MCDRV_eFlexPwm3PhOutEn(par))
//#define M2_MCDRV_PWM3PH_DIS(par) (MCDRV_eFlexPwm3PhOutDis(par))
#define M2_MCDRV_PWM3PH_DIS(par) (MCDRV_eFlexPwm3PhOutDis_Optim(par))
#define M2_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eFlexPwm3PhFltGet(par))
#define M2_MCDRV_PWM3PH_FLT_OV_GET(par) (MCDRV_eFlexPwm3PhFlt2Get(par))
        
/******************************************************************************
 * Define position and speed sensor for motor 2 - EnDat2.2
 ******************************************************************************/

/* Example specific position/speed sensor defines */
#define M2_MCDRV_ENDAT2P2_PERIPH_INIT() (InitEndat2p2_1())
#define M2_MCDRV_ENDAT2P2_GET(par)      (MCDRV_Endat2p2DataRead(par))
     
/* Common position/speed sensor defines */
#define M2_MCDRV_ENC_GET_DATA_FAST(par) (MCDRV_EnDatGetPositionFoc(par))
#define M2_MCDRV_ENC_GET_DATA_SLOW(par) (MCDRV_EnDatGetPositionFullAndSpeed(par))
#define M2_MCDRV_ENC_CLEAR(par)         (MCDRV_Endat2p2Clear(par))
#define M2_MCDRV_ENC_SET_OFFSET(par)    (MCDRV_Endat2p2SetOffset(par)) 
#define M2_MCDRV_ENC_SET_DIRECTION(par)
#define M2_MCDRV_ENC_SET_PULSES(par)
#define M2_MCDRV_ENC_GET_POSITION(par)
#define M2_MCDRV_ENC_GET_SPEED(par)


/******************************************************************************
 * Global variable definitions
 ******************************************************************************/

extern clock_setup_t g_sClockSetup;
extern mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;
extern mcdrv_pwm3ph_pwma_t g_sM2Pwm3ph;

extern mcdrv_endat2p2_t g_sM1Enc;
extern mcdrv_endat2p2_t g_sM2Enc;

extern mcdrv_sinc_t g_sM1Curr3phDcBus;
extern mcdrv_sinc_t g_sM2Curr3phDcBus;


/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void MCDRV_Init(void);
void InitClock(void);
void InitTMR1(void);
void M1_InitPWM(void);
void M2_InitPWM(void);
void InitEndat2p2_1(void);
void InitEndat2p2_2(void);
void Sinc1_Init(void);
void Sinc2_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* _MC_PERIPH_INIT_H_  */
