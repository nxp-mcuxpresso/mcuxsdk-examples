/*!\file ncp_tlv_adapter.h
 *\brief This file provides NCP TLV adapter interfaces. 
 *
 */
/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef __NCP_TLV_ADAPTER_H__
#define __NCP_TLV_ADAPTER_H__

#include "ncp_intf_pm.h"
#include "ncp_adapter.h"
#include "fsl_os_abstraction.h"
#include "psa/crypto_types.h"


/* define whether use encryption communication, temporarily locate it here */
#define CONFIG_NCP_USE_ENCRYPT         1  
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

typedef void (*tlv_callback_t)(void *tlv, size_t tlv_sz, int status);


typedef void (*tlv_send_callback_t)(void *arg);
typedef struct _ncp_intf_ops
{
    int (*init)(void *);
    int (*deinit)(void *);
    int (*send)(uint8_t *buf, size_t len, tlv_send_callback_t cb);
    int (*recv)(uint8_t *buf, size_t *len);
    ncp_intf_pm_ops_t *pm_ops;
} ncp_intf_ops_t;

typedef struct _ncp_tlv_adapter
{
    ncp_intf_ops_t *intf_ops;
    tlv_callback_t tlv_handler[NCP_MAX_CLASS];
#if CONFIG_NCP_USE_ENCRYPT
    crypt_param_t *crypt;
#endif
} ncp_tlv_adapter_t;

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
#if CONFIG_NCP_UART
    stats_interface_t uart;
#endif
#if CONFIG_NCP_SPI
    stats_interface_t spi;
#endif
#if CONFIG_NCP_USB
    stats_interface_t usb;
#endif
#if CONFIG_NCP_SDIO
    stats_interface_t sdio;
#endif
} ncp_stats_t;

/* Global variable containing NCP internal statistics */
extern ncp_stats_t ncp_stats;

#define NCP_STATS_INC(x) ++ncp_stats.x
#define NCP_STATS_DEC(x) --ncp_stats.x
#else
#define NCP_STATS_INC(x)
#define NCP_STATS_DEC(x)
#endif /* CONFIG_NCP_DEBUG */


#define NCP_TLV_STATS_INC(x) NCP_STATS_INC(tlvq.x)
#if (CONFIG_NCP_DEBUG) && (CONFIG_NCP_UART)
#define NCP_UART_STATS_INC(x) NCP_STATS_INC(uart.x)
#else
#define NCP_UART_STATS_INC(x)
#endif

#if (CONFIG_NCP_DEBUG) && (CONFIG_NCP_SPI)
#define NCP_SPI_STATS_INC(x) NCP_STATS_INC(spi.x)
#else
#define NCP_SPI_STATS_INC(x)
#endif

#if (CONFIG_NCP_DEBUG) && (CONFIG_NCP_USB)
#define NCP_USB_STATS_INC(x) NCP_STATS_INC(usb.x)
#else
#define NCP_USB_STATS_INC(x)
#endif

#if (CONFIG_NCP_DEBUG) && (CONFIG_NCP_SDIO)
#define NCP_SDIO_STATS_INC(x) NCP_STATS_INC(sdio.x)
#else
#define NCP_SDIO_STATS_INC(x)
#endif

/* End of NCP debug options */

void ncp_tlv_dispatch(void *tlv, size_t tlv_sz);


/*NCP ADAPTER TX CODE*/

/* NCP ADAPTER TLV TX task function */

/* NCP ADAPTER tlv send */
ncp_status_t ncp_tlv_send(void *tlv_buf, size_t tlv_sz);
ncp_status_t ncp_tlv_ref_send(void *tlv_buf, size_t tlv_sz, uint32_t is_ref);

/* NCP ADAPTER TX queue element */
typedef NCP_TLV_PACK_START struct
{
    uint32_t is_ref;
    size_t tlv_sz;
    uint8_t *tlv_buf;
} NCP_TLV_PACK_END ncp_tlv_qelem_t;


#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

/* NCP ADAPTER TX queue max length */
#define NCP_TLV_QUEUE_LENGTH 40
/* NCP ADAPTER TX task stack size */
#define NCP_TLV_TX_TASK_STACK_SIZE 1024
#endif /* __NCP_TLV_ADAPTER_H__ */
