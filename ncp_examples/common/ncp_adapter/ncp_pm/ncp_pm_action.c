/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#include "ncp_pm_action.h"
#include "ncp_pm_sm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct {
    ncp_pm_action_tx_cb_t tx_ctrl_cb;
    ncp_pm_action_tx_cb_t tx_data_cb;
    ncp_pm_action_rx_cb_t rx_cb;
} ncp_pm_action_entry_t;

typedef struct {
    ncp_pm_role_t role;
    ncp_pm_event_poster_t event_poster;
    const ncp_pm_action_entry_t *action_entry;
} ncp_pm_action_ctx_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Action callback lookup table indexed by [state] */
static const ncp_pm_action_entry_t s_action_table[NCP_PM_SM_STATE_MAX] = {
    [NCP_PM_SM_STATE_IDLE]      = {NULL, NULL, ncp_pm_action_rx_idle},
    [NCP_PM_SM_STATE_ENTER]     = {ncp_pm_action_tx_ctrl_enter, NULL, ncp_pm_action_rx_enter},
    [NCP_PM_SM_STATE_CONFIRM]   = {NULL, NULL, ncp_pm_action_rx_confirm},
    [NCP_PM_SM_STATE_ACK]       = {ncp_pm_action_tx_ctrl_ack, ncp_pm_action_tx_data_ack, ncp_pm_action_rx_ack},
    [NCP_PM_SM_STATE_FINISH]    = {NULL, ncp_pm_action_tx_data_finish, ncp_pm_action_rx_finish},
    [NCP_PM_SM_STATE_EXIT]      = {ncp_pm_action_tx_ctrl_exit, ncp_pm_action_tx_data_exit, NULL},
    [NCP_PM_SM_STATE_TIME_WAIT] = {NULL, ncp_pm_action_tx_data_time_wait, ncp_pm_action_rx_time_wait},
};

static ncp_pm_action_ctx_t s_pm_action_ctx = {
    .role = NCP_PM_ROLE_MAX,
    .event_poster = NULL,
    .action_entry = NULL
};

/*******************************************************************************
 * Code
 ******************************************************************************/

static inline void post_event(uint32_t event)
{
    if (s_pm_action_ctx.event_poster)
    {
        s_pm_action_ctx.event_poster(event);
    }
}

void ncp_pm_action_init(ncp_pm_role_t role, ncp_pm_event_poster_t poster)
{
    s_pm_action_ctx.role = role;
    s_pm_action_ctx.event_poster = poster;
    s_pm_action_ctx.action_entry = NULL;
}

void ncp_pm_action_deinit(void)
{
    s_pm_action_ctx.role = NCP_PM_ROLE_MAX;
    s_pm_action_ctx.event_poster = NULL;
    s_pm_action_ctx.action_entry = NULL;
}

void ncp_pm_action_entry_update(int state)
{
    if (state >= 0 && state < NCP_PM_SM_STATE_MAX) {
        s_pm_action_ctx.action_entry = &s_action_table[(ncp_pm_sm_state_t)state];
    }
}

int ncp_pm_action_tx_ctrl_dispatch(void)
{
    if (s_pm_action_ctx.action_entry &&
        s_pm_action_ctx.action_entry->tx_ctrl_cb != NULL)
    {
        return s_pm_action_ctx.action_entry->tx_ctrl_cb();
    }
    return NCP_PM_STATUS_SUCCESS;
}

int ncp_pm_action_tx_data_dispatch(void)
{
    if (s_pm_action_ctx.action_entry &&
        s_pm_action_ctx.action_entry->tx_data_cb != NULL)
    {
        return s_pm_action_ctx.action_entry->tx_data_cb();
    }
    return NCP_PM_STATUS_SUCCESS;
}

int ncp_pm_action_rx_dispatch(void *buf, size_t buf_sz)
{
    if (s_pm_action_ctx.action_entry &&
        s_pm_action_ctx.action_entry->rx_cb != NULL)
    {
        return s_pm_action_ctx.action_entry->rx_cb(buf, buf_sz);
    }
    return NCP_PM_STATUS_SUCCESS;
}

/* Action callback */
int ncp_pm_action_tx_ctrl_enter(void)
{
    post_event(NCP_PM_SM_EVENT_TX_DONE);
    return NCP_PM_STATUS_SUCCESS;
}

int ncp_pm_action_tx_ctrl_ack(void)
{
    post_event(NCP_PM_SM_EVENT_TX_DONE);
    return NCP_PM_STATUS_SUCCESS;
}

int ncp_pm_action_tx_ctrl_exit(void)
{
    post_event(NCP_PM_SM_EVENT_TX_DONE);
    return NCP_PM_STATUS_SUCCESS;
}

int ncp_pm_action_tx_data_ack(void)
{
    post_event(NCP_PM_SM_EVENT_AWAKE);
    return NCP_PM_STATUS_SKIP;
}

int ncp_pm_action_tx_data_finish(void)
{
    post_event(NCP_PM_SM_EVENT_AWAKE);
    return NCP_PM_STATUS_SKIP;
}

int ncp_pm_action_tx_data_exit(void)
{
    /* No action needed */
    return NCP_PM_STATUS_SKIP;
}

int ncp_pm_action_tx_data_time_wait(void)
{
    /* No action needed */
    return NCP_PM_STATUS_SKIP;
}

int ncp_pm_action_rx_idle(void *buf, size_t buf_sz)
{
    (void)buf_sz;
    int ret = NCP_PM_STATUS_SUCCESS;

    switch (s_pm_action_ctx.role)
    {
        case NCP_PM_ROLE_DEVICE:
            /*  No action needed */
            break;
        case NCP_PM_ROLE_HOST:
            if (ncp_pm_msg_is_valid(buf))
            {
                if (ncp_pm_msg_is_match(buf, PM_MSG_SLEEP_ENTER))
                {
                    post_event(NCP_PM_SM_EVENT_START);
                }
                ret = NCP_PM_STATUS_SKIP;
            }
            break;
        default:
            break;
    }

    return ret;
}

int ncp_pm_action_rx_enter(void *buf, size_t buf_sz)
{
    (void)buf_sz;
    int ret = NCP_PM_STATUS_SUCCESS;

    switch (s_pm_action_ctx.role)
    {
        case NCP_PM_ROLE_DEVICE:
            if (ncp_pm_msg_is_match(buf, PM_MSG_SLEEP_CFM))
            {
                post_event(NCP_PM_SM_EVENT_SLEEP_CFM);
                ret = NCP_PM_STATUS_SKIP;
            }
            break;
        case NCP_PM_ROLE_HOST:
            if (ncp_pm_msg_is_match(buf, PM_MSG_SLEEP_ACK))
            {
                post_event(NCP_PM_SM_EVENT_SLEEP_ACK);
                ret = NCP_PM_STATUS_SKIP;
            }
            break;
        default:
            break;
    }

    return ret;
}

int ncp_pm_action_rx_confirm(void *buf, size_t buf_sz)
{
    (void)buf_sz;
    int ret = NCP_PM_STATUS_SUCCESS;

    switch (s_pm_action_ctx.role)
    {
        case NCP_PM_ROLE_DEVICE:
            if (ncp_pm_msg_is_match(buf, PM_MSG_SLEEP_CFM))
            {
                post_event(NCP_PM_SM_EVENT_SLEEP_CFM);
                ret = NCP_PM_STATUS_SKIP;
            }
            break;
        case NCP_PM_ROLE_HOST:
            /*  No action needed */
            break;
        default:
            break;
    }

    return ret;
}

int ncp_pm_action_rx_ack(void *buf, size_t buf_sz)
{
    (void)buf_sz;
    int ret = NCP_PM_STATUS_SUCCESS;

    switch (s_pm_action_ctx.role)
    {
        case NCP_PM_ROLE_DEVICE:
            /*  No action needed */
            break;
        case NCP_PM_ROLE_HOST:
            if (ncp_pm_msg_is_match(buf, PM_MSG_SLEEP_ACK))
            {
                post_event(NCP_PM_SM_EVENT_SLEEP_ACK);
                ret = NCP_PM_STATUS_SKIP;
            }
            break;
        default:
            break;
    }

    return ret;
}

int ncp_pm_action_rx_finish(void *buf, size_t buf_sz)
{
    (void)buf_sz;
    int ret = NCP_PM_STATUS_SUCCESS;

    switch (s_pm_action_ctx.role)
    {
        case NCP_PM_ROLE_DEVICE:
            post_event(NCP_PM_SM_EVENT_AWAKE);
            break;
        case NCP_PM_ROLE_HOST:
            post_event(NCP_PM_SM_EVENT_STOP);
            if (ncp_pm_msg_is_match(buf, PM_MSG_SLEEP_EXIT))
            {
                ret = NCP_PM_STATUS_SKIP;
            }
            break;
        default:
            break;
    }

    return ret;
}

int ncp_pm_action_rx_time_wait(void *buf, size_t buf_sz)
{
    (void)buf_sz;
    int ret = NCP_PM_STATUS_SUCCESS;

    switch (s_pm_action_ctx.role)
    {
        case NCP_PM_ROLE_DEVICE:
            /*  No action needed */
            break;
        case NCP_PM_ROLE_HOST:
            post_event(NCP_PM_SM_EVENT_STOP);
            if (ncp_pm_msg_is_valid(buf))
            {
                if (ncp_pm_msg_is_match(buf, PM_MSG_SLEEP_ENTER))
                {
                    post_event(NCP_PM_SM_EVENT_START);
                }
                ret = NCP_PM_STATUS_SKIP;
            }
            break;
        default:
            break;
    }

    return ret;
}