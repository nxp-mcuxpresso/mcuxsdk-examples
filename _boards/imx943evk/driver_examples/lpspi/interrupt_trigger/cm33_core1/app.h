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
#define EXAMPLE_LPSPI_MASTER_BASEADDR   (LPSPI8)
#define EXAMPLE_LPSPI_MASTER_IRQN       (LPSPI8_IRQn)
#define EXAMPLE_LPSPI_MASTER_IRQHandler (LPSPI8_IRQHandler)

#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

#define EXAMPLE_LPSPI_MASTER_CLK_ID (kCLOCK_Lpspi8)
/* Slave related */
#define EXAMPLE_LPSPI_SLAVE_BASEADDR   (LPSPI3)
#define EXAMPLE_LPSPI_SLAVE_IRQN       (LPSPI3_IRQn)
#define EXAMPLE_LPSPI_SLAVE_IRQHandler (LPSPI3_IRQHandler)

#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)

#define EXAMPLE_LPSPI_SLAVE_CLK_ID (kCLOCK_Lpspi3)

#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ CLOCK_GetRate(EXAMPLE_LPSPI_MASTER_CLK_ID) 
#define EXAMPLE_LPSPI_SLAVE_CLOCK_FREQ  CLOCK_GetRate(EXAMPLE_LPSPI_SLAVE_CLK_ID)

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

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
