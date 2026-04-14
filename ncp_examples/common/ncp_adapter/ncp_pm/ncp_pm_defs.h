/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#ifndef _NCP_PM_DEFS_H_
#define _NCP_PM_DEFS_H_

#include "fsl_common.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/** Sleep event notification */
#define NCP_PM_NOTIFY_EVENT_PRE         (1 << 0)
/** Awake event notification */
#define NCP_PM_NOTIFY_EVENT_POST        (1 << 1)
/** Data ready event notification */
#define NCP_PM_NOTIFY_EVENT_DATA_READY  (1 << 2)

/**
 * @brief Enumerated data type definition for supported power modes
 */
typedef enum _ncp_pm_state
{
    NCP_PM_STATE_PM0 = 0,     /**< Active mode */
    NCP_PM_STATE_PM1 = 1,     /**< Wait For Interrupt */
    NCP_PM_STATE_PM2 = 2,     /**< Sleep mode */
    NCP_PM_STATE_PM3 = 3,     /**< Power down mode */
    NCP_PM_STATE_PM4 = 4,     /**< Deep power down mode */
} ncp_pm_state_t;

/**
 * @brief Power management status codes
 */
typedef enum _ncp_pm_status
{
    NCP_PM_STATUS_ERROR      = -1, /**< Error */
    NCP_PM_STATUS_NOT_READY  = -2, /**< Not ready */
    NCP_PM_STATUS_SUCCESS    = 0,  /**< Success */
    NCP_PM_STATUS_SKIP       = 1,  /**< Skip action */
} ncp_pm_status_t;

/**
 * @brief Power management role
 */
typedef enum _ncp_pm_role
{
    NCP_PM_ROLE_DEVICE, /**< Device role */
    NCP_PM_ROLE_HOST,   /**< Host role */
    NCP_PM_ROLE_MAX,    /**< Boundary marker */
} ncp_pm_role_t;

typedef enum _ncp_pm_wake_src
{
    NCP_PM_WAKE_SRC_NONE = 0,
    NCP_PM_WAKE_SRC_WLAN,         /**< WLAN wake source */
    NCP_PM_WAKE_SRC_BLE,          /**< BLE wake source */
    NCP_PM_WAKE_SRC_RTC,          /**< RTC wake source */
    NCP_PM_WAKE_SRC_PIN1,         /**< PIN1 wake source */
    NCP_PM_WAKE_SRC_GPIOA,        /**< GPIOA wake source */
    NCP_PM_WAKE_SRC_USB,          /**< USB wake source */
    NCP_PM_WAKE_SRC_WKDEEPSLEEP,  /**< WKDEEPSLEEP wake source */
    NCP_PM_WAKE_SRC_SDIO,         /**< SDIO wake source */
    NCP_PM_WAKE_SRC_DMA0,         /**< DMA0 wake source */
} ncp_pm_wake_src_t;

#define NCP_PM_MSG_MAGIC (0xACAC)

typedef enum _pm_msg_id
{
    PM_MSG_INVALID      = 0x0000,
    PM_MSG_SLEEP_ENTER  = 0x0001,
    PM_MSG_SLEEP_CFM    = 0x0002,
    PM_MSG_SLEEP_ACK    = 0x0003,
    PM_MSG_SLEEP_EXIT   = 0x0004,
} pm_msg_id_t;

/**
 * @brief Power management message structure
 */
typedef struct _ncp_pm_msg
{
    uint16_t magic;      /**< Magic: 0xACAC */
    uint16_t msg_id;     /**< Message ID */
    uint16_t size;       /**< Total length */
    uint16_t seqnum;     /**< Sequence number */
    uint32_t reserved;   /**< Reserved fields */
} ncp_pm_msg_t;

typedef struct _pm_notify_state
{
    unsigned wlan : 1;
    unsigned pm : 1;
} pm_notify_state_t;

/**
 * @brief NCP Power management transmit interface
 * Used to send messages and events.
 */
typedef struct _ncp_pm_tx_if
{
    int (*send_msg)(uint32_t event, void *msg, uint16_t size);   /**< Send a message buffer */
    void (*post_event)(uint32_t event);               /**< Send a simple event code */
} ncp_pm_tx_if_t;

/**
 * @brief Low power mode constraint callback type
 * @param power_mode Power mode value
 * @return Status code
 */
typedef int32_t (*ncp_pm_constraint_cb_t)(int32_t power_mode);

/**
 * @brief Low power mode constraint callbacks
 */
typedef struct _ncp_pm_constraint_cbs
{
    ncp_pm_constraint_cb_t set_constraint;      /**< Power manager set constraint function, default set as NULL. */
    ncp_pm_constraint_cb_t release_constraint;  /**< Power manager release constraint function, default set as NULL. */
} ncp_pm_constraint_cbs_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Check if the message has a valid PM magic number
 * @param msg Pointer to the message
 * @return true if magic is valid, false otherwise
 */
static inline bool ncp_pm_msg_is_valid(const ncp_pm_msg_t *msg)
{
    return ((msg != NULL) && (msg->magic == NCP_PM_MSG_MAGIC));
}

/**
 * @brief Get PM message ID from message buffer
 * @param msg Pointer to the message
 * @return Message ID if valid, PM_MSG_INVALID otherwise
 */
static inline pm_msg_id_t ncp_pm_msg_get_id(const ncp_pm_msg_t *msg)
{
    if ((msg != NULL) && (msg->magic == NCP_PM_MSG_MAGIC))
    {
        return (pm_msg_id_t)msg->msg_id;
    }
    return PM_MSG_INVALID;
}

/**
 * @brief Check if the PM message is valid and matches the expected ID
 * @param msg Pointer to the message
 * @param id Expected message ID
 * @return true if magic is valid and ID matches, false otherwise
 */
static inline bool ncp_pm_msg_is_match(const ncp_pm_msg_t *msg, pm_msg_id_t id)
{
    return ((msg != NULL) &&
            (msg->magic == NCP_PM_MSG_MAGIC) &&
            (msg->msg_id == id));
}

/** Register low power constraint callbacks */
void ncp_pm_register_lp_constraint_cbs(const ncp_pm_constraint_cbs_t *cbs);
/** Unregister low power constraint callbacks */
void ncp_pm_unregister_lp_constraint_cbs(void);

/** Low power critical entry function, default set as NULL. */
int ncp_pm_enter_critical(void);
/** Low power critical exit function, default set as NULL. */
int ncp_pm_exit_critical(void);

/** Configure next low power mode */
void ncp_pm_configure_next_lowpower_mode(int next_mode, uint32_t duration_ms);

/** Wake up from low power mode */
int ncp_pm_wakeup(void);

#endif /* _NCP_PM_DEFS_H_ */
