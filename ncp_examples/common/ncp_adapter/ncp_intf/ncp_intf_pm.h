/*
 * Copyright 2022-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef __NCP_INTF_PM_H__
#define __NCP_INTF_PM_H__

#include "fsl_common.h"

/**
 * @brief Active SRAM bank information structure.
 */
typedef struct active_sram_bank
{
    uint32_t start;
    uint32_t end;
    uint32_t active_mask;
    uint8_t  intf_flags;
} active_sram_bank_t;

/**
 * @brief Power management operations interface.
 */
typedef struct _ncp_intf_pm_ops
{
    int (*init)(void);
    int (*prep)(uint8_t pm_state, uint8_t event_type, void *data);
    int (*enter)(uint8_t pm_state);
    int (*exit)(uint8_t pm_state);
} ncp_intf_pm_ops_t;

/**
 * @brief Initializes the PM module for the NCP interface.
 * @return 0 on success, negative on error.
 */
int ncp_intf_pm_init(void);

/**
 * @brief Prepare for power state change.
 * @param pm_state   Power management state.
 * @param event_type Event type.
 * @param data       User data.
 * @return 0 on success, negative on error.
 */
int ncp_intf_pm_prep(uint8_t pm_state, uint8_t event_type, void *data);

/**
 * @brief Enter power state.
 * @param pm_state Power management state.
 * @return 0 on success, negative on error.
 */
int ncp_intf_pm_enter(uint8_t pm_state);

/**
 * @brief Exit power state.
 * @param pm_state Power management state.
 * @return 0 on success, negative on error.
 */
int ncp_intf_pm_exit(uint8_t pm_state);

#endif /* __NCP_INTF_PM_H__ */
