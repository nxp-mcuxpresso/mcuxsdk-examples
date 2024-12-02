/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define APP_DEBUG_UART_BAUDRATE       (115200U)             /* Debug console baud rate. */
#define APP_DEBUG_UART_DEFAULT_CLKSRC kCLOCK_IpSrcSircAsync /* SCG SIRC clock. */

/* LPTMR0 is LLWU internal module 0. */
#define SYSTICK_LLWU_MODULE (0U)
/* Allow systick to be a wakeup source in VLLS. */
#define SYSTICK_LLWU_WAKEUP (false)

/* Enable GPIO PAD low power operation.
 * NOTE: ONLY ON EVK BOARD CAN THIS OPTION SET TO 1.
 * Rational: i.MX7ULP supports voltage detection circuitry on pads which results in additional power consumption.
 *           We can disable this feature to save power, but it may cause malfunction or even SoC pad damage.
 *           Please read "GPIO pads operating range configuration" in Reference Manual SIM module carefully
 *           before turn APP_ENABLE_GPIO_PAD_LOW_POWER to "1". If there's any change on board, the pad range setting
 *           code need to be modified accordingly.
 */
#define APP_ENABLE_GPIO_PAD_LOW_POWER (1)

#define APP_LPTMR1_IRQ_PRIO (5U)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/* Set the clock configuration for HSRUN mode. */
void APP_SetClockHsrun(void);
/* Set the clock configuration for RUN mode from HSRUN mode. */
void APP_SetClockRunFromHsrun(void);
/* Set the clock configuration for RUN mode from VLPR mode. */
void APP_SetClockRunFromVlpr(void);
/* Set the clock configuration for VLPR mode. */
void APP_SetClockVlpr(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
