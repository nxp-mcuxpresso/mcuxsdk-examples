/*
 * Copyright 2022-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#include "ncp_intf_pm.h"
#include "ncp_tlv_adapter.h"

static const ncp_tlv_adapter_t *tlv_adap = NULL;

static inline const ncp_intf_pm_ops_t *get_intf_pm_ops(void)
{
    if (!tlv_adap) {
        tlv_adap = ncp_tlv_adapter_get();
    }

    if (tlv_adap && tlv_adap->intf_ops && tlv_adap->intf_ops->pm_ops) {
        return tlv_adap->intf_ops->pm_ops;
    }

    return NULL;
}

int ncp_intf_pm_init(void)
{
    const ncp_intf_pm_ops_t *pm = get_intf_pm_ops();
    return (pm && pm->init) ? pm->init() : 0;
}

int ncp_intf_pm_prep(uint8_t pm_state, uint8_t event_type, void *data)
{
    const ncp_intf_pm_ops_t *pm = get_intf_pm_ops();
    return (pm && pm->prep) ? pm->prep(pm_state, event_type, data) : 0;
}

int ncp_intf_pm_enter(uint8_t pm_state)
{
    const ncp_intf_pm_ops_t *pm = get_intf_pm_ops();
    return (pm && pm->enter) ? pm->enter(pm_state) : 0;
}

int ncp_intf_pm_exit(uint8_t pm_state)
{
    const ncp_intf_pm_ops_t *pm = get_intf_pm_ops();
    return (pm && pm->exit) ? pm->exit(pm_state) : 0;
}
