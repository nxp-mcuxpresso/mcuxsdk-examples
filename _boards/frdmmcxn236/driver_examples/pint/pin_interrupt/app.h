/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_PINT_PIN_INT0_SRC kINPUTMUX_GpioPort0Pin6ToPintsel
#define DEMO_PINT_PIN_INT1_SRC kINPUTMUX_GpioPort0Pin20ToPintsel
#define DEMO_PIN_NUM           2
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
