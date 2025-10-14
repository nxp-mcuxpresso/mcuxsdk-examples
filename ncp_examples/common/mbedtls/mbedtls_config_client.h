/*
 * Copyright 2022-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef MBEDTLS_CONFIG_CLIENT_H
#define MBEDTLS_CONFIG_CLIENT_H

#if CONFIG_NCP_OT
#include "rw612-mbedtls-config.h"
#else
#if CONFIG_WPA_SUPP_MBEDTLS
#include "wpa_supp_els_pkc_mbedtls_config.h"
#else
/* Fix build error for wmcrypto */
#define MBEDTLS_SSL_TLS_C
#define MBEDTLS_SSL_PROTO_TLS1_2
#define MBEDTLS_SSL_CLI_C
#define MBEDTLS_SSL_SRV_C
#define MBEDTLS_X509_USE_C
#define MBEDTLS_X509_CRT_PARSE_C
#endif /* CONFIG_WPA_SUPP_MBEDTLS */
#endif /* CONFIG_NCP_OT */
#endif /* MBEDTLS_CONFIG_CLIENT_H */
