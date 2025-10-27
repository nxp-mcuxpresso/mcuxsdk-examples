/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#ifndef _NCP_PM_ACTION_H_
#define _NCP_PM_ACTION_H_

#include "ncp_pm_defs.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * @brief TX callback type for PM action
 */
typedef int (* ncp_pm_action_tx_cb_t)(void);
/**
 * @brief RX callback type for PM action
 * @param buf Message buffer
 * @param buf_sz Buffer size
 */
typedef int (* ncp_pm_action_rx_cb_t)(void *buf, size_t buf_sz);

/**
 * @brief Event poster callback type
 * @param event Event code
 */
typedef void (*ncp_pm_event_poster_t)(uint32_t event);

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Initialize PM action module
 * @param role Device/host role
 * @param poster Event poster callback
 */
void ncp_pm_action_init(ncp_pm_role_t role, ncp_pm_event_poster_t poster);

/**
 * @brief Deinitialize PM action module
 */
void ncp_pm_action_deinit(void);

/**
 * @brief Register TX/RX callbacks for PM action
 *
 * Register action callbacks used by the power state machine. The tx_cb is
 * called when the state machine needs to perform a transmit action; rx_cb is
 * invoked when incoming messages are dispatched.
 *
 * @param tx_cb TX callback
 * @param rx_cb RX callback
 */
void ncp_pm_action_cb_register(ncp_pm_action_tx_cb_t tx_ctrl_cb, ncp_pm_action_tx_cb_t tx_data_cb, ncp_pm_action_rx_cb_t rx_cb);

/**
 * @brief Dispatch TX ctrl action using registered callback
 * @return Status code from callback or NCP_PM_STATUS_SUCCESS
 */
int ncp_pm_action_tx_ctrl_dispatch(void);

/**
 * @brief Dispatch TX data action using registered callback
 * @return Status code from callback or NCP_PM_STATUS_SUCCESS
 */
int ncp_pm_action_tx_data_dispatch(void);

/**
 * @brief Dispatch RX action using registered callback
 * @param buf Message buffer
 * @param buf_sz Buffer size
 * @return Status code from callback or NCP_PM_STATUS_SUCCESS
 */
int ncp_pm_action_rx_dispatch(void *buf, size_t buf_sz);

/** TX ctrl enter action */
int ncp_pm_action_tx_ctrl_enter(void);
/** TX ctrl ack action */
int ncp_pm_action_tx_ctrl_ack(void);
/** TX ctrl exit action */
int ncp_pm_action_tx_ctrl_exit(void);

/** TX data ack action */
int ncp_pm_action_tx_data_ack(void);
/** TX data finish action */
int ncp_pm_action_tx_data_finish(void);
/** TX data exit action */
int ncp_pm_action_tx_data_exit(void);
/** TX data time wait action */
int ncp_pm_action_tx_data_time_wait(void);

/** RX idle action */
int ncp_pm_action_rx_idle(void *buf, size_t buf_sz);
/** RX confirm action */
int ncp_pm_action_rx_confirm(void *buf, size_t buf_sz);
/** RX ack action */
int ncp_pm_action_rx_ack(void *buf, size_t buf_sz);
/** RX finish action */
int ncp_pm_action_rx_finish(void *buf, size_t buf_sz);
/** RX time wait action */
int ncp_pm_action_rx_time_wait(void *buf, size_t buf_sz);

#endif /* _NCP_PM_ACTION_H_ */

