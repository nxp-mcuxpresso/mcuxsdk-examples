/*!\file ncp_tlv_adapter.h
 *\brief This file provides NCP TLV adapter interfaces.
 *
 */
/*
 * Copyright 2024 - 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef __NCP_TLV_ADAPTER_H__
#define __NCP_TLV_ADAPTER_H__

#include "ncp_pm.h"
#include "ncp_intf_pm.h"
#include "ncp_adapter.h"
#include "fsl_os_abstraction.h"
#include "psa/crypto_types.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* define whether use encryption communication, temporarily locate it here */
#define CONFIG_NCP_USE_ENCRYPT                    1
#if CONFIG_NCP_USE_ENCRYPT
#define CONFIG_NCP_MBEDTLS_DBG_LEVEL              3   /* 0-4, higher means more log output */
#define CONFIG_NCP_IS_PVTKEY_ENCRYPTED            0
#define CONFIG_NCP_HOST_AUTO_TRIG_ENCRYPT         0
#endif

#define TLV_CMD_HEADER_LEN      12
#define TLV_CMD_SIZE_LOW_BYTES  4
#define TLV_CMD_SIZE_HIGH_BYTES 5
#define TLV_CMD_BUF_SIZE        4096
#define NCP_CHKSUM_LEN          4
#define NCP_MAX_CLASS           5

#define NCP_GET_PEER_CHKSUM(tlv, tlv_sz)  (*((uint32_t *)((uint8_t *)tlv + tlv_sz)))
#define NCP_GET_CLASS(tlv)                (((tlv) & 0xf0000000) >> 28)

#define container_of(ptr, type, member)                    \
    ({                                                     \
        const typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type, member)); \
    })

#define ARG_UNUSED(x) (void)(x)

#if CONFIG_NCP_USE_ENCRYPT

#define NCP_ENDECRYPT_KEY_LEN          16
#define NCP_ENDECRYPT_IV_LEN           16
#define NCP_GCM_TAG_LEN                16

typedef struct _crypt_param_t {
    uint8_t  flag;
    uint8_t  rsv[3];
    uint8_t  key_len;
    uint8_t  iv_len;
    uint8_t  key_enc[NCP_ENDECRYPT_KEY_LEN];
    uint8_t  key_dec[NCP_ENDECRYPT_KEY_LEN];
    uint8_t  iv_enc[NCP_ENDECRYPT_IV_LEN];
    uint8_t  iv_dec[NCP_ENDECRYPT_IV_LEN];
    mbedtls_svc_key_id_t key_enc_id;
    mbedtls_svc_key_id_t key_dec_id;
} crypt_param_t;

int ncp_tlv_adapter_encrypt_init(const uint8_t *key_enc, const uint8_t *key_dec, 
                                 const uint8_t *iv_enc, const uint8_t *iv_dec,
                                 uint16_t key_len, uint16_t iv_len);
int ncp_tlv_adapter_encrypt_deinit(void);
int ncp_tlv_adapter_encrypt_enable(void);
int ncp_tlv_adapter_encrypt_disable(void);
int ncp_tlv_adapter_is_encrypt_mode(void);

#endif /* CONFIG_NCP_USE_ENCRYPT */

/* NCP Debug options */
#if CONFIG_NCP_DEBUG
/* Interface related stats*/
typedef struct _stats_interface
{
    uint32_t tx;
    uint32_t rx;
    uint32_t err;
    uint32_t chkerr;
    uint32_t drop;
    uint32_t lenerr;
    uint32_t ringerr;
} stats_interface_t;

/* NCP Interface stats container */
typedef struct _ncp_stats
{
    stats_interface_t tlvq;
    stats_interface_t intf;
} ncp_stats_t;

/* Global variable containing NCP internal statistics */
extern ncp_stats_t ncp_stats;

#define NCP_STATS_INC(x) ++ncp_stats.x
#define NCP_STATS_DEC(x) --ncp_stats.x
#else
#define NCP_STATS_INC(x)
#define NCP_STATS_DEC(x)
#endif /* CONFIG_NCP_DEBUG */
/* End of NCP debug options */

/* TX event sent before entering sleep mode */
#define NCP_TX_EVENT_CTRL_PRE   NCP_PM_NOTIFY_EVENT_PRE
/* TX event sent after waking up from sleep */
#define NCP_TX_EVENT_CTRL_POST  NCP_PM_NOTIFY_EVENT_POST
/* TX event indicating data is ready to send */
#define NCP_TX_EVENT_DATA_READY NCP_PM_NOTIFY_EVENT_DATA_READY
/* Mask for all TX events */
#define NCP_TX_EVENT_ALL        (NCP_TX_EVENT_CTRL_PRE | \
                                NCP_TX_EVENT_CTRL_POST | \
                                NCP_TX_EVENT_DATA_READY)

/* Maximum length of TX data queue */
#define NCP_TLV_DATA_QUEUE_LENGTH 40
/* Maximum length of TX control queue */
#define NCP_TLV_CTRL_QUEUE_LENGTH 2
/* TX task stack size */
#define NCP_TLV_TX_TASK_STACK_SIZE 1024

/**
 * @brief TX data queue element structure
 *
 * Used for queuing data messages to be transmitted
 */
typedef struct
{
    uint32_t is_ref;      /**< Reference flag */
    uint16_t tlv_sz;      /**< Data size */
    uint8_t *tlv_buf;     /**< Data buffer pointer */
    void *ref_buf;        /**< Reference buffer */
    void (*ref_free_cb)(void *buf); /**< Reference buffer free callback  */
    uint8_t reserved[14]; /**< explicit padding to make sizeof == 32 */
} ncp_tlv_data_qelem_t;

/**
 * @brief TX control queue element structure
 *
 * Used for queuing control messages to be transmitted
 */
typedef struct
{
    uint32_t event;       /**< Event code (see TX Event Types) */
    uint32_t seqnum;      /**< Message sequence number */
    uint16_t ctrl_sz;     /**< Control data size in bytes */
    uint8_t *ctrl_buf;    /**< Pointer to control data buffer */
    uint8_t  reserved[18];/**< explicit padding to make sizeof == 32 */
} ncp_tlv_ctrl_qelem_t;

/**
 * @brief TLV message handler callback function
 * @param tlv Pointer to TLV message
 * @param tlv_sz TLV message size
 * @param status Processing status
 */
typedef void (*tlv_callback_t)(void *tlv, size_t tlv_sz, int status);

typedef void (*tlv_send_callback_t)(void *arg);

/**
 * @brief NCP interface operations structure
 *
 * Defines the interface layer operations for different transport types
 */
typedef struct _ncp_intf_ops
{
    int (*init)(void *);
    int (*deinit)(void *);
    int (*send)(uint8_t *buf, size_t len, tlv_send_callback_t cb);
    int (*recv)(uint8_t *buf, size_t *len);
    void (*reset)(void);
    int (*reset_cb)(bool enable);
    ncp_intf_pm_ops_t *pm_ops;
    void (*set_host_type)(int type);
} ncp_intf_ops_t;

/**
 * @brief NCP device reset callback fn type
 */
typedef int (*ncp_reset_callback_t)(uint8_t *res);

/**
 * @brief Set NCP device reset callback fn handler
 * @param dev_reset_cb Pointer to ncp_reset_callback_t
 */
void ncp_adapter_set_cb(ncp_reset_callback_t dev_reset_cb);

/**
 * @brief NCP device reset context save on AON memory
 */
typedef struct _ncp_reset_context
{
    volatile uint32_t reset_flag;
    volatile uint8_t host_type;
    volatile uint16_t cmd_seq;
} ncp_reset_context_t;

/**
 * @brief NCP TLV adapter main structure
 *
 * Central structure containing all adapter components
 */
typedef struct _ncp_tlv_adapter
{
    const ncp_intf_ops_t *intf_ops;           /**< Interface operations */
    const ncp_pm_ops_t *pm_ops;               /**< Power management operations */
    tlv_callback_t tlv_handler[NCP_MAX_CLASS]; /**< TLV class handlers */
    ncp_reset_context_t *reset_context_aon; /**Reset context addr on AON mem */
#if CONFIG_NCP_USE_ENCRYPT
    crypt_param_t *crypt;               /**< Encryption parameters */
#endif
} ncp_tlv_adapter_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Dispatch received TLV message to appropriate handler
 * @param tlv Pointer to TLV message
 * @param tlv_sz TLV message size
 */
void ncp_tlv_dispatch(void *tlv, size_t tlv_sz);

/* --- TX API Functions --- */
/**
 * @brief Send TLV data message (copy mode)
 *
 * The TLV buffer will be copied to internal queue
 *
 * @param tlv_buf Pointer to TLV buffer
 * @param tlv_sz TLV buffer size in bytes
 * @return NCP_STATUS_SUCCESS on success, error code otherwise
 */
ncp_status_t ncp_tlv_send(void *tlv_buf, uint16_t tlv_sz);

/**
 * @brief Send TLV data message (reference mode)
 * @param tlv_buf TLV buffer pointer
 * @param tlv_sz TLV buffer size
 * @param is_ref Reference flag
 * @param ref_free_cb Reference buffer free callback
 * @param ref_buf Reference buffer
 * @return Status code
 */
ncp_status_t ncp_tlv_ref_send(void *tlv_buf, uint16_t tlv_sz, uint32_t is_ref,
                              void (*ref_free_cb)(void *buf), void *ref_buf);

/**
 * @brief Send control message
 *
 * @param event Event code (see TX Event Types)
 * @param ctrl_buf Pointer to control buffer
 * @param ctrl_sz Control buffer size in bytes
 * @return 0 on success, negative error code on failure
 */
int ncp_tlv_ctrl_send(uint32_t event, void *ctrl_buf, uint16_t ctrl_sz);

/**
 * @brief Set TX event for notification
 * @param event Event mask to set
 */
void ncp_tlv_tx_set_event(uint32_t event);

/**
 * @brief Get pointer to global TLV adapter instance
 * @return Pointer to ncp_tlv_adapter_t structure
 */
const ncp_tlv_adapter_t *ncp_tlv_adapter_get(void);

ncp_reset_context_t *ncp_get_reset_context(void);

int ncp_tlv_data_queue_empty();

#endif /* __NCP_TLV_ADAPTER_H__ */
