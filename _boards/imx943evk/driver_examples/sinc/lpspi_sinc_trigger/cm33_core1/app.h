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
/* Master related */
#define DEMO_LPSPI_MASTER_BASEADDR   (LPSPI3)
#define DEMO_LPSPI_MASTER_IRQN       (LPSPI3_IRQn)
#define DEMO_LPSPI_MASTER_IRQHandler (LPSPI3_IRQHandler)

#define DEMO_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define DEMO_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)
#define DEMO_LPSPI_DEFAULT_VALUE           (0x7FU)

#define DEMO_LPSPI_CLOCK hal_clock_lpspi3
#define LPSPI_MASTER_CLK_FREQ HAL_ClockGetRate(DEMO_LPSPI_CLOCK)

#define DEMO_SINC                 (SINC2)
#define DEMO_SINC_IRQn            (SINC2_CH0_IRQn)
#define DEMO_SINC_IRQ_HANDLER     SINC2_CH0_IRQHandler
#define DEMO_SINC_MOD_CLK_DIVIDER (4UL)
#define DEMO_SINC_OverSampleRatio (139U)
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
#define LPTPM_CLOCK_ROOT hal_clock_tpm6
#define TPM_SOURCE_CLOCK HAL_ClockGetRate(LPTPM_CLOCK_ROOT)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/
#endif
