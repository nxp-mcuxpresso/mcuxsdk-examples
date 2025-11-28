/*
 *  Copyright 2025 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __NCP_HTTPS_MBEDTLS__
#define __NCP_HTTPS_MBEDTS__

#include "mbedtls/entropy.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/x509.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/ssl.h"
#include "mbedtls/platform.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl_ciphersuites.h"
#include "mbedtls/mbedtls_common.h"
#include "entropy_poll.h"


#define MBEDTLS_ERR_NET_SOCKET_FAILED    -0x0042
#define MBEDTLS_ERR_NET_CONNECT_FAILED   -0x0044
#define MBEDTLS_ERR_NET_BIND_FAILED      -0x0046
#define MBEDTLS_ERR_NET_LISTEN_FAILED    -0x0048
#define MBEDTLS_ERR_NET_ACCEPT_FAILED    -0x004A
#define MBEDTLS_ERR_NET_RECV_FAILED      -0x004C
#define MBEDTLS_ERR_NET_SEND_FAILED      -0x004E
#define MBEDTLS_ERR_NET_CONN_RESET       -0x0050
#define MBEDTLS_ERR_NET_UNKNOWN_HOST     -0x0052
#define MBEDTLS_ERR_NET_BUFFER_TOO_SMALL -0x0043
#define MBEDTLS_ERR_NET_INVALID_CONTEXT  -0x0045
#define CONFIG_NCP_HTTPS_MBEDTLS_DBG_LEVEL 4

typedef struct {
    char *hostname;
    uint8_t *cert_file;
    unsigned int cert_file_len;
    int      fd;
    /* Filled after called to wm_mbedtls_ssl_new() */
    mbedtls_ssl_context      ssl;
    mbedtls_entropy_context  entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_config       conf;
    mbedtls_x509_crt         ca_cert;
    mbedtls_x509_crt         own_cert;
    mbedtls_pk_context       pkey;
    uint8_t                  is_server;
    uint8_t                  entropy_buf[MBEDTLS_ENTROPY_BLOCK_SIZE];
} https_ssl;

int https_bio_recv(void *ctx, unsigned char *buf, size_t len);
int https_bio_write(void *ctx, const unsigned char *buf, size_t len);
int https_mbedtls_setup(https_ssl *ssl);
int https_mbedtls_teardown(https_ssl *ssl);
void https_mbedtls_close_notify(https_ssl *h_ssl);
int https_mbedtls_ssl_read(mbedtls_ssl_context *ssl, unsigned char *buf, size_t len);
int https_mbedtls_ssl_write(mbedtls_ssl_context *ssl, const unsigned char *buf, size_t len);
#define https_mbedtls_e(...) wmlog("https_mbedtls", ##__VA_ARGS__)
#define https_mbedtls_w(...) wmlog("https_mbedtls", ##__VA_ARGS__)
#if CONFIG_HTTPS_MBEDTLS_DEBUG
#define https_mbedtls_d(...) wmlog("https_mbedtls", ##__VA_ARGS__)
#else
#define https_mbedtls_d(...)
#endif



#endif
