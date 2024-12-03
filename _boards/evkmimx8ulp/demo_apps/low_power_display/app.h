/*
 * Copyright 2023 NXP
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

/* LPTMR0 is WUU internal module 0. */
#define WUU_MODULE_SYSTICK WUU_MODULE_LPTMR0
/* Allow systick to be a wakeup source in Power Down mode. */
#define SYSTICK_WUU_WAKEUP_EVENT (kWUU_InternalModuleInterrupt)

#define APP_LPTMR1_IRQ_PRIO (5U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t BOARD_InitPsRam_Fro(void);
void BOARD_SwitchToFRO_ForSleep(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
