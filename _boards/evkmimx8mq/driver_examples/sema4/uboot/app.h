/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_SEMA4                    SEMA4
#define APP_PROC_NUM                 1
#define APP_OTHER_PROC_NUM           0
#define APP_SEMA4_GATE               0
#define APP_SEMA4_GATE_ADDR          ((uint32_t)(&(APP_SEMA4->Gate00)))
#define SEMA4_IRQHandler             HS_CP1_IRQHandler
#define SOC_EnableSEMA4Intterrupt()  NVIC_EnableIRQ(HS_CP1_IRQn)
#define SOC_DisableSEMA4Intterrupt() NVIC_DisableIRQ(HS_CP1_IRQn)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/
