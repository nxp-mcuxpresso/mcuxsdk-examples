/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#ifndef _NCP_PM_SM_H_
#define _NCP_PM_SM_H_

#include "ncp_pm_defs.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * @brief Power management state machine states
 */
typedef enum _ncp_pm_sm_state
{
    NCP_PM_SM_STATE_IDLE = 0,      /**< Idle state, the state machine is inactive */
    NCP_PM_SM_STATE_ENTER,         /**< Enter sleep, initiating the low power handshake process */
    NCP_PM_SM_STATE_CONFIRM,       /**< Handshake confirm */
    NCP_PM_SM_STATE_ACK,           /**< Handshake ACK */
    NCP_PM_SM_STATE_FINISH,        /**< Handshake is complete, ready to enter low power mode */
    NCP_PM_SM_STATE_EXIT,          /**< Exiting low power mode and resuming normal operation */
    NCP_PM_SM_STATE_TIME_WAIT,     /**< Time wait state,
                                    * This state serves dual purposes depending on the system role:
                                    *
                                    * - [Device role]: Indicates that the system is still in a low power operation
                                    *   phase (e.g., handshake or sleep), and the PM policy constraint cannot be released immediately.
                                    *   The state ensures that all power-related transitions are fully completed before allowing policy
                                    *   updates or state transitions.
                                    *
                                    * - [Host role]: Represents the phase where the host initiates a wake-up sequence for the device and
                                    *   waits for the device to complete its wake-up process. This includes waiting for the device to
                                    *   exit sleep mode and become fully operational before proceeding.
                                    *
                                    * This state acts as a synchronization point to ensure safe and consistent power management transitions
                                    * across roles.
                                    */
    NCP_PM_SM_STATE_MAX,           /**< Boundary marker, not used as an actual state */
} ncp_pm_sm_state_t;


/** Trigger the state machine to start the low power handshake */
#define NCP_PM_SM_EVENT_START        (1 << 0)
/** Normally exit low power mode and resume normal operation */
#define NCP_PM_SM_EVENT_STOP         (1 << 1)
/** The power state machine waits for the handshake message to be sent. */
#define NCP_PM_SM_EVENT_TX_DONE      (1 << 2)
/** Received sleep confirmation from peer */
#define NCP_PM_SM_EVENT_SLEEP_CFM    (1 << 3)
/** Received sleep ack from peer */
#define NCP_PM_SM_EVENT_SLEEP_ACK    (1 << 4)
/** Detected low power mode during TX/RX; triggers state transition to exit sleep */
#define NCP_PM_SM_EVENT_AWAKE        (1 << 5)
/** PM policy constraint is pending release; wait for current low power operation to finish */
#define NCP_PM_SM_EVENT_TIME_WAIT    (1 << 6)
/** All events mask */
#define NCP_PM_SM_EVENT_ALL          (NCP_PM_SM_EVENT_START | NCP_PM_SM_EVENT_STOP | NCP_PM_SM_EVENT_TX_DONE | \
                                      NCP_PM_SM_EVENT_SLEEP_CFM | NCP_PM_SM_EVENT_SLEEP_ACK | NCP_PM_SM_EVENT_AWAKE | \
                                      NCP_PM_SM_EVENT_TIME_WAIT)

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Initialize power management state machine
 * @param role Device/host role
 * @param tx_if Transmit interface pointer
 * @return Status code
 */
int ncp_pm_sm_init(ncp_pm_role_t role, void *tx_if);

/**
 * @brief Deinitialize power management state machine
 * @return Status code
 */
int ncp_pm_sm_deinit(void);

/**
 * @brief Check if state machine is idle
 * @return true if idle
 */
bool ncp_pm_sm_is_idle(void);

/**
 * @brief Check if the sleep enter handshake has completed.
 *
 * This function returns true if the low power entry handshake
 * sequence has finished and the system is ready to enter sleep mode.
 *
 * @return true if done
 */
bool ncp_pm_sm_is_done(void);

/**
 * @brief Set one or more event flags of an event object.
 */
void ncp_pm_sm_post_event(uint32_t event);

#endif /* _NCP_PM_SM_H_ */
