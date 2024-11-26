/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define RPMSG_LITE_LINK_ID            (RL_PLATFORM_IMX943_M70_M71_USER_LINK_ID)
/* M71: Master M70: Slave M7_1 DTCM as shared memory */
#define RPMSG_LITE_SHMEM_BASE         (0x20030000)
#define RPMSG_LITE_NS_ANNOUNCE_STRING "rpmsg-openamp-demo-channel"

#define APP_DEBUG_UART_BAUDRATE (115200U) /* Debug console baud rate. */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
