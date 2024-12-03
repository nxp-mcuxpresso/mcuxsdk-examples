/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_FIPS_CONFIG_H_
#define _ELS_PKC_FIPS_CONFIG_H_

#include "els_pkc_fips_cipher.h"
#include "els_pkc_fips_rsa.h"
#include "els_pkc_fips_ecdsa.h"
#include "els_pkc_fips_mac.h"
#include "els_pkc_fips_hash.h"
#include "els_pkc_fips_drbg.h"
#include "els_pkc_fips_kdf.h"
#include "els_pkc_fips_ecdh.h"
#include "els_pkc_fips_key_gen.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Data structure for executing cryptographic algorithms based on s_UserOptions. */
typedef struct
{
    uint64_t option;
    char name[FUNCTION_NAME_MAX_SIZE];
    void (*executionFunction)(uint64_t options, char name[]);
} AlgorithmMapping;

/* Struct mapping the algorithm define to the corresponding function together
 * with the algorithm name for logging.
 */
static AlgorithmMapping s_AlgorithmMappings[] = {{FIPS_ECB_DRBG, "ECB-DRBG", &execute_drbg_kat},
                                                 {FIPS_CTR_DRBG, "CTR-DRBG", &execute_drbg_kat},
                                                 {FIPS_CKDF_SP800108, "CKDF-SP800-108", &execute_kdf_kat},
                                                 {FIPS_HKDF_SP80056C, "HKDF-SP800-56C", &execute_kdf_kat},
                                                 {FIPS_HKDF_RFC5869, "HKDF-RFC5869", &execute_kdf_kat},
                                                 {FIPS_ECDSA_256P, "NON-DET ECDSA-256P", &execute_ecdsa_kat},
                                                 {FIPS_ECDSA_384P, "ECDSA-384P", &execute_ecdsa_kat},
                                                 {FIPS_ECDSA_521P, "ECDSA-521P", &execute_ecdsa_kat},
                                                 {FIPS_EDDSA, "ED25519", &execute_eddsa_kat},
                                                 {FIPS_ECDH256P, "ECDH-256P", &execute_ecdh_kat},
                                                 {FIPS_ECDH384P, "ECDH-384P", &execute_ecdh_kat},
                                                 {FIPS_ECDH521P, "ECDH-521P", &execute_ecdh_kat},
                                                 {FIPS_ECC_KEYGEN_256P, "ECC-KEYGEN-256P", &execute_ecc_keygen_pct},
                                                 {FIPS_ECC_KEYGEN_384P, "ECC-KEYGEN-384P", &execute_ecc_keygen_pct},
                                                 {FIPS_ECC_KEYGEN_521P, "ECC-KEYGEN-521P", &execute_ecc_keygen_pct},
                                                 {FIPS_RSA_PKCS15_2048, "RSA-PKCS15-2048", &execute_rsa_kat},
                                                 {FIPS_RSA_PKCS15_3072, "RSA-PKCS15-3072", &execute_rsa_kat},
                                                 {FIPS_RSA_PKCS15_4096, "RSA-PKCS15-4096", &execute_rsa_kat},
                                                 {FIPS_RSA_PSS_2048, "RSA-PSS-2048", &execute_rsa_kat},
                                                 {FIPS_RSA_PSS_3072, "RSA-PSS-3072", &execute_rsa_kat},
                                                 {FIPS_RSA_PSS_4096, "RSA-PSS-4096", &execute_rsa_kat},
                                                 {FIPS_AES_CCM_128, "AES-CCM-128", &execute_ccm_kat},
                                                 {FIPS_AES_CCM_256, "AES-CCM-256", &execute_ccm_kat},
                                                 {FIPS_AES_GCM_128, "AES-GCM-128", &execute_gcm_kat},
                                                 {FIPS_AES_GCM_192, "AES-GCM-192", &execute_gcm_kat},
                                                 {FIPS_AES_GCM_256, "AES-GCM-256", &execute_gcm_kat},
                                                 {FIPS_AES_CTR_128, "AES-CTR-128", &execute_ctr_kat},
                                                 {FIPS_AES_CTR_192, "AES-CTR-192", &execute_ctr_kat},
                                                 {FIPS_AES_CTR_256, "AES-CTR-256", &execute_ctr_kat},
                                                 {FIPS_AES_ECB_128, "AES-ECB-128", &execute_ecb_kat},
                                                 {FIPS_AES_ECB_192, "AES-ECB-192", &execute_ecb_kat},
                                                 {FIPS_AES_ECB_256, "AES-ECB-256", &execute_ecb_kat},
                                                 {FIPS_AES_CBC_128, "AES-CBC-128", &execute_cbc_kat},
                                                 {FIPS_AES_CBC_192, "AES-CBC-192", &execute_cbc_kat},
                                                 {FIPS_AES_CBC_256, "AES-CBC-256", &execute_cbc_kat},
                                                 {FIPS_AES_CMAC_128, "AES-CMAC-128", &execute_cmac_kat},
                                                 {FIPS_AES_CMAC_256, "AES-CMAC-256", &execute_cmac_kat},
                                                 {FIPS_HMAC_SHA224, "HMAC-SHA224", &execute_hmac_kat},
                                                 {FIPS_HMAC_SHA256, "HMAC-SHA256", &execute_hmac_kat},
                                                 {FIPS_HMAC_SHA384, "HMAC-SHA384", &execute_hmac_kat},
                                                 {FIPS_HMAC_SHA512, "HMAC-SHA512", &execute_hmac_kat},
                                                 {FIPS_SHA224, "SHA224", &execute_sha_kat},
                                                 {FIPS_SHA256, "SHA256", &execute_sha_kat},
                                                 {FIPS_SHA384, "SHA384", &execute_sha_kat},
                                                 {FIPS_SHA512, "SHA512", &execute_sha_kat}};

/* Specify here, which algorithm to test. If all should get tested
 * specify: 'FIPS_ALL_TESTS'. If e.g. only ECB-128 and HMAC-SHA224
 * then specify: 'FIPS_AES_ECB_128 | FIPS_HMAC_SHA224'.
 */
static uint64_t s_UserOptions = FIPS_ALL_TESTS;

#endif /* _ELS_PKC_FIPS_CONFIG_H_ */
