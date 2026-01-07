/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_clock.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_MASTER             I3C2
#define I3C_MASTER_CLOCK_ROOT      kCLOCK_I3c2slow
#define I3C_MASTER_CLOCK_FREQUENCY CLOCK_GetRate(I3C_MASTER_CLOCK_ROOT)
#define SENSOR_SLAVE_ADDR          0x49U

static inline uint64_t CLOCK_GetCoreSysClkFreq(void)
{
    return CLOCK_GetRate(kCLOCK_M33sync);
}
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
