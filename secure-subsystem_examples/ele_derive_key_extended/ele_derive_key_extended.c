/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

#include "fsl_sss_mgmt.h"
#include "fsl_sss_sscp.h"
#include "fsl_sscp_mu.h"
#include "fsl_elemu.h"
#include "ele_200_fw.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CORE_CLK_FREQ          CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define ELE_MAX_SUBSYSTEM_WAIT (0xFFFFFFFFu)
#define ELE_SUBSYSTEM          (kType_SSS_Ele200)
#define OK_STRING              ("OK\r\n")
#define ERROR_STRING           ("ERROR\r\n")
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Variables used by example */
static sscp_context_t sscpContext    = {0};
static sss_sscp_session_t sssSession = {0};
static sss_sscp_key_store_t keyStore = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*! @brief Salt data from Test Vector 1 of RFC5869. */
static const uint8_t saltData[]         = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c
};
/*! @brief Info from Test Vector 1 of RFC5869. */
static const uint8_t info[]              = {
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9
};
/*! @brief IKM data from Test Vector 1 of RFC5869. */
static const uint8_t ikmData[]          = {
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b
};
/*! @brief Expected OKM from Test Vector 1 of RFC5869. */
static const uint8_t okmDataExpected[] = {
    0x3c, 0xb2, 0x5f, 0x25, 0xfa, 0xac, 0xd5, 0x7a, 0x90, 0x43, 0x4f, 0x64,
    0xd0, 0x36, 0x2f, 0x2a, 0x2d, 0x2d, 0x0a, 0x90, 0xcf, 0x1a, 0x5a, 0x4c,
    0x5d, 0xb0, 0x2d, 0x56, 0xec, 0xc4, 0xc5, 0xbf, 0x34, 0x00, 0x72, 0x08,
    0xd5, 0xb8, 0x87, 0x18, 0x58, 0x65
};

/*! @brief KDF example showing SHA256 HKDF into a single key object. */
static status_t hkdf()
{
    status_t status                        = kStatus_Fail;
    sss_sscp_derive_key_t deriveKeyContext = {0};
    uint8_t outBuf[42]                     = {0u};
    size_t outBufLen                       = 0u;
    size_t outBufLenBits                   = 0u;
    sss_sscp_object_t ikm                  = {0};
    sss_sscp_object_t prk                  = {0};
    sss_sscp_object_t okm                  = {0};

    do
    {
        PRINTF("**** HDKF key derivation ****\r\n");

        /* Key object inits */
        PRINTF("Init IKM key object...");
        status = sss_sscp_key_object_init(&ikm, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init PRK key object...");
        status = sss_sscp_key_object_init(&prk, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init OKM key object...");
        status = sss_sscp_key_object_init(&okm, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Key handle allocation */
        PRINTF("Allocate IKM key handle...");
        status = sss_sscp_key_object_allocate_handle(&ikm, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 32u, kSSS_KeyProp_CryptoAlgo_KDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate PRK key handle...");
        status = sss_sscp_key_object_allocate_handle(&prk, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 32u, kSSS_KeyProp_CryptoAlgo_KDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate OKM key handle...");
        status = sss_sscp_key_object_allocate_handle(&okm, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 42u, kSSS_KeyProp_CryptoAlgo_KDF | kSSS_KeyProp_CryptoAlgo_AES);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Set IKM...");
        status = sss_sscp_key_store_set_key(&keyStore, &ikm, ikmData, sizeof(ikmData), sizeof(ikmData) * 8u, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* HDKF Extract step to generate the intermediate PRK value */
        PRINTF("Initialize HKDF SHA256 extract step context...");
        status = sss_sscp_derive_key_context_init(&deriveKeyContext, &sssSession, &ikm, kAlgorithm_SSS_HKDF_SHA256_EXTRACT, kMode_SSS_ComputeSharedSecret);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("HKDF SHA256 extract...");
        status = sss_sscp_derive_key(&deriveKeyContext, saltData, sizeof(saltData), &prk, 256u);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Free HKDF SHA256 extract step context...");
        status = sss_sscp_derive_key_context_free(&deriveKeyContext);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* HDKF Expand step to generate the output OKM */
        PRINTF("Initialize HKDF SHA256 expand step context...");
        status = sss_sscp_derive_key_context_init(&deriveKeyContext, &sssSession, &prk, kAlgorithm_SSS_HKDF_SHA256_EXPAND, kMode_SSS_ComputeSharedSecret);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("HKDF SHA256 expand...");
        status = sss_sscp_derive_key(&deriveKeyContext, info, sizeof(info), &okm, 336u);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Check if OKM is correct */
        PRINTF("Get OKM...");
        memset(outBuf, 0, sizeof(outBuf));
        outBufLen = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &okm, outBuf, &outBufLen, &outBufLenBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Derived OKM matches expected OKM...");
        if (memcmp(outBuf, okmDataExpected, sizeof(okmDataExpected)) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* Cleanup */
        PRINTF("Cleanup...");
        if ((sss_sscp_key_object_free(&ikm, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&prk, 0u) != kStatus_SSS_Success)  ||
            (sss_sscp_key_object_free(&okm, 0u) != kStatus_SSS_Success)  ||
            (sss_sscp_derive_key_context_free(&deriveKeyContext) != kStatus_SSS_Success))
        {
            break;
        }
        PRINTF(OK_STRING);

        status = kStatus_Success;
    } while (0);

    if (status != kStatus_Success)
    {
        PRINTF(ERROR_STRING);
    }
    PRINTF("\r\n");

    return status;
}

/*! @brief Input salt data for HKDF Extract. */
static const uint8_t cccSalt[32]     = {0u};
/*! @brief Input info data for HKDF Expand ("SystemKeys" in hex). */
static const uint8_t cccInfo[10]     = {
    0x53u, 0x79u, 0x73u, 0x74u, 0x65u, 0x6du, 0x4bu, 0x65u, 0x79u, 0x73u
};
/*! @brief The SK input key material for HKDF Extract. */
static const uint8_t sk[16]          = {
    0x9fu, 0x69u, 0x50u, 0x17u, 0xbau, 0x5eu, 0x1du, 0xf7u, 0x4au, 0x1bu, 0x8bu,
    0xbfu, 0x27u, 0xfeu, 0x1eu, 0x0du
};
/*! @brief The expected Kenc value. */
static const uint8_t kencRef[16]     = {
    0x16u, 0x18u, 0x86u, 0xcbu, 0x9au, 0xe7u, 0x40u, 0x3du, 0x8du, 0xbcu, 0xcfu,
    0xe3u, 0x6bu, 0x8au, 0x04u, 0x26u
};
/*! @brief The expected Kmac value. */
static const uint8_t kmacRef[16]     = {
    0x63u, 0x87u, 0xbau, 0x65u, 0x47u, 0x9cu, 0xb7u, 0xebu, 0x9du, 0xf9u, 0x7bu,
    0xd4u, 0x8au, 0xc3u, 0x31u, 0x59u
};
/*! @brief The expected Krmac value. */
static const uint8_t krmacRef[16]    = {
    0x41u, 0x67u, 0x7fu, 0xb6u, 0x39u, 0x84u, 0x59u, 0x19u, 0x9fu, 0x1eu, 0x56u,
    0x97u, 0x60u, 0xdfu, 0x91u, 0xc1u
};
/*! @brief The expected long term shared secret value. */
static const uint8_t ltssRef[16]     = {
    0x5cu, 0x4eu, 0x19u, 0xdau, 0x55u, 0x35u, 0x24u, 0xe3u, 0x86u, 0xfau, 0x1eu,
    0xcau, 0x91u, 0xe8u, 0xadu, 0x0eu
};
/*! @brief The expected Kble_intro value. */
static const uint8_t kBleIntroRef[16] = {
    0x47u, 0xaeu, 0x79u, 0x07u, 0x2bu, 0x47u, 0xd9u, 0x8bu, 0xd2u, 0x81u, 0x69u,
    0xd2u, 0xd8u, 0x03u, 0xa0u, 0x7fu
};
/*! @brief The expected Kble_oob_master value. */
static const uint8_t kBleOobMasterRef[16]  = {
    0x3cu, 0x00u, 0x9du, 0x26u, 0xe5u, 0xe2u, 0xd9u, 0x69u, 0x79u, 0x99u, 0xa9u,
    0x18u, 0xdcu, 0x2fu, 0x8fu, 0x08u
};

/*!
 * @brief KDF example showing SHA256 HKDF with derivation into multiple
 *        key objects.
 *
 * Usecases of HKDF include splitting the key derivation result into multiple
 * keys for encryption, MAC, and other security operations. To support such
 * automotive and connectivity usecases, this example demonstrates
 * how to utilize the extended HKDF API to derive up to six separate output
 * keys of equal length. Output key bit lengths of 128 or 256 bits are possible.
 */
static status_t hkdf_multi()
{
    status_t status                        = kStatus_Fail;
    sss_sscp_derive_key_t deriveKeyContext = {0};
    uint8_t outBuf[42]                     = {0u};
    size_t outBufLen                       = 0u;
    size_t outBufLenBits                   = 0u;
    sss_sscp_object_t ikm                  = {0};
    sss_sscp_object_t prk                  = {0};
    sss_sscp_object_t okm1                 = {0};
    sss_sscp_object_t okm2                 = {0};
    sss_sscp_object_t okm3                 = {0};
    sss_sscp_object_t okm4                 = {0};
    sss_sscp_object_t okm5                 = {0};
    sss_sscp_object_t okm6                 = {0};

    do
    {
        PRINTF("**** HDKF key derivation into multiple key objects ****\r\n");

        /* Key object inits */
        PRINTF("Init IKM key object...");
        status = sss_sscp_key_object_init(&ikm, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init PRK key object...");
        status = sss_sscp_key_object_init(&prk, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init OKM1 key object...");
        status = sss_sscp_key_object_init(&okm1, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init OKM2 key object...");
        status = sss_sscp_key_object_init(&okm2, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init OKM3 key object...");
        status = sss_sscp_key_object_init(&okm3, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init OKM4 key object...");
        status = sss_sscp_key_object_init(&okm4, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init OKM5 key object...");
        status = sss_sscp_key_object_init(&okm5, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init OKM6 key object...");
        status = sss_sscp_key_object_init(&okm6, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Key handle allocation */
        PRINTF("Allocate IKM key handle...");
        status = sss_sscp_key_object_allocate_handle(&ikm, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 32u, kSSS_KeyProp_CryptoAlgo_KDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate PRK key handle...");
        status = sss_sscp_key_object_allocate_handle(&prk, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 32u, kSSS_KeyProp_CryptoAlgo_KDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate OKM1 key handle...");
        status = sss_sscp_key_object_allocate_handle(&okm1, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 16u, kSSS_KeyProp_CryptoAlgo_KDF | kSSS_KeyProp_CryptoAlgo_AES);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate OKM2 key handle...");
        status = sss_sscp_key_object_allocate_handle(&okm2, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 16u, kSSS_KeyProp_CryptoAlgo_KDF | kSSS_KeyProp_CryptoAlgo_AES);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate OKM3 key handle...");
        status = sss_sscp_key_object_allocate_handle(&okm3, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 16u, kSSS_KeyProp_CryptoAlgo_KDF | kSSS_KeyProp_CryptoAlgo_AES);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate OKM4 key handle...");
        status = sss_sscp_key_object_allocate_handle(&okm4, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 16u, kSSS_KeyProp_CryptoAlgo_KDF | kSSS_KeyProp_CryptoAlgo_AES);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate OKM5 key handle...");
        status = sss_sscp_key_object_allocate_handle(&okm5, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 16u, kSSS_KeyProp_CryptoAlgo_KDF | kSSS_KeyProp_CryptoAlgo_AES);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate OKM6 key handle...");
        status = sss_sscp_key_object_allocate_handle(&okm6, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 16u, kSSS_KeyProp_CryptoAlgo_KDF | kSSS_KeyProp_CryptoAlgo_AES);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Set IKM...");
        status = sss_sscp_key_store_set_key(&keyStore, &ikm, sk, sizeof(sk), sizeof(sk) * 8u, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Initialize HKDF SHA256 extract step context...");
        status = sss_sscp_derive_key_context_init(&deriveKeyContext, &sssSession, &ikm, kAlgorithm_SSS_HKDF_SHA256_EXTRACT, kMode_SSS_SymmetricKDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* In this example, an explicit zero-filled salt is used, but if no salt
         * is required, the caller may use NULL and provide salt length as 0.
        */
        PRINTF("HKDF SHA256 extract...");
        status = sss_sscp_derive_key(&deriveKeyContext, cccSalt, sizeof(cccSalt), &prk, 256u);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Free HKDF SHA256 extract step context...");
        status = sss_sscp_derive_key_context_free(&deriveKeyContext);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* HDKF Expand step to generate the output OKMs */
        PRINTF("Initialize HKDF SHA256 expand step context...");
        status = sss_sscp_derive_key_context_init(&deriveKeyContext, &sssSession, &prk, kAlgorithm_SSS_HKDF_SHA256_EXPAND, kMode_SSS_SymmetricKDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("HKDF SHA256 expand into six separate, equal-length, OKMs...");
        status = sss_sscp_derive_key_multi(&deriveKeyContext, cccInfo, sizeof(cccInfo), &okm1, &okm2, &okm3, &okm4, &okm5, &okm6, 128u, 6u);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Check if all OKMs are correct */
        PRINTF("Get OKM1...");
        (void)memset(outBuf, 0, sizeof(outBuf));
        outBufLen = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &okm1, outBuf, &outBufLen, &outBufLenBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Derived OKM matches expected value...");
        if (memcmp(outBuf, kencRef, sizeof(kencRef)) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get OKM2...");
        (void)memset(outBuf, 0, sizeof(outBuf));
        outBufLen = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &okm2, outBuf, &outBufLen, &outBufLenBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Derived OKM matches expected value...");
        if (memcmp(outBuf, kmacRef, sizeof(kmacRef)) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get OKM3...");
        (void)memset(outBuf, 0, sizeof(outBuf));
        outBufLen = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &okm3, outBuf, &outBufLen, &outBufLenBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Derived OKM matches expected value...");
        if (memcmp(outBuf, krmacRef, sizeof(krmacRef)) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get OKM4...");
        (void)memset(outBuf, 0, sizeof(outBuf));
        outBufLen = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &okm4, outBuf, &outBufLen, &outBufLenBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Derived OKM matches expected value...");
        if (memcmp(outBuf, ltssRef, sizeof(ltssRef)) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get OKM5...");
        (void)memset(outBuf, 0, sizeof(outBuf));
        outBufLen = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &okm5, outBuf, &outBufLen, &outBufLenBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Derived OKM matches expected value...");
        if (memcmp(outBuf, kBleIntroRef, sizeof(kBleIntroRef)) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get OKM6...");
        (void)memset(outBuf, 0, sizeof(outBuf));
        outBufLen = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &okm6, outBuf, &outBufLen, &outBufLenBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Derived OKM matches expected value...");
        if (memcmp(outBuf, kBleOobMasterRef, sizeof(kBleOobMasterRef)) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* Cleanup */
        PRINTF("Cleanup...");
        if ((sss_sscp_key_object_free(&ikm, 0u) != kStatus_SSS_Success)  ||
            (sss_sscp_key_object_free(&prk, 0u) != kStatus_SSS_Success)  ||
            (sss_sscp_key_object_free(&okm1, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&okm2, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&okm3, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&okm4, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&okm5, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&okm6, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_derive_key_context_free(&deriveKeyContext) != kStatus_SSS_Success))
        {
            break;
        }
        PRINTF(OK_STRING);

        status = kStatus_Success;
    } while (false);

    if (status != kStatus_Success)
    {
        PRINTF(ERROR_STRING);
    }
    PRINTF("\r\n");

    return status;
}

/*! @brief Salt for the CMAC key derivation function. */
static const uint8_t ckdfSalt[10]        = {0u};
/*! @brief Key for the CMAC key derivation function. */
static const uint8_t ckdfKey[32]         = {
    0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u,
    0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u,
    0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u
};
/*! @brief Expected result of the CMAC key derivation function. */
static const uint8_t ckdfExpectedKey[65] = {
    0x9fu, 0x43u, 0x1au, 0x77u, 0xd2u, 0x6fu, 0x0au, 0x79u, 0x58u, 0xdfu, 0x70u,
    0x37u, 0x77u, 0x8au, 0xcfu, 0x93u, 0x38u, 0xb4u, 0xb5u, 0x0fu, 0x62u, 0xcau,
    0xf0u, 0xf7u, 0x34u, 0x44u, 0xd9u, 0xf1u, 0x8bu, 0xebu, 0x83u, 0xbbu, 0xf9u,
    0xc8u, 0x8cu, 0x6bu, 0x3eu, 0x2bu, 0xb2u, 0x1du, 0x39u, 0x18u, 0x67u, 0xbbu,
    0xbau, 0x04u, 0xf5u, 0xd0u, 0x01u, 0x01u, 0xe3u, 0xa0u, 0xabu, 0xefu, 0xf0u,
    0xb1u, 0x00u, 0x2eu, 0xccu, 0x0bu, 0x8au, 0xcbu, 0xb5u, 0x99u, 0x5bu
};

/*! @brief Example showing KDF in counter mode with CMAC as PRF.
 *
 * KDF compatible with NIST 800-108 with CMAC as PRF in counter mode implemented
 * as follows:
 *  KD(i) = CMAC (KIN, salt data || counter)
 *  KD = KD(1) || KD(2) … || KD(n)
 * where
 *  i = 1 .. n
 *  KIN - key object ID used for key derivation set in DERIVE_KEY_INIT command
 *  KD - destination derived key object ID
 *  counter - four byte counter value in big endian format
 * The counter and n are calculated internally based on required derived key
 * length.
 */
static status_t ckdf()
{
    status_t status                        = kStatus_Fail;
    sss_sscp_derive_key_t deriveKeyContext = {0};
    uint8_t outBuf[100]                    = {0u};
    size_t outBufLen                       = 0u;
    size_t outBufLenBits                   = 0u;
    sss_sscp_object_t mainKey              = {0};
    sss_sscp_object_t derivedKey           = {0};

    do
    {
        PRINTF("**** CKDF key derivation ****\r\n");

        /* Key object inits */
        PRINTF("Init mainKey key object...");
        status = sss_sscp_key_object_init(&mainKey, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init derivedKey key object...");
        status = sss_sscp_key_object_init(&derivedKey, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Key handle allocation */
        PRINTF("Allocate mainKey key handle...");
        status = sss_sscp_key_object_allocate_handle(&mainKey, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 32u, kSSS_KeyProp_CryptoAlgo_KDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate derivedKey key handle...");
        status = sss_sscp_key_object_allocate_handle(&derivedKey, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 96u, kSSS_KeyProp_CryptoAlgo_AES);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Set mainKey...");
        status = sss_sscp_key_store_set_key(&keyStore, &mainKey, ckdfKey, sizeof(ckdfKey), sizeof(ckdfKey) * 8u, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Initialize CKDF context...");
        status = sss_sscp_derive_key_context_init(&deriveKeyContext, &sssSession, &mainKey, kAlgorithm_SSS_CKDF, kMode_SSS_SymmetricKDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("CKDF derivation...");
        status = sss_sscp_derive_key(&deriveKeyContext, ckdfSalt, sizeof(ckdfSalt), &derivedKey, sizeof(ckdfExpectedKey) * 8u);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Check if derived key is correct */
        PRINTF("Get derived key...");
        (void)memset(outBuf, 0, sizeof(outBuf));
        outBufLen = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &derivedKey, outBuf, &outBufLen, &outBufLenBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Derived key matches expected key...");
        if (memcmp(outBuf, ckdfExpectedKey, sizeof(ckdfExpectedKey)) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* Cleanup */
        PRINTF("Cleanup...");
        if ((sss_sscp_key_object_free(&mainKey, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&derivedKey, 0u) != kStatus_SSS_Success)  ||
            (sss_sscp_derive_key_context_free(&deriveKeyContext) != kStatus_SSS_Success))
        {
            break;
        }
        PRINTF(OK_STRING);

        status = kStatus_Success;
    } while (false);

    if (status != kStatus_Success)
    {
        PRINTF(ERROR_STRING);
    }
    PRINTF("\r\n");

    return status;
}

/*! @brief Key data for Miyaguchi-Preneel KDF derivation. */
static const uint8_t mp_k[16]             = {
    0x00u, 0x01u, 0x02u, 0x03u, 0x04u, 0x05u, 0x06u, 0x07u, 0x08u, 0x09u, 0x0au,
    0x0bu, 0x0cu, 0x0du, 0x0eu, 0x0fu
};
/*! @brief Constant KEY_UPDATE_ENC_C as used within SHE. */
static const uint8_t mp_c[16]             = {
    0x01u, 0x01u, 0x53u, 0x48u, 0x45u, 0x00u, 0x80u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0xb0u
};
/*! @brief Expected derived key for Miyaguchi-Preneel KDF derivation. */
static const uint8_t mp_derivedKeyRef[16] = {
    0x11u, 0x8au, 0x46u, 0x44u, 0x7au, 0x77u, 0x0du, 0x87u, 0x82u, 0x8au, 0x69u,
    0xc2u, 0x22u, 0xe2u, 0xd1u, 0x7eu
};

/*! @brief Example showing Miyaguchi-Preneel KDF based on AUTOSAR SHE1.1. */
static status_t mp_kdf()
{
    status_t status                        = kStatus_Fail;
    sss_sscp_derive_key_t deriveKeyContext = {0};
    uint8_t outBuf[100]                    = {0u};
    size_t outBufLen                       = 0u;
    size_t outBufLenBits                   = 0u;
    sss_sscp_object_t mainKey              = {0};
    sss_sscp_object_t derivedKey           = {0};

    do
    {
        PRINTF("**** Miyaguchi-Preneel key derivation ****\r\n");

        /* Key object inits */
        PRINTF("Init mainKey key object...");
        status = sss_sscp_key_object_init(&mainKey, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init derivedKey key object...");
        status = sss_sscp_key_object_init(&derivedKey, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Key handle allocation */
        PRINTF("Allocate mainKey key handle...");
        status = sss_sscp_key_object_allocate_handle(&mainKey, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 16u, kSSS_KeyProp_CryptoAlgo_KDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate derivedKey key handle...");
        status = sss_sscp_key_object_allocate_handle(&derivedKey, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 16u, kSSS_KeyProp_CryptoAlgo_AES);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Set mainKey...");
        status = sss_sscp_key_store_set_key(&keyStore, &mainKey, mp_k, sizeof(mp_k), sizeof(mp_k) * 8u, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Initialize MP_KDF context...");
        status = sss_sscp_derive_key_context_init(&deriveKeyContext, &sssSession, &mainKey, kAlgorithm_SSS_AES_MP_KDF, kMode_SSS_SymmetricKDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("MP_KDF derivation...");
        status = sss_sscp_derive_key(&deriveKeyContext, mp_c, sizeof(mp_c), &derivedKey, sizeof(mp_derivedKeyRef) * 8u);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get derived key...");
        outBufLen = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &derivedKey, outBuf, &outBufLen, &outBufLenBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Derived key matches expected key...");
        if (memcmp(outBuf, mp_derivedKeyRef, sizeof(mp_derivedKeyRef)) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* Cleanup */
        PRINTF("Cleanup...");
        if ((sss_sscp_key_object_free(&mainKey, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&derivedKey, 0u) != kStatus_SSS_Success)  ||
            (sss_sscp_derive_key_context_free(&deriveKeyContext) != kStatus_SSS_Success))
        {
            break;
        }
        PRINTF(OK_STRING);

        status = kStatus_Success;
    } while (false);

    if (status != kStatus_Success)
    {
        PRINTF(ERROR_STRING);
    }
    PRINTF("\r\n");

    return status;
}

/*! @brief The secure channel base key. */
static const uint8_t scp03_key[16]            = {
    0x0Eu, 0x2Au, 0x51u, 0xC8u, 0xAAu, 0x15u, 0x2Eu, 0x5Fu, 0x9Du, 0x96u, 0x3Au,
    0xDBu, 0x0Fu, 0xBFu, 0x17u, 0x41u
};
/*! @brief Derivation data for SCP03 key derivation (example data for S-MAC). */
static const uint8_t scp03_derivationData[32] = {
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x06u, 0x00u, 0x00u, 0x80u, 0x01u, 0x30u, 0x1Fu, 0x49u, 0xD6u, 0xFEu, 0xF2u,
    0x72u, 0x1Du, 0x63u, 0xF6u, 0x76u, 0x12u, 0x66u, 0xBCu, 0x05u, 0x6Du
};
/*! @brief Expected derived key for SCP03 key derivation. */
static const uint8_t scp03_derivedKeyRef[16]  = {
    0x3Cu, 0x20u, 0xDFu, 0xDFu, 0x82u, 0x08u, 0x3Fu, 0x33u, 0x0Du, 0xE5u, 0x83u,
    0xDEu, 0x3Du, 0x5Du, 0x26u, 0xEBu
};

/*! @brief Example showing KDF in counter mode with CMAC as PRF as
 *         specified by SCP03.
 */
static status_t ckdf_scp03()
{
    status_t status                        = kStatus_Fail;
    sss_sscp_derive_key_t deriveKeyContext = {0};
    uint8_t outBuf[100]                    = {0u};
    size_t outBufLen                       = 0u;
    size_t outBufLenBits                   = 0u;
    sss_sscp_object_t mainKey              = {0};
    sss_sscp_object_t derivedKey           = {0};

    do
    {
        PRINTF("**** CKDF SCP03 key derivation ****\r\n");

        /* Key object inits */
        PRINTF("Init mainKey key object...");
        status = sss_sscp_key_object_init(&mainKey, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init derivedKey key object...");
        status = sss_sscp_key_object_init(&derivedKey, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Key handle allocation */
        PRINTF("Allocate mainKey key handle...");
        status = sss_sscp_key_object_allocate_handle(&mainKey, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 32u, kSSS_KeyProp_CryptoAlgo_KDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate derivedKey key handle...");
        status = sss_sscp_key_object_allocate_handle(&derivedKey, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 96u, kSSS_KeyProp_CryptoAlgo_AES);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Set mainKey...");
        status = sss_sscp_key_store_set_key(&keyStore, &mainKey, scp03_key, sizeof(scp03_key), sizeof(scp03_key) * 8u, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Initialize CKDF SCP03 context...");
        status = sss_sscp_derive_key_context_init(&deriveKeyContext, &sssSession, &mainKey, kAlgorithm_SSS_CKDF_SCP03, kMode_SSS_SymmetricKDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("CKDF SCP03 derivation...");
        status = sss_sscp_derive_key(&deriveKeyContext, scp03_derivationData, sizeof(scp03_derivationData), &derivedKey, sizeof(scp03_derivedKeyRef) * 8u);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Check if derived key is correct */
        PRINTF("Get derived key...");
        (void)memset(outBuf, 0, sizeof(outBuf));
        outBufLen = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &derivedKey, outBuf, &outBufLen, &outBufLenBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Derived key matches expected key...");
        if (memcmp(outBuf, scp03_derivedKeyRef, sizeof(scp03_derivedKeyRef)) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* Cleanup */
        PRINTF("Cleanup...");
        if ((sss_sscp_key_object_free(&mainKey, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&derivedKey, 0u) != kStatus_SSS_Success)  ||
            (sss_sscp_derive_key_context_free(&deriveKeyContext) != kStatus_SSS_Success))
        {
            break;
        }
        PRINTF(OK_STRING);

        status = kStatus_Success;
    } while (false);

    if (status != kStatus_Success)
    {
        PRINTF(ERROR_STRING);
    }
    PRINTF("\r\n");

    return status;
}


/*!
 * @brief Main function
 */
int main(void)
{
    char ch;
    status_t status = kStatus_Fail;

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("ELE Automotive Key Derivation Extensions via SSSAPI Example\r\n\r\n");

    /*
     * This code example demonstrates EdgeLock usage of the Automotive KDF
     * SSSAPI extensions.
     * The example is performed in following steps:
     * 1.  Load ELE firmware.
     * 2.  Open an EdgeLock session
     * 3.  Create a key store
     * 4.  Initialize key objects and allocate handles
     * 5.  Showcase single key derivation with HKDF
     * 6.  Showcase multiple key derivation with HKDF
     * 7.  Showcase Miyaguchi-Preneel key derivation
     * 8.  Showcase CMAC-based key derivation
     * 9.  Showcase CMAC-based key derivation as specified by SCP03
     * 10. Free the key store
     * 11. Close the EdgeLock session
     * Note: This example does not close already opened contexts or objects in case of failed command.
     */

    do
    {
        status = ELEMU_mu_wait_for_ready(ELEMUA, ELE_MAX_SUBSYSTEM_WAIT);
        if (status != kStatus_Success)
        {
            break;
        }

        /****************** Start   ***********************/
        status = sscp_mu_init(&sscpContext, (ELEMU_Type *)(uintptr_t)ELEMUA);
        if (status != kStatus_SSCP_Success)
        {
            break;
        }

        status = ELEMU_loadFw(ELEMUA, (uint32_t *)fw);
        if (status != kStatus_Success)
        {
            status = kStatus_Fail;
            break;
        }

        /* Open session to security subsystem */
        status = sss_sscp_open_session(&sssSession, 0u, ELE_SUBSYSTEM, &sscpContext);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        /* Init keystore  */
        status = sss_sscp_key_store_init(&keyStore, &sssSession);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        status = hkdf();
        if (status != kStatus_Success)
        {
            break;
        }

        status = hkdf_multi();
        if (status != kStatus_Success)
        {
            break;
        }

        status = mp_kdf();
        if (status != kStatus_Success)
        {
            break;
        }

        status = ckdf();
        if (status != kStatus_Success)
        {
            break;
        }

        status = ckdf_scp03();
        if (status != kStatus_Success)
        {
            break;
        }

        /* Close keystore*/
        status = sss_sscp_key_store_free(&keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        /* Close session */
        status = sss_sscp_close_session(&sssSession);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        status = kStatus_Success;
    } while (false);

    if (status == kStatus_Success)
    {
        PRINTF("End of Example with SUCCESS!!\r\n\r\n");
    }
    else
    {
        PRINTF("ERROR: execution of commands on Security Sub-System failed!\r\n\r\n");
    }

    PRINTF("Example end\r\n");

    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
