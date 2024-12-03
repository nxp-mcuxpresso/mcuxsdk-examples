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
#define CORE_CLK_FREQ          CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define ELE_MAX_SUBSYSTEM_WAIT (0xFFFFFFFFu)
#define ELE_SUBSYSTEM          (kType_SSS_Ele200)
#define KEY_ID                 (0u)
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

/* Message to be signed ("Sign me!") */
static uint8_t message[]        = {0x53u, 0x69u, 0x67u, 0x6eu, 0x20u, 0x6du, 0x65u, 0x21u};
static uint8_t message_sha512[] = {
    0x40u, 0x9bu, 0x07u, 0x0du, 0x1du, 0x63u, 0xa6u, 0x05u, 0x3au, 0xf2u, 0xd7u, 0x94u, 0x38u, 0xa2u, 0x54u, 0xa3u,
    0x10u, 0xb9u, 0x82u, 0x5fu, 0xc8u, 0x35u, 0x43u, 0x45u, 0xf9u, 0x05u, 0x6bu, 0xa4u, 0x19u, 0x33u, 0x20u, 0x7bu,
    0x87u, 0xc1u, 0x9fu, 0x99u, 0x68u, 0x0du, 0x84u, 0xfeu, 0xe0u, 0x42u, 0xf5u, 0x60u, 0xb5u, 0xefu, 0x2au, 0x9eu,
    0x3bu, 0xb6u, 0xe0u, 0xc3u, 0xa1u, 0x57u, 0xfcu, 0x19u, 0x4fu, 0xdbu, 0xa0u, 0xc1u, 0x7fu, 0xbau, 0x5bu, 0x69u};

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t test_ecdsa(void)
{
    status_t status                      = kStatus_Fail;
    uint8_t signature[132]               = {0u};
    size_t signature_len                 = sizeof(signature);
    sss_sscp_asymmetric_t context_sign   = {0u};
    sss_sscp_asymmetric_t context_verify = {0u};
    sss_sscp_rng_t context_rng           = {0u};
    sss_sscp_object_t key_object         = {0u};

    do
    {
        PRINTF("**** ECDSA with P-521 [opaque key] ****\r\n");

        /* Init key object */
        PRINTF("Init key object...");
        status = sss_sscp_key_object_init(&key_object, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Allocate handle */
        PRINTF("Allocate key object handle...");
        // For NIST P-521 keypair at most 197 Bytes are needed
        status = sss_sscp_key_object_allocate_handle(&key_object, 0u, kSSS_KeyPart_Pair, kSSS_CipherType_EC_NIST_P,
                                                     197u, kSSS_KeyProp_CryptoAlgo_AsymSignVerify);
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

        /* Generate P-521 opaque key pair */
        PRINTF("Generate P-521 opaque key pair...");
        status = sss_sscp_key_store_generate_key(&keyStore, &key_object, 521u, NULL);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Initialize asymmetric context for signing */
        PRINTF("Init asymmetric context for signing...");
        status = sss_sscp_asymmetric_context_init(&context_sign, &sssSession, &key_object, kAlgorithm_SSS_ECDSA_SHA512,
                                                  kMode_SSS_Sign);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Sign message digest */
        PRINTF("Sign message digest...");
        status = sss_sscp_asymmetric_sign_digest(&context_sign, message_sha512, sizeof(message_sha512), signature,
                                                 &signature_len);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Initialize asymmetric context for verification */
        PRINTF("Init asymmetric context for verification...");
        status = sss_sscp_asymmetric_context_init(&context_verify, &sssSession, &key_object,
                                                  kAlgorithm_SSS_ECDSA_SHA512, kMode_SSS_Verify);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Verify signature */
        PRINTF("Verify signature...");
        status = sss_sscp_asymmetric_verify_digest(&context_verify, message_sha512, sizeof(message_sha512), signature,
                                                   signature_len);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Clean up if all went well */
        PRINTF("Clean up...");
        if (sss_sscp_asymmetric_context_free(&context_verify) != kStatus_SSS_Success ||
            sss_sscp_asymmetric_context_free(&context_sign) != kStatus_SSS_Success ||
            sss_sscp_key_object_free(&key_object, 1u) != kStatus_SSS_Success)
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

status_t test_eddsa(void)
{
    status_t status                      = kStatus_Fail;
    uint8_t signature[64]                = {0u};
    size_t signature_len                 = sizeof(signature);
    sss_sscp_asymmetric_t context_sign   = {0u};
    sss_sscp_asymmetric_t context_verify = {0u};
    sss_sscp_object_t key_object         = {0u};
    const uint8_t key_pair[]             = {
        0xd7u, 0x5au, 0x98u, 0x01u, 0x82u, 0xb1u, 0x0au, 0xb7u, 0xd5u, 0x4bu, 0xfeu, 0xd3u, 0xc9u, 0x64u, 0x07u, 0x3au,
        0x0eu, 0xe1u, 0x72u, 0xf3u, 0xdau, 0xa6u, 0x23u, 0x25u, 0xafu, 0x02u, 0x1au, 0x68u, 0xf7u, 0x07u, 0x51u, 0x1au,
        0x9du, 0x61u, 0xb1u, 0x9du, 0xefu, 0xfdu, 0x5au, 0x60u, 0xbau, 0x84u, 0x4au, 0xf4u, 0x92u, 0xecu, 0x2cu, 0xc4u,
        0x44u, 0x49u, 0xc5u, 0x69u, 0x7bu, 0x32u, 0x69u, 0x19u, 0x70u, 0x3bu, 0xacu, 0x03u, 0x1cu, 0xaeu, 0x7fu, 0x60u};

    do
    {
        PRINTF("**** EdDSA with Curve25519 [transparent key] ****\r\n");

        /* Init key object */
        PRINTF("Init key object...");
        status = sss_sscp_key_object_init(&key_object, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Allocate handle */
        /* NOTE: For generating an EdDSA signature with a transparent key,
         *       a full *key pair* (kSSS_KeyPart_Pair) must be loaded
         *       to the keystore. The public part of the key pair may be left
         *       uninitialized in case no verification is needed.
         */
        PRINTF("Allocate key object handle...");
        status = sss_sscp_key_object_allocate_handle(&key_object, 0u, kSSS_KeyPart_Pair, kSSS_CipherType_EC_TWISTED_ED,
                                                     64u, kSSS_KeyProp_CryptoAlgo_AsymSignVerify);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Set key pair */
        PRINTF("Set Curve25519 key pair...");
        status = sss_sscp_key_store_set_key(&keyStore, &key_object, key_pair, 64u, 256u, kSSS_KeyPart_Pair);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Initialize asymmetric context for signing */
        PRINTF("Init asymmetric context for signing...");
        status = sss_sscp_asymmetric_context_init(&context_sign, &sssSession, &key_object, kAlgorithm_SSS_EdDSA_Ed25519,
                                                  kMode_SSS_Sign);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Sign message */
        PRINTF("Sign message...");
        status = sss_sscp_asymmetric_sign_digest(&context_sign, message, sizeof(message), signature, &signature_len);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Initialize asymmetric context for verification */
        PRINTF("Init asymmetric context for verification...");
        status = sss_sscp_asymmetric_context_init(&context_verify, &sssSession, &key_object,
                                                  kAlgorithm_SSS_EdDSA_Ed25519, kMode_SSS_Verify);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Verify signature */
        PRINTF("Verify signature...");
        status = sss_sscp_asymmetric_verify_digest(&context_verify, message, sizeof(message), signature, signature_len);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Clean up if all went well */
        PRINTF("Clean up...");
        if (sss_sscp_asymmetric_context_free(&context_sign) != kStatus_SSS_Success ||
            sss_sscp_asymmetric_context_free(&context_verify) != kStatus_SSS_Success ||
            sss_sscp_key_object_free(&key_object, 1u) != kStatus_SSS_Success)
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

    PRINTF("ELE Asymmetric via SSSAPI Example\r\n\r\n");

    /*
     * This code example demonstrates EdgeLock usage for ECDSA P-521 and EdDSA Curve25519 operation via SSSAPI.
     * The example is performed in following steps:
     * 1.  Open an EdgeLock session
     * 2.  Create a key store
     * 3.  Create and allocate key object for the P-521 curve key pair
     * 4.  Initialize RNG and generate the opaque key pair
     * 5.  Initialize an asymmetric ECDSA signing context and sign a message digest
     * 6.  Initialize an asymmetric ECDSA verification context and verify the signature
     * 7.  Clean up after ECDSA operations
     * 8.  Create and allocate key object for the Curve25519 key pair
     * 9.  Set the transparent key pair
     * 10. Initialize an asymmetric EdDSA signing context and sign a message digest
     * 11. Initialize an asymmetric EdDSA verification context and verify the signature
     * 12. Clean up after EdDSA operations
     * 13. Free the key store
     * 14. Close the EdgeLock session
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

        status = test_ecdsa();
        if (status != kStatus_Success)
        {
            break;
        }

        status = test_eddsa();
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
