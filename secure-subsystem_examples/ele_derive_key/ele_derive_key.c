/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "fsl_sss_mgmt.h"
#include "fsl_sss_sscp.h"
#include "fsl_sscp_mu.h"

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
/* Variables used by SSSAPI */
static sscp_context_t sscpContext    = {0u};
static sss_sscp_session_t sssSession = {0u};
static sss_sscp_key_store_t keyStore = {0u};

/* Test Vector 1 from RFC5869 for HKDF */
static uint8_t const salt_data[]         = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c };
static size_t salt_data_len              = sizeof(salt_data);
static uint8_t const info[]              = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9 };
static size_t info_len                   = sizeof(info);
static uint8_t const ikm_data[]          = { 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
                                             0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b };
static size_t ikm_data_len               = sizeof(ikm_data);
static uint8_t const okm_data_expected[] = { 0x3c, 0xb2, 0x5f, 0x25, 0xfa, 0xac, 0xd5, 0x7a, 0x90, 0x43, 0x4f,
                                             0x64, 0xd0, 0x36, 0x2f, 0x2a, 0x2d, 0x2d, 0x0a, 0x90, 0xcf, 0x1a,
                                             0x5a, 0x4c, 0x5d, 0xb0, 0x2d, 0x56, 0xec, 0xc4, 0xc5, 0xbf, 0x34,
                                             0x00, 0x72, 0x08, 0xd5, 0xb8, 0x87, 0x18, 0x58, 0x65 };
static size_t okm_data_expected_len      = sizeof(okm_data_expected);

/* Test key data for ECDH */
static uint8_t const alice_keypair[] = {
    // Public part X
    0xe6, 0x4f, 0xeb, 0xe9, 0x07, 0xfc, 0x1b, 0x35, 0xaf, 0x8e, 0x52, 0x26,
    0x5f, 0x78, 0x0e, 0xf6, 0x66, 0xfa, 0xc9, 0x13, 0x93, 0xc1, 0x8a, 0x17,
    0x4f, 0x86, 0x44, 0x49, 0xac, 0x83, 0x6e, 0x73,
    // Public part Y
    0xaf, 0xd6, 0xa5, 0x47, 0x30, 0x07, 0x8b, 0xaf, 0x47, 0x34, 0x93, 0xed,
    0x12, 0xc2, 0x65, 0x80, 0xe4, 0x33, 0xa0, 0xe2, 0x25, 0x7e, 0xa4, 0x41,
    0x5a, 0xe8, 0xbb, 0x46, 0x9f, 0xc5, 0xb1, 0xde,
    // Private part
    0x24, 0xd4, 0x5f, 0x62, 0x6e, 0xf9, 0xcd, 0x55, 0xff, 0xab, 0xb9, 0xd4,
    0x81, 0xa0, 0x16, 0xd6, 0x35, 0x01, 0xca, 0xa3, 0x35, 0xb2, 0xf4, 0x55,
    0x55, 0x8a, 0xa7, 0xe7, 0x5f, 0xca, 0x1c, 0xf8
};
static size_t alice_keypair_length = sizeof(alice_keypair);

static uint8_t const bob_pubkey[] = {
    // Public part X
    0x38, 0xa4, 0xd7, 0xae, 0xa9, 0x37, 0xb3, 0x9f, 0x81, 0x02, 0xa6, 0x76,
    0x99, 0x02, 0x19, 0xe0, 0x1d, 0xf6, 0x85, 0x3d, 0x99, 0x4a, 0xc7, 0xa2,
    0xd9, 0xcf, 0x42, 0xbd, 0x84, 0x10, 0x6b, 0x6d,
    // Public part Y
    0x73, 0x5e, 0x59, 0x15, 0x32, 0x94, 0xc6, 0x61, 0x73, 0xfa, 0xda, 0x75,
    0x2b, 0x8f, 0x96, 0x89, 0x00, 0xcc, 0xbb, 0x42, 0xa1, 0x0e, 0xad, 0x5a,
    0xeb, 0x3c, 0xf4, 0x37, 0x0c, 0xf8, 0x80, 0xb8
};
static size_t bob_pubkey_length = sizeof(bob_pubkey);

static uint8_t const shared_secret_expected[] = {
    0xd8, 0x7e, 0x2d, 0x92, 0xc2, 0x9f, 0xc3, 0x3e, 0x99, 0x9e, 0x0e, 0xc5,
    0xed, 0xa6, 0xdd, 0x78, 0xda, 0x27, 0x60, 0xd0, 0x09, 0xe9, 0x21, 0xa7,
    0xfd, 0xc7, 0x95, 0x15, 0xf2, 0x14, 0x18, 0xb5
};
static size_t shared_secret_expected_length = sizeof(shared_secret_expected);


/*******************************************************************************
 * Code
 ******************************************************************************/

static status_t ecdh()
{
    status_t status                          = kStatus_Fail;
    uint8_t out_buf[32]                      = {0u};
    size_t out_buf_len                       = sizeof(out_buf);
    size_t out_buf_bitlen                    = sizeof(out_buf) * 8;
    sss_sscp_rng_t context_rng               = {0u};
    sss_sscp_object_t alice_key_object       = {0u};
    sss_sscp_object_t bob_key_object         = {0u};
    sss_sscp_object_t shared_key_key_object  = {0u};
    sss_sscp_derive_key_t derive_key_context = {0u};

    do
    {
        PRINTF("**** Elliptic Curve Diffie-Hellman ****\r\n");

        /* Init key object (we are Alice) */
        PRINTF("Init Alice key object...");
        status = sss_sscp_key_object_init(&alice_key_object, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init Bob key object...");
        status = sss_sscp_key_object_init(&bob_key_object, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init shared key object...");
        status = sss_sscp_key_object_init(&shared_key_key_object, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Allocate handle */
        PRINTF("Allocate Alice key object handle...");
        status = sss_sscp_key_object_allocate_handle(&alice_key_object, 0u, kSSS_KeyPart_Pair, kSSS_CipherType_EC_NIST_P,
                                                     197u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate Bob key object handle...");
        status = sss_sscp_key_object_allocate_handle(&bob_key_object, 0u, kSSS_KeyPart_Public, kSSS_CipherType_EC_NIST_P,
                                                     197u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate shared key object handle...");
        status = sss_sscp_key_object_allocate_handle(&shared_key_key_object, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC,
                                                     197u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Initialize RNG before key generation */
        PRINTF("Initialize RNG before key generation...");

        // We want RNG number quality to be nonzero, but high quality (e.g. 1)
        sss_sscp_rng_context_init(&sssSession, &context_rng, 1u);

        // We don't actually need any random number to be returned,
        // we only need RNG to be initialized before calling key generation
        status = sss_sscp_rng_get_random(&context_rng, NULL, 0u);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Import P-256 key pair */
        PRINTF("Import Alice key pair...");
        status = sss_sscp_key_store_set_key(&keyStore, &alice_key_object, alice_keypair, alice_keypair_length, 256u, kSSS_KeyPart_Pair);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Import Bob public key...");
        status = sss_sscp_key_store_set_key(&keyStore, &bob_key_object, bob_pubkey, bob_pubkey_length, 256u, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Derive shared secret */
        PRINTF("Init Derive Key Context...");
        status = sss_sscp_derive_key_context_init(&derive_key_context, &sssSession, &alice_key_object, kAlgorithm_SSS_ECDH, kMode_SSS_ComputeSharedSecret);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("ECDH key derivation...");
        status = sss_sscp_asymmetric_dh_derive_key(&derive_key_context, &bob_key_object, &shared_key_key_object);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Check if value matches */
        PRINTF("Get derived shared secret...");
        status = sss_sscp_key_store_get_key(&keyStore, &shared_key_key_object, out_buf, &out_buf_len, &out_buf_bitlen, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Derived shared secret matches expected shared secret...");
        if (memcmp(out_buf, shared_secret_expected, shared_secret_expected_length) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* Cleanup */
        PRINTF("Cleanup...");
        if ((sss_sscp_key_object_free(&alice_key_object, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&bob_key_object, 0u) != kStatus_SSS_Success)  ||
            (sss_sscp_key_object_free(&shared_key_key_object, 0u) != kStatus_SSS_Success)  ||
            (sss_sscp_derive_key_context_free(&derive_key_context) != kStatus_SSS_Success))
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

/* KDF example showing SHA256 HKDF for chip revision A2.1 */
static status_t kdf_a2_1()
{
    status_t status                          = kStatus_Fail;
    sss_sscp_derive_key_t derive_key_context = {0};
    uint8_t out_buf[42]                      = {0};
    size_t out_buf_len                       = 0u;
    size_t out_buf_len_bits                  = 0u;
    sss_sscp_object_t ikm, prk, okm;

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
        status = sss_sscp_key_store_set_key(&keyStore, &ikm, ikm_data, ikm_data_len, ikm_data_len * 8u, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* HDKF Extract step to generate the intermediate PRK value */
        PRINTF("Initialize HKDF SHA256 extract step context...");
        status = sss_sscp_derive_key_context_init(&derive_key_context, &sssSession, &ikm, kAlgorithm_SSS_HKDF_SHA256_EXTRACT, kMode_SSS_SymmetricKDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("HKDF SHA256 extract...");
        status = sss_sscp_derive_key(&derive_key_context, salt_data, salt_data_len, &prk, 256u);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Free HKDF SHA256 extract step context...");
        status = sss_sscp_derive_key_context_free(&derive_key_context);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* HDKF Expand step to generate the output OKM */
        PRINTF("Initialize HKDF SHA256 expand step context...");
        status = sss_sscp_derive_key_context_init(&derive_key_context, &sssSession, &prk, kAlgorithm_SSS_HKDF_SHA256_EXPAND, kMode_SSS_SymmetricKDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("HKDF SHA256 expand...");
        status = sss_sscp_derive_key(&derive_key_context, info, info_len, &okm, 336u);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Check if OKM is correct */
        PRINTF("Get OKM...");
        memset(out_buf, 0, sizeof(out_buf));
        out_buf_len = sizeof(out_buf);
        status = sss_sscp_key_store_get_key(&keyStore, &okm, out_buf, &out_buf_len, &out_buf_len_bits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Derived OKM matches expected OKM...");
        if (memcmp(out_buf, okm_data_expected, okm_data_expected_len) != 0)
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
            (sss_sscp_derive_key_context_free(&derive_key_context) != kStatus_SSS_Success))
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

/* KDF example showing SHA256 HKDF for chip revisions A0 to A2 */
static status_t kdf()
{
    status_t status                          = kStatus_Fail;
    sss_sscp_derive_key_t derive_key_context = {0};
    uint8_t out_buf[42]                      = {0};
    size_t out_buf_len                       = 0u;
    size_t out_buf_len_bits                  = 0u;
    sss_sscp_object_t salt, prk, okm;

    do
    {
        PRINTF("**** HDKF key derivation ****\r\n");

        /* Key object inits */
        PRINTF("Init salt key object...");
        status = sss_sscp_key_object_init(&salt, &keyStore);
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
        PRINTF("Allocate salt key handle...");
        status = sss_sscp_key_object_allocate_handle(&salt, 0u, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 32u, kSSS_KeyProp_CryptoAlgo_KDF);
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

        /**
         * NOTE: If one does not wish to use salt in the HKDF, then this
         *       salt setting step should be skipped.
         */
        PRINTF("Set salt...");
        status = sss_sscp_key_store_set_key(&keyStore, &salt, salt_data, salt_data_len, salt_data_len * 8u, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* HDKF Extract step to generate the intermediate PRK value */
        PRINTF("Initialize HKDF SHA256 extract step context...");
        status = sss_sscp_derive_key_context_init(&derive_key_context, &sssSession, &salt, kAlgorithm_SSS_HKDF_SHA256_EXTRACT, kMode_SSS_SymmetricKDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("HKDF SHA256 extract...");
        status = sss_sscp_derive_key(&derive_key_context, ikm_data, ikm_data_len, &prk, 256u);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Free HKDF SHA256 extract step context...");
        status = sss_sscp_derive_key_context_free(&derive_key_context);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* HDKF Expand step to generate the output OKM */
        PRINTF("Initialize HKDF SHA256 expand step context...");
        status = sss_sscp_derive_key_context_init(&derive_key_context, &sssSession, &prk, kAlgorithm_SSS_HKDF_SHA256_EXPAND, kMode_SSS_SymmetricKDF);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("HKDF SHA256 expand...");
        status = sss_sscp_derive_key(&derive_key_context, info, info_len, &okm, 336u);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Check if OKM is correct */
        PRINTF("Get OKM...");
        memset(out_buf, 0, sizeof(out_buf));
        out_buf_len = sizeof(out_buf);
        status = sss_sscp_key_store_get_key(&keyStore, &okm, out_buf, &out_buf_len, &out_buf_len_bits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Derived OKM matches expected OKM...");
        if (memcmp(out_buf, okm_data_expected, okm_data_expected_len) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* Cleanup */
        PRINTF("Cleanup...");
        if ((sss_sscp_key_object_free(&salt, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&prk, 0u) != kStatus_SSS_Success)  ||
            (sss_sscp_key_object_free(&okm, 0u) != kStatus_SSS_Success)  ||
            (sss_sscp_derive_key_context_free(&derive_key_context) != kStatus_SSS_Success))
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


/*!
 * @brief Main function
 */
int main(void)
{
    char ch;
    status_t status = kStatus_Fail;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("ELE Derive Key via SSSAPI Example\r\n\r\n");

    /*
     * This code example demonstrates EdgeLock usage ECDH and HKDF key derivation via SSSAPI.
     * The example is performed in following steps:
     * 1.  Open an EdgeLock session
     * 2.  Open a key store
     * 3.  Initialize key objects for ECDH
     * 4.  Initialize derive key context
     * 5.  Load elliptic curve keys
     * 6.  Utilize ECDH key derivation
     * 7.  Check if shared secret matches expected value
     * 8.  Clean up key objects and contexts
     * 9.  Initialize key objects for HKDF
     * 10. Initialize derive key context
     * 11. Load salt (or IKM, depending on chip revision; see WARNING comment below)
     * 12. Utilize HKDF key derivation
     * 13. Check if output keying material matches expected value
     * 14. Clean up key objects and contexts
     * 15. Close keystore and EdgeLock session
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

        status = ecdh();
        if (status != kStatus_Success)
        {
            break;
        }

        /* WARNING:
         *       Please note that HKDF flow depends on the revision of your
         *       chip. Expected inputs for various steps of HKDF have changed
         *       with chip revision A2.1. Below is a brief overview of the two
         *       variants.
         *
         *       The change relates only to the HKDF Extract step.
         *
         * For revisions A0 through A2:
         *       When initializing a key derivation context for any variant of
         *       kAlgorithm_SSS_HKDF_*_EXTRACT via sss_sscp_derive_key_context_init(),
         *       salt is to be passed as a key object.
         *       Next, when calling sss_sscp_derive_key() for the Extract step,
         *       the Input Keying Material (IKM) is passed as an input buffer.
         *
         * For revision A2.1:
         *       The order and approach to passing salt and IKM are swapped
         *       compared to previous revisions.
         *       When initializing a key derivation context for any variant of
         *       kAlgorithm_SSS_HKDF_*_EXTRACT via sss_sscp_derive_key_context_init(),
         *       IKM is to be passed as a key object.
         *       Next, when calling sss_sscp_derive_key() for the Extract step,
         *       the salt is passed as an input buffer.
         *
         *       This approach allows for keeping the IKM opaque (e.g. taking
         *       IKM from the result of an ECDH or SPAKE2+ operation).
         */
        status = IS_CHIP_REVISION_A2_1()
                    ? kdf_a2_1()
                    : kdf();
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
    } while (0);

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
