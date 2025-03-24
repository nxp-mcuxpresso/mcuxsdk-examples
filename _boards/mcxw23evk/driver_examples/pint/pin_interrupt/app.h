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
#define DEMO_PINT_PIN_INT0_SRC     kINPUTMUX_GpioPort0Pin18ToPintsel
#define DEMO_PINT_PIN_INT1_SRC     kINPUTMUX_GpioPort0Pin20ToPintsel
#define DEMO_PINT_PIN_INT2_SRC     kINPUTMUX_GpioPort0Pin21ToPintsel
#define DEMO_SEC_PINT_PIN_INT0_SRC kINPUTMUX_GpioPort0Pin18ToPintSecsel
#define DEMO_PIN_NUM               3
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
