/*
 * Copyright 2024 NXP
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
#define EXAMPLE_MASTER             I3C2
#define I3C_MASTER_CLOCK_ROOT      hal_clock_i3c2
#define I3C_MASTER_CLOCK_FREQUENCY HAL_ClockGetRate(I3C_MASTER_CLOCK_ROOT)
#define SENSOR_SLAVE_ADDR          0x48U

static inline uint32_t CLOCK_GetCoreSysClkFreq(void)
{    return SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY;
}
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
