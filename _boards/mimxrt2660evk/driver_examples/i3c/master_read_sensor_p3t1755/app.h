/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_MASTER             HSP__I3C
#define SENSOR_SLAVE_ADDR          0x48U
#define I3C_MASTER_CLOCK_FREQUENCY (CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_i3c0_fclk))
#define CLOCK_GetCoreSysClkFreq()  (SystemCoreClock)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
