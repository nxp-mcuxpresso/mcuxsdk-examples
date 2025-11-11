/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MC_PERIPH_INIT_H_
#define _MC_PERIPH_INIT_H_


/* Select below by macro USE_ENCODER which encoder will be used. LVPMSM-FA board setting: 
 * EnDat3
 - jumpers: J72 1-3, J72 2-4, J73 1-2, J300 1-2, others as default
 - SW30[1:4] - [OFF OFF ON OFF]
 - SW90[1:4] - [OFF ON OFF ON]
 * EnDat2.2
 - jumpers: J300 1-2, others as default
 - SW30[1:4] - [OFF OFF ON OFF]
 - SW90[1:4] - [OFF ON OFF ON]
 * BiSS
 - jumpers: J300 1-2, others as default
 - SW30[1:4] - [OFF ON OFF OFF]
 - SW90[1:4] - [ON OFF OFF ON]
*/
#define USE_ENCODER_ENDAT3     (1U)    /* EnDat3 encoder */
#define USE_ENCODER_ENDAT2P2   (2U)    /* EnDat2.2 encoder */
#define USE_ENCODER_BISS       (3U)    /* BiSS encoder */

#define USE_ENCODER     (USE_ENCODER_ENDAT3)    /* Select which encoder will be used. */

#include "fsl_common.h"
#include "fsl_xbar.h"
#include "fsl_clock.h"
#include "fsl_endat3.h"
#include "fsl_endat2p2.h"
#include "fsl_biss.h"

#include "board.h"

#include "mcdrv_pwm3ph_pwma.h"
#include "mcdrv_endat3.h"
#include "mcdrv_endat2p2.h"
#include "mcdrv_bissc.h"
#include "mcdrv_sinc.h"

#if (USE_ENCODER == USE_ENCODER_BISS)
#include "m2_pmsm_appconfig.h"          /* In the demo example, a different motor is used with the BiSS encoder setup. */
#else
#include "m1_pmsm_appconfig.h"
#endif

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
} clock_setup_t;


/******************************************************************************
 * Clock & PWM definition for motor 1
 ******************************************************************************/
#define M1_PWM_FREQ (32000)         /* PWM frequency - 32kHz */
#define M1_FOC_FREQ_VS_PWM_FREQ (2) /* FOC calculation is called every n-th PWM reload */
#define M1_SPEED_LOOP_FREQ (4000)   /* Speed loop frequency */
#define M1_PWM_DEADTIME (500)       /* Output PWM deadtime value in nanoseconds */

/* Over Current Fault detection */
#define M1_FAULT_NUM (0)
#define M1_FAULT_OV_NUM (1)

/* Braking resistor macros */
#define M1_BRAKE_SET()
#define M1_BRAKE_CLEAR()
/* DC bus braking threshold hysteresis */
#define M1_U_DCB_HYSTERESIS (0.05F)


/******************************************************************************
 * Define common phase currents and DC bus measurement functions for motor 1
 ******************************************************************************/
#define M1_MCDRV_CURR_3PH_VOLT_DCB_GET(par)    /* Need to be empty - SINC data are read and processed in SINC IRQ. */

#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par)      /* Keep this macro to avoid project build error (macro is used in MID). */
#define M1_MCDRV_CURR_3PH_CALIB_INIT(par) 
#define M1_MCDRV_CURR_3PH_CALIB(par)
#define M1_MCDRV_CURR_3PH_CALIB_SET(par)

/******************************************************************************
 * Define motor 1 slow control loop timer
 ******************************************************************************/
#define M1_MCDRV_TMR_SLOWLOOP_INIT() InitTMR1()


/******************************************************************************
 * Define 3-ph PWM control functions for motor 1 (PWM2 instance)
 ******************************************************************************/
#define M1_MCDRV_PWM_PERIPH_INIT() (M1_InitPWM())
#define M1_MCDRV_PWM3PH_SET(par) (MCDRV_eFlexPwm3PhSet(par))
#define M1_MCDRV_PWM3PH_EN(par) (MCDRV_eFlexPwm3PhOutEn(par))
#define M1_MCDRV_PWM3PH_DIS(par) (MCDRV_eFlexPwm3PhOutDis(par))
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eFlexPwm3PhFltGet(par))
#define M1_MCDRV_PWM3PH_FLT_OV_GET(par) (MCDRV_eFlexPwm3PhFlt2Get(par))
        

/******************************************************************************
 * Define SINC filter
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
 * Define position and speed sensor for motor 1 - EnDat2.2, EnDat3, BiSS
 ******************************************************************************/
#define DIG_ENCODER_MUX_HIPERFACE_DSL   0x0
#define DIG_ENCODER_MUX_ENDAT2P2        0x1
#define DIG_ENCODER_MUX_ENDAT3          0x2
#define DIG_ENCODER_MUX_BISS            0x3

/* Define for EnDat3 encoder */
#if (USE_ENCODER == USE_ENCODER_ENDAT3)
#define ENDAT3_SOURCE_CLOCK     CLOCK_GetRate(kCLOCK_Endat31fast)
#define ENDAT3_STRETCHER_CTRL_ASYNC_EN 1
#define ENDAT3_STRETCHER_CTRL_HW_STROBE_COUNTER  7
#define ENDAT3_FG_IRQn     Reserved164_IRQn
#define ENCODER_IRQHandler Reserved164_IRQHandler

/* Example specific position/speed sensor defines */
#define M1_MCDRV_ENCODER_PERIPH_INIT() InitEndat3()
#define M1_MCDRV_ENCODER_GET(par)      (MCDRV_Endat3DataRead(par))

/* Common position/speed sensor defines */
#define M1_MCDRV_ENC_GET_DATA_FAST(par) (MCDRV_EnDat3GetPositionFoc(par))
#define M1_MCDRV_ENC_GET_DATA_SLOW(par) (MCDRV_EnDat3GetPositionFullAndSpeed(par))
#define M1_MCDRV_ENC_CLEAR(par)         (MCDRV_Endat3Clear(par))
#define M1_MCDRV_ENC_SET_OFFSET(par)    (MCDRV_Endat3SetOffset(par))
#endif  /* EnDat3 encoder is used. */


/* Define for EnDat2.2 encoder - motor connector 1 */
#if (USE_ENCODER == USE_ENCODER_ENDAT2P2)
#define ENDAT2P2_IRQn           Reserved165_IRQn
#define ENCODER_IRQHandler      Reserved165_IRQHandler
   
/* Example specific position/speed sensor defines */
#define M1_MCDRV_ENCODER_PERIPH_INIT() InitEndat2p2()
#define M1_MCDRV_ENCODER_GET(par) (MCDRV_Endat2p2DataRead(par))

/* Common position/speed sensor defines */
#define M1_MCDRV_ENC_GET_DATA_FAST(par) (MCDRV_EnDat2p2GetPositionFoc(par))
#define M1_MCDRV_ENC_GET_DATA_SLOW(par) (MCDRV_EnDat2p2GetPositionFullAndSpeed(par))
#define M1_MCDRV_ENC_CLEAR(par)         (MCDRV_Endat2p2Clear(par))
#define M1_MCDRV_ENC_SET_OFFSET(par)    (MCDRV_Endat2p2SetOffset(par))

#endif  /* EnDat2.2 encoder is used. */


/* Define for BiSS encoder - motor connector 1 */
#if (USE_ENCODER == USE_ENCODER_BISS)
/* BLK_CTRL_WAKEUPMIX registers should not be accessible by CM7 core.
 * BiSS EOT is routed to an XBAR input and used to trigger and XBAR output interrupt.
 */
#define XBAR1_IRQn              XBAR1_CH0_CH1_IRQn
#define ENCODER_IRQHandler      XBAR1_CH0_CH1_IRQHandler
   
/* Example specific position/speed sensor defines */
#define M1_MCDRV_ENCODER_PERIPH_INIT() InitBiSS1()
#define M1_MCDRV_ENCODER_GET(par) (MCDRV_BissCDataRead(par))

/* Common position/speed sensor defines */
#define M1_MCDRV_ENC_GET_DATA_FAST(par) (MCDRV_BissCGetPositionFoc(par))
#define M1_MCDRV_ENC_GET_DATA_SLOW(par) (MCDRV_BissCGetPositionFullAndSpeed(par))
#define M1_MCDRV_ENC_CLEAR(par)         (MCDRV_BissCClear(par))
#define M1_MCDRV_ENC_SET_OFFSET(par)    (MCDRV_BissCSetOffset(par))

#define BISS_SYS_CLK_ROOT       kCLOCK_Biss
#define BISS_SYS_CLK_FREQ       20000000 /* 20MHz */
#define BISS_MA_CLK_FREQ        10000000  /* 10MHz */
#define BISS_AGS_CLK_FREQ       100000   /* 100KHz */
#define BISS_DEVICE_WR_ER_LEN   2
#define BISS_DEVICE_MT_LEN      12
#define BISS_DEVICE_ST_LEN      16
#define BISS_DEVICE_DATA_LEN    (BISS_DEVICE_MT_LEN + BISS_DEVICE_ST_LEN + BISS_DEVICE_WR_ER_LEN)
#define BISS_DEVICE_CRC_LEN     6

#endif  /* BiSS encoder is used. */

#define M1_MCDRV_ENC_SET_DIRECTION(par)
#define M1_MCDRV_ENC_SET_PULSES(par)
#define M1_MCDRV_ENC_GET_POSITION(par)
#define M1_MCDRV_ENC_GET_SPEED(par)


/******************************************************************************
 * Global variable definitions
 ******************************************************************************/
extern mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;
extern clock_setup_t g_sClockSetup;
extern mcdrv_sinc_t g_sM1Curr3phDcBus;

#if (USE_ENCODER == USE_ENCODER_ENDAT3)
extern mcdrv_endat3_t g_sM1Enc;
#endif

#if (USE_ENCODER == USE_ENCODER_ENDAT2P2)
extern mcdrv_endat2p2_t g_sM1Enc;
#endif

#if (USE_ENCODER == USE_ENCODER_BISS)
extern BISSC_Type g_sM1Enc;
#endif

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
void Sinc1_Init(void);
void InitEndat3(void);
void InitEndat2p2(void);
void InitBiSS1(void);


#ifdef __cplusplus
}
#endif

#endif /* _MC_PERIPH_INIT_H_  */
