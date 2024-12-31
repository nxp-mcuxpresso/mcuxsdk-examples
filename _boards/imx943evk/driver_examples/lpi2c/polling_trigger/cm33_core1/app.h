/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "hal_clock.h"
/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define LPI2C_MASTER_CLOCK_ROOT  hal_clock_lpi2c4
#define LPI2C_MASTER_CLOCK_FREQUENCY    HAL_ClockGetRate(LPI2C_MASTER_CLOCK_ROOT)
#define EXAMPLE_I2C_MASTER_BASE LPI2C4

#define LPI2C_SLAVE_CLOCK_ROOT  hal_clock_lpi2c6
#define LPI2C_SLAVE_CLOCK_FREQUENCY    HAL_ClockGetRate(LPI2C_MASTER_CLOCK_ROOT)
#define EXAMPLE_I2C_SLAVE_BASE LPI2C6

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
#define LPTPM_CLOCK_ROOT hal_clock_tpm6
#define TPM_SOURCE_CLOCK HAL_ClockGetRate(LPTPM_CLOCK_ROOT)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
