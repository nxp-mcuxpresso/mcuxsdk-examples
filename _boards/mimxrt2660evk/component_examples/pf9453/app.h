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
/* PF9453 is on HSP LPI2C1 (SDA = PIO2_24, SCL = PIO2_25) on the MIMXRT2660-EVK. */
#define DEMO_PF9453_LPI2C             HSP__LPI2C_1
#define DEMO_PF9453_LPI2C_CLKSRC_FREQ (CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpi2c1_fclk))
#define DEMO_PF9453_LPI2C_BAUDRATE    (100000U)
/* 7-bit slave address for the default OTP configuration on this board. */
#define DEMO_PF9453_I2C_ADDR          (0x32U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
