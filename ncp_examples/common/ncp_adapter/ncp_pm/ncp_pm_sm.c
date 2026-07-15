/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#include "fsl_os_abstraction.h"
#include "ncp_pm_sm.h"
#include "ncp_pm_action.h"
#include "ncp_log.h"

NCP_LOG_MODULE_DECLARE(ncp_pm);

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * @brief Internal data for power state machine
 */
typedef struct _ncp_pm_sm_ctx
{
    ncp_pm_role_t role;                     /**< NCP Power management role */
    bool initialized;                       /**< Indicates whether the state machine context has been initialized */
    ncp_pm_sm_state_t state;                /**< Current state of the power management state machine */
    OSA_TASK_HANDLE_DEFINE(task_handle);    /**< Handle for the state machine task */
    OSA_EVENT_HANDLE_DEFINE(event_handle);  /**< Handle for events driving the state machine */
    uint32_t events;                        /**< Bitmask of pending events to process */
    uint32_t notify_events;                 /**< Bitmask of notification events */
    bool time_wait;                         /**< Flag indicating TIME_WAIT condition (e.g., policy constraint pending release) */
    const ncp_pm_tx_if_t *tx_if;            /**< Interface for sending handshake messages */
} ncp_pm_sm_ctx_t;

#define NCP_PM_SM_TASK_STACK_SIZE (1024U)
#define NCP_PM_WAKEUP_MAX_RETRY   (20U)
#define NCP_PM_WAKEUP_TIMEOUT_MS  (25U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void ncp_pm_sm_task(void *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/

OSA_TASK_DEFINE(ncp_pm_sm_task, PRIORITY_RTOS_TO_OSA((CONFIG_NUM_PREEMPT_PRIORITIES - 1)), 1, NCP_PM_SM_TASK_STACK_SIZE, 0);

static ncp_pm_sm_ctx_t s_psm_ctx;
static uint16_t s_psm_msg_seqnum = 0;

/**
 * @brief Compact message using bit packing
 * Bits: [7:4] = msg_id, [3:2] = reserved, [1:0] = event_type
 */
static const uint8_t msg_spec_table[NCP_PM_ROLE_MAX][NCP_PM_SM_STATE_MAX] = {
    {/* NCP_PM_ROLE_DEVICE: msg_id << 4 | event_type */
        [NCP_PM_SM_STATE_ENTER] = (PM_MSG_SLEEP_ENTER << 4) | NCP_PM_NOTIFY_EVENT_POST,  /* POST */
        [NCP_PM_SM_STATE_ACK]   = (PM_MSG_SLEEP_ACK << 4) | NCP_PM_NOTIFY_EVENT_PRE,     /* PRE */
        [NCP_PM_SM_STATE_EXIT]  = (PM_MSG_SLEEP_EXIT << 4) | NCP_PM_NOTIFY_EVENT_PRE,    /* PRE */
    },
    {/* NCP_PM_ROLE_HOST */
        [NCP_PM_SM_STATE_ENTER] = (PM_MSG_SLEEP_CFM << 4) | NCP_PM_NOTIFY_EVENT_POST,    /* POST */
    }
};

/*******************************************************************************
 * Code
 ******************************************************************************/

static const char *ncp_pm_sm_state_str(ncp_pm_sm_state_t state)
{
    switch (state)
    {
        case NCP_PM_SM_STATE_IDLE:
            return "IDLE";
        case NCP_PM_SM_STATE_ENTER:
            return "ENTER";
        case NCP_PM_SM_STATE_CONFIRM:
            return "CONFIRM";
        case NCP_PM_SM_STATE_ACK:
            return "ACK";
        case NCP_PM_SM_STATE_FINISH:
            return "FINISH";
        case NCP_PM_SM_STATE_EXIT:
            return "EXIT";
        case NCP_PM_SM_STATE_TIME_WAIT:
            return "TIME_WAIT";
        default:
            return "UNKNOWN";
    }
}

static const char *ncp_pm_sm_event_str(uint32_t event)
{
    static char buf[128];
    buf[0] = '\0';

    if (event == NCP_PM_SM_EVENT_ALL) {
        strcpy(buf, "ALL");
        return buf;
    }

    if (event & NCP_PM_SM_EVENT_START)      strcat(buf, "START|");
    if (event & NCP_PM_SM_EVENT_STOP)       strcat(buf, "STOP|");
    if (event & NCP_PM_SM_EVENT_AWAKE)      strcat(buf, "AWAKE|");
    if (event & NCP_PM_SM_EVENT_SLEEP_CFM)  strcat(buf, "SLEEP_CFM|");
    if (event & NCP_PM_SM_EVENT_SLEEP_ACK)  strcat(buf, "SLEEP_ACK|");
    if (event & NCP_PM_SM_EVENT_TX_DONE)    strcat(buf, "TX_DONE|");
    if (event & NCP_PM_SM_EVENT_TIME_WAIT)  strcat(buf, "TIME_WAIT|");
    if (buf[0] == '\0')                       strcat(buf, "UNKNOWN|");

    size_t len = strlen(buf);
    if (len > 0 && buf[len-1] == '|') buf[len-1] = '\0';
    return buf;
}

static const char *ncp_pm_sm_notify_event_str(uint32_t events)
{
    static char buf[64];
    buf[0] = '\0';

    if (events & NCP_PM_NOTIFY_EVENT_DATA_READY) strcat(buf, "DATA_READY|");
    if (events & NCP_PM_NOTIFY_EVENT_PRE)        strcat(buf, "CTRL_PRE|");
    if (events & NCP_PM_NOTIFY_EVENT_POST)       strcat(buf, "CTRL_POST|");

    size_t len = strlen(buf);
    if (len > 0 && buf[len-1] == '|')
        buf[len-1] = '\0';
    else if (len == 0)
        strcpy(buf, "NONE");

    return buf;
}

/**
 * @brief Set the state of the power state machine.
 */
static void ncp_pm_sm_set_state(ncp_pm_sm_ctx_t *ctx, ncp_pm_sm_state_t state)
{
    NCP_LOG_DBG("State: %s -> %s", ncp_pm_sm_state_str(ctx->state), ncp_pm_sm_state_str(state));
    ctx->state = state;
}

/**
 * @brief Wait for a specific event(s) with timeout.
 * @return -1 if timeout or error, else the events value.
 */
static int ncp_pm_sm_wait_event(ncp_pm_sm_ctx_t *ctx, uint32_t event_mask, uint32_t timeout_ms)
{
    osa_status_t status = KOSA_StatusSuccess;

    NCP_LOG_DBG("Event: wait for %s", ncp_pm_sm_event_str(event_mask));

    status = OSA_EventWait(ctx->event_handle, event_mask, 0, timeout_ms, &ctx->events);
    if (status != KOSA_StatusSuccess)
    {
        // NCP_LOG_ERR("Event: wait failed or timeout");
        return NCP_PM_STATUS_ERROR;
    }

    NCP_LOG_DBG("Event: got %s", ncp_pm_sm_event_str(ctx->events));

    return ctx->events;
}

/**
 * @brief Get event's flags that specified by prameter event_mask.
 * @return -1 if error, else the events value.
 */
static int ncp_pm_sm_get_event(ncp_pm_sm_ctx_t *ctx, uint32_t event_mask, uint32_t auto_clear)
{
    osa_status_t status = KOSA_StatusSuccess;

    status = OSA_EventGet(ctx->event_handle, event_mask, &ctx->events);
    if (status != KOSA_StatusSuccess)
    {
        NCP_LOG_ERR("Event: get failed");
        return NCP_PM_STATUS_ERROR;
    }

    if (auto_clear)
    {
        OSA_EventClear(ctx->event_handle, event_mask);
    }

    return ctx->events;
}

/**
 * @brief Set one or more event flags of an event object.
 */
void ncp_pm_sm_post_event(uint32_t event)
{
    OSA_EventSet(s_psm_ctx.event_handle, event);
}

static ncp_pm_status_t ncp_pm_sm_send(ncp_pm_sm_ctx_t *ctx, uint32_t event)
{
    uint8_t packed = 0;
    uint16_t msg_id = 0;
    uint32_t notify_event = 0;

    if (event == NCP_PM_NOTIFY_EVENT_DATA_READY)
    {
        if (ctx->tx_if && ctx->tx_if->post_event)
        {
            ctx->tx_if->post_event(event);
            return NCP_PM_STATUS_SUCCESS;
        }
        return NCP_PM_STATUS_ERROR;
    }

    packed = msg_spec_table[ctx->role][ctx->state];
    if (!packed)
    {
        NCP_LOG_DBG("No message defined for state %s, role %d",
                      ncp_pm_sm_state_str(ctx->state), ctx->role);
        return NCP_PM_STATUS_SUCCESS;
    }

    msg_id = packed >> 4;
    notify_event = event | (packed & 0x03);

    ncp_pm_msg_t msg = {
        .magic = NCP_PM_MSG_MAGIC,
        .msg_id = msg_id,
        .size = sizeof(ncp_pm_msg_t),
        .seqnum = s_psm_msg_seqnum,
    };

    if (ctx->tx_if && ctx->tx_if->send_msg)
    {
        NCP_LOG_DBG("Sending message: state=%s, msg_id=%d, seqnum=0x%02x",
                      ncp_pm_sm_state_str(ctx->state),
                      msg_id,
                      s_psm_msg_seqnum);
        if (ctx->tx_if->send_msg(notify_event, &msg, msg.size) == NCP_PM_STATUS_SUCCESS)
        {
            s_psm_msg_seqnum++;
            NCP_LOG_DBG("Message sent done, event: %s", ncp_pm_sm_notify_event_str(event));
            return NCP_PM_STATUS_SUCCESS;
        }
        NCP_LOG_ERR("Failed to send message, event: %s", ncp_pm_sm_notify_event_str(event));
    }

    return NCP_PM_STATUS_ERROR;
}

static void ncp_pm_sm_reset_flag(ncp_pm_sm_ctx_t *ctx)
{
    ctx->events = 0;
    ctx->notify_events = 0;
    ctx->time_wait = false;
}

/**
 * @brief The main state machine logic. Handles all state transitions and actions.
 */
static void pm_state_machine_step(ncp_pm_sm_ctx_t *ctx)
{
    ncp_pm_action_entry_update(ctx->state);

    switch (ctx->state)
    {
        case NCP_PM_SM_STATE_IDLE:
            NCP_LOG_DBG("=================START=================");
            ncp_pm_sm_reset_flag(ctx);
            ncp_pm_sm_wait_event(ctx, NCP_PM_SM_EVENT_START, osaWaitForever_c);
            ncp_pm_sm_set_state(ctx, NCP_PM_SM_STATE_ENTER);
            break;
        case NCP_PM_SM_STATE_ENTER:
            /* Initiates low power handshake by sending SLEEP_ENTER/SLEEP_CFM message
             * and entering critical section. */
            ctx->notify_events = NCP_PM_NOTIFY_EVENT_POST;
            if (ncp_pm_sm_send(ctx, ctx->notify_events) != NCP_PM_STATUS_SUCCESS)
            {
                NCP_LOG_ERR("Failed to enqueue SLEEP message");
                break;
            }
            ncp_pm_enter_critical();
            /* Wait until SLEEP message transmission is completed. */
            ncp_pm_sm_wait_event(ctx, NCP_PM_SM_EVENT_TX_DONE, osaWaitForever_c);
            ncp_pm_sm_set_state(ctx, NCP_PM_SM_STATE_CONFIRM);
            break;
        case NCP_PM_SM_STATE_CONFIRM:
            if (ctx->role == NCP_PM_ROLE_DEVICE)
            {
                /* Handshake is ongoing. Device waits for SLEEP_CFM. */
                ncp_pm_sm_wait_event(ctx, NCP_PM_SM_EVENT_SLEEP_CFM, osaWaitForever_c);
            }
            ncp_pm_sm_set_state(ctx, NCP_PM_SM_STATE_ACK);
            break;
        case NCP_PM_SM_STATE_ACK:
            if (ctx->role == NCP_PM_ROLE_DEVICE)
            {
                /* Sending SLEEP_ACK message */
                ctx->notify_events = NCP_PM_NOTIFY_EVENT_PRE;
                if (ncp_pm_sm_send(ctx, ctx->notify_events) != NCP_PM_STATUS_SUCCESS)
                {
                    NCP_LOG_ERR("Failed to enqueue ACK message");
                    break;
                }
                /* Wait until SLEEP message transmission is completed. */
                ncp_pm_sm_wait_event(ctx, NCP_PM_SM_EVENT_TX_DONE, osaWaitForever_c);
            }
            else if (ctx->role == NCP_PM_ROLE_HOST)
            {
                /* Handshake is ongoing. Host waits for SLEEP_ACK. */
                ncp_pm_sm_wait_event(ctx, NCP_PM_SM_EVENT_SLEEP_ACK, osaWaitForever_c);
            }
            ncp_pm_exit_critical();
            ncp_pm_sm_set_state(ctx, NCP_PM_SM_STATE_FINISH);
            break;
        case NCP_PM_SM_STATE_FINISH:
            /* Handshake complete. Waits for STOP, AWAKE, or TIME_WAIT to determine next step. */
            ncp_pm_sm_wait_event(ctx, NCP_PM_SM_EVENT_STOP | NCP_PM_SM_EVENT_AWAKE | NCP_PM_SM_EVENT_TIME_WAIT, osaWaitForever_c);
            ncp_pm_sm_set_state(ctx, NCP_PM_SM_STATE_EXIT);
            break;
        case NCP_PM_SM_STATE_EXIT:
            if (ctx->role == NCP_PM_ROLE_DEVICE)
            {
                /* Check if TIME_WAIT event is already set, that is, if the low power timer has expired. */
                ctx->time_wait = (ctx->events & NCP_PM_SM_EVENT_TIME_WAIT) ? true : false;
                ctx->notify_events = NCP_PM_NOTIFY_EVENT_PRE;
                if (ctx->events & NCP_PM_SM_EVENT_AWAKE)
                {
                    ctx->notify_events |= NCP_PM_NOTIFY_EVENT_DATA_READY;
                }
                /* Send exit message to host */
                if (ncp_pm_sm_send(ctx, ctx->notify_events) != NCP_PM_STATUS_SUCCESS)
                {
                    NCP_LOG_ERR("Failed to enqueue AWAKE message");
                    break;
                }
                /* Wait for TX_DONE event to confirm exit message was sent */
                ncp_pm_sm_wait_event(ctx, NCP_PM_SM_EVENT_TX_DONE, osaWaitForever_c);
                if (!ctx->time_wait)
                {
                    /* Check if TIME_WAIT event is already set, that is, if the low power timer has expired. */
                    ncp_pm_sm_get_event(ctx, NCP_PM_SM_EVENT_TIME_WAIT, 1);
                    ctx->time_wait = (ctx->events & NCP_PM_SM_EVENT_TIME_WAIT) ? true : false;
                }
            }
            else if (ctx->role == NCP_PM_ROLE_HOST)
            {
                if (!(ctx->events & NCP_PM_SM_EVENT_STOP))
                {
                    /* Check if the host actively wakes up the device */
                    ctx->time_wait = (ctx->events & NCP_PM_SM_EVENT_AWAKE) ? true : false;
                }
            }
            ncp_pm_sm_set_state(ctx, ctx->time_wait ? NCP_PM_SM_STATE_TIME_WAIT : NCP_PM_SM_STATE_IDLE);
            NCP_LOG_DBG("=================STOP==================");
            break;
        case NCP_PM_SM_STATE_TIME_WAIT:
            if (ctx->role == NCP_PM_ROLE_DEVICE)
            {
                /* Release PM policy constraint. */
                ncp_pm_configure_next_lowpower_mode(NCP_PM_STATE_PM1, 0U);
                NCP_LOG_DBG("===============TIME_WAIT===============");
            }
            else if (ctx->role == NCP_PM_ROLE_HOST)
            {
                /* If wake-up is required, perform wake-up actions and wait for exit message from device.
                 * Otherwise, skip waiting and proceed to send data directly. */
                int retry_count = 0;
                int wakeup_timeout_ms;
wakeup_retry:
                if (ncp_pm_wakeup() != NCP_PM_STATUS_SKIP)
                {
                    retry_count++;
                    wakeup_timeout_ms = NCP_PM_WAKEUP_TIMEOUT_MS * retry_count; /* Progressive delay: 25ms, 50ms, 75ms... */
                    NCP_LOG_DBG("Wakeup attempt %d/%d (timeout: %dms)", retry_count, NCP_PM_WAKEUP_MAX_RETRY, wakeup_timeout_ms);
                    if (ncp_pm_sm_wait_event(ctx, NCP_PM_SM_EVENT_STOP, wakeup_timeout_ms) < 0)
                    {
                        NCP_LOG_DBG("Wakeup timeout, no response from device");
                        if (retry_count < NCP_PM_WAKEUP_MAX_RETRY)
                        {
                            goto wakeup_retry;
                        }
                    }
                    else
                    {
                        NCP_LOG_DBG("Wakeup successful");
                    }
                }
                ctx->notify_events = NCP_PM_NOTIFY_EVENT_DATA_READY;
                ncp_pm_sm_send(ctx, ctx->notify_events);
                NCP_LOG_DBG("=================WAKEUP=================");
            }
            ncp_pm_sm_set_state(ctx, NCP_PM_SM_STATE_IDLE);
            break;
        default:
            NCP_LOG_ERR("Unknown state!");
            break;
    }
}

static void ncp_pm_sm_task(void *param)
{
    ncp_pm_sm_ctx_t *ctx = (ncp_pm_sm_ctx_t *)param;
    while (1)
    {
        pm_state_machine_step(ctx);
    }
}

int ncp_pm_sm_init(ncp_pm_role_t role, void *tx_if)
{
    if (s_psm_ctx.initialized)
        return NCP_PM_STATUS_SUCCESS;

    (void)memset(&s_psm_ctx, 0, sizeof(s_psm_ctx));

    s_psm_ctx.role = role;
    s_psm_ctx.state = NCP_PM_SM_STATE_IDLE;
    s_psm_ctx.time_wait = false;
    s_psm_ctx.events = 0;
    s_psm_ctx.tx_if = (ncp_pm_tx_if_t *)tx_if;

    ncp_pm_action_init(s_psm_ctx.role, ncp_pm_sm_post_event);

    if (OSA_EventCreate((osa_event_handle_t)s_psm_ctx.event_handle, 1) != KOSA_StatusSuccess)
        goto fail;

    if (OSA_TaskCreate((osa_task_handle_t)s_psm_ctx.task_handle, OSA_TASK(ncp_pm_sm_task), &s_psm_ctx) != KOSA_StatusSuccess)
    {
        OSA_EventDestroy((osa_event_handle_t)s_psm_ctx.event_handle);
        goto fail;
    }

    s_psm_ctx.initialized = true;
    return NCP_PM_STATUS_SUCCESS;

fail:
    ncp_pm_action_deinit();
    s_psm_ctx.initialized = false;

    NCP_LOG_ERR("ncp_pm_sm_init failed!");
    return NCP_PM_STATUS_ERROR;
}

int ncp_pm_sm_deinit(void)
{
    if (!s_psm_ctx.initialized)
        return NCP_PM_STATUS_SUCCESS;

    OSA_TaskDestroy((osa_task_handle_t)s_psm_ctx.task_handle);
    OSA_EventDestroy((osa_event_handle_t)s_psm_ctx.event_handle);

    ncp_pm_action_deinit();

    s_psm_ctx.initialized = false;
    return NCP_PM_STATUS_SUCCESS;
}

int ncp_pm_sm_reinit(ncp_pm_role_t role, void *tx_if)
{
    int ret = NCP_PM_STATUS_SUCCESS;

    ret = ncp_pm_sm_deinit();
    if (ret != NCP_PM_STATUS_SUCCESS)
    {
        return ret;
    }

    ret = ncp_pm_sm_init(role, tx_if);
    if (ret != NCP_PM_STATUS_SUCCESS)
    {
        return ret;
    }

    return NCP_PM_STATUS_SUCCESS;
}

bool ncp_pm_sm_is_idle(void)
{
    return (s_psm_ctx.state == NCP_PM_SM_STATE_IDLE);
}

bool ncp_pm_sm_is_done(void)
{
    return (s_psm_ctx.state == NCP_PM_SM_STATE_FINISH);
}
