/*
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef __NCP_INTF_UART_H__
#define __NCP_INTF_UART_H__

#include "pin_mux.h"
#include "fsl_os_abstraction.h"
#include "fsl_os_abstraction_free_rtos.h"
#include "ncp_tlv_adapter.h"
#include "ncp_intf_pm.h"
#include "ncp_pm.h"
#include "ncp_log.h"

/*******************************************************************************
 * Common Definitions
 ******************************************************************************/

#define NCP_UART_BAUDRATE          3000000U

#if (NCP_UART_BAUDRATE > 115200U)
#define NCP_UART_IS_HIGH_BAUD      1
#else
#define NCP_UART_IS_HIGH_BAUD      0
#endif

#define NCP_UART_TASK_PRIORITY    (PRIORITY_RTOS_TO_OSA(((configMAX_PRIORITIES - CONFIG_WIFI_TASK_PRIORITY_OFFSET) - 3)))
#if CONFIG_NCP_USE_ENCRYPT
#define NCP_UART_TASK_STACK_SIZE  4096
#else
#define NCP_UART_TASK_STACK_SIZE  1024
#endif

/* Event bit definitions */
#define UART_EVENT_TX_DONE          (1U << 0)
#define UART_EVENT_TX_ERROR         (1U << 1)
#define UART_EVENT_RX_HEADER        (1U << 2)
#define UART_EVENT_RX_DONE          (1U << 3)
#define UART_EVENT_RX_ERROR         (1U << 4)
#define UART_EVENT_RX_FRAME_ERROR   (1U << 5)

#define UART_EVENT_TX_MASK      (UART_EVENT_TX_DONE | UART_EVENT_TX_ERROR)
#define UART_EVENT_RX_MASK      (UART_EVENT_RX_DONE | UART_EVENT_RX_ERROR | UART_EVENT_RX_HEADER | UART_EVENT_RX_FRAME_ERROR)

#if (CONFIG_NCP_DEBUG)
#define NCP_UART_STATS_INC(x) NCP_STATS_INC(intf.x)
#else
#define NCP_UART_STATS_INC(x)
#endif

typedef enum {
    RX_STATE_IDLE,
    RX_STATE_HEADER,
    RX_STATE_PAYLOAD,
} rx_state_t;

#endif /* __NCP_INTF_UART_H__ */
