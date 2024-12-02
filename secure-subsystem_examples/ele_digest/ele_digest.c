/*
 * Copyright 2024 NXP
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

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ELE_MAX_SUBSYSTEM_WAIT (0xFFFFFFFFu)
#define ELE_SUBSYSTEM          (kType_SSS_Ele200)
#define KEY_ID                 (0u)
#define CMAC_KEY_SIZE          (16u)
#define HMAC_KEY_SIZE          (32u)
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
static const uint8_t message[] =
    "Be that word our sign of parting, bird or fiend! I shrieked upstarting"
    "Get thee back into the tempest and the Nights Plutonian shore!"
    "Leave no black plume as a token of that lie thy soul hath spoken!"
    "Leave my loneliness unbroken! quit the bust above my door!"
    "Take thy beak from out my heart, and take thy form from off my door!"
    "Quoth the raven, Nevermore.  ";
static const size_t message_length     = sizeof(message) - 1;

static const uint8_t sha1_expected[]   = {0x41, 0x82, 0xa6, 0x54, 0x16, 0x4a, 0x18, 0x12, 0xe0, 0xad,
                                          0x0d, 0xed, 0x8d, 0xa1, 0x41, 0xe4, 0xcf, 0xe9, 0xb4, 0x31};

static const uint8_t sha256_expected[] = {0x63, 0x76, 0xea, 0xcc, 0xc9, 0xa2, 0xc0, 0x43, 0xf4, 0xfb, 0x01,
                                          0x34, 0x69, 0xb3, 0x0c, 0xf5, 0x28, 0x63, 0x5c, 0xfa, 0xa5, 0x65,
                                          0x60, 0xef, 0x59, 0x7b, 0xd9, 0x1c, 0xac, 0xaa, 0x31, 0xf7};

static const uint8_t sha384_expected[] = {0x0c, 0x14, 0x7f, 0x16, 0x80, 0xd3, 0xae, 0x80, 0x0c, 0xdf, 0x17, 0x52,
                                          0x07, 0xc6, 0xdf, 0x4b, 0xdf, 0x1c, 0xac, 0x29, 0x4d, 0x48, 0x21, 0x96,
                                          0x27, 0xa1, 0x96, 0x97, 0x95, 0x73, 0xcc, 0x71, 0xa6, 0xce, 0xd8, 0xf4,
                                          0xc6, 0x3d, 0x75, 0x3d, 0x61, 0x24, 0xf3, 0xe1, 0x8a, 0x19, 0xa1, 0x3e};

static const uint8_t sha512_expected[] = {0xc6, 0x1b, 0xbe, 0xbd, 0x54, 0x11, 0x4d, 0x6c, 0x0d, 0x08, 0x0a, 0xe7, 0x77,
                                          0xee, 0x11, 0x2c, 0x22, 0xc2, 0xbb, 0x2f, 0x4f, 0x6c, 0x68, 0xcb, 0x9d, 0x79,
                                          0xc0, 0xe6, 0xf7, 0x6b, 0x1e, 0x5a, 0x0b, 0xf0, 0xd5, 0xb7, 0xc4, 0x55, 0x5f,
                                          0xf7, 0x72, 0x5b, 0x2b, 0xbc, 0x5e, 0xc6, 0x06, 0x58, 0x07, 0xf2, 0x99, 0x22,
                                          0xf4, 0xf1, 0x0e, 0xae, 0x44, 0xb2, 0x44, 0x94, 0x30, 0xe6, 0xbd, 0x6c};

static const uint8_t mac_key_data[32]  = {0x53, 0x74, 0x72, 0x6F, 0x6E, 0x67, 0x20, 0x70, 0x61, 0x73, 0x73,
                                          0x77, 0x6F, 0x72, 0x64, 0x20, 0x6D, 0x61, 0x6B, 0x65, 0x73, 0x20,
                                          0x79, 0x6F, 0x75, 0x20, 0x73, 0x65, 0x63, 0x75, 0x72, 0x65};

static const uint8_t hmac_expected[] = {0x5f, 0xa7, 0x5c, 0x4e, 0xf5, 0x6a, 0xe8, 0x09, 0x51, 0x0f, 0x66,
                                        0xf7, 0x87, 0xcf, 0xb3, 0x24, 0x4e, 0x2f, 0x37, 0xea, 0x93, 0x2a,
                                        0x11, 0x7c, 0x74, 0x66, 0x08, 0x5f, 0x47, 0xdf, 0xdd, 0x02};

static const uint8_t cmac_expected[] = {0x0f, 0x90, 0x15, 0x9a, 0x2d, 0xd2, 0xc9, 0x12,
                                        0x30, 0x07, 0x14, 0xbf, 0x06, 0xce, 0x76, 0x7c};

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t test_sha_one_go(void)
{
    status_t status       = kStatus_Fail;
    sss_sscp_digest_t ctx = {0};
    uint8_t digest[32]    = {0};
    size_t digest_length  = sizeof(digest);

    do
    {
        PRINTF("**** SHA256 One-Go ****\r\n");

        /* Init the digest context */
        PRINTF("Init digest context...");
        status = sss_sscp_digest_context_init(&ctx, &sssSession, kAlgorithm_SSS_SHA256, kMode_SSS_Digest);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Generate SHA256 digest with one-go operation */
        PRINTF("Generate digest with one-go operation...");
        status = sss_sscp_digest_one_go(&ctx, message, message_length, digest, &digest_length);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Check if digest matches expected value */
        PRINTF("Check if digest matches expected value...");
        if (memcmp(sha256_expected, digest, digest_length))
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* Free the digest context */
        PRINTF("Free digest context...");
        status = sss_sscp_digest_context_free(&ctx);
        if (status != kStatus_SSS_Success)
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

static status_t init_digest_contexts(sss_sscp_digest_t *ctx_sha1,
                                     sss_sscp_digest_t *ctx_sha256,
                                     sss_sscp_digest_t *ctx_sha384,
                                     sss_sscp_digest_t *ctx_sha512)
{
    status_t status = kStatus_Fail;

    status = sss_sscp_digest_context_init(ctx_sha1, &sssSession, kAlgorithm_SSS_SHA1, kMode_SSS_Digest);
    if (status != kStatus_SSS_Success)
    {
        return kStatus_Fail;
    }

    status = sss_sscp_digest_context_init(ctx_sha256, &sssSession, kAlgorithm_SSS_SHA256, kMode_SSS_Digest);
    if (status != kStatus_SSS_Success)
    {
        return kStatus_Fail;
    }

    status = sss_sscp_digest_context_init(ctx_sha384, &sssSession, kAlgorithm_SSS_SHA384, kMode_SSS_Digest);
    if (status != kStatus_SSS_Success)
    {
        return kStatus_Fail;
    }

    status = sss_sscp_digest_context_init(ctx_sha512, &sssSession, kAlgorithm_SSS_SHA512, kMode_SSS_Digest);
    if (status != kStatus_SSS_Success)
    {
        return kStatus_Fail;
    }

    return status;
}

static status_t free_digest_contexts(sss_sscp_digest_t *ctx_sha1,
                                     sss_sscp_digest_t *ctx_sha256,
                                     sss_sscp_digest_t *ctx_sha384,
                                     sss_sscp_digest_t *ctx_sha512)
{
    status_t status = kStatus_Fail;

    status = sss_sscp_digest_context_free(ctx_sha1);
    if (status != kStatus_SSS_Success)
    {
        return kStatus_Fail;
    }

    status = sss_sscp_digest_context_free(ctx_sha256);
    if (status != kStatus_SSS_Success)
    {
        return kStatus_Fail;
    }

    status = sss_sscp_digest_context_free(ctx_sha384);
    if (status != kStatus_SSS_Success)
    {
        return kStatus_Fail;
    }

    status = sss_sscp_digest_context_free(ctx_sha512);
    if (status != kStatus_SSS_Success)
    {
        return kStatus_Fail;
    }

    return status;
}

status_t test_sha_context_switching(void)
{
    status_t status              = kStatus_Fail;
    sss_sscp_digest_t ctx_sha1   = {0};
    sss_sscp_digest_t ctx_sha256 = {0};
    sss_sscp_digest_t ctx_sha384 = {0};
    sss_sscp_digest_t ctx_sha512 = {0};
    uint8_t digest[64]           = {0};
    size_t digest_length         = sizeof(digest);

    // We'll do two updates in this example. Full message is 352B,
    // so this works out neatly with half its length.
    size_t example_hash_update_size = message_length / 2;

    do
    {
        PRINTF("**** SHA1/256/384/512 Multi-Part with Context Switching ****\r\n");

        /* Initialize all four hash contexts */
        PRINTF("Init contexts for all four digests...");
        status = init_digest_contexts(&ctx_sha1, &ctx_sha256, &ctx_sha384, &ctx_sha512);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("\r\n");

        /**** Init ****/

        /* SHA1 digest init */
        PRINTF("Init SHA1...");
        status = sss_sscp_digest_init(&ctx_sha1);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        /* SHA256 digest init */
        PRINTF("Init SHA256...");
        status = sss_sscp_digest_init(&ctx_sha256);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        /* SHA384 digest init */
        PRINTF("Init SHA384...");
        status = sss_sscp_digest_init(&ctx_sha384);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        /* SHA512 digest init */
        PRINTF("Init SHA512...");
        status = sss_sscp_digest_init(&ctx_sha512);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("\r\n");

        /**** Update ****/

        /* SHA1 digest first update */
        PRINTF("Update SHA1   with first %d bytes...", example_hash_update_size);
        status = sss_sscp_digest_update(&ctx_sha1, (uint8_t *)&message[0], example_hash_update_size);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        /* SHA256 digest first update */
        PRINTF("Update SHA256 with first %d bytes...", example_hash_update_size);
        status = sss_sscp_digest_update(&ctx_sha256, (uint8_t *)&message[0], example_hash_update_size);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        /* SHA384 digest first update */
        PRINTF("Update SHA384 with first %d bytes...", example_hash_update_size);
        status = sss_sscp_digest_update(&ctx_sha384, (uint8_t *)&message[0], example_hash_update_size);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        /* SHA512 digest first update */
        PRINTF("Update SHA512 with first %d bytes...", example_hash_update_size);
        status = sss_sscp_digest_update(&ctx_sha512, (uint8_t *)&message[0], example_hash_update_size);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("\r\n");

        /* SHA512 digest second update */
        PRINTF("Update SHA512 with remaining %d bytes...", example_hash_update_size);
        status = sss_sscp_digest_update(&ctx_sha512, (uint8_t *)&message[example_hash_update_size],
                                        example_hash_update_size);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        /* SHA384 digest second update */
        PRINTF("Update SHA384 with remaining %d bytes...", example_hash_update_size);
        status = sss_sscp_digest_update(&ctx_sha384, (uint8_t *)&message[example_hash_update_size],
                                        example_hash_update_size);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        /* SHA256 digest second update */
        PRINTF("Update SHA256 with remaining %d bytes...", example_hash_update_size);
        status = sss_sscp_digest_update(&ctx_sha256, (uint8_t *)&message[example_hash_update_size],
                                        example_hash_update_size);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        /* SHA1 digest second update */
        PRINTF("Update SHA1   with remaining %d bytes...", example_hash_update_size);
        status =
            sss_sscp_digest_update(&ctx_sha1, (uint8_t *)&message[example_hash_update_size], example_hash_update_size);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("\r\n");

        /**** Finish and correctness check ****/

        /* SHA1 digest finish */
        PRINTF("Finish SHA1...");
        status = sss_sscp_digest_finish(&ctx_sha1, digest, &digest_length);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        /* Check SHA1 digest correctness */
        PRINTF("Check SHA1 digest...");
        if (memcmp(sha1_expected, digest, ctx_sha1.digestFullLen))
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* SHA256 digest finish */
        PRINTF("Finish SHA256...");
        status = sss_sscp_digest_finish(&ctx_sha256, digest, &digest_length);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        /* Check SHA256 digest correctness */
        PRINTF("Check SHA256 digest...");
        if (memcmp(sha256_expected, digest, ctx_sha256.digestFullLen))
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* SHA384 digest finish */
        PRINTF("Finish SHA384...");
        status = sss_sscp_digest_finish(&ctx_sha384, digest, &digest_length);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        /* Check SHA384 digest correctness */
        PRINTF("Check SHA384 digest...");
        if (memcmp(sha384_expected, digest, ctx_sha384.digestFullLen))
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* SHA512 digest finish */
        PRINTF("Finish SHA512...");
        status = sss_sscp_digest_finish(&ctx_sha512, digest, &digest_length);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        /* Check SHA512 digest correctness */
        PRINTF("Check SHA512 digest...");
        if (memcmp(sha512_expected, digest, ctx_sha512.digestFullLen))
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("\r\n");

        /* Clean up if all went well */
        PRINTF("Free contexts of all four digests...");
        status = free_digest_contexts(&ctx_sha1, &ctx_sha256, &ctx_sha384, &ctx_sha512);
        if (status != kStatus_SSS_Success)
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

static status_t mac_transparent(sss_sscp_mac_t *ctx_cmac_transparent,
                                sss_sscp_mac_t *ctx_hmac_transparent,
                                sss_sscp_object_t *key_object_cmac,
                                sss_sscp_object_t *key_object_hmac)
{
    sss_status_t status = kStatus_Fail;
    uint8_t hmac[32]    = {0};
    uint8_t cmac[16]    = {0};
    size_t hmac_length  = sizeof(hmac);
    size_t cmac_length  = sizeof(cmac);

    do
    {
        /* Set keys */
        PRINTF("Set transparent CMAC key...");
        status = sss_sscp_key_store_set_key(&keyStore, key_object_cmac, mac_key_data, CMAC_KEY_SIZE, CMAC_KEY_SIZE * 8u,
                                            kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Set transparent HMAC key...");
        status = sss_sscp_key_store_set_key(&keyStore, key_object_hmac, mac_key_data, HMAC_KEY_SIZE, HMAC_KEY_SIZE * 8u,
                                            kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Init MAC contexts */
        PRINTF("Init CMAC context...");
        status = sss_sscp_mac_context_init(ctx_cmac_transparent, &sssSession, key_object_cmac, kAlgorithm_SSS_CMAC_AES,
                                           kMode_SSS_Mac);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init HMAC context...");
        status = sss_sscp_mac_context_init(ctx_hmac_transparent, &sssSession, key_object_hmac,
                                           kAlgorithm_SSS_HMAC_SHA256, kMode_SSS_Mac);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Generate MACs */
        PRINTF("Generate CMAC keyed digest...");
        status = sss_sscp_mac_one_go(ctx_cmac_transparent, message, message_length, cmac, &cmac_length);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Generate HMAC keyed digest...");
        status = sss_sscp_mac_one_go(ctx_hmac_transparent, message, message_length, hmac, &hmac_length);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Check if CMAC correct */
        PRINTF("Check if CMAC correct...");
        if (memcmp(cmac_expected, cmac, cmac_length))
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Check if HMAC correct...");
        if (memcmp(hmac_expected, hmac, hmac_length))
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        status = kStatus_Success;
    } while (0);

    return status;
}

static status_t mac_opaque(sss_sscp_mac_t *ctx_cmac_opaque,
                           sss_sscp_mac_t *ctx_hmac_opaque,
                           sss_sscp_object_t *key_object_cmac,
                           sss_sscp_object_t *key_object_hmac)
{
    sss_status_t status = kStatus_Fail;
    uint8_t hmac[32]    = {0};
    uint8_t cmac[16]    = {0};
    size_t cmac_length  = sizeof(cmac);
    size_t hmac_length  = sizeof(hmac);

    do
    {
        /* Generate keys */
        PRINTF("Generate opaque CMAC key...");
        status = sss_sscp_key_store_generate_key(&keyStore, key_object_cmac, CMAC_KEY_SIZE * 8u, NULL);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Generate opaque HMAC key...");
        status = sss_sscp_key_store_generate_key(&keyStore, key_object_hmac, HMAC_KEY_SIZE * 8u, NULL);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Init MAC contexts */
        PRINTF("Init CMAC context...");
        status = sss_sscp_mac_context_init(ctx_cmac_opaque, &sssSession, key_object_cmac, kAlgorithm_SSS_CMAC_AES,
                                           kMode_SSS_Mac);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init HMAC context...");
        status = sss_sscp_mac_context_init(ctx_hmac_opaque, &sssSession, key_object_hmac, kAlgorithm_SSS_HMAC_SHA256,
                                           kMode_SSS_Mac);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Generate MACs */
        PRINTF("Generate CMAC keyed digest...");
        status = sss_sscp_mac_one_go(ctx_cmac_opaque, message, message_length, cmac, &cmac_length);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Generate HMAC keyed digest...");
        status = sss_sscp_mac_one_go(ctx_hmac_opaque, message, message_length, hmac, &hmac_length);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        status = kStatus_Success;
    } while (0);

    return status;
}

status_t test_mac_one_go_many_contexts(void)
{
    sss_status_t status                 = kStatus_Fail;
    sss_sscp_mac_t ctx_cmac_opaque      = {0};
    sss_sscp_mac_t ctx_hmac_opaque      = {0};
    sss_sscp_mac_t ctx_cmac_transparent = {0};
    sss_sscp_mac_t ctx_hmac_transparent = {0};
    sss_sscp_rng_t ctx_rng              = {0};
    sss_sscp_object_t key_object_cmac   = {0};
    sss_sscp_object_t key_object_hmac   = {0};

    do
    {
        PRINTF("**** Opaque and transparent One-Go CMAC / HMAC  ****\r\n");

        /* Init key objects */
        PRINTF("Init CMAC key object...");
        status = sss_sscp_key_object_init(&key_object_cmac, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init HMAC key object...");
        status = sss_sscp_key_object_init(&key_object_hmac, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Allocate key handles */
        PRINTF("Allocate CMAC key handle...");
        status = sss_sscp_key_object_allocate_handle(&key_object_cmac, KEY_ID, kSSS_KeyPart_Default,
                                                     kSSS_CipherType_CMAC, CMAC_KEY_SIZE, kSSS_KeyProp_CryptoAlgo_MAC);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate HMAC key handle...");
        status =
            sss_sscp_key_object_allocate_handle(&key_object_hmac, KEY_ID, kSSS_KeyPart_Default, kSSS_CipherType_HMAC,
                                                HMAC_KEY_SIZE * 2, kSSS_KeyProp_CryptoAlgo_MAC);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Initialize RNG before key generation */
        PRINTF("Initialize RNG for opaque key generation...");

        // We want RNG number quality to be nonzero, but high quality (e.g. 1)
        sss_sscp_rng_context_init(&sssSession, &ctx_rng, 1u);

        // We don't actually need any random number to be returned,
        // we only need RNG to be initialized before calling key generation
        status = sss_sscp_rng_get_random(&ctx_rng, NULL, 0u);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("\r\n");

        /* Show transparent MAC */
        status = mac_transparent(&ctx_cmac_transparent, &ctx_hmac_transparent, &key_object_cmac, &key_object_hmac);
        if (status != kStatus_Success)
        {
            break;
        }
        PRINTF("\r\n");

        /* Show opaque MAC */
        status = mac_opaque(&ctx_cmac_opaque, &ctx_hmac_opaque, &key_object_cmac, &key_object_hmac);
        if (status != kStatus_Success)
        {
            break;
        }
        PRINTF("\r\n");

        PRINTF("Free contexts and key objects...");
        if (sss_sscp_rng_free(&ctx_rng) != kStatus_SSS_Success ||
            sss_sscp_mac_context_free(&ctx_cmac_opaque) != kStatus_SSS_Success ||
            sss_sscp_mac_context_free(&ctx_cmac_transparent) != kStatus_SSS_Success ||
            sss_sscp_mac_context_free(&ctx_hmac_opaque) != kStatus_SSS_Success ||
            sss_sscp_mac_context_free(&ctx_hmac_transparent) != kStatus_SSS_Success ||
            sss_sscp_key_object_free(&key_object_cmac, 0u) != kStatus_SSS_Success ||
            sss_sscp_key_object_free(&key_object_hmac, 1u) != kStatus_SSS_Success)
        {
            status = kStatus_Fail;
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

/*!
 * @brief Main function
 */
int main(void)
{
    char ch;
    status_t status = kStatus_Fail;

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("ELE Digest via SSSAPI Example\r\n\r\n");

    /*
     * This code example demonstrates EdgeLock usage of one-go hashing and MAC and multi-part hashing operations via
     * SSSAPI. The example is performed in following steps:
     * 1.  Open an EdgeLock session
     * 2.  Open a key store
     * 3.  Initialize a SHA256 digest context
     * 4.  Hash a message with SHA256 via a one-go operation and check digest correctness
     * 5.  Free the SHA256 context
     * 6.  Initialize four separate hash contexts (SHA1, SHA256, SHA384, SHA512)
     * 7.  Initialize the multi-part hashing operations for all four hashes
     * 8.  Update the four digests with the first half of the message
     * 9.  Update the four digests with the second half of the message
     * 10. Finish the multi-part hash operations and check digest correctness
     * 11. Free the four separate hash contexts
     * 12. Prepare opaque and transparent MAC key objects and initialize RNG
     * 13. Set transparent keys for two of the MACs and initialize two MAC contexts
     * 14. Generate and check MACs
     * 13. Generate opaque keys for two MACs and initialize two additional MAC contexts
     * 14. Generate MACs
     * 15. Close all four MAC contexts, free key objects and close the RNG context
     * 14. Close the key store
     * 15. Close the EdgeLock session
     * Note: This example does not close already opened contexts or objects in case of failed command.
     */

    do
    {
        status = ELEMU_mu_wait_for_ready(ELEMUA, ELE_MAX_SUBSYSTEM_WAIT);
        if (status != kStatus_Success)
        {
            break;
        }

        /****************** Start ***********************/
        status = sscp_mu_init(&sscpContext, (ELEMU_Type *)(uintptr_t)ELEMUA);
        if (status != kStatus_SSCP_Success)
        {
            break;
        }
        /* open session to specific security subsystem */
        status = sss_sscp_open_session(&sssSession, 0u, ELE_SUBSYSTEM, &sscpContext);
        if (status != kStatus_SSS_Success)
        {
            return status;
        }

        status = sss_sscp_key_store_init(&keyStore, &sssSession);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        /* Showcase One-Go digest */
        status = test_sha_one_go();
        if (status != kStatus_Success)
        {
            break;
        }

        /* Showcase Multi-Part digest with context switching */
        status = test_sha_context_switching();
        if (status != kStatus_Success)
        {
            break;
        }

        status = test_mac_one_go_many_contexts();
        if (status != kStatus_Success)
        {
            break;
        }

    } while (0);

    if (status == kStatus_Success)
    {
        PRINTF("End of Example with SUCCESS!!\r\n\r\n");
    }
    else
    {
        PRINTF("ERROR: execution of commands on Security Sub-System failed!\r\n\r\n");
    }

    /* Close keystore*/
    status = sss_sscp_key_store_free(&keyStore);
    /* Close session */
    status = sss_sscp_close_session(&sssSession);

    PRINTF("Example end\r\n");

    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
