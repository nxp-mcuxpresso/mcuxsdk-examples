/*
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef MBEDTLS_CONFIG_CLIENT_H
#define MBEDTLS_CONFIG_CLIENT_H

#if CONFIG_NCP_OT
#include "rw612-mbedtls-config.h"
#elif CONFIG_WPA_SUPP_MBEDTLS
#include "wpa_supp_els_pkc_mbedtls_config.h"
#else
/* Fix build error for wmcrypto */
#define MBEDTLS_PLATFORM_MEMORY
#define MBEDTLS_SSL_TLS_C
#define MBEDTLS_SSL_PROTO_TLS1_2
#define MBEDTLS_SSL_CLI_C
#define MBEDTLS_SSL_SRV_C
#define MBEDTLS_X509_USE_C
#define MBEDTLS_X509_CRT_PARSE_C

#ifndef MBEDTLS_ALLOW_PRIVATE_ACCESS
#define MBEDTLS_ALLOW_PRIVATE_ACCESS
#endif

#ifdef MBEDTLS_PRIVATE
#undef MBEDTLS_PRIVATE
#define MBEDTLS_PRIVATE(member) member
#endif

#endif /* CONFIG_NCP_OT */
#endif /* MBEDTLS_CONFIG_CLIENT_H */
