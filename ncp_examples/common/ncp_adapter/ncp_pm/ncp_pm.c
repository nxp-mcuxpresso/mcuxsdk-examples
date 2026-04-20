/*
 * Copyright 2024 - 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "fsl_os_abstraction.h"
#include "ncp_pm.h"
#include "ncp_pm_sm.h"
#include "ncp_pm_action.h"
#include "ncp_pm_os.h"
#include "ncp_intf_pm.h"
#include "ncp_log.h"

NCP_LOG_MODULE_DEFINE(ncp_pm, CONFIG_LOG_NCP_PM_LEVEL);
NCP_LOG_MODULE_REGISTER(ncp_pm, CONFIG_LOG_NCP_PM_LEVEL);

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * @brief Internal data for NCP Power management
 */
typedef struct _ncp_pm_ctx
{
    bool initialized;                           /**< Indicates whether the NCP PM has been initialized */
    ncp_pm_role_t role;                         /**< NCP Power management role (e.g., host or device) */
    const ncp_pm_constraint_cbs_t *lp_constraint_cbs;  /**< Low power constraint callbacks */
    uint8_t pm_runtime_constraint;              /**< Indicates runtime blocking conditions for low power entry.
                                                 * Set when peripherals or subsystems are not ready,
                                                 * typically forces system to stay in PM1 until safe to sleep.
                                                 */
    uint8_t pm_policy_constraint;               /**< Defines the deepest allowed low power mode based on system policy.
                                                 * Unlike runtime constraints, it can be set by LP timer. */
    uint32_t critical_enter_cnt;                /**< Counter for critical section entry to manage nested critical sections */
    const ncp_pm_tx_if_t *tx_if;                /**< Interface for transmitting handshake messages */
} ncp_pm_ctx_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

static ncp_pm_ctx_t s_pm_ctx = {0};
static ncp_pm_cfg_t s_pm_cfg = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Register low-power constraint callbacks. */
void ncp_pm_register_lp_constraint_cbs(const ncp_pm_constraint_cbs_t *cbs)
{
    s_pm_ctx.lp_constraint_cbs = cbs;
}

/* Unregister low-power constraint callbacks. */
void ncp_pm_unregister_lp_constraint_cbs(void)
{
    s_pm_ctx.lp_constraint_cbs = NULL;
}

static int ncp_pm_set_lp_constraint(int power_mode)
{
    ncp_pm_status_t ret = NCP_PM_STATUS_SUCCESS;

    if (s_pm_ctx.lp_constraint_cbs && s_pm_ctx.lp_constraint_cbs->set_constraint)
    {
        switch (power_mode)
        {
            case NCP_PM_STATE_PM0:
                break;
            case NCP_PM_STATE_PM1:
                ret = (ncp_pm_status_t)s_pm_ctx.lp_constraint_cbs->set_constraint((int32_t)0);
                break;
            case NCP_PM_STATE_PM2:
                ret = (ncp_pm_status_t)s_pm_ctx.lp_constraint_cbs->set_constraint((int32_t)NCP_PM_STATE_PM2);
                break;
            case NCP_PM_STATE_PM3:
                ret = (ncp_pm_status_t)s_pm_ctx.lp_constraint_cbs->set_constraint((int32_t)NCP_PM_STATE_PM3);
                break;
            case NCP_PM_STATE_PM4:
                ret = (ncp_pm_status_t)s_pm_ctx.lp_constraint_cbs->set_constraint((int32_t)NCP_PM_STATE_PM4);
                break;
            default:
                break;
        }
    }

    return ret;
}

static int ncp_pm_release_lp_constraint(int power_mode)
{
    ncp_pm_status_t ret = NCP_PM_STATUS_SUCCESS;

    if (s_pm_ctx.lp_constraint_cbs && s_pm_ctx.lp_constraint_cbs->release_constraint)
    {
        switch (power_mode)
        {
            case NCP_PM_STATE_PM0:
                break;
            case NCP_PM_STATE_PM1:
                ret = (ncp_pm_status_t)s_pm_ctx.lp_constraint_cbs->release_constraint((int32_t)0);
                break;
            case NCP_PM_STATE_PM2:
                ret = (ncp_pm_status_t)s_pm_ctx.lp_constraint_cbs->release_constraint((int32_t)NCP_PM_STATE_PM2);
                break;
            case NCP_PM_STATE_PM3:
                ret = (ncp_pm_status_t)s_pm_ctx.lp_constraint_cbs->release_constraint((int32_t)NCP_PM_STATE_PM3);
                break;
            case NCP_PM_STATE_PM4:
                ret = (ncp_pm_status_t)s_pm_ctx.lp_constraint_cbs->release_constraint((int32_t)NCP_PM_STATE_PM4);
                break;
            default:
                break;
        }
    }

    return ret;
}

int ncp_pm_enter_critical(void)
{
    int ret = NCP_PM_STATUS_SUCCESS;
    uint32_t pri_mask = DisableGlobalIRQ();

    if (s_pm_ctx.critical_enter_cnt == 0U)
    {
        ret = ncp_pm_set_lp_constraint(s_pm_ctx.pm_runtime_constraint);
        if (ret != NCP_PM_STATUS_SUCCESS)
        {
            ret = NCP_PM_STATUS_ERROR;
            NCP_LOG_ERR("Failed to set pm constraint");
            goto out;
        }
    }

    s_pm_ctx.critical_enter_cnt++;

out:
    EnableGlobalIRQ(pri_mask);

    return ret;
}

int ncp_pm_exit_critical(void)
{
    int ret = NCP_PM_STATUS_SUCCESS;
    uint32_t pri_mask = DisableGlobalIRQ();

    if (s_pm_ctx.critical_enter_cnt == 0U) {
        ret = NCP_PM_STATUS_ERROR;
        goto out;
    }

    s_pm_ctx.critical_enter_cnt--;

    if (s_pm_ctx.critical_enter_cnt == 0U)
    {
        ret = ncp_pm_release_lp_constraint(s_pm_ctx.pm_runtime_constraint);
        if (ret != NCP_PM_STATUS_SUCCESS) {
            s_pm_ctx.critical_enter_cnt++;
            ret = NCP_PM_STATUS_ERROR;
            NCP_LOG_ERR("Failed to release pm constraint");
            goto out;
        }
    }

out:
    EnableGlobalIRQ(pri_mask);

    return ret;
}

void ncp_pm_configure_next_lowpower_mode(int next_mode, uint32_t duration_ms)
{
    (void)ncp_pm_release_lp_constraint(s_pm_ctx.pm_policy_constraint); /* MISRA CID 26556646 */
    (void)ncp_pm_set_lp_constraint(next_mode);

    s_pm_ctx.pm_policy_constraint = next_mode;

    ncp_pm_os_activate_lp_timer(duration_ms);
}

int ncp_pm_wakeup(void)
{
    int ret = NCP_PM_STATUS_SUCCESS;

    switch (s_pm_cfg.pm_mode)
    {
        case NCP_PM_STATE_PM0:
        case NCP_PM_STATE_PM1:
            break;
        case NCP_PM_STATE_PM2:
            NCP_LOG_DBG("PM2 WAKEUP");
            ret = ncp_intf_pm_exit(NCP_PM_STATE_PM2);
            break;
        case NCP_PM_STATE_PM3:
            NCP_LOG_DBG("PM3 WAKEUP");
            ncp_pm_gpio_wakeup_peer();
            break;
        case NCP_PM_STATE_PM4:
            break;
        default:
            break;
    }

    return ret;
}

static int ncp_pm_tx_ctrl_action(void)
{
    return ncp_pm_action_tx_ctrl_dispatch();
}

static int ncp_pm_tx_data_action(void)
{
    return ncp_pm_action_tx_data_dispatch();
}

static int ncp_pm_rx_action(void *buf, size_t buf_sz)
{
    return ncp_pm_action_rx_dispatch(buf, buf_sz);
}

static int ncp_pm_init(ncp_pm_role_t role, const ncp_pm_tx_if_t * tx_if)
{
    if (s_pm_ctx.initialized)
        return NCP_PM_STATUS_SUCCESS;

    (void)memset(&s_pm_ctx, 0, sizeof(s_pm_ctx));

    s_pm_ctx.role = role;
    s_pm_ctx.lp_constraint_cbs = NULL;
    s_pm_ctx.pm_runtime_constraint = NCP_PM_STATE_PM1;
    s_pm_ctx.pm_policy_constraint  = NCP_PM_STATE_PM1;
    s_pm_ctx.tx_if = tx_if;

    if (ncp_pm_os_init() != NCP_PM_STATUS_SUCCESS)
    {
        goto fail;
    }

    if (ncp_pm_sm_init(s_pm_ctx.role, (void *)s_pm_ctx.tx_if) != NCP_PM_STATUS_SUCCESS)
    {
        goto fail;
    }

    s_pm_ctx.initialized = true;

    return NCP_PM_STATUS_SUCCESS;

fail:
    NCP_LOG_ERR("NCP PM init failed");
    return NCP_PM_STATUS_ERROR;
}

static int ncp_pm_deinit(void)
{
    if (!s_pm_ctx.initialized)
        return NCP_PM_STATUS_SUCCESS;

    ncp_pm_sm_deinit();
    ncp_pm_os_deinit();

    s_pm_ctx.initialized = false;
    return NCP_PM_STATUS_SUCCESS;
}

static int ncp_pm_reset(void)
{
    if (!s_pm_ctx.initialized)
        return NCP_PM_STATUS_SUCCESS;

    s_pm_ctx.initialized = false;

    if (ncp_pm_sm_reinit(s_pm_ctx.role, (void *)s_pm_ctx.tx_if) != NCP_PM_STATUS_SUCCESS)
    {
        NCP_LOG_ERR("NCP PM init failed");
        return NCP_PM_STATUS_ERROR;
    }

    s_pm_ctx.initialized = true;

    return NCP_PM_STATUS_SUCCESS;
}

static const ncp_pm_ops_t ncp_pm_ops =
{
    .init   = ncp_pm_init,
    .deinit = ncp_pm_deinit,
    .reset  = ncp_pm_reset,
    .enter_critical = ncp_pm_enter_critical,
    .exit_critical = ncp_pm_exit_critical,
    .tx_ctrl_action = ncp_pm_tx_ctrl_action,
    .tx_data_action = ncp_pm_tx_data_action,
    .rx_action = ncp_pm_rx_action,
    .init_wakeup_src = ncp_pm_init_wakeup_source,
    .enable_wakeup_src = ncp_pm_enable_wakeup_source,
    .disable_wakeup_src = ncp_pm_disable_wakeup_source,
    .get_wakeup_src = ncp_pm_get_wakeup_source,
};

const ncp_pm_ops_t *ncp_pm_get_ops(void)
{
    return &ncp_pm_ops;
}

ncp_pm_cfg_t *ncp_pm_get_config(void)
{
    return &s_pm_cfg;
}

void ncp_pm_delay_us(uint32_t delay_us)
{
    SDK_DelayAtLeastUs(delay_us, SystemCoreClock);
}