/*
 * Copyright 2024 - 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#ifndef _NCP_PM_H_
#define _NCP_PM_H_

#include "ncp_pm_defs.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/** Power configuration */
typedef struct _ncp_pm_cfg
{
    /** enable flag, 1: enable, 0: disable */
    uint8_t enable;
	/** power management mode, 1: PM1, 2: PM2, 3: PM3 */
    uint8_t pm_mode;
	/** true: MEF, false: not MEF */
    uint8_t is_mef;
	/** value for default wowlan conditions */
    uint32_t wake_up_conds;
	/** is used to configure timeout for RTC timer and it is used with Power Manager only.
	If no other wakeup source wakes up NCP device, the RTC timer will wakeup device when it times out.
	*/
    uint32_t timeout;
	/** 0: MCU device won’t wakeup external host, 1: wakeup extern host */
    uint8_t wakeup_host;
} ncp_pm_cfg_t;

/**
 * @brief NCP Power management operations interface
 */
typedef struct _ncp_pm_ops
{
    int (*init)(ncp_pm_role_t role, const ncp_pm_tx_if_t *tx_if); /**< Initialize PM module */
    int (*deinit)(void);                               /**< Deinitialize PM module */
    int (*reset)(void);                               /**< Reset PM module */
    int (*enter_critical)(void);                       /**< PM critical entry function, default set as NULL. */
    int (*exit_critical)(void);                        /**< PM critical exit function, default set as NULL. */
    void (*set_lp_mode)(uint8_t next_mode, uint32_t duration_ms); /**< Configure next low power mode */
    int (*tx_ctrl_action)(void);                       /**< Dispatch TX control message action */
    int (*tx_data_action)(void);                       /**< Dispatch TX data message action */
    int (*rx_action)(void *buf, size_t buf_sz);        /**< Dispatch RX action */
    void (*init_wakeup_src)(void *ws, uint32_t wsId, bool enable);  /**< Initialize wakeup source */
    void (*enable_wakeup_src)(void *ws);               /**< Enable wakeup source */
    void (*disable_wakeup_src)(void *ws);              /**< Disable wakeup source */
    void (*get_wakeup_src)(void *ws);                  /**< Get wakeup source */
} ncp_pm_ops_t;

/*******************************************************************************
 * API
 ******************************************************************************/
/**
 * @brief Get pointer to PM operations interface
 * @return Pointer to ncp_pm_ops_t
 */
const ncp_pm_ops_t *ncp_pm_get_ops(void);

ncp_pm_cfg_t *ncp_pm_get_config(void);

void ncp_pm_delay_us(uint32_t delay_us);

#endif /* _NCP_PM_H_ */