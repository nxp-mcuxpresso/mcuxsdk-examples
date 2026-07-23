/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_H_
#define APP_H_

/*${header:start}*/
#include "fsl_biss.h"
#include "pin_mux.h"
#include "fsl_pwm.h"
#include "fsl_xbar.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define MOTOR_CTRL1          (1U)
#define MOTOR_CTRL2          (2U)

#define BISS_MUX                MOTOR_CTRL1
#define BISS_BASE               BISS1

/*! Number of device controls */
#define DEV_SM_NUM_CTRL  27UL

/*!
 * @name Device control indexes
 */
/** @{ */
#define DEV_SM_CTRL_PDM_CLK_SEL          0U   /*!< AON PDM clock sel */
#define DEV_SM_CTRL_MQS1_SETTINGS        1U   /*!< AON MQS settings */
#define DEV_SM_CTRL_MQS2_SETTINGS        2U   /*!< WAKE MQS settings */
#define DEV_SM_CTRL_SAI1_MCLK            3U   /*!< AON SAI1 MCLK */
#define DEV_SM_CTRL_SAI2_MCLK            4U   /*!< WAKE SAI2 MCLK */
#define DEV_SM_CTRL_SAI3_MCLK            5U   /*!< WAKE SAI3 MCLK */
#define DEV_SM_CTRL_SAI4_MCLK            6U   /*!< WAKE SAI4 MCLK */
#define DEV_SM_CTRL_ADC_TEST             7U   /*!< BBSM SNVS ADC enable */
#define DEV_SM_CTRL_GPT_MUX              8U   /*!< GPT mux */
#define DEV_SM_CTRL_XBAR_DIR_CTRL        9U   /*!< XBAR IO direction */
#define DEV_SM_CTRL_XBAR_TRIG_SYNC       10U  /*!< XBAR trigger sync ctrl1 */
#define DEV_SM_CTRL_ADC_TRIGGER          11U  /*!< ADC trigger */
#define DEV_SM_CTRL_HPF1_SYNC_SRC_CFG1   12U  /*!< Hiperface#1 sync src cfg#1 */
#define DEV_SM_CTRL_HPF1_SYNC_SRC_CFG2   13U  /*!< Hiperface#1 sync src cfg#2 */
#define DEV_SM_CTRL_HPF2_SYNC_SRC_CFG1   14U  /*!< Hiperface#2 sync src cfg#1 */
#define DEV_SM_CTRL_HPF2_SYNC_SRC_CFG2   15U  /*!< Hiperface#2 sync src cfg#2 */
#define DEV_SM_CTRL_HPF1_INTR_CTRL       16U  /*!< Hiperface#1 interrupt ctrl */
#define DEV_SM_CTRL_HPF2_INTR_CTRL       17U  /*!< Hiperface#2 interrupt ctrl */
#define DEV_SM_CTRL_ENDAT3_STATUS        18U  /*!< EnDat 3.0 status register */
#define DEV_SM_CTRL_ENC_DIAG_MUX_SEL     19U  /*!< Diagnostic bus mux sel reg */
#define DEV_SM_CTRL_HPF_SYNC_OUT_CTL     20U  /*!< Hiperface ext sync out ctrl */
#define DEV_SM_CTRL_ENDAT_STRETCH_CTRL   21U  /*!< ENDAT_STRETCHER_CTRL */
#define DEV_SM_CTRL_BISS1_PULSE_STR_CTL  22U  /*!< BISS#1 pulse stretch ctrl */
#define DEV_SM_CTRL_XBAR_TRIG_SYNC_2     23U  /*!< XBAR trigger sync ctrl#2  */
#define DEV_SM_CTRL_XBAR_TRIG_SYNC_3     24U  /*!< XBAR trigger sync ctrl#3  */
#define DEV_SM_CTRL_XBAR_TRIG_SYNC_4     25U  /*!< XBAR trigger sync ctrl#4  */
#define DEV_SM_CTRL_XBAR_DIR_CTRL_2      26U   /*!< XBAR IO direction ctrl#2 */
/** @} */

#define DIG_ENCODER_MUX_HIPERFACE_DSL   0x0
#define DIG_ENCODER_MUX_ENDAT2P2        0x1
#define DIG_ENCODER_MUX_ENDAT3          0x2
#define DIG_ENCODER_MUX_BISS            0x3

/* Interrupt number and interrupt handler for the TPM instance used.
 * Due to the safety framework, the BLK_CTRL registers only be accessed
 * by System Manager. XBAR is used to route the EOT signal to trigger
 * XBAR output interrupt.
 */

#define DEMO_XBARA_BASEADDR               kXBAR_DSC1
#define DEMO_XBARA_IRQn                   XBAR1_CH0_CH1_IRQn
#define DEMO_XBARA_IRQHandler             XBAR1_CH0_CH1_IRQHandler
#define DEMO_XBARA_IRQ_OUTPIT_SIGNAL      kXBAR1_OutputEdma4IpdReq76

#define BISS_IRQn               DEMO_XBARA_IRQn
#define BISS_IRQHandler         DEMO_XBARA_IRQHandler

#define BISS_SYS_CLK_ROOT       kCLOCK_Biss
#define BISS_SYS_CLK_FREQ       20000000UL /* 20MHz */
#define BISS_MA_CLK_FREQ        200000  /* 200KHz */
#define BISS_AGS_CLK_FREQ       1000   /* 1KHz */

#define BISS_DEVICE_WR_ER_LEN   2
#define BISS_DEVICE_MT_LEN      12
#define BISS_DEVICE_ST_LEN      16
#define BISS_DEVICE_DATA_LEN    (BISS_DEVICE_MT_LEN + BISS_DEVICE_ST_LEN + BISS_DEVICE_WR_ER_LEN)
#define BISS_DEVICE_CRC_LEN     6

/* The PWM base address */
#define BOARD_PWM_BASEADDR              PWM1
#define PWM_SRC_CLK_FREQ                CLOCK_GetRate(kCLOCK_Busaon)
#define DEMO_PWM_CLOCK_DEVIDER          kPWM_Prescale_Divide_128
#define APP_DEFAULT_PWM_FREQUENCE       (16UL)
#define TRANSACTION_TIME_US             10U
#define ENCODER_ACCESS_FREQ_VS_PWM_FRE0 16

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void PWM_Trigger_Init(PWM_Type *PWMBase);
void BISS_IRQ_Clear(void);
/*${prototype:end}*/

#endif /* APP_H_ */
