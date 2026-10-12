/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_PINT_PIN_INT0_SRC     kINPUTMUX_IoPPadIn0_20ToPINTSELn
#define DEMO_SEC_PINT_PIN_INT0_SRC kINPUTMUX_IoPPadIn0_20ToPINTSECSELn
#define DEMO_PIN_NUM               1
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
