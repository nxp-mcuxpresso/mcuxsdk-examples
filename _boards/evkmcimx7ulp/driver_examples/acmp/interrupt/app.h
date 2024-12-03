/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
//${macro:start}
#define DEMO_ACMP_BASEADDR         CMP1
#define DEMO_ACMP_MINUS_INPUT      6U
#define DEMO_ACMP_PLUS_INPUT       7U /*  Internal 8bit DAC output. */
#define DEMO_ACMP_IRQ_ID           CMP1_IRQn
#define DEMO_ACMP_IRQ_HANDLER_FUNC CMP1_IRQHandler

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
//${prototype:start}
void BOARD_InitHardware(void);
//${prototype:end}
