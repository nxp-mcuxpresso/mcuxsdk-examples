/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define LIN_CLOCK_NAME kCLOCK_Lpuart0
#define TJA_WAKEUP     0
#define TIMER_TPM      1
/* Whether to disable the PrintBuffer function */
#define DISABLE_PRINT_BUFFER 0

#define DEMO_TPM_BASEADDR   TPM_0
#define DEMO_TPM_IRQn       TPM_0_IRQn
#define DEMO_TPM_IRQHandler TPM_0_IRQHandler
#define DEMO_TPM_CLOCK      kCLOCK_Tpm0
#define DEMO_TPM_CH_OUT_NUM 0
#define DEMO_TPM_CH_OUT     kTPM_Chnl_0
#define DEMO_TPM_CH_IN      kTPM_Chnl_3
#define DEMO_TPM_CH_OUT_FLG kTPM_Chnl0Flag
#define DEMO_TPM_CH_IN_FLG  kTPM_Chnl3Flag
#define DEMO_TPM_CH_OUT_IRQ kTPM_Chnl0InterruptEnable
#define DEMO_TPM_CH_IN_IRQ  kTPM_Chnl3InterruptEnable
/* timer frequency */
#define TIMER_FREQ CLOCK_GetIpFreq(DEMO_TPM_CLOCK)
/* (timer period (us) * (timer clock frequency)(Hz)) - 1 ) */
#define MODULO_VALUE ((500U * (CLOCK_GetIpFreq(DEMO_TPM_CLOCK) / 1000000U)) - 1U)
/* nanoseconds / timer clock frequency  */
#define TIMER_1TICK_DURATION_PS (1000000000000U / TIMER_FREQ)

#define DEMO_LIN_IRQn       LPUART0_IRQn
#define DEMO_LIN_IRQHandler LPUART0_IRQHandler

#define LI0_Slave 0x0

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
