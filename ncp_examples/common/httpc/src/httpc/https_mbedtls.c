/*
 *  Copyright 2025 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "https_mbedtls.h"
#include <lwip/sockets.h>
#include "fsl_debug_console.h"
#include <wmlog.h>

static const mbedtls_x509_crt_profile https_mbedtls_x509_crt_profile_default = {
    /* Hashes from RIPEMD160 and above */
    MBEDTLS_X509_ID_FLAG(MBEDTLS_MD_RIPEMD160) | MBEDTLS_X509_ID_FLAG(MBEDTLS_MD_SHA224) |
        MBEDTLS_X509_ID_FLAG(MBEDTLS_MD_SHA256) | MBEDTLS_X509_ID_FLAG(MBEDTLS_MD_SHA384) |
        MBEDTLS_X509_ID_FLAG(MBEDTLS_MD_SHA512),
    0xFFFFFFF, /* Any PK alg    */
    0xFFFFFFF, /* Any curve     */
    2048,
};

static void https_mbedtls_dbgmsg_output(void *arg, int dbg_level, 
                        const char *file, int line, const char *str)
{
    (void)PRINTF("%s\r\n", str);
}

int https_mbedtls_ssl_read(mbedtls_ssl_context *ssl, unsigned char *buf, size_t len)
{
    int ret = 0;

    /* Actual datalen read may be less than len bytes. So, we can't force
     * a 'for' loop like in wm_mbedtls_ssl_write() below. */
    do
        ret = mbedtls_ssl_read(ssl, buf, len);
    while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

    if (ret < 0)
    {
        https_mbedtls_d("mbedtls_ssl_read failed, ret = -0x%02X", -ret);
    }

    return ret;
}

int https_mbedtls_ssl_write(mbedtls_ssl_context *ssl, const unsigned char *buf, size_t len)
{
    int ret = 0, written;

    /* Exact len bytes must be written during writes. Otherwise, it is
     * an error. */
    for (written = 0; written < len; written += ret)
    {
        do
            ret = mbedtls_ssl_write(ssl, buf + written, len - written);
        while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

        if (ret <= 0)
        {
            https_mbedtls_d("mbedtls_ssl_write failed, ret = -0x%02X", -ret);
            return ret;
        }
    }

    return written;
}

/* Read at most 'len' characters */
int https_bio_recv(void *ctx, unsigned char *buf, size_t len)
{
    int ret;
    int fd = (int)ctx;

    if (fd < 0)
        return MBEDTLS_ERR_NET_INVALID_CONTEXT;

    ret = (int)read(fd, buf, len);

    if (ret < 0)
    {
        if (lwip_fcntl(fd, F_GETFL, 0) != 0)
            return MBEDTLS_ERR_SSL_WANT_READ;

        if (errno == EPIPE || errno == ECONNRESET)
            return MBEDTLS_ERR_NET_CONN_RESET;

        if (errno == EINTR)
            return MBEDTLS_ERR_SSL_WANT_READ;

        return MBEDTLS_ERR_NET_RECV_FAILED;
    }

    return ret;
}

/*
 * Read at most 'len' characters, blocking for at most 'timeout' ms
 */
int https_bio_recv_timeout(void *ctx, unsigned char *buf, size_t len, uint32_t timeout)
{
    int ret;
    int fd = (int)ctx;

    if (fd < 0)
        return MBEDTLS_ERR_NET_INVALID_CONTEXT;

    /*------------------------------------------------------------
     * Get previous timeout on socket before receive.
     * This value will be used to restore timeout on socket.
     */
#if LWIP_SO_SNDRCVTIMEO_NONSTANDARD
    uint32_t previous_timeout = 0;
    socklen_t timeout_len 	= sizeof(previous_timeout);
    uint32_t timo = timeout;
    socklen_t timo_len	= sizeof(timo);
#else
    struct timeval previous_timeout = {0, 0};
    socklen_t timeout_len	= sizeof(previous_timeout);
    struct timeval timo = {timeout / 1000, (timeout % 1000) * 1000};
    socklen_t timo_len	= sizeof(timo);
#endif

    if (WM_SUCCESS != getsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &previous_timeout, &timeout_len))
    {
        https_mbedtls_d("Failed to get previous socket timeout");
        return -WM_FAIL;
    }

    /*------------------------------------------------------------
     * Set timeout on socket before receive
     */
    if (WM_SUCCESS != setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timo, timo_len))
    {
        https_mbedtls_d(
            "Failed to set socket "
            "timeout: %d",
            timeout);
        return -WM_FAIL;
    }
    /*------------------------------------------------------------
     */
    /* This call will not block */
    ret = https_bio_recv(ctx, buf, len);

    /*------------------------------------------------------------
     * Restore previous timeout on socket
     */
    if (WM_SUCCESS != setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &previous_timeout, timeout_len))
    {
        https_mbedtls_d(
            "Failed to restore previous socket "
            "timeout: %d",
            previous_timeout);
        return -WM_FAIL;
    }
    /*------------------------------------------------------------
     */
    return ret;
}

/*
 * Write at most 'len' characters
 */
int https_bio_write(void *ctx, const unsigned char *buf, size_t len)
{
    int ret;
    int fd = (int)ctx;

    if (fd < 0)
        return MBEDTLS_ERR_NET_INVALID_CONTEXT;

    ret = (int)write(fd, buf, len);

    if (ret < 0)
    {
        if (lwip_fcntl(fd, F_GETFL, 0) != 0)
            return MBEDTLS_ERR_SSL_WANT_WRITE;

        if (errno == EPIPE || errno == ECONNRESET)
            return MBEDTLS_ERR_NET_CONN_RESET;

        if (errno == EINTR)
            return MBEDTLS_ERR_SSL_WANT_WRITE;

        return MBEDTLS_ERR_NET_SEND_FAILED;
    }
    return ret;
}

int https_mbedtls_setup(https_ssl *h_ssl)
{
    int ret                          = 0;
    const unsigned char *custom_name = (const unsigned char *)"NCP_HTTPS";
    size_t custom_name_len           = strlen((const char *)custom_name);

    https_mbedtls_d("*** https mbedtls init start\n");
    mbedtls_ssl_init(&h_ssl->ssl);
#ifdef MBEDTLS_DEBUG_C
    mbedtls_debug_set_threshold(CONFIG_NCP_HTTPS_MBEDTLS_DBG_LEVEL);
#endif
    /* configurate ssl conf */
    mbedtls_ssl_config_init(&h_ssl->conf);
    mbedtls_ssl_conf_dbg(&h_ssl->conf, https_mbedtls_dbgmsg_output, NULL);
    mbedtls_ssl_config_defaults(&h_ssl->conf, MBEDTLS_SSL_IS_CLIENT,
                                      MBEDTLS_SSL_TRANSPORT_STREAM,
                                      MBEDTLS_SSL_PRESET_DEFAULT);
#if MBEDTLS_VERSION_NUMBER >= 0x03000000 /* mbedtls 3.0.0 */
    mbedtls_ssl_conf_min_tls_version(&h_ssl->conf, MBEDTLS_SSL_VERSION_TLS1_2);
#ifdef MBEDTLS_SSL_PROTO_TLS1_3
    mbedtls_ssl_conf_max_tls_version(&h_ssl->conf, MBEDTLS_SSL_VERSION_TLS1_3);
#else
    mbedtls_ssl_conf_max_tls_version(&h_ssl->conf, MBEDTLS_SSL_VERSION_TLS1_2);
#endif
#endif
    mbedtls_ssl_conf_cert_profile(&h_ssl->conf, &https_mbedtls_x509_crt_profile_default);
    mbedtls_ssl_conf_authmode(&h_ssl->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
    
    /* set server hostname */
    mbedtls_ssl_set_hostname(&h_ssl->ssl, h_ssl->hostname);
    
    /* parse CA certicate */
    mbedtls_x509_crt_init(&h_ssl->ca_cert);
    ret = mbedtls_x509_crt_parse(&h_ssl->ca_cert, (const unsigned char *)h_ssl->cert_file, h_ssl->cert_file_len);
    if (ret != 0) {
        https_mbedtls_e("mbedtls_x509_crt_parse ca_cert err %d\n", ret);
        return -TLS_ERR_PARSE_CERT;
    }
    mbedtls_ssl_conf_ca_chain(&h_ssl->conf, &h_ssl->ca_cert, NULL);

    /* drbg configuration */
    mbedtls_entropy_init(&h_ssl->entropy);
    mbedtls_ctr_drbg_init(&h_ssl->ctr_drbg);
    if ((ret = mbedtls_ctr_drbg_seed(&h_ssl->ctr_drbg, mbedtls_entropy_func, &h_ssl->entropy,
                                     custom_name, custom_name_len)) != 0)
    {
        https_mbedtls_e(
            "mbedtls_ctr_drbg_seed returned, "
            "ret = -0x%02X",
            -ret);
        return -1;
    }
    /* set ssl conf random generator */
    mbedtls_ssl_conf_rng(&h_ssl->conf, mbedtls_ctr_drbg_random, &h_ssl->ctr_drbg);

    /* Setup a TLS context */
    ret = mbedtls_ssl_setup(&h_ssl->ssl, &h_ssl->conf);
    if (ret != 0) {
        https_mbedtls_e("mbedtls_ssl_setup err %d\n", ret);
        return -TLS_ERR_SSL_SETUP;
    }
    mbedtls_ssl_session_reset(&h_ssl->ssl);
    /* set ssl send and recv callback */
    mbedtls_ssl_set_bio(&h_ssl->ssl, (void *)(h_ssl->fd), https_bio_write, https_bio_recv, NULL);
    ret = mbedtls_ssl_handshake(&h_ssl->ssl);
    if (ret != 0)
    {
        https_mbedtls_e("https mbedtls_ssl_handshake fail %d", ret);
    }
    else
    {
        https_mbedtls_d("https Negotiated SSL protocol is %s and SSL cipher suite is %s\r\n",
                mbedtls_ssl_get_version(s->ssl), mbedtls_ssl_get_ciphersuite(s->ssl));
        https_mbedtls_d("SSL Connect success");
    }
#if defined(MBEDTLS_X509_CRT_PARSE_C)
    uint32_t flags;
    if ((flags = mbedtls_ssl_get_verify_result(&h_ssl->ssl)) != 0)
    {
        char vrfy_buf[512];

        mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "==> ", flags);

        https_mbedtls_d("--------------------------------------");
        https_mbedtls_d(
            "Verification results for flags 0x%02X "
            "are:",
            flags);
        https_mbedtls_d("%s", vrfy_buf);
        https_mbedtls_d("--------------------------------------");
    }
#endif /* MBEDTLS_X509_CRT_PARSE_C */
    if (ret)
    {
        mbedtls_ssl_free(&h_ssl->ssl);
        mbedtls_ssl_config_free(&h_ssl->conf);
        mbedtls_ctr_drbg_free(&h_ssl->ctr_drbg);
        mbedtls_entropy_free(&h_ssl->entropy);
        mbedtls_x509_crt_free(&h_ssl->ca_cert);
    }
    https_mbedtls_d("*** https mbedtls init exit with %d\n", ret);
    return ret;
}

int https_mbedtls_teardown(https_ssl *h_ssl)
{
    https_mbedtls_d("*** https mbedtls teardown enter with %d\n", ret);
    mbedtls_ssl_free(&h_ssl->ssl);
    mbedtls_ssl_config_free(&h_ssl->conf);
    mbedtls_ctr_drbg_free(&h_ssl->ctr_drbg);
    mbedtls_entropy_free(&h_ssl->entropy);
    mbedtls_x509_crt_free(&h_ssl->ca_cert);
    https_mbedtls_d("*** https mbedtls teardown exit with %d\n", ret);
    return 0;
}

void https_mbedtls_close_notify(https_ssl *h_ssl)
{
    mbedtls_ssl_close_notify(&h_ssl->ssl);
}
