/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#include "ncp_pm_action.h"
#include "ncp_pm_sm.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

static ncp_pm_action_tx_cb_t s_tx_data_action_cb = NULL;
static ncp_pm_action_tx_cb_t s_tx_ctrl_action_cb = NULL;
static ncp_pm_action_rx_cb_t s_rx_action_cb = NULL;
static ncp_pm_event_poster_t s_event_poster = NULL;
static ncp_pm_role_t s_pm_role;

/*******************************************************************************
 * Code
 ******************************************************************************/

void ncp_pm_action_cb_register(ncp_pm_action_tx_cb_t tx_ctrl_cb, ncp_pm_action_tx_cb_t tx_data_cb, ncp_pm_action_rx_cb_t rx_cb)
{
    s_tx_ctrl_action_cb = tx_ctrl_cb;
    s_tx_data_action_cb = tx_data_cb;
    s_rx_action_cb = rx_cb;
}

void ncp_pm_action_init(ncp_pm_role_t role, ncp_pm_event_poster_t poster)
{
    s_pm_role = role;
    s_event_poster = poster;
    ncp_pm_action_cb_register(NULL, NULL, NULL);
}

void ncp_pm_action_deinit(void)
{
    s_pm_role = NCP_PM_ROLE_MAX;
    s_event_poster = NULL;
    ncp_pm_action_cb_register(NULL, NULL, NULL);
}

int ncp_pm_action_tx_ctrl_dispatch(void)
{
    int ret = NCP_PM_STATUS_SUCCESS;
    if (s_tx_ctrl_action_cb != NULL)
    {
        ret = s_tx_ctrl_action_cb();
    }
    return ret;
}

int ncp_pm_action_tx_data_dispatch(void)
{
    int ret = NCP_PM_STATUS_SUCCESS;
    if (s_tx_data_action_cb != NULL)
    {
        ret = s_tx_data_action_cb();
    }
    return ret;
}

int ncp_pm_action_rx_dispatch(void *buf, size_t buf_sz)
{
    int ret = NCP_PM_STATUS_SUCCESS;
    if (s_rx_action_cb != NULL)
    {
        ret = s_rx_action_cb(buf, buf_sz);
    }
    return ret;
}

int ncp_pm_action_tx_ctrl_enter(void)
{
    if (s_event_poster)
    {
        s_event_poster(NCP_PM_SM_EVENT_TX_DONE);
    }

    return NCP_PM_STATUS_SUCCESS;
}

int ncp_pm_action_tx_ctrl_ack(void)
{
    if (s_event_poster)
    {
        s_event_poster(NCP_PM_SM_EVENT_TX_DONE);
    }

    return NCP_PM_STATUS_SUCCESS;
}

int ncp_pm_action_tx_ctrl_exit(void)
{
    if (s_event_poster)
    {
        s_event_poster(NCP_PM_SM_EVENT_TX_DONE);
    }

    return NCP_PM_STATUS_SUCCESS;
}

int ncp_pm_action_tx_data_ack(void)
{
    if (s_event_poster)
    {
        s_event_poster(NCP_PM_SM_EVENT_AWAKE);
    }

    return NCP_PM_STATUS_SKIP;
}

int ncp_pm_action_tx_data_finish(void)
{
    if (s_event_poster)
    {
        s_event_poster(NCP_PM_SM_EVENT_AWAKE);
    }

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

    switch (s_pm_role)
    {
        case NCP_PM_ROLE_DEVICE:
            /*  No action needed */
            break;
        case NCP_PM_ROLE_HOST:
            if (ncp_pm_msg_is_valid(buf))
            {
                if (ncp_pm_msg_is_match(buf, PM_MSG_SLEEP_ENTER))
                {
                    if (s_event_poster)
                    {
                        s_event_poster(NCP_PM_SM_EVENT_START);
                    }
                }
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

    switch (s_pm_role)
    {
        case NCP_PM_ROLE_DEVICE:
            if (ncp_pm_msg_is_match(buf, PM_MSG_SLEEP_CFM))
            {
                if (s_event_poster)
                {
                    s_event_poster(NCP_PM_SM_EVENT_SLEEP_CFM);
                }
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

    switch (s_pm_role)
    {
        case NCP_PM_ROLE_DEVICE:
            /*  No action needed */
            break;
        case NCP_PM_ROLE_HOST:
            if (ncp_pm_msg_is_match(buf, PM_MSG_SLEEP_ACK))
            {
                if (s_event_poster)
                {
                    s_event_poster(NCP_PM_SM_EVENT_SLEEP_ACK);
                }
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

    switch (s_pm_role)
    {
        case NCP_PM_ROLE_DEVICE:
            if (s_event_poster)
            {
                s_event_poster(NCP_PM_SM_EVENT_AWAKE);
            }
            break;
        case NCP_PM_ROLE_HOST:
            if (s_event_poster)
            {
                s_event_poster(NCP_PM_SM_EVENT_STOP);
            }
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

    switch (s_pm_role)
    {
        case NCP_PM_ROLE_DEVICE:
            /*  No action needed */
            break;
        case NCP_PM_ROLE_HOST:
            if (s_event_poster)
            {
                s_event_poster(NCP_PM_SM_EVENT_STOP);
            }
            if (ncp_pm_msg_is_valid(buf))
            {
                if (ncp_pm_msg_is_match(buf, PM_MSG_SLEEP_ENTER))
                {
                    if (s_event_poster)
                    {
                        s_event_poster(NCP_PM_SM_EVENT_START);
                    }
                }
                ret = NCP_PM_STATUS_SKIP;
            }
            break;
        default:
            break;
    }

    return ret;
}