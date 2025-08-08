/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_FLEXIO_BASE      FLEXIO1

#define FLEXIO_A_FORMAT_DR_PIN    1U
#define FLEXIO_A_FORMAT_TX_PIN    2U
#define FLEXIO_A_FORMAT_RX_PIN    3U

#define A_FORMAT_TX_SHIFTER_INDEX 0U
#define A_FORMAT_RX_SHIFTER_INDEX 1U
#define A_FORMAT_DR_TIMER_INDEX   0U
#define A_FORMAT_TX_TIMER_INDEX   1U
#define A_FORMAT_RX_TIMER_INDEX   2U
#define DEMO_FLEXIO_CLOCK         (kCLOCK_Flexio1)
#define FLEXIO_CLOCK_FREQUENCY    CLOCK_GetRate(DEMO_FLEXIO_CLOCK)

/* define instance */
#define BOARD_TPM_BASEADDR           TPM6
#define BOARD_TPM_CHANNEL            kTPM_Chnl_0

/* Interrupt to enable; depends on the TPM channel used */
#define BOARD_TPM_INTERRUPT_ENABLE   kTPM_TimeOverflowInterruptEnable

/* Get source clock for TPM driver */
#define LPTPM_CLOCK_ROOT             kCLOCK_Tpm6
#define BOARD_TPM_SOURCE_CLOCK       CLOCK_GetRate(LPTPM_CLOCK_ROOT)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
