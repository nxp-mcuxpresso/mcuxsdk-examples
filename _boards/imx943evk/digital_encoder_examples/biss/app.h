/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_H_
#define APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define BISS_BASE               BISS1

/* Interrupt number and interrupt handler for the TPM instance used */
#define BISS_SOT_IRQn           Reserved165_IRQn
#define BISS_SOT_IRQHandler     Reserved165_IRQHandler
#define BISS_EOT_IRQn           Reserved166_IRQn
#define BISS_EOT_IRQHandler     Reserved166_IRQHandler

#define BISS_SYS_CLK_FREQ       20000000 /* 20MHz */
#define BISS_MA_CLK_FREQ        5000000  /* 5MHz */
#define BISS_AGS_CLK_FREQ       100000   /* 100KHz */

#define BISS_DEVICE_WR_ER_LEN   2
#define BISS_DEVICE_MT_LEN      12
#define BISS_DEVICE_ST_LEN      12
#define BISS_DEVICE_DATA_LEN    (BISS_DEVICE_MT_LEN + BISS_DEVICE_ST_LEN + BISS_DEVICE_WR_ER_LEN)
#define BISS_DEVICE_CRC_LEN     6

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* APP_H_ */
