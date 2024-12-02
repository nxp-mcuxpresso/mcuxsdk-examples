/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define LIN_CLOCK_NAME kCLOCK_ScgSircClk
#define TJA_WAKEUP     1
#define TIMER_TPM      1
/* Whether to disable the PrintBuffer function */
#define DISABLE_PRINT_BUFFER 0

#define DEMO_TPM_BASEADDR   TPM0
#define DEMO_TPM_IRQn       TPM0_IRQn
#define DEMO_TPM_IRQHandler TPM0_IRQHandler
#define DEMO_TPM_CLOCK      kCLOCK_Tpm0
#define DEMO_TPM_CH_OUT_NUM 0
#define DEMO_TPM_CH_OUT     kTPM_Chnl_0
#define DEMO_TPM_CH_IN      kTPM_Chnl_3
#define DEMO_TPM_CH_OUT_FLG kTPM_Chnl0Flag
#define DEMO_TPM_CH_IN_FLG  kTPM_Chnl3Flag
#define DEMO_TPM_CH_OUT_IRQ kTPM_Chnl0InterruptEnable
#define DEMO_TPM_CH_IN_IRQ  kTPM_Chnl3InterruptEnable
/* timer frequency */
#define TIMER_FREQ CLOCK_GetFreq(kCLOCK_ScgSircClk)
/* (timer period (us) * (timer clock frequency)(Hz)) - 1 ) */
#define MODULO_VALUE ((500U * (CLOCK_GetFreq(kCLOCK_ScgSircClk) / 1000000U)) - 1U)
/* nanoseconds / timer clock frequency  */
#define TIMER_1TICK_DURATION_PS (1000000000000U / TIMER_FREQ)

#define DEMO_LIN_IRQn       LPUART0_IRQn
#define DEMO_LIN_IRQHandler LPUART0_IRQHandler

#define DEMO_SLPN_GPIO GPIOC
#define DEMO_SLPN_PIN  7
#define LI0_Slave      0x0

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
