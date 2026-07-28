/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef NETC_SWITCH_H
#define NETC_SWITCH_H

#include <stdint.h>
#include <stdbool.h>
#include "fsl_common.h"
#include "fsl_netc_endpoint.h"

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Initialize the NETC switch
 *
 * @return status_t kStatus_Success if successful, error code otherwise
 */
status_t APP_SWT_Init(void);

/**
 * @brief Enable or disable BPDU trapping to management port
 *
 * @param enable true to enable BPDU trapping, false to disable
 */
void netc_sw_trap_bpdu(bool enable);

/**
 * @brief Transmit a frame through the specified switch port
 *
 * @param portid Port index (0-based)
 * @param data Pointer to frame data
 * @param len Length of frame data
 * @return 0 if successful, negative error code otherwise
 */
status_t netc_sw_transmit(uint8_t portid, uint8_t *data, uint16_t len);

/**
 * @brief Receive a frame from the switch
 *
 * @param rx_buffer Buffer to store received frame
 * @param length Pointer to store frame length
 * @param portid Pointer to store source port index
 * @return status_t kStatus_Success if successful, error code otherwise
 */
status_t APP_SWT_ReceiveFrame(uint8_t *rx_buffer, uint32_t *length, uint8_t *portid);

/**
 * @brief Enable or disable learning on a switch port for STP
 *
 * @param portIdx Port index (0-based)
 * @param enable true to enable learning, false to disable
 */
void netc_sw_stp_port_learning(uint8_t portIdx, bool enable);

/**
 * @brief Enable or disable forwarding on a switch port for STP
 *
 * @param portIdx Port index (0-based)
 * @param enable true to enable forwarding, false to disable
 */
void netc_sw_stp_port_forwarding(uint8_t portIdx, bool enable);

/**
 * @brief Flush FDB entries for a specific port
 *
 * @param portIdx Port index (0-based)
 */
void netc_sw_port_fdb_flush(uint8_t portIdx);

/**
 * @brief Create the NETC receive task
 *
 * This function creates a FreeRTOS task to handle received frames
 */
void netc_rx_task_create(void);

void phy_poll_task_create(void);

#endif /* NETC_SWITCH_H */

