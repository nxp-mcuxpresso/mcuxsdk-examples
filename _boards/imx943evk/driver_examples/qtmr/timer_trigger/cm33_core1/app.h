/*
 * Copyright 2025 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* The QTMR instance/channel used for board */
#define BOARD_QTMR_BASEADDR       TMR1
#define BOARD_FIRST_QTMR_CHANNEL  kQTMR_Channel_0
#define BOARD_SECOND_QTMR_CHANNEL kQTMR_Channel_1
#define QTMR_ClockCounterOutput   kQTMR_ClockCounter0Output

/* Interrupt number and interrupt handler for the QTMR instance used */
#define QTMR_IRQ_ID      TMR1_IRQn
#define QTMR_IRQ_HANDLER TMR1_IRQHandler

/* QTMR Clock source divider for Ipg clock source, the value of two macros below should be aligned. */
#define QTMR_PRIMARY_SOURCE       (kQTMR_ClockDivide_128)
#define QTMR_CLOCK_SOURCE_DIVIDER (128U)

/* Get source clock for QTMR driver */
#define QTMR_SOURCE_CLOCK (CLOCK_GetRate(kCLOCK_Buswakeup) / QTMR_CLOCK_SOURCE_DIVIDER)
/*${macro:end}*/

/* define instance */
#define BOARD_TPM_BASEADDR TPM6
#define BOARD_TPM_CHANNEL  kTPM_Chnl_0

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl0InterruptEnable
#define TPM_CHANNEL_FLAG             kTPM_Chnl0Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM6_IRQn
#define TPM_LED_HANDLER      TPM6_IRQHandler

/* Get source clock for TPM driver */
#define LPTPM_CLOCK_ROOT kCLOCK_Tpm6
#define TPM_SOURCE_CLOCK CLOCK_GetRate(LPTPM_CLOCK_ROOT)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
