/*
 * Copyright 2022-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "ncp_crc.h"
#include "ncp_tlv_adapter.h"
#include "ncp_adapter.h"
#include "fsl_os_abstraction.h"
#if CONFIG_NCP_USE_ENCRYPT
#include "mbedtls/gcm.h"
#include "ncp_cmd_common.h"
#include "mbedtls_common.h"
#endif
#include "ncp_cmd_system.h"
#include "ncp_utils.h"
#include "ncp_log.h"

NCP_LOG_MODULE_REGISTER(ncp_adapter, CONFIG_LOG_NCP_ADAPTER_LEVEL);

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define NCP_TLV_STATS_INC(x) NCP_STATS_INC(tlvq.x)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern const ncp_intf_ops_t *ncp_intf_get_ops(void);
static void ncp_tlv_process(osa_task_param_t arg);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static ncp_tlv_adapter_t ncp_tlv_adapter;
static uint32_t s_ncp_adap_seqnum = 0;

#if CONFIG_NCP_DEBUG
/* Global variable containing NCP internal statistics */
ncp_stats_t ncp_stats;
#endif

/* NCP adapter TX variable*/
/* NCP adapter event group */
OSA_EVENT_HANDLE_DEFINE(ncp_tx_event_handle);
/* NCP adapter tx data queue */
OSA_MSGQ_HANDLE_DEFINE(ncp_tlv_data_msgq_handle, NCP_TLV_DATA_QUEUE_LENGTH,  sizeof(ncp_tlv_data_qelem_t *));
/* NCP adapter tx ctrl queue */
OSA_MSGQ_HANDLE_DEFINE(ncp_tlv_ctrl_msgq_handle, NCP_TLV_CTRL_QUEUE_LENGTH,  sizeof(ncp_tlv_ctrl_qelem_t *));

/* NCP adapter tx task */
OSA_TASK_HANDLE_DEFINE(ncp_tlv_thread);
OSA_TASK_DEFINE(ncp_tlv_process, PRIORITY_RTOS_TO_OSA((configMAX_PRIORITIES - 3)), 1, NCP_TLV_TX_TASK_STACK_SIZE, 0);

#define NCP_RESET_CONTEXT_AON_ADDR (0x4015C100)
static ncp_reset_callback_t ncp_dev_reset_cb = NULL;
uint32_t ncp_dev_reset_cb_cnt = 0;


static bool ncp_initialized = false;
static const ncp_pm_tx_if_t s_ncp_pm_tx_if = {
    .send_msg = ncp_tlv_ctrl_send,
    .post_event = ncp_tlv_tx_set_event,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

#if CONFIG_NCP_USE_ENCRYPT
int ncp_tlv_adapter_encrypt_init(const uint8_t *key_enc, const uint8_t *key_dec,
                                 const uint8_t *iv_enc, const uint8_t *iv_dec,
                                 uint16_t key_len, uint16_t iv_len)
{
    ncp_tlv_adapter_t *adapter = &ncp_tlv_adapter;
    psa_status_t status;

    NCP_ASSERT(key_enc && key_dec && iv_enc && iv_dec);

    if ((adapter->crypt) || (key_len > NCP_ENDECRYPT_KEY_LEN)
                         || (iv_len > NCP_ENDECRYPT_IV_LEN))
    {
        return NCP_STATUS_ERROR;
    }

    adapter->crypt = (crypt_param_t *)OSA_MemoryAllocate(sizeof(crypt_param_t));
    if (!adapter->crypt)
    {
        return NCP_STATUS_NOMEM;
    }
    memset(adapter->crypt, 0, sizeof(crypt_param_t));

    /* Key attributes */
    psa_key_attributes_t attr = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&attr, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attr, PSA_ALG_GCM);
    psa_set_key_type(&attr, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attr, key_len * 8);

    /* Import encrypt key */
    status = psa_import_key(&attr, key_enc, key_len, &adapter->crypt->key_enc_id);
    if (status != PSA_SUCCESS) {
        OSA_MemoryFree(adapter->crypt);
        adapter->crypt = NULL;
        return NCP_STATUS_ERROR;
    }

    /* Import decrypt key */
    status = psa_import_key(&attr, key_dec, key_len, &adapter->crypt->key_dec_id);
    if (status != PSA_SUCCESS) {
        psa_destroy_key(adapter->crypt->key_enc_id);
        OSA_MemoryFree(adapter->crypt);
        adapter->crypt = NULL;
        return NCP_STATUS_ERROR;
    }

    memcpy(adapter->crypt->iv_enc, iv_enc, iv_len);
    memcpy(adapter->crypt->iv_dec, iv_dec, iv_len);
    adapter->crypt->iv_len = iv_len;

    return NCP_STATUS_SUCCESS;
}

int ncp_tlv_adapter_encrypt_deinit(void)
{
    ncp_tlv_adapter_t *adapter = &ncp_tlv_adapter;

    if (!adapter->crypt)
    {
        return NCP_STATUS_ERROR;
    }

    psa_destroy_key(adapter->crypt->key_enc_id);
    psa_destroy_key(adapter->crypt->key_dec_id);

    (void) memset(adapter->crypt, 0, sizeof(crypt_param_t));
    (void) OSA_MemoryFree(adapter->crypt);
    adapter->crypt = NULL;

    return NCP_STATUS_SUCCESS;
}

int ncp_tlv_adapter_encrypt_enable(void)
{
    ncp_tlv_adapter_t *adapter = &ncp_tlv_adapter;
    if (!adapter->crypt)
    {
        return NCP_STATUS_ERROR;
    }

    adapter->crypt->flag = 1;
    return NCP_STATUS_SUCCESS;
}

int ncp_tlv_adapter_encrypt_disable(void)
{
    ncp_tlv_adapter_t *adapter = &ncp_tlv_adapter;
    if (adapter->crypt)
    {
        adapter->crypt->flag = 0;
    }
    return NCP_STATUS_SUCCESS;
}

int ncp_tlv_adapter_is_encrypt_mode(void)
{
    ncp_tlv_adapter_t *adapter = &ncp_tlv_adapter;
    return adapter->crypt && adapter->crypt->flag;
}

static ncp_status_t ncp_tlv_encrypt(unsigned char *input, unsigned char *output, size_t input_len)
{
    ncp_tlv_adapter_t *adapter = &ncp_tlv_adapter;
    psa_status_t status;
    size_t output_len = 0;
    size_t aead_output_len = input_len + NCP_GCM_TAG_LEN;

    NCP_ASSERT(input && output);

    if (!adapter->crypt || !adapter->crypt->flag) {
        return NCP_STATUS_ERROR;
    }

    status = psa_aead_encrypt(adapter->crypt->key_enc_id,
                              PSA_ALG_GCM,
                              adapter->crypt->iv_enc, adapter->crypt->iv_len,
                              NULL, 0,
                              input, input_len,
                              output, aead_output_len, &output_len);
    NCP_LOG_DBG("%s input_len:%u, aead_output_len:%u, output_len:%u", __FUNCTION__, input_len, aead_output_len, output_len);
    if (status != PSA_SUCCESS || aead_output_len != output_len) {
        return NCP_STATUS_ERROR;
    }

    return NCP_STATUS_SUCCESS;
}

static ncp_status_t ncp_tlv_decrypt(unsigned char *input, size_t input_len)
{
    ncp_tlv_adapter_t *adapter = &ncp_tlv_adapter;
    psa_status_t status;
    size_t output_len = 0;

    if (!adapter->crypt || !adapter->crypt->flag) {
        return NCP_STATUS_ERROR;
    }

    /* AEAD input = ciphertext length + tag length */
    size_t plaintext_len = input_len - NCP_GCM_TAG_LEN;

    status = psa_aead_decrypt(adapter->crypt->key_dec_id,
                              PSA_ALG_GCM,
                              adapter->crypt->iv_dec, adapter->crypt->iv_len,
                              NULL, 0,
                              input, input_len,
                              input, plaintext_len, &output_len);
    NCP_LOG_DBG("%s input_len:%u, plaintext_len:%u, output_len:%u", __FUNCTION__, input_len, plaintext_len, output_len);
    if (status != PSA_SUCCESS || output_len != plaintext_len) {
        return NCP_STATUS_ERROR;
    }

    return NCP_STATUS_SUCCESS;
}
#endif  /* CONFIG_NCP_USE_ENCRYPT */

static bool ncp_crypt_is_needed(void *tlv_buf, size_t tlv_sz)
{
#if CONFIG_NCP_USE_ENCRYPT
    if(ncp_tlv_adapter.crypt && ncp_tlv_adapter.crypt->flag && tlv_sz > TLV_CMD_HEADER_LEN)
    {
        struct _NCP_CMD_HEADER *cmd_hdr = (struct _NCP_CMD_HEADER *)tlv_buf;
        if (ncp_cmd_is_data_cmd(cmd_hdr->cmd) == 0)
        {
            return true;
        }
    }
#endif
    return false;
}

/**
 * @brief Get the TLV adapter instance
 *
 * @return Pointer to the TLV adapter instance
 * @retval Non-NULL Pointer to the valid TLV adapter instance
 * @retval NULL     Should never occur in normal operation
 */
const ncp_tlv_adapter_t *ncp_tlv_adapter_get(void)
{
    return &ncp_tlv_adapter;
}

ncp_reset_context_t *ncp_get_reset_context(void)
{
    const ncp_tlv_adapter_t *tlv_adap = ncp_tlv_adapter_get();

    return (tlv_adap ? tlv_adap->reset_context_aon : NULL);
}


/**
 * @brief Set TX event for TLV adapter
 *
 * This function sets an event flag to notify the TX task about pending
 * operations. The TX task waits on these events and processes them
 * accordingly (e.g., data ready, control messages).
 *
 * @param[in] event Event bitmask to be set (NCP_TX_EVENT_*)
 *                  - NCP_TX_EVENT_DATA_READY: Data queued for transmission
 *                  - NCP_TX_EVENT_CTRL_PRE:   Control message before data
 *                  - NCP_TX_EVENT_CTRL_POST:  Control message after data
 *
 * @warning Ensure ncp_tx_event_handle is initialized before calling this function.
 */
void ncp_tlv_tx_set_event(uint32_t event)
{
    /* Set the event flag to wake up TX task */
    if (OSA_EventSet(ncp_tx_event_handle, event) != KOSA_StatusSuccess)
    {
        NCP_LOG_ERR("[%s] Failed to set event: 0x%08x", __func__, event);
        NCP_TLV_STATS_INC(err);
    }
    else
    {
        NCP_LOG_DBG("[%s] Event set successfully: 0x%08x", __func__, event);
    }
}

/**
 * @brief Enqueue TLV data message for transmission
 *
 * This function enqueues a TLV data queue element to the transmission queue
 * and sets the data ready event to notify the TX task.
 *
 * @param[in] qelem Pointer to the data queue element to be enqueued
 *
 * @retval NCP_STATUS_SUCCESS Successfully enqueued the data message
 * @retval NCP_STATUS_ERROR   Failed to enqueue the data message
 */
static ncp_status_t ncp_tlv_tx_data_enqueue(ncp_tlv_data_qelem_t *qelem)
{
    if (OSA_MsgQPutBlock(ncp_tlv_data_msgq_handle, &qelem, osaWaitForever_c) != KOSA_StatusSuccess)
    {
        NCP_LOG_ERR("[%s] Failed to enqueue TLV data: size=%lu", __func__, qelem->tlv_sz);
        NCP_TLV_STATS_INC(err);
        NCP_TLV_STATS_INC(drop);
        return NCP_STATUS_ERROR;
    }

    NCP_TLV_STATS_INC(tx);
    OSA_EventSet(ncp_tx_event_handle, NCP_TX_EVENT_DATA_READY);

    NCP_LOG_DBG("enqueue data success");

    return NCP_STATUS_SUCCESS;
}

/**
 * @brief Enqueue TLV control message for transmission
 * @param qelem Control queue element pointer
 * @return Status code
 */
static ncp_status_t ncp_tlv_tx_ctrl_enqueue(ncp_tlv_ctrl_qelem_t *qelem)
{
    if (OSA_MsgQPutBlock(ncp_tlv_ctrl_msgq_handle, &qelem, osaWaitForever_c) != KOSA_StatusSuccess)
    {
        NCP_LOG_ERR("[%s] Failed to enqueue TLV control message: qelem=%p, size=%lu, event=0x%08x, seqnum=%u",
                    __func__, qelem, qelem->ctrl_sz, qelem->event, qelem->seqnum);
        NCP_TLV_STATS_INC(err);
        NCP_TLV_STATS_INC(drop);
        return NCP_STATUS_ERROR;
    }

    NCP_TLV_STATS_INC(tx);
    OSA_EventSet(ncp_tx_event_handle, qelem->event);

    NCP_LOG_DBG("enqueue ctrl success, event: 0x%08x, seqnum: %u", qelem->event, qelem->seqnum);

    return NCP_STATUS_SUCCESS;
}

/**
 * @brief Free TLV data queue element
 * @param qbuf Pointer to queue element pointer
 */
static void ncp_tlv_free_data_elmt(ncp_tlv_data_qelem_t **qbuf)
{
    if ((*qbuf)->is_ref && (*qbuf)->ref_free_cb)
        (*qbuf)->ref_free_cb((*qbuf)->ref_buf);
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) && defined(DCP_USE_DCACHE) && (DCP_USE_DCACHE == 1U)
    OSA_MemoryFreeAlign(*qbuf);
#else
    OSA_MemoryFree(*qbuf);
#endif
    *qbuf = NULL;
}

static void ncp_tlv_tx_ctrl_queue_flush(void)
{
    ncp_tlv_ctrl_qelem_t *msg = NULL;

    while (OSA_MsgQGet(ncp_tlv_ctrl_msgq_handle, &msg, osaWaitNone_c) == KOSA_StatusSuccess)
    {
        if (msg)
        {
            NCP_LOG_DBG("%s: flush control message: qelem=%p, size=%lu, event=0x%08x, seqnum=%u",
                        __FUNCTION__, msg, msg->ctrl_sz, msg->event, msg->seqnum);
            OSA_MemoryFree(msg);
        }
    }

    OSA_EventClear((osa_event_handle_t)ncp_tx_event_handle, NCP_TX_EVENT_CTRL_PRE | NCP_TX_EVENT_CTRL_POST);
}

/**
 * @brief Dequeue and send all TLV data messages
 */
static void ncp_tlv_tx_data_dequeue(void)
{
    ncp_tlv_data_qelem_t *msg = NULL;

    do {
        if (ncp_tlv_adapter.pm_ops->tx_data_action())
        {
            NCP_LOG_DBG("TX data action done");
            break;
        }

        if (OSA_MsgQGet(ncp_tlv_data_msgq_handle, &msg, osaWaitNone_c) != KOSA_StatusSuccess)
        {
            break;
        }

        NCP_LOG_DBG("%s: send Data =%p: tlv_buf=%p tlv_sz=%lu",
                    __FUNCTION__, msg, msg->tlv_buf, msg->tlv_sz);

        ncp_tlv_adapter.intf_ops->send(msg->tlv_buf, msg->tlv_sz, NULL);

        if (ncp_dev_reset_cb)
        {
            if (ncp_dev_reset_cb(msg->tlv_buf) == NCP_STATUS_SUCCESS)
            {
                NCP_LOG_DBG("Resetting power state machine");
                if (ncp_tlv_adapter.pm_ops->reset() == NCP_STATUS_SUCCESS)
                {
                    NCP_LOG_DBG("Power state machine reset complete");
                    ncp_tlv_tx_ctrl_queue_flush();
                }
            }

            if (ncp_tlv_adapter_is_encrypt_mode())
            {
                ncp_tlv_adapter_encrypt_deinit();
            }
        }

        /* free element */
        ncp_tlv_free_data_elmt(&msg);
    } while (1);
}

/**
 * @brief Dequeue and send all TLV control messages
 */
static void ncp_tlv_tx_ctrl_dequeue(void)
{
    ncp_tlv_ctrl_qelem_t *msg = NULL;

    while (OSA_MsgQGet(ncp_tlv_ctrl_msgq_handle, &msg, osaWaitNone_c) == KOSA_StatusSuccess)
    {
        NCP_LOG_DBG("%s: send Control msg=%p: ctrl_buf=%p ctrl_sz=%lu",
                    __FUNCTION__, msg, msg->ctrl_buf, msg->ctrl_sz);
        ncp_tlv_adapter.intf_ops->send(msg->ctrl_buf, msg->ctrl_sz, NULL);
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) && defined(DCP_USE_DCACHE) && (DCP_USE_DCACHE == 1U)
        OSA_MemoryFreeAlign(msg);
#else
        OSA_MemoryFree(msg);
#endif
        msg = NULL;
    }

    ncp_tlv_adapter.pm_ops->tx_ctrl_action();
}

/**
 * @brief TLV adapter TX task main loop
 * @param arg Task parameter (unused)
 */
static void ncp_tlv_process(osa_task_param_t arg)
{
    uint32_t events = 0U;

    while (1)
    {
        (void)OSA_EventWait(ncp_tx_event_handle, NCP_TX_EVENT_ALL, 0, osaWaitForever_c, &events);

        ncp_tlv_adapter.pm_ops->enter_critical();

        /** Event: handle control messages prior to sending data frames */
        if (events & NCP_TX_EVENT_CTRL_PRE)
        {
            NCP_LOG_DBG("Processing CTRL_PRE event");
            ncp_tlv_tx_ctrl_dequeue();
        }

        if (events & NCP_TX_EVENT_DATA_READY)
        {
            NCP_LOG_DBG("Processing DATA event");
            ncp_tlv_tx_data_dequeue();
        }

        /** Event: handle control messages after data frames have been sent */
        if (events & NCP_TX_EVENT_CTRL_POST)
        {
            NCP_LOG_DBG("Processing CTRL_POST event");
            ncp_tlv_tx_ctrl_dequeue();
        }

        ncp_tlv_adapter.pm_ops->exit_critical();
    }
}

static ncp_status_t ncp_tlv_tx_init(void)
{
    struct {
        unsigned event_group : 1;
        unsigned data_msgq   : 1;
        unsigned ctrl_msgq   : 1;
        unsigned tx_task     : 1;
    } created = {0};

    ncp_status_t status = NCP_STATUS_ERROR;

    do {
        if (OSA_EventCreate((osa_event_handle_t)ncp_tx_event_handle, 1) != KOSA_StatusSuccess) {
            NCP_LOG_ERR("ncp tx event group create fail");
            break;
        }
        created.event_group = 1;

        if (OSA_MsgQCreate((osa_msgq_handle_t)ncp_tlv_data_msgq_handle,
                           NCP_TLV_DATA_QUEUE_LENGTH,
                           sizeof(ncp_tlv_data_qelem_t *)) != KOSA_StatusSuccess) {
            NCP_LOG_ERR("ncp tlv data msg queue create fail");
            break;
        }
        created.data_msgq = 1;

        if (OSA_MsgQCreate((osa_msgq_handle_t)ncp_tlv_ctrl_msgq_handle,
                           NCP_TLV_CTRL_QUEUE_LENGTH,
                           sizeof(ncp_tlv_ctrl_qelem_t *)) != KOSA_StatusSuccess) {
            NCP_LOG_ERR("ncp tlv ctrl msg queue create fail");
            break;
        }
        created.ctrl_msgq = 1;

        if (OSA_TaskCreate((osa_task_handle_t)ncp_tlv_thread,
                           OSA_TASK(ncp_tlv_process),
                           NULL) != KOSA_StatusSuccess) {
            NCP_LOG_ERR("ncp tlv process task create fail");
            break;
        }
        created.tx_task = 1;

        NCP_LOG_DBG("ncp tx init success");
        return NCP_STATUS_SUCCESS;

    } while (0);

    if (created.tx_task) {
        OSA_TaskDestroy((osa_task_handle_t)ncp_tlv_thread);
    }
    if (created.ctrl_msgq) {
        OSA_MsgQDestroy((osa_msgq_handle_t)ncp_tlv_ctrl_msgq_handle);
    }
    if (created.data_msgq) {
        OSA_MsgQDestroy((osa_msgq_handle_t)ncp_tlv_data_msgq_handle);
    }
    if (created.event_group) {
        OSA_EventDestroy((osa_event_handle_t)ncp_tx_event_handle);
    }

    return status;
}

static void ncp_tlv_tx_deinit(void)
{
    ncp_tlv_data_qelem_t *data_msg = NULL;
    ncp_tlv_ctrl_qelem_t *ctrl_msg = NULL;

    if (OSA_TaskDestroy((osa_task_handle_t)ncp_tlv_thread) != KOSA_StatusSuccess) {
        NCP_LOG_ERR("ncp adapter tx deinit task fail");
    }

    while (OSA_MsgQGet(ncp_tlv_data_msgq_handle, &data_msg, 0) == KOSA_StatusSuccess) {
        /* free element */
        ncp_tlv_free_data_elmt(&data_msg);
    }
    NCP_LOG_DBG("ncp adapter tx deinit: data queue flushed");

    if (OSA_MsgQDestroy((osa_msgq_handle_t)ncp_tlv_data_msgq_handle) != KOSA_StatusSuccess) {
        NCP_LOG_ERR("ncp adapter tx deinit: data queue destroy failed");
    }

    while (OSA_MsgQGet(ncp_tlv_ctrl_msgq_handle, &ctrl_msg, 0) == KOSA_StatusSuccess)
    {
        OSA_MemoryFree(ctrl_msg);
        ctrl_msg = NULL;
    }
    NCP_LOG_DBG("ncp adapter tx deinit: ctrl queue flushed");

    if (OSA_MsgQDestroy((osa_msgq_handle_t)ncp_tlv_ctrl_msgq_handle) != KOSA_StatusSuccess) {
        NCP_LOG_ERR("ncp adapter tx deinit: ctrl queue destroy failed");
    }

    if (OSA_EventDestroy((osa_event_handle_t)ncp_tx_event_handle) != KOSA_StatusSuccess) {
        NCP_LOG_ERR("ncp adapter tx deinit: event group destroy failed");
    }

    NCP_LOG_DBG("ncp adapter tx deinit: completed");
}

void ncp_adapter_set_cb(ncp_reset_callback_t dev_reset_cb)
{
    NCP_LOG_DBG("%s: enter cb_in=%p cb=%p cnt=%u", __FUNCTION__, dev_reset_cb, ncp_dev_reset_cb, ncp_dev_reset_cb_cnt);
    if (dev_reset_cb != NULL)
    {
        ncp_dev_reset_cb_cnt++;
        ncp_dev_reset_cb = dev_reset_cb;
    }
    else
    {
        if (ncp_dev_reset_cb_cnt)
            ncp_dev_reset_cb_cnt--;
        if (ncp_dev_reset_cb_cnt == 0)
            ncp_dev_reset_cb = dev_reset_cb;
    }
    NCP_LOG_DBG("%s: exit cb_in=%p cb=%p cnt=%u", __FUNCTION__, dev_reset_cb, ncp_dev_reset_cb, ncp_dev_reset_cb_cnt);
}

ncp_status_t ncp_adapter_init(int role)
{
    ncp_status_t status = NCP_STATUS_SUCCESS;

    if (ncp_initialized == true)
    {
        return status;
    }

    ncp_tlv_adapter.reset_context_aon = (ncp_reset_context_t *)(NCP_RESET_CONTEXT_AON_ADDR);

    /* Init CRC32 */
    ncp_tlv_chksum_init();
    status = ncp_tlv_tx_init();
    if (status != NCP_STATUS_SUCCESS)
    {
        return status;
    }

    /* Init interface */
    ncp_tlv_adapter.intf_ops = ncp_intf_get_ops();
    status = (ncp_status_t)ncp_tlv_adapter.intf_ops->init(NULL);
    if (status != NCP_STATUS_SUCCESS)
    {
        NCP_LOG_ERR("ncp adapter init fail");
        ncp_tlv_tx_deinit();
        return status;
    }

    /* Init PM state machine */
    ncp_tlv_adapter.pm_ops = ncp_pm_get_ops();
    status = (ncp_status_t)ncp_tlv_adapter.pm_ops->init((ncp_pm_role_t)role, &s_ncp_pm_tx_if);
    if (status != NCP_STATUS_SUCCESS)
    {
        NCP_LOG_ERR("ncp pm init fail");
        return status;
    }

    ncp_initialized = true;

    return status;
}

ncp_status_t ncp_adapter_deinit(void)
{
    ncp_status_t status = NCP_STATUS_SUCCESS;

    ncp_initialized = false;

    status = (ncp_status_t)ncp_tlv_adapter.pm_ops->deinit();
    ncp_tlv_adapter.pm_ops = NULL;
    /* Deinit interface */
    status = (ncp_status_t)ncp_tlv_adapter.intf_ops->deinit(NULL);

    ncp_tlv_adapter.intf_ops = NULL;
    ncp_tlv_tx_deinit();
    return status;
}

void ncp_tlv_install_handler(uint8_t class, void *func_cb)
{
    NCP_ASSERT((uint8_t)NCP_MAX_CLASS > class);
    NCP_ASSERT(NULL != func_cb);

    ncp_tlv_adapter.tlv_handler[class] = (tlv_callback_t)func_cb;
}

void ncp_tlv_uninstall_handler(uint8_t class)
{
    NCP_ASSERT((uint8_t)NCP_MAX_CLASS > class);

    ncp_tlv_adapter.tlv_handler[class] = NULL;
}

void ncp_tlv_dispatch(void *tlv, size_t tlv_sz)
{
    ncp_status_t status = NCP_STATUS_SUCCESS;
    uint32_t local_checksum = 0, remote_checksum = 0;
    uint8_t class = 0;

    NCP_TLV_STATS_INC(rx);

    NCP_LOG_DBG("%s: tlv=%p tlv_sz=%lu", __FUNCTION__, tlv, tlv_sz);

    if (ncp_tlv_adapter.pm_ops->rx_action(tlv, tlv_sz))
    {
        return;
    }

    NCP_LOG_DBG("Receive TLV command, dispatch it!");

    /* check CRC */
    remote_checksum = NCP_GET_PEER_CHKSUM((uint8_t *)tlv, tlv_sz);
    local_checksum  = ncp_tlv_chksum(tlv, (uint16_t)tlv_sz);
    if (remote_checksum != local_checksum)
    {
        status = NCP_STATUS_CHKSUMERR;
        /* Send NCP_EVENT_CRC_CHECK_ERROR  */
        uint8_t crc_check_error_event[TLV_CMD_HEADER_LEN] = {0x0f, 0x00, 0x32, 0x40, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        ncp_tlv_send(crc_check_error_event, TLV_CMD_HEADER_LEN);

        NCP_TLV_STATS_INC(err);
        NCP_TLV_STATS_INC(drop);
        NCP_LOG_ERR("Checksum validation failed! local = 0x%08x, remote = 0x%08x", local_checksum, remote_checksum);
        return;
    }

    if (ncp_crypt_is_needed(tlv, tlv_sz))
    {
#if CONFIG_NCP_USE_ENCRYPT
        struct _NCP_CMD_HEADER *cmd_hdr = (struct _NCP_CMD_HEADER *)tlv;
        status = ncp_tlv_decrypt((unsigned char *)tlv + TLV_CMD_HEADER_LEN, tlv_sz - TLV_CMD_HEADER_LEN);
        if (status != NCP_STATUS_SUCCESS)
        {
            NCP_TLV_STATS_INC(err);
            NCP_TLV_STATS_INC(drop);
            NCP_LOG_ERR("ncp tlv decrypt err %d", (int)status);
            return;
        }
        /* Remove the tag length */
        tlv_sz -= NCP_GCM_TAG_LEN;
        cmd_hdr->size -= NCP_GCM_TAG_LEN;
#endif /* CONFIG_NCP_USE_ENCRYPT */
    }

    /* TLV command class */
    class = NCP_GET_CLASS(*((uint32_t *)tlv));
    if (ncp_tlv_adapter.tlv_handler[class])
        ncp_tlv_adapter.tlv_handler[class](tlv, tlv_sz, status);
}

/**
 * @brief Send TLV control message
 * @param event Event code
 * @param ctrl_buf Control buffer pointer
 * @param ctrl_sz Control buffer size
 * @return Status code
 */
int ncp_tlv_ctrl_send(uint32_t event, void *ctrl_buf, uint16_t ctrl_sz)
{
    ncp_status_t status = NCP_STATUS_SUCCESS;
    ncp_tlv_ctrl_qelem_t *qelem = NULL;
    uint8_t *qbuf = NULL;
    uint16_t qlen = 0;
    uint32_t chksum = 0;
    uint8_t *chksum_buf = NULL;

    qlen = sizeof(ncp_tlv_ctrl_qelem_t) + ctrl_sz + NCP_CHKSUM_LEN;
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) && defined(DCP_USE_DCACHE) && (DCP_USE_DCACHE == 1U)
    /* Allocate aligned buffers with sufficient trailing memory for safe clean/invalidate */
    qbuf = (uint8_t *)OSA_MemoryAllocateAlign(DCACHE_ALIGNED_SIZE(qlen), DCACHE_LINESIZE_BYTE);
#else
    qbuf = (uint8_t *)OSA_MemoryAllocate(qlen);
#endif
    if (!qbuf)
    {
        NCP_TLV_STATS_INC(err);
        NCP_TLV_STATS_INC(drop);
        NCP_LOG_ERR("%s: failed to allocate memory for tlv ctrl queue element qlen=%u", __FUNCTION__, qlen);
        return NCP_STATUS_NOMEM;
    }

    qelem = (ncp_tlv_ctrl_qelem_t *)qbuf;
    qelem->ctrl_buf = qbuf + sizeof(ncp_tlv_ctrl_qelem_t);
    qelem->event = event;
    qelem->seqnum = s_ncp_adap_seqnum;
    qelem->ctrl_sz = ctrl_sz + NCP_CHKSUM_LEN;

    if (ctrl_buf != NULL && ctrl_sz > 0)
    {
        (void)memcpy(qelem->ctrl_buf, ctrl_buf, ctrl_sz);
    }

    chksum = ncp_tlv_chksum(qelem->ctrl_buf, ctrl_sz);
    chksum_buf = qelem->ctrl_buf + ctrl_sz;
    for (int i = 0; i < sizeof(chksum); i++) {
        chksum_buf[i] = (uint8_t)(chksum >> (8 * i));
    }

    NCP_LOG_DBG("%s: ctrl_buf=%p ctrl_sz=%lu", __FUNCTION__, qelem->ctrl_buf, qelem->ctrl_sz);
    status = ncp_tlv_tx_ctrl_enqueue(qelem);
    if(status != NCP_STATUS_SUCCESS)
    {
        NCP_TLV_STATS_INC(err);
        NCP_TLV_STATS_INC(drop);
        NCP_LOG_ERR("ncp tlv ctrl enqueue element fail");
        if (qbuf)
        {
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) && defined(DCP_USE_DCACHE) && (DCP_USE_DCACHE == 1U)
            OSA_MemoryFreeAlign(qbuf);
#else
            OSA_MemoryFree(qbuf);
#endif
            qbuf = NULL;
        }

        return status;
    }

    s_ncp_adap_seqnum++;
    return status;
}

/**
 * @brief Send TLV data message (copy mode)
 *
 * qbuf_len = sizeof(ncp_tlv_data_qelem_t) + sdio_intf_head + tlv_sz + NCP_CHKSUM_LEN
 * qbuf_tlv = qbuf + sizeof(ncp_tlv_data_qelem_t)
 * memcpy_buf = qbuf + sizeof(ncp_tlv_data_qelem_t) + sdio_intf_head
 * chksum_buf = qbuf + sizeof(ncp_tlv_data_qelem_t) + sdio_intf_head + tlv_sz
 * sdio_intf_head: reserved length for sdio interface header
 *
 * @param tlv_buf TLV buffer pointer
 * @param tlv_sz TLV buffer size
 * @return Status code
 */
ncp_status_t ncp_tlv_send(void *tlv_buf, uint16_t tlv_sz)
{
    ncp_status_t status = NCP_STATUS_SUCCESS;
    ncp_tlv_data_qelem_t *qelem = NULL;
    uint8_t *qbuf = NULL;
    uint8_t *chksum_buf = NULL;
    uint16_t qlen = 0;
    uint32_t chksum = 0;

    qlen = sizeof(ncp_tlv_data_qelem_t) + tlv_sz
#if CONFIG_NCP_USE_ENCRYPT
           + NCP_GCM_TAG_LEN
#endif
           + NCP_CHKSUM_LEN;

#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) && defined(DCP_USE_DCACHE) && (DCP_USE_DCACHE == 1U)
    /* Allocate aligned buffers with sufficient trailing memory for safe clean/invalidate */
    qbuf = (uint8_t *)OSA_MemoryAllocateAlign(DCACHE_ALIGNED_SIZE(qlen), DCACHE_LINESIZE_BYTE);
#else
    qbuf = (uint8_t *)OSA_MemoryAllocate(qlen);
#endif
    if (!qbuf)
    {
        NCP_LOG_ERR("%s: failed to allocate memory for tlv queue element qlen=%u", __FUNCTION__, qlen);
        status = NCP_STATUS_NOMEM;
        goto error_exit;
    }

    qelem = (ncp_tlv_data_qelem_t *)qbuf;
    qelem->tlv_sz = tlv_sz + NCP_CHKSUM_LEN;
    qelem->is_ref = 0;
    qelem->tlv_buf = qbuf + sizeof(ncp_tlv_data_qelem_t);

    if (ncp_crypt_is_needed(tlv_buf, tlv_sz))
    {
#if CONFIG_NCP_USE_ENCRYPT
        (void)memcpy(qelem->tlv_buf, tlv_buf, TLV_CMD_HEADER_LEN);
        NCP_COMMAND *header = (NCP_COMMAND *)qelem->tlv_buf;
        status = ncp_tlv_encrypt((unsigned char *)tlv_buf + TLV_CMD_HEADER_LEN,
                        (unsigned char *)qelem->tlv_buf + TLV_CMD_HEADER_LEN,
                        tlv_sz - TLV_CMD_HEADER_LEN);
        if (status != NCP_STATUS_SUCCESS)
        {
            NCP_LOG_ERR("ncp tlv encrypt err %d", (int)status);
            status = NCP_STATUS_ERROR;
            goto error_exit;
        }

        /* Appending tag after the encrypted data */
        qelem->tlv_sz += NCP_GCM_TAG_LEN;
        tlv_sz += NCP_GCM_TAG_LEN;
        header->size += NCP_GCM_TAG_LEN;
        NCP_LOG_DBG("%s tlv_sz:%u", __FUNCTION__, tlv_sz);
#endif
    }
    else
    {
        (void)memcpy(qelem->tlv_buf, tlv_buf, tlv_sz);
    }

    chksum = ncp_tlv_chksum(qelem->tlv_buf, (uint16_t)tlv_sz);
    chksum_buf = qelem->tlv_buf + tlv_sz;
    for (int i = 0; i < sizeof(chksum); i++) {
        chksum_buf[i] = (uint8_t)(chksum >> (8 * i));
    }

    NCP_LOG_DBG("%s: tlv_buf=%p tlv_sz=%lu", __FUNCTION__, qelem->tlv_buf, qelem->tlv_sz);
#if CONFIG_WIFI_IO_DUMP
    NCP_LOG_ERR("%s: qbuf %p %u (%u %p %ld)", __FUNCTION__, qbuf, qlen, qelem->is_ref, qelem->tlv_buf, qelem->tlv_sz);
    ncp_dump_hex(qelem->tlv_buf, MIN(qelem->tlv_sz, 128));
#endif

    status = ncp_tlv_tx_data_enqueue(qelem);
    if(status != NCP_STATUS_SUCCESS)
    {
        NCP_LOG_ERR("ncp tlv data enque element fail");
        status = NCP_STATUS_ERROR;
        goto error_exit;
    }

    return status;

error_exit:
    NCP_TLV_STATS_INC(err);
    NCP_TLV_STATS_INC(drop);
    if (qbuf != NULL)
    {
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) && defined(DCP_USE_DCACHE) && (DCP_USE_DCACHE == 1U)
        OSA_MemoryFreeAlign(qbuf);
#else
        OSA_MemoryFree(qbuf);
#endif
        qbuf = NULL;
    }

    return status;
}

/* NOTE: Reference send is not supported with DCP dcache aligned buffer, please use ncp_tlv_send instead */
ncp_status_t ncp_tlv_ref_send(void *tlv_buf, uint16_t tlv_sz, uint32_t is_ref,
                              void (*ref_free_cb)(void *buf), void *ref_buf)
{
    ncp_status_t status = NCP_STATUS_SUCCESS;
    ncp_tlv_data_qelem_t *qelem = NULL;
    uint8_t *qbuf = NULL;
    uint8_t *chksum_buf = NULL;
    uint16_t qlen = 0;
    uint32_t chksum = 0;

    if (is_ref == 0)
        return ncp_tlv_send(tlv_buf, tlv_sz);
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U) && defined(DCP_USE_DCACHE) && (DCP_USE_DCACHE == 1U)
    else
    {
        NCP_LOG_ERR("Reference send not supported with dcache!");
        return NCP_STATUS_ERROR;
    }
#endif

    qlen = sizeof(ncp_tlv_data_qelem_t);
    qbuf = (uint8_t *)OSA_MemoryAllocate(qlen);
    if (!qbuf)
    {
        NCP_LOG_ERR("%s: failed to allocate memory for tlv queue element qlen=%u", __FUNCTION__, qlen);
        status = NCP_STATUS_NOMEM;
        goto error_exit;
    }

    qelem = (ncp_tlv_data_qelem_t *)qbuf;
    qelem->tlv_sz = tlv_sz + NCP_CHKSUM_LEN;
    qelem->is_ref = 1;
    qelem->ref_buf = ref_buf;
    qelem->ref_free_cb = ref_free_cb;
    qelem->tlv_buf = (uint8_t *)tlv_buf;

    if (ncp_crypt_is_needed(tlv_buf, tlv_sz))
    {
#if CONFIG_NCP_USE_ENCRYPT
        NCP_COMMAND *header = (NCP_COMMAND *)tlv_buf;
        status = ncp_tlv_encrypt((unsigned char *)tlv_buf + TLV_CMD_HEADER_LEN,
                        (unsigned char *)qelem->tlv_buf + TLV_CMD_HEADER_LEN,
                        tlv_sz - TLV_CMD_HEADER_LEN);
        if (status != NCP_STATUS_SUCCESS)
        {
            NCP_LOG_ERR("ncp tlv encrypt err %d", (int)status);
            status = NCP_STATUS_ERROR;
            goto error_exit;
        }
        /* Appending tag after the encrypted data */
        qelem->tlv_sz += NCP_GCM_TAG_LEN;
        tlv_sz += NCP_GCM_TAG_LEN;
        header->size += NCP_GCM_TAG_LEN;
        NCP_LOG_DBG("%s tlv_sz:%u", __FUNCTION__, tlv_sz);
#endif /* CONFIG_NCP_USE_ENCRYPT */
    }

    chksum = ncp_tlv_chksum(qelem->tlv_buf, (uint16_t)tlv_sz);
    chksum_buf = qelem->tlv_buf + tlv_sz;
    for (int i = 0; i < sizeof(chksum); i++) {
        chksum_buf[i] = (uint8_t)(chksum >> (8 * i));
    }
#if CONFIG_WIFI_IO_DUMP
    NCP_LOG_ERR("%s: qbuf %p %u (%u %p %ld)", __FUNCTION__, qbuf, qlen, qelem->is_ref, qelem->tlv_buf, qelem->tlv_sz);
    dump_hex(qelem->tlv_buf, MIN(qelem->tlv_sz, 128));
#endif

    status = ncp_tlv_tx_data_enqueue(qelem);
    if (status != NCP_STATUS_SUCCESS)
    {
        NCP_LOG_ERR("%s: ncp tlv enque element fail", __FUNCTION__);
        status = NCP_STATUS_ERROR;
        goto error_exit;
    }

    return status;

error_exit:
    NCP_TLV_STATS_INC(err);
    NCP_TLV_STATS_INC(drop);
    if (qbuf)
    {
        OSA_MemoryFree(qbuf);
        qbuf = NULL;
    }

    return status;
}

int ncp_tlv_data_queue_empty()
{
    return !OSA_MsgQAvailableMsgs(ncp_tlv_data_msgq_handle);
}
