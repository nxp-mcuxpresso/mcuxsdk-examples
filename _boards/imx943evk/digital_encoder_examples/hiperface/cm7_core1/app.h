/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* The PWM base address */
#define BOARD_PWM_BASEADDR PWM1

#define PWM_SRC_CLK_FREQ       HAL_ClockGetRate(hal_clock_busaon)
#define DEMO_PWM_CLOCK_DEVIDER kPWM_Prescale_Divide_128
#define APP_DEFAULT_PWM_FREQUENCE (200UL)
#define TRANSACTION_TIME_US 10U
#define ENCODER_ACCESS_FREQ_VS_PWM_FRE0 		16

#define DEMO_XBARA_BASEADDR            XBARA1
#define DEMO_XBARA_IRQn                XBAR1_CH0_CH1_IRQn
#define DEMO_XBARA_IRQHandler          XBAR1_CH0_CH1_IRQHandler

#define BOARD_HIPERFACE_BASEADDR 				HIPERFACE1
#define DEMO_HIPERFACE_IRQn						Reserved164_IRQn
#define DEMO_HIPERFACE_IRQHandler 				Reserved164_IRQHandler
#define DEMO_HIPERFACE_S_IRQn					Reserved163_IRQn
#define DEMO_HIPERFACE_S_IRQHandler				Reserved163_IRQHandler
#define DEMO_HIPERFACE_POS_RCVD_IRQn			Reserved226_IRQn
#define DEMO_HIPERFACE_POS_RCVD_IRQHandler		Reserved226_IRQHandler

#define HIPERFACE_CLOCK_ROOT       	hal_clock_hiperface1
#define HIPERFACE_SOURCE_CLOCK     	HAL_ClockGetFreq(HIPERFACE_CLOCK_ROOT)

#define ENCODER_TYPE_ROTARY

#define HIPERFACE_EVT_IRQ_ENABLE() 		BLK_CTRL_WAKEUPMIX->HIPERFACE1_INT_CTL |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_INT_CTL_hf1_evt_mask_n_MASK
#define HIPERFACE_EVT_IRQ_DISABLE() 	BLK_CTRL_WAKEUPMIX->HIPERFACE1_INT_CTL &= ~BLK_CTRL_WAKEUPMIX_HIPERFACE1_INT_CTL_hf1_evt_mask_n_MASK

#define HIPERFACE_SYNC_LOCKED_IRQ_ENABLE()      BLK_CTRL_WAKEUPMIX->HIPERFACE1_INT_CTL |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_INT_CTL_hf1_sync_locked_int_mask_n_MASK
#define HIPERFACE_SYNC_LOCKED_IRQ_DISABLE()     BLK_CTRL_WAKEUPMIX->HIPERFACE1_INT_CTL &= ~BLK_CTRL_WAKEUPMIX_HIPERFACE1_INT_CTL_hf1_sync_locked_int_mask_n_MASK

#define HIPERFACE_SAFE_POS2_IRQ_ENABLE()      BLK_CTRL_WAKEUPMIX->HIPERFACE1_INT_CTL |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_INT_CTL_hf1_safe_pos2_int_mask_n_MASK
#define HIPERFACE_SAFE_POS2_IRQ_DISABLE()      BLK_CTRL_WAKEUPMIX->HIPERFACE1_INT_CTL &= ~BLK_CTRL_WAKEUPMIX_HIPERFACE1_INT_CTL_hf1_safe_pos2_int_mask_n_MASK

#define HIPERFACE_FAST_POS_IRQ_ENABLE()       BLK_CTRL_WAKEUPMIX->HIPERFACE1_INT_CTL |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_INT_CTL_hf1_fast_pos_int_mask_n_MASK
#define HIPERFACE_FAST_POS_IRQ_ENABLE()       BLK_CTRL_WAKEUPMIX->HIPERFACE1_INT_CTL |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_INT_CTL_hf1_fast_pos_int_mask_n_MASK

#define HIPERFACE_SAFE_POS1_IRQ_ENABLE()      BLK_CTRL_WAKEUPMIX->HIPERFACE1_INT_CTL |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_INT_CTL_hf1_safe_pos1_int_mask_n_MASK
#define HIPERFACE_SAFE_POS1_IRQ_DISABLE()      BLK_CTRL_WAKEUPMIX->HIPERFACE1_INT_CTL &= ~BLK_CTRL_WAKEUPMIX_HIPERFACE1_INT_CTL_hf1_safe_pos1_int_mask_n_MASK

#define HIPERFACE_SAFE_POS1_IRQ_CLR()	BLK_CTRL_WAKEUPMIX->HIPERFACE1_INT_CTL |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_INT_CTL_hf1_safe_pos1_int_clear_MASK
#define HIPERFACE_FAST_POS_IRQ_CLR()	BLK_CTRL_WAKEUPMIX->HIPERFACE1_INT_CTL |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_INT_CTL_hf1_fast_pos_int_clear_MASK
#define HIPERFACE_SAFE_POS2_IRQ_CLR()	BLK_CTRL_WAKEUPMIX->HIPERFACE1_INT_CTL |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_INT_CTL_hf1_safe_pos2_int_clear_MASK
#define HIPERFACE_SYNC_LOCKED_IRQ_CLR()	BLK_CTRL_WAKEUPMIX->HIPERFACE1_INT_CTL |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_INT_CTL_hf1_sync_locked_int_clear_MASK

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void PWM_Trigger_Init(PWM_Type *PWMBase);
/*${prototype:end}*/

#endif /* _APP_H_ */

