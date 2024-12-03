/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "board.h"
#include "app.h"
#include "PN76_SKM.h"
#include "PN76_mbedcrypto_symm.h"
#include "mbedcrypto_symm_wrap.h"
#include "PN76_mbedcrypto_asymm.h"
#include "mbedcrypto_asymm_wrap.h"
#include "memory_buffer_alloc.h"
#include "platform.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_DOM2_BUF_LEN(contextLength) (32 + 1 + 1 + (contextLength))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_InitMbedCrypto(void);

static void APP_DumpArray(char *arrayName, const uint8_t *array, uint32_t arrayLen);

static void APP_FillDom2Buf(const uint8_t *context, int32_t contextLength, uint8_t phFlag, uint8_t *dom2Buf);

static int APP_AES_ECB(int mode);
static int APP_AES_CBC(int mode);
static int APP_CCM(void);
static int APP_GCM(void);
static int APP_SHA256(void);
static int APP_HMAC(void);
static int APP_ECCKeyGeneration(void);
static int APP_ECDSA(void);
static int APP_HKDF(void);
static int APP_HKDF_ExtractAndExpand(void);
static int APP_ECDH_ComputeShared(void);
static int APP_EdDSA_VerifyEd25519(void);
static int APP_EdDSA_VerifyEd25519ctx(void);
static int APP_EdDSA_VerifyEd25519ph(void);
static int APP_GetAsymmPubKeyFromPrivateKey(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t bufferapp[2048];

/* clang-format off */
static const struct
{
    mbedtls_ecp_group_id id;
    const char *name;
    uint16_t keySize;
} ecpInfo[] = {
    {
       MBEDTLS_ECP_DP_SECP256R1,
       "SECP256R1",
       256 / 8,
    },
    {
       MBEDTLS_ECP_DP_SECP384R1,
       "SECP384R1",
       384 / 8,
    },
    {
       MBEDTLS_ECP_DP_BP256R1,
       "BP256R1",
       256 / 8,
    },
    {
       MBEDTLS_ECP_DP_BP384R1,
       "BP384R1",
       384 / 8,
    }
};
/* clang-format on */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    int errors = 0;
    /* Board pin init */
    BOARD_InitHardware();

    PRINTF("\r\nPN mbedcrypto example started\r\n");

    APP_InitMbedCrypto();

    errors += APP_AES_ECB(MBEDTLS_AES_ENCRYPT);
    errors += APP_AES_ECB(MBEDTLS_AES_DECRYPT);
    errors += APP_AES_CBC(MBEDTLS_AES_ENCRYPT);
    errors += APP_AES_CBC(MBEDTLS_AES_DECRYPT);
    errors += APP_CCM();
    errors += APP_GCM();
    errors += APP_SHA256();
    errors += APP_HMAC();
    errors += APP_ECCKeyGeneration();
    errors += APP_ECDSA();
    errors += APP_HKDF();
    errors += APP_HKDF_ExtractAndExpand();
    errors += APP_ECDH_ComputeShared();
    errors += APP_EdDSA_VerifyEd25519();
    errors += APP_EdDSA_VerifyEd25519ctx();
    errors += APP_EdDSA_VerifyEd25519ph();
    errors += APP_GetAsymmPubKeyFromPrivateKey();

    if (errors > 0)
    {
        PRINTF("\r\nProject failed with %d errors\r\n", errors);
    }
    else
    {
        PRINTF("\r\nProject success\r\n");
    }

    while (1)
        ;
}

static void APP_DumpArray(char *arrayName, const uint8_t *array, uint32_t arrayLen)
{
    uint32_t i;

    PRINTF("%s:\r\n", arrayName);

    for (i = 0; i < arrayLen; i++)
    {
        PRINTF("%02x ", *array++);

        if ((i % 16) == 15)
        {
            PRINTF("\r\n");
        }
    }

    PRINTF("\r\n");
}

static void APP_FillDom2Buf(const uint8_t *context, int32_t contextLength, uint8_t phFlag, uint8_t *dom2Buf)
{
    /*
     * DOM2 result format:
     *
     * | SigEd25519 no Ed25519 collisions | phFlag | contextLength |         context          |
     * |             0:31                 |  32    |     33        |   34:34+contextLength-1  |
     */

    memcpy(dom2Buf, "SigEd25519 no Ed25519 collisions", 32);
    dom2Buf[32] = phFlag;
    dom2Buf[33] = contextLength;
    memcpy(&dom2Buf[34], context, contextLength);
}

static void APP_InitMbedCrypto(void)
{
    PN76_Status_t eKeyStoreStatus;
    uint8_t bKeyStoreStatus = 0;

    /*Initialize the crypto modules */
    phmbedcrypto_Init();

    eKeyStoreStatus = PN76_Sys_KeyStore_Init(&bKeyStoreStatus);

    /* bKeyStoreStatus 6th bit means fatal error. */
    if ((eKeyStoreStatus != PN76_STATUS_SUCCESS) || ((bKeyStoreStatus & 0x40U) != 0U))
    {
        PRINTF("Crypto initialization error\r\n");
        while (1)
            ; /* if Failed Do not go further */
    }

    mbedtls_memory_buffer_alloc_init(bufferapp, sizeof(bufferapp));
}

static int APP_AES_ECB(int mode)
{
    mbedtls_aes_context ctx;
    uint8_t key[32] = {0}, src[16] = {0}, output[16];
    uint16_t keySize;
    int result, ret = 0;
    const uint8_t *refOutput;

    /* clang-format off */
    static const uint8_t s_AesEcbDecRef[][16] =
    {
        /*
         * AES-ECB-128
         * KEY=0000000000000000000000000000000
         * CT=00000000000000000000000000000000
         */
        {0x14, 0x0F, 0x0F, 0x10, 0x11, 0xB5, 0x22, 0x3D, 0x79, 0x58, 0x77, 0x17, 0xFF, 0xD9, 0xEC, 0x3A},
        /*
         * AES-ECB-256
         * KEY=00000000000000000000000000000000000000000000000000000000000000
         * CT=00000000000000000000000000000000
         */
        {0x67, 0x67, 0x1C, 0xE1, 0xFA, 0x91, 0xDD, 0xEB, 0x0F, 0x8F, 0xBB, 0xB3, 0x66, 0xB5, 0x31, 0xB4},
    };

    static const uint8_t s_AesEcbEncRef[][16] =
    {
        /*
         * AES-ECB-128
         * KEY=0000000000000000000000000000000
         * PT=00000000000000000000000000000000
         */
        {0x66, 0xE9, 0x4B, 0xD4, 0xEF, 0x8A, 0x2C, 0x3B, 0x88, 0x4C, 0xFA, 0x59, 0xCA, 0x34, 0x2B, 0x2E},
        /*
         * AES-ECB-256
         * KEY=00000000000000000000000000000000000000000000000000000000000000
         * PT=00000000000000000000000000000000
         */
        {0xDC, 0x95, 0xC0, 0x78, 0xA2, 0x40, 0x89, 0x89, 0xAD, 0x48, 0xA2, 0x14, 0x92, 0x84, 0x20, 0x87},
    };
    /* clang-format on */

    static const uint16_t s_keySize[] = {128, 256};

    for (uint8_t i = 0; i < ARRAY_SIZE(s_keySize); i++)
    {
        keySize = s_keySize[i];

        PRINTF("AES-ECB-%u %s: ", keySize, (mode == MBEDTLS_AES_DECRYPT) ? "DEC" : "ENC");

        mbedtls_aes_init(&ctx);

        memset(output, 0x00, sizeof(output));

        if (mode == MBEDTLS_AES_ENCRYPT)
        {
            result    = mbedtls_aes_setkey_enc(&ctx, key, keySize);
            refOutput = s_AesEcbEncRef[i];
        }
        else
        {
            result    = mbedtls_aes_setkey_dec(&ctx, key, keySize);
            refOutput = s_AesEcbDecRef[i];
        }

        if (result != 0)
        {
            PRINTF("AES SetKey Failed\r\n");
            mbedtls_aes_free(&ctx);
            ret = 1;
            break;
        }

        result = mbedtls_aes_crypt_ecb(&ctx, mode, src, output);

        mbedtls_aes_free(&ctx);

        if ((result != 0) || (0 != memcmp(output, refOutput, sizeof(output))))
        {
            PRINTF("Failed\r\n");

            APP_DumpArray("Reference result", refOutput, sizeof(output));
            APP_DumpArray("Actual result", output, sizeof(output));

            ret = 1;
            break;
        }
        else
        {
            PRINTF("Pass\r\n");
        }
    }

    return ret;
}

static int APP_AES_CBC(int mode)
{
    mbedtls_aes_context ctx;
    uint8_t key[32] = {0}, src[32] = {0}, iv[16] = {0}, output[32];
    uint16_t keySize;
    int result, ret = 0;
    const uint8_t *refOutput;

    /* clang-format off */
    static const uint8_t s_AesCbcDecRef[][32] =
    {
        /*
         * AES-CBC-128
         * KEY=0000000000000000000000000000000
         * IV=00000000000000000000000000000000
         * CT=00000000000000000000000000000000000000000000000000000000000000000
         */
        {
            0x14, 0x0F, 0x0F, 0x10, 0x11, 0xB5, 0x22, 0x3D, 0x79, 0x58, 0x77, 0x17, 0xFF, 0xD9, 0xEC, 0x3A, /* 0x00-0x0F */
            0x14, 0x0F, 0x0F, 0x10, 0x11, 0xB5, 0x22, 0x3D, 0x79, 0x58, 0x77, 0x17, 0xFF, 0xD9, 0xEC, 0x3A, /* 0x10-0x1F */
        },
        /*
         * AES-CBC-256
         * KEY=0000000000000000000000000000000000000000000000000000000000000000
         * IV=00000000000000000000000000000000
         * CT=00000000000000000000000000000000000000000000000000000000000000000
         */
        {
            0x67, 0x67, 0x1C, 0xE1, 0xFA, 0x91, 0xDD, 0xEB, 0x0F, 0x8F, 0xBB, 0xB3, 0x66, 0xB5, 0x31, 0xB4, /* 0x00-0x0F */
            0x67, 0x67, 0x1C, 0xE1, 0xFA, 0x91, 0xDD, 0xEB, 0x0F, 0x8F, 0xBB, 0xB3, 0x66, 0xB5, 0x31, 0xB4, /* 0x10-0x1F */
        }
    };

    static const uint8_t s_AesCbcEncRef[][32] =
    {
        /*
         * AES-CBC-128
         * KEY=00000000000000000000000000000000
         * IV=00000000000000000000000000000000
         * PT=00000000000000000000000000000000000000000000000000000000000000000
         */
         {
             0x66, 0xE9, 0x4B, 0xD4, 0xEF, 0x8A, 0x2C, 0x3B, 0x88, 0x4C, 0xFA, 0x59, 0xCA, 0x34, 0x2B, 0x2E, /* 0x00-0x0F */
             0xF7, 0x95, 0xBD, 0x4A, 0x52, 0xE2, 0x9E, 0xD7, 0x13, 0xD3, 0x13, 0xFA, 0x20, 0xE9, 0x8D, 0xBC, /* 0x10-0x1F */
         },
         /*
          * AES-CBC-256
          * KEY=0000000000000000000000000000000000000000000000000000000000000000
          * IV=00000000000000000000000000000000
          * PT=00000000000000000000000000000000000000000000000000000000000000000
          */
         {
             0xDC, 0x95, 0xC0, 0x78, 0xA2, 0x40, 0x89, 0x89, 0xAD, 0x48, 0xA2, 0x14, 0x92, 0x84, 0x20, 0x87, /* 0x00-0x0F */
             0x08, 0xC3, 0x74, 0x84, 0x8C, 0x22, 0x82, 0x33, 0xC2, 0xB3, 0x4F, 0x33, 0x2B, 0xD2, 0xE9, 0xD3, /* 0x10-0x1F */
         }
    };
    /* clang-format on */

    static const uint16_t s_keySize[] = {128, 256};

    for (uint8_t i = 0; i < ARRAY_SIZE(s_keySize); i++)
    {
        keySize = s_keySize[i];

        PRINTF("AES-CBC-%u %s: ", keySize, (mode == MBEDTLS_AES_DECRYPT) ? "DEC" : "ENC");

        mbedtls_aes_init(&ctx);

        memset(output, 0x00, sizeof(output));
        memset(iv, 0, sizeof(iv));

        if (mode == MBEDTLS_AES_ENCRYPT)
        {
            result    = mbedtls_aes_setkey_enc(&ctx, key, keySize);
            refOutput = s_AesCbcEncRef[i];
        }
        else
        {
            result    = mbedtls_aes_setkey_dec(&ctx, key, keySize);
            refOutput = s_AesCbcDecRef[i];
        }

        if (result != 0)
        {
            PRINTF("AES SetKey Failed\r\n");
            mbedtls_aes_free(&ctx);
            ret = 1;
            break;
        }

        result = mbedtls_aes_crypt_cbc(&ctx, mode, sizeof(src), iv, src, output);

        mbedtls_aes_free(&ctx);

        if ((result != 0) || (0 != memcmp(output, refOutput, sizeof(output))))
        {
            PRINTF("Failed\r\n");

            APP_DumpArray("Reference result", refOutput, sizeof(output));
            APP_DumpArray("Actual result", output, sizeof(output));

            ret = 1;
            break;
        }
        else
        {
            PRINTF("Pass\r\n");
        }
    }

    return ret;
}

static int APP_CCM(void)
{
#define CCM_MSG_LEN 32
#define CCM_IV_LEN  12
#define CCM_ADD_LEN 16
#define CCM_TAG_LEN 16

    mbedtls_ccm_context ctx;
    uint8_t key[32] = {0}, plainData[CCM_MSG_LEN] = {0}, iv[CCM_IV_LEN] = {0}, add[CCM_ADD_LEN] = {0};
    uint8_t cipherData[CCM_MSG_LEN], tag[CCM_TAG_LEN], decryptPlainData[CCM_MSG_LEN];
    uint16_t keySize;
    int result, ret = 0;
    const uint8_t *refCipherData;
    const uint8_t *refTag;

    /* clang-format off */
    static const uint8_t s_CcmTagRef[][CCM_TAG_LEN] =
    {
        /* Key size: 128-bit */
        {0x71, 0xDD, 0xBE, 0xA1, 0x66, 0xED, 0xDB, 0x57, 0x22, 0xC8, 0xC2, 0x6A, 0x69, 0xF3, 0x42, 0x8D},
        /* Key size: 256-bit */
        {0x0E, 0xE4, 0x12, 0x1E, 0x03, 0x4E, 0x6C, 0x8F, 0x40, 0x06, 0xA8, 0x64, 0xED, 0x53, 0x33, 0x68}
    };

    static const uint8_t s_CcmCipherRef[][CCM_MSG_LEN] =
    {
        /* Key size: 128-bit */
        {
            0x6E, 0xC7, 0x5F, 0xB2, 0xE2, 0xB4, 0x87, 0x46, 0x1E, 0xDD, 0xCB, 0xB8, 0x97, 0x11, 0x92, 0xBA, /* 0x00-0x0F */
            0x33, 0xCB, 0x66, 0x92, 0x4B, 0x96, 0x7C, 0xD9, 0x95, 0x9F, 0x78, 0x23, 0x0C, 0x20, 0xB3, 0x42, /* 0x10-0x1F */
        },
        /* Key size: 256-bit */
        {
            0xC1, 0x94, 0x40, 0x44, 0xC8, 0xE7, 0xAA, 0x95, 0xD2, 0xDE, 0x95, 0x13, 0xC7, 0xF3, 0xDD, 0x8C, /* 0x00-0x0F */
            0xCF, 0x5F, 0x8A, 0xC0, 0x52, 0xDB, 0xD3, 0x1F, 0xF1, 0x29, 0x76, 0x12, 0x43, 0x7F, 0xB1, 0x96, /* 0x10-0x1F */
        }
    };
    /* clang-format on */

    static const uint16_t s_keySize[] = {128, 256};

    for (uint8_t i = 0; i < ARRAY_SIZE(s_keySize); i++)
    {
        keySize       = s_keySize[i];
        refTag        = s_CcmTagRef[i];
        refCipherData = s_CcmCipherRef[i];

        PRINTF("CCM with key size %d-bit: ", keySize);

        memset(cipherData, 0, sizeof(cipherData));
        memset(decryptPlainData, 0, sizeof(decryptPlainData));
        memset(tag, 0, sizeof(tag));

        mbedtls_ccm_init(&ctx);

        result = mbedtls_ccm_setkey(&ctx, MBEDTLS_CIPHER_ID_AES, key, keySize);

        if (result != 0)
        {
            PRINTF("\r\nCCM set key error\r\n");
            mbedtls_ccm_free(&ctx);
            ret = 1;
            break;
        }

        /* Encrypt and Tag. */
        result = mbedtls_ccm_encrypt_and_tag(&ctx, sizeof(plainData), iv, sizeof(iv), add, sizeof(add), plainData,
                                             cipherData, tag, sizeof(tag));

        if ((result != 0) || (memcmp(refTag, tag, sizeof(tag))) ||
            (memcmp(refCipherData, cipherData, sizeof(cipherData))))
        {
            PRINTF("\r\nCCM encrypt and tag error\r\n");

            APP_DumpArray("Reference cipher data", refCipherData, sizeof(cipherData));
            APP_DumpArray("Actual cipher data", cipherData, sizeof(cipherData));

            APP_DumpArray("Reference tag", refTag, sizeof(tag));
            APP_DumpArray("Actual tag", tag, sizeof(tag));

            mbedtls_ccm_free(&ctx);
            ret = 1;
            break;
        }

        /* CCM Auth and Decrypt */
        result = mbedtls_ccm_auth_decrypt(&ctx, sizeof(plainData), iv, sizeof(iv), add, sizeof(add), cipherData,
                                          decryptPlainData, tag, sizeof(tag));

        if ((result != 0) || (memcmp(decryptPlainData, plainData, sizeof(plainData))))
        {
            PRINTF("\r\nCCM auth and decrypt error\r\n");

            APP_DumpArray("Reference plain data", plainData, sizeof(plainData));
            APP_DumpArray("Actual plain data", decryptPlainData, sizeof(decryptPlainData));

            ret = 1;
        }
        else
        {
            PRINTF("Pass\r\n");
        }

        mbedtls_ccm_free(&ctx);
    }

    return ret;
}

static int APP_SHA256(void)
{
    uint8_t input[8] = {0};
    uint8_t output[32];
    int len;
    int result;
    int ret = 0;

    /* clang-format off */
    const uint8_t refSha256[] =
    {
        0xAF, 0x55, 0x70, 0xF5, 0xA1, 0x81, 0x0B, 0x7A, 0xF7, 0x8C, 0xAF, 0x4B, 0xC7, 0x0A, 0x66, 0x0F, /* 0x00-0x0F */
        0x0D, 0xF5, 0x1E, 0x42, 0xBA, 0xF9, 0x1D, 0x4D, 0xE5, 0xB2, 0x32, 0x8D, 0xE0, 0xE8, 0x3D, 0xFC, /* 0x10-0x1F */
    };
    /* clang-format on */

    len = 256;
    PRINTF("SHA256: ");

    memset(output, 0, sizeof(output));

    result = mbedtls_sha256_ret(input, sizeof(input), output, 0); /* SHA224 is not supported. */

    if ((0 != result) || (memcmp(refSha256, output, len / 8) != 0))
    {
        PRINTF("Failed\r\n");

        APP_DumpArray("Reference result", refSha256, sizeof(refSha256));
        APP_DumpArray("Actual result", output, sizeof(output));

        ret = 1;
    }
    else
    {
        PRINTF("Pass\r\n");
    }

    return ret;
}

static int APP_GCM(void)
{
#define GCM_MSG_LEN 32
#define GCM_IV_LEN  12
#define GCM_ADD_LEN 16
#define GCM_TAG_LEN 16

    mbedtls_gcm_context ctx;
    uint8_t key[32] = {0}, plainData[GCM_MSG_LEN] = {0}, iv[GCM_IV_LEN] = {0}, add[GCM_ADD_LEN] = {0};
    uint8_t cipherData[GCM_MSG_LEN], tag[GCM_TAG_LEN], decryptPlainData[GCM_MSG_LEN];
    uint16_t keySize;
    int result, ret = 0;
    const uint8_t *refCipherData;
    const uint8_t *refTag;

    /* clang-format off */
    static const uint8_t s_GcmTagRef[][GCM_TAG_LEN] =
    {
        /* Key size: 128-bit */
        {0x39, 0x69, 0x1D, 0x1A, 0xB7, 0x0A, 0x22, 0x08, 0x4A, 0xE9, 0x33, 0x7C, 0x69, 0x7C, 0x3B, 0xEE},
        /* Key size: 256-bit */
        {0xAF, 0x99, 0xD7, 0x9A, 0xDB, 0x9A, 0x2E, 0x0D, 0x4F, 0x7B, 0x46, 0x21, 0xAB, 0xA1, 0x48, 0xF0}
    };

    static const uint8_t s_GcmCipherRef[][GCM_MSG_LEN] =
    {
        /* Key size: 128-bit */
        {
          0x03, 0x88, 0xDA, 0xCE, 0x60, 0xB6, 0xA3, 0x92, 0xF3, 0x28, 0xC2, 0xB9, 0x71, 0xB2, 0xFE, 0x78, /* 0x00-0x0F */
          0xF7, 0x95, 0xAA, 0xAB, 0x49, 0x4B, 0x59, 0x23, 0xF7, 0xFD, 0x89, 0xFF, 0x94, 0x8B, 0xC1, 0xE0, /* 0x10-0x1F */
        },
        /* Key size: 256-bit */
        {
          0xCE, 0xA7, 0x40, 0x3D, 0x4D, 0x60, 0x6B, 0x6E, 0x07, 0x4E, 0xC5, 0xD3, 0xBA, 0xF3, 0x9D, 0x18, /* 0x00-0x0F */
          0x72, 0x60, 0x03, 0xCA, 0x37, 0xA6, 0x2A, 0x74, 0xD1, 0xA2, 0xF5, 0x8E, 0x75, 0x06, 0x35, 0x8E, /* 0x10-0x1F */
        }
    };
    /* clang-format on */

    static const uint16_t s_keySize[] = {128, 256};

    for (uint8_t i = 0; i < ARRAY_SIZE(s_keySize); i++)
    {
        keySize       = s_keySize[i];
        refTag        = s_GcmTagRef[i];
        refCipherData = s_GcmCipherRef[i];

        PRINTF("GCM with key size %d-bit: ", keySize);

        memset(cipherData, 0, sizeof(cipherData));
        memset(decryptPlainData, 0, sizeof(decryptPlainData));
        memset(tag, 0, sizeof(tag));

        mbedtls_gcm_init(&ctx);

        result = mbedtls_gcm_setkey(&ctx, MBEDTLS_CIPHER_ID_AES, key, keySize);

        if (result != 0)
        {
            PRINTF("\r\nGCM set key error\r\n");
            mbedtls_gcm_free(&ctx);
            ret = 1;
            break;
        }

        /* Encrypt and Tag. */
        result = mbedtls_gcm_crypt_and_tag(&ctx, MBEDTLS_GCM_ENCRYPT, sizeof(plainData), iv, sizeof(iv), add,
                                           sizeof(add), plainData, cipherData, sizeof(tag), tag);

        if ((result != 0) || (memcmp(refTag, tag, sizeof(tag))) ||
            (memcmp(refCipherData, cipherData, sizeof(cipherData))))
        {
            PRINTF("\r\nGCM encrypt and tag error\r\n");

            APP_DumpArray("Reference cipher data", refCipherData, sizeof(cipherData));
            APP_DumpArray("Actual cipher data", cipherData, sizeof(cipherData));

            APP_DumpArray("Reference tag", refTag, sizeof(tag));
            APP_DumpArray("Actual tag", tag, sizeof(tag));

            mbedtls_gcm_free(&ctx);
            ret = 1;
            break;
        }

        /* GCM Auth and Decrypt */
        result = mbedtls_gcm_auth_decrypt(&ctx, sizeof(plainData), iv, sizeof(iv), add, sizeof(add), tag, sizeof(tag),
                                          cipherData, decryptPlainData);

        if ((result != 0) || (memcmp(decryptPlainData, plainData, sizeof(plainData))))
        {
            PRINTF("\r\nGCM auth and decrypt error\r\n");

            APP_DumpArray("Reference plain data", plainData, sizeof(plainData));
            APP_DumpArray("Actual plain data", decryptPlainData, sizeof(decryptPlainData));

            ret = 1;
        }
        else
        {
            PRINTF("Pass\r\n");
        }

        mbedtls_gcm_free(&ctx);
    }

    return ret;
}

static int APP_HMAC(void)
{
    uint8_t key[16]    = {0};
    uint8_t input[16]  = {0};
    uint8_t output[32] = {0};
    int result;
    int ret = 0;

    PRINTF("HMAC using SHA256: ");

    /* clang-format off */
    const uint8_t ref[] =
    {
        0x85, 0x3C, 0x74, 0x03, 0x93, 0x7D, 0x8B, 0x62, 0x39, 0x56, 0x9B, 0x18, 0x4E, 0xB7, 0x99, 0x3F, /* 0x00-0x0F */
        0xC5, 0xF7, 0x51, 0xAE, 0xFC, 0xEA, 0x28, 0xF2, 0xC8, 0x63, 0x85, 0x8E, 0x2D, 0x29, 0xC5, 0x0B, /* 0x10-0x1F */
    };
    /* clang-format on */

    memset(output, 0x00, sizeof(output));

    /* Only support SHA256, md_info is ignored. */
    result = mbedtls_md_hmac(NULL, key, sizeof(key), input, sizeof(input), output);

    if ((result != 0) && (memcmp(ref, output, sizeof(output)) != 0))
    {
        PRINTF("Failed\r\n");

        APP_DumpArray("Reference result", ref, sizeof(ref));
        APP_DumpArray("Actual result", output, sizeof(output));

        ret = 1;
    }
    else
    {
        PRINTF("Pass\r\n");
    }

    return ret;
}

static int APP_ECCKeyGeneration(void)
{
    mbedtls_ecp_group grp;
    mbedtls_ecp_point Q;
    mbedtls_mpi d;
    int result;
    int ret = 0;
    uint8_t buflen;
    uint8_t aTmpBuff[384 / 8] = {0};

    PRINTF("ECC Key Generation\r\n");

    mbedtls_mpi_init(&d);
    mbedtls_mpi_init(&Q.X);
    mbedtls_mpi_init(&Q.Y);
    mbedtls_mpi_init(&Q.Z);

    for (int i = 0; i < ARRAY_SIZE(ecpInfo); i++)
    {
        grp.id = ecpInfo[i].id;
        buflen = ecpInfo[i].keySize;

        PRINTF("%s\r\n", ecpInfo[i].name);

        result = mbedtls_ecp_gen_keypair(&grp, &d, &Q, NULL, NULL);

        if (result != 0)
        {
            PRINTF("ECC Key Pair generation failed\r\n");
            ret = 1;
            break;
        }

        mbedtls_mpi_write_binary(&d, aTmpBuff, buflen);
        APP_DumpArray("ECC Private Key", aTmpBuff, buflen);

        mbedtls_mpi_write_binary(&Q.X, aTmpBuff, buflen);
        APP_DumpArray("ECC Public Key X", aTmpBuff, buflen);

        mbedtls_mpi_write_binary(&Q.Y, aTmpBuff, buflen);
        APP_DumpArray("ECC Public Key Y", aTmpBuff, buflen);
    }

    mbedtls_mpi_free(&d);
    mbedtls_mpi_free(&Q.X);
    mbedtls_mpi_free(&Q.Y);
    mbedtls_mpi_free(&Q.Z);

    return ret;
}

static int APP_ECDSA(void)
{
    mbedtls_ecp_group grp;
    mbedtls_ecp_point Q;
    mbedtls_mpi d, r, s;
    int ret                = 0;
    uint8_t src[8]         = {0};
    uint8_t hashoutput[32] = {0};

    mbedtls_mpi_init(&d);
    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);
    mbedtls_mpi_init(&Q.X);
    mbedtls_mpi_init(&Q.Y);
    mbedtls_mpi_init(&Q.Z);

    PRINTF("ECDSA sign and verify:\r\n");

    do
    {
        /* Get the hash value of source data. */
        if (0 != mbedtls_sha256_ret(src, sizeof(src), hashoutput, 0))
        {
            PRINTF("SHA256 Hashing Failed\r\n");
            ret = 1;
            break;
        }

        for (int i = 0; i < ARRAY_SIZE(ecpInfo); i++)
        {
            grp.id = ecpInfo[i].id;
            PRINTF("%s: ", ecpInfo[i].name);

            /* Get the key pair for sign and verify. */
            if (0 != mbedtls_ecp_gen_keypair(&grp, &d, &Q, NULL, NULL))
            {
                PRINTF("Get Key Pair Failed\r\n");
                ret = 1;
                break;
            }

            if (0 != mbedtls_ecdsa_sign(&grp, &r, &s, &d, hashoutput, sizeof(hashoutput), NULL, NULL))
            {
                PRINTF("ECDSA Sign Failed\r\n");
                ret = 1;
                break;
            }

            if (0 != mbedtls_ecdsa_verify(&grp, hashoutput, sizeof(hashoutput), &Q, &r, &s))
            {
                PRINTF("ECDSA Verify Failed\r\n");
                ret = 1;
                break;
            }

            PRINTF("Pass\r\n");
        }

    } while (0);

    mbedtls_mpi_free(&d);
    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);
    mbedtls_mpi_free(&Q.X);
    mbedtls_mpi_free(&Q.Y);
    mbedtls_mpi_free(&Q.Z);

    return ret;
}

static int APP_HKDF(void)
{
    uint8_t key[16]    = {0};
    uint8_t output[32] = {0};
    uint8_t salt[8]    = {0};
    uint8_t info[128]  = {0};
    int result;
    int ret = 0;

    static const uint8_t ref[] = {
        0xF0, 0x57, 0xEE, 0x6A, 0xDA, 0x0D, 0xD2, 0xA6, 0xB5, 0x79, 0xD7, 0x6B, 0xBE, 0x97, 0x97, 0xE0, /* 0x00-0x0F */
        0x80, 0x10, 0xA3, 0xF9, 0x54, 0x38, 0x2C, 0xB5, 0x60, 0x54, 0xFC, 0x00, 0x27, 0x97, 0x1A, 0x45, /* 0x10-0x1F */
    };

    PRINTF("HKDF using SHA256: ");

    /* Only support SHA256. */
    result = mbedtls_hkdf(NULL, salt, sizeof(salt), key, sizeof(key), info, sizeof(info), output, sizeof(output));

    if ((0 != result) && (memcmp(ref, output, sizeof(output)) != 0))
    {
        PRINTF("Fail.\r\n");

        APP_DumpArray("Reference result", ref, sizeof(ref));
        APP_DumpArray("Actual result", output, sizeof(output));
        ret = 1;
    }
    else
    {
        PRINTF("Pass.\r\n");
    }

    return ret;
}

static int APP_HKDF_ExtractAndExpand(void)
{
    uint8_t key[16]       = {0};
    uint8_t outputKey[32] = {0};
    uint8_t salt[8]       = {0};
    uint8_t info[128]     = {0};
    uint8_t outputPrk[32] = {0};
    int ret               = 0;
    int result;

    static const uint8_t refOutputKey[] = {
        0xF0, 0x57, 0xEE, 0x6A, 0xDA, 0x0D, 0xD2, 0xA6, 0xB5, 0x79, 0xD7, 0x6B, 0xBE, 0x97, 0x97, 0xE0, /* 0x00-0x0F */
        0x80, 0x10, 0xA3, 0xF9, 0x54, 0x38, 0x2C, 0xB5, 0x60, 0x54, 0xFC, 0x00, 0x27, 0x97, 0x1A, 0x45, /* 0x10-0x1F */
    };

    static const uint8_t refPrk[] = {
        0xB3, 0x06, 0xE7, 0x32, 0x67, 0x2D, 0x86, 0x35, 0x77, 0x59, 0x8C, 0x89, 0x91, 0xF9, 0xD8, 0xB9, /* 0x00-0x0F */
        0x40, 0x49, 0x55, 0xD1, 0xD9, 0xF8, 0xE5, 0xCA, 0x8E, 0xE0, 0xB6, 0x3D, 0x5C, 0x42, 0xB8, 0x54, /* 0x10-0x1F */
    };

    do
    {
        /* Only support SHA256, so md_info is NULL. */

        /* Extract. */
        PRINTF("HKDF extract using SHA256: ");

        result = mbedtls_hkdf_extract(NULL, salt, sizeof(salt), key, sizeof(key), outputPrk);

        if ((0 != result) && (memcmp(refPrk, outputPrk, sizeof(outputPrk)) != 0))
        {
            PRINTF("Fail.\r\n");

            APP_DumpArray("Reference PRK", refPrk, sizeof(refPrk));
            APP_DumpArray("Actual PRK", outputPrk, sizeof(outputPrk));

            ret = 1;
            break;
        }
        else
        {
            PRINTF("Pass.\r\n");
        }

        /* Expand */
        PRINTF("HKDF expand using SHA256: ");

        result =
            mbedtls_hkdf_expand(NULL, outputPrk, sizeof(outputPrk), info, sizeof(info), outputKey, sizeof(outputKey));
        if ((0 != result) && (memcmp(refOutputKey, outputKey, sizeof(outputKey)) != 0))
        {
            PRINTF("Fail.\r\n");

            APP_DumpArray("Reference output key", refOutputKey, sizeof(refOutputKey));
            APP_DumpArray("Actual output key", outputKey, sizeof(outputKey));

            ret = 1;
            break;
        }
        else
        {
            PRINTF("Pass.\r\n");
        }
    } while (0);

    return ret;
}

/*
 * In this function, Bob and Alice have their own private key d_Bob, d_Alice,
 * and public key Q_Bob, Q_Alice.
 * Then Bob's shared key ss_Bob is calculated based on d_Bob and Q_Alice.
 * Alice's shared key ss_Alice is calculated based on d_Alice and Q_Bob.
 * ss_Bob should be the same with ss_Alice.
 */
static int APP_ECDH_ComputeShared(void)
{
    mbedtls_ecp_group grp;
    mbedtls_ecp_point Q_Bob, Q_Alice;
    mbedtls_mpi d_Bob, d_Alice;
    mbedtls_mpi ss_Bob, ss_Alice;

    int result;
    int ret = 0;

    mbedtls_mpi_init(&ss_Bob);
    mbedtls_mpi_init(&d_Bob);
    mbedtls_mpi_init(&Q_Bob.X);
    mbedtls_mpi_init(&Q_Bob.Y);
    mbedtls_mpi_init(&Q_Bob.Z);
    mbedtls_mpi_init(&ss_Alice);
    mbedtls_mpi_init(&d_Alice);
    mbedtls_mpi_init(&Q_Alice.X);
    mbedtls_mpi_init(&Q_Alice.Y);
    mbedtls_mpi_init(&Q_Alice.Z);

    for (int i = 0; i < ARRAY_SIZE(ecpInfo); i++)
    {
        grp.id = ecpInfo[i].id;

        PRINTF("ECDH using %s: ", ecpInfo[i].name);

        /* Create key pair for Bob and Alice */
        result = mbedtls_ecp_gen_keypair(&grp, &d_Bob, &Q_Bob, NULL, NULL);
        result += mbedtls_ecp_gen_keypair(&grp, &d_Alice, &Q_Alice, NULL, NULL);

        if (result != 0)
        {
            PRINTF("ECC Key Pair generation failed\r\n");
            ret = 1;
            break;
        }

        /* Compute the shared key. */
        result = mbedtls_ecdh_compute_shared(&grp, &ss_Bob, &Q_Alice, &d_Bob, NULL, NULL);
        result += mbedtls_ecdh_compute_shared(&grp, &ss_Alice, &Q_Bob, &d_Alice, NULL, NULL);

        if (result != 0)
        {
            PRINTF("Compute shared failed\r\n");
            ret = 1;
            break;
        }

        /* The shared key should be the same. */
        if (mbedtls_mpi_cmp_mpi(&ss_Bob, &ss_Alice) != 0)
        {
            PRINTF("Fail\r\n");
            ret = 1;
            break;
        }
        else
        {
            PRINTF("Pass\r\n");
        }
    }

    mbedtls_mpi_free(&ss_Bob);
    mbedtls_mpi_free(&d_Bob);
    mbedtls_mpi_free(&Q_Bob.X);
    mbedtls_mpi_free(&Q_Bob.Y);
    mbedtls_mpi_free(&Q_Bob.Z);
    mbedtls_mpi_free(&ss_Alice);
    mbedtls_mpi_free(&d_Alice);
    mbedtls_mpi_free(&Q_Alice.X);
    mbedtls_mpi_free(&Q_Alice.Y);
    mbedtls_mpi_free(&Q_Alice.Z);

    return ret;
}

static int APP_EdDSA_VerifyEd25519(void)
{
    int ret = 0;

    static uint8_t digest[1] = {0x72U};

    static uint8_t publicKey[] = {
        0x0C, 0x66, 0xF4, 0x2A, 0xF1, 0x55, 0xCD, 0xC0, 0x8C, 0x96, 0xC4, 0x2E, 0xCF, 0x2C, 0x98, 0x9C, /* 0x00-0x0F. */
        0xBC, 0x7E, 0x1B, 0x4D, 0xA7, 0x0A, 0xB7, 0x92, 0x5A, 0x89, 0x43, 0xE8, 0xC3, 0x17, 0x40, 0x3D  /* 0x10-0x1F. */
    };

    /* Signature R. */
    static uint8_t R[] = {
        0xDA, 0x69, 0xDB, 0xEB, 0x23, 0x22, 0x76, 0xB3, 0x8F, 0x3F, 0x50, 0x16, 0x54, 0x7B, 0xB2, 0xA2, /* 0x00-0x0F. */
        0x40, 0x25, 0x64, 0x5F, 0x0B, 0x82, 0x0E, 0x72, 0xB8, 0xCA, 0xD4, 0xF0, 0xA9, 0x09, 0xA0, 0x92  /* 0x10-0x1F. */
    };

    /* Signature S. */
    static uint8_t S[] = {
        0x00, 0x0C, 0xBB, 0x12, 0x16, 0x29, 0x0D, 0xB0, 0xEE, 0x2A, 0x30, 0xB4, 0xAE, 0x2E, 0x7B, 0x38, /* 0x00-0x0F. */
        0x8C, 0x1D, 0xF1, 0xD0, 0x13, 0x36, 0x8F, 0x45, 0x6E, 0x99, 0x15, 0x3E, 0xE4, 0xC1, 0x5A, 0x08, /* 0x10-0x1F. */
    };

    PRINTF("EdDSA Ed25519: ");

    if (0 != phmbedcrypto_EdDSA_Verify(digest, sizeof(digest), NULL, publicKey, R, S))
    {
        PRINTF("Fail.\r\n");
        ret = 1;
    }
    else
    {
        PRINTF("Pass.\r\n");
    }

    return ret;
}

static int APP_EdDSA_VerifyEd25519ctx(void)
{
    int ret = 0;
    MPInt_t dom2Context;
    uint8_t *dom2Buf;
    uint16_t dom2BufLength;

    static uint8_t digest[0x10] = {0xf7, 0x26, 0x93, 0x6d, 0x19, 0xc8, 0x00, 0x49,
                                   0x4e, 0x3f, 0xda, 0xff, 0x20, 0xb2, 0x76, 0xa8};

    static uint8_t publicKey[] = {
        0x92, 0x82, 0x24, 0xAB, 0xE0, 0xFE, 0x0C, 0x86, 0xFB, 0x8B, 0xAD, 0xB4, 0x2B, 0x1C, 0x85, 0xD6, /* 0x00-0x0F. */
        0xAE, 0xF9, 0xF5, 0x9C, 0x25, 0xF0, 0x29, 0x0C, 0x7F, 0x8F, 0x96, 0x4F, 0x5E, 0x42, 0xC9, 0xDF  /* 0x10-0x1F. */
    };

    /* Signature R. */
    static uint8_t R[] = {
        0xE3, 0xAE, 0xB1, 0xBC, 0x6D, 0xAB, 0xAB, 0x97, 0xB0, 0x6D, 0x60, 0x44, 0xC0, 0xBC, 0x92, 0x8F, /* 0x00-0x0F. */
        0x80, 0xD5, 0x51, 0x43, 0x5B, 0x8B, 0x9F, 0xA6, 0x3A, 0x6B, 0xC4, 0x2F, 0x87, 0xD5, 0x60, 0xFC  /* 0x10-0x1F. */
    };

    /* Signature S. */
    static uint8_t S[] = {
        0x0D, 0xE9, 0xB3, 0x4E, 0x3D, 0x04, 0xC2, 0x91, 0x62, 0x34, 0x68, 0x66, 0xDF, 0xA2, 0x5C, 0x5A, /* 0x00-0x0F. */
        0xE1, 0x8E, 0x8F, 0xD2, 0xD1, 0x86, 0xB1, 0xB5, 0x31, 0x64, 0xB6, 0xB3, 0xE8, 0x48, 0x6C, 0x21  /* 0x10-0x1F. */
    };

    /* Context. */
    static const uint8_t context[] = {0x62, 0x61, 0x72};

    PRINTF("EdDSA Ed25519ctx: ");

    /* Allocate buffer for DOM2 context. */
    dom2BufLength = APP_DOM2_BUF_LEN(sizeof(context));

    dom2Buf = mbedtls_calloc(dom2BufLength, sizeof(uint8_t));

    if (NULL == dom2Buf)
    {
        PRINTF("Memory allocation failed\r\n");
        return 1;
    }

    /* Fill the context dom2 buffer, phFlag is 0 here. */
    APP_FillDom2Buf(context, sizeof(context), 0, dom2Buf);

    dom2Context.pMPInt    = dom2Buf;
    dom2Context.wNumBytes = dom2BufLength;

    /* Verify. */
    if (0 != phmbedcrypto_EdDSA_Verify(digest, sizeof(digest), &dom2Context, publicKey, R, S))
    {
        PRINTF("Fail.\r\n");
        ret = 1;
    }
    else
    {
        PRINTF("Pass.\r\n");
    }

    mbedtls_free(dom2Buf);

    return ret;
}

static int APP_EdDSA_VerifyEd25519ph(void)
{
    int ret = 0;
    MPInt_t dom2Context;
    uint8_t *dom2Buf;
    uint16_t dom2BufLength;
    uint8_t sha512hash[64];

    static uint8_t publicKey[] = {
        0xBF, 0xE2, 0x67, 0x34, 0x68, 0x19, 0xF8, 0xEB, 0x64, 0x4D, 0xFD, 0x2E, 0xEF, 0x67, 0x54, 0xC3, /* 0x00-0x0F. */
        0x34, 0x50, 0x24, 0xE1, 0x70, 0x2C, 0x93, 0xF4, 0x3B, 0x56, 0x5E, 0xAD, 0x93, 0x2B, 0x17, 0xEC, /* 0x10-0x1F. */
    };

    /* Signature R. */
    static uint8_t R[] = {
        0x41, 0xAE, 0x6D, 0x4E, 0xB9, 0x9B, 0x49, 0x39, 0x76, 0xF8, 0x7F, 0x9C, 0x46, 0x2B, 0x46, 0x9E, /* 0x00-0x0F. */
        0x80, 0x3F, 0x68, 0x3D, 0x81, 0x0F, 0xD3, 0xA9, 0x1A, 0x12, 0xB8, 0xF0, 0x22, 0x02, 0xA7, 0x98  /* 0x10-0x1F. */
    };

    /* Signature S. */
    static uint8_t S[] = {
        0x06, 0x34, 0x08, 0x26, 0x2A, 0x1C, 0xD1, 0xAA, 0x2A, 0x06, 0x36, 0xE6, 0x61, 0x8C, 0x0B, 0xA1, /* 0x00-0x0F. */
        0xAA, 0xF5, 0xAD, 0x62, 0xD0, 0x03, 0x20, 0x5A, 0x35, 0x2A, 0x3C, 0x46, 0x42, 0x50, 0xF8, 0x31  /* 0x10-0x1F. */
    };

    /* Context. */
    static const uint8_t context[] = {0x61, 0x62, 0x63};

    PRINTF("EdDSA Ed25519ph: ");

    /* In this mode, the digest is SHA512 of context. */
    if (mbedtls_sha512_ret(context, sizeof(context), sha512hash, 0) != 0)
    {
        PRINTF("SHA512 calculation error\r\n");
        return 1;
    }

    /* Allocate buffer for DOM2 context. */
    dom2BufLength = APP_DOM2_BUF_LEN(0);

    dom2Buf = mbedtls_calloc(dom2BufLength, sizeof(uint8_t));

    if (NULL == dom2Buf)
    {
        PRINTF("Memory allocation failed\r\n");
        return 1;
    }

    /* Fill the context dom2 buffer, phFlag is 1 here. */
    APP_FillDom2Buf(NULL, 0, 1, dom2Buf);

    dom2Context.pMPInt    = dom2Buf;
    dom2Context.wNumBytes = dom2BufLength;

    /* Verify. */
    if (0 != phmbedcrypto_EdDSA_Verify(sha512hash, sizeof(sha512hash), &dom2Context, publicKey, R, S))
    {
        PRINTF("Fail.\r\n");
        ret = 1;
    }
    else
    {
        PRINTF("Pass.\r\n");
    }

    mbedtls_free(dom2Buf);

    return ret;
}

/*
 * In this function, ECC key pair is created using mbedtls_ecp_gen_keypair,
 * private key is d, and public key is refQ.
 * Then function phmbedcrypto_Get_AsymmPubKey calculates public key Q based on the
 * private key d. At last Q is compared with refQ, they should be the same.
 */
static int APP_GetAsymmPubKeyFromPrivateKey(void)
{
    int ret = 0;
    mbedtls_ecp_group grp;
    mbedtls_mpi d;
    mbedtls_ecp_point Q, refQ;

    mbedtls_mpi_init(&d);
    mbedtls_mpi_init(&Q.X);
    mbedtls_mpi_init(&Q.Y);
    mbedtls_mpi_init(&Q.Z);
    mbedtls_mpi_init(&refQ.X);
    mbedtls_mpi_init(&refQ.Y);
    mbedtls_mpi_init(&refQ.Z);

    PRINTF("Get Asymmetric Public Key From Private Key:\r\n");

    for (int i = 0; i < ARRAY_SIZE(ecpInfo); i++)
    {
        grp.id = ecpInfo[i].id;
        PRINTF("%s: ", ecpInfo[i].name);

        /* Get the key pair for sign and verify. */
        if (0 != mbedtls_ecp_gen_keypair(&grp, &d, &refQ, NULL, NULL))
        {
            PRINTF("Get Key Pair Failed\r\n");
            ret = 1;
            break;
        }

        if (0 != phmbedcrypto_Get_AsymmPubKey(&grp, &d, &Q))
        {
            PRINTF("Get Public Key Failed\r\n");
            ret = 1;
            break;
        }

        /* Q should be the same with refQ */
        if ((mbedtls_mpi_cmp_mpi(&Q.X, &refQ.X) != 0) || (mbedtls_mpi_cmp_mpi(&Q.Y, &refQ.Y) != 0))
        {
            PRINTF("Calculated Public Key Wrong\r\n");
            ret = 1;
            break;
        }

        PRINTF("Pass\r\n");
    }

    mbedtls_mpi_free(&d);
    mbedtls_mpi_free(&Q.X);
    mbedtls_mpi_free(&Q.Y);
    mbedtls_mpi_free(&Q.Z);
    mbedtls_mpi_free(&refQ.X);
    mbedtls_mpi_free(&refQ.Y);
    mbedtls_mpi_free(&refQ.Z);

    return ret;
}
