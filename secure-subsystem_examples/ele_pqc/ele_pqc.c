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

static sscp_context_t sscpContext    = {0};
static sss_sscp_session_t sssSession = {0};
static sss_sscp_key_store_t keyStore = {0};

/*! Dummy message for ML-DSA */
static uint8_t message[] = {
    0x2Du, 0x84u, 0x1Du, 0xB9u, 0xC0u, 0x1Au, 0x4Fu, 0x93u,
    0x72u, 0x50u, 0x34u, 0x61u, 0xC5u, 0x59u, 0x4Du, 0x2Cu,
    0x8Au, 0x70u, 0x5Eu, 0xD5u, 0x3Cu, 0x3Au, 0xC1u, 0x38u,
    0x1Bu, 0x4Cu, 0x2Eu, 0xA5u, 0x2Fu, 0x55u, 0x2Bu, 0x26u,
    0xF4u, 0x74u, 0xD4u, 0x24u, 0xE3u, 0x35u, 0xC1u, 0x16u,
    0x0Cu, 0x77u, 0xA8u, 0xF6u, 0x4Du, 0x02u, 0xCEu, 0x15u,
    0xE0u, 0xACu, 0x3Du, 0xC1u, 0x14u, 0x1Eu, 0x4Du, 0x01u,
    0xA8u, 0x01u, 0xEEu, 0x82u, 0x30u, 0x5Bu, 0x0Bu, 0x1Au,
    0x46u, 0x81u, 0x63u, 0x93u, 0x6Eu, 0x2Eu, 0xD9u, 0xC7u,
    0x32u, 0x73u, 0xF4u, 0x8Au, 0x3Bu, 0x6Au, 0x3Au, 0x09u,
    0xCEu, 0x5Fu, 0x00u, 0x3Au, 0x70u, 0x72u, 0xA8u, 0x57u,
    0x07u, 0x42u, 0x52u, 0xBCu, 0xFCu, 0xF3u, 0x89u, 0x01u,
    0x02u, 0xE4u, 0x60u, 0x87u, 0x0Cu, 0x9Cu, 0x75u, 0xE1u,
    0xE7u, 0xADu, 0x34u, 0xBDu, 0x76u, 0x55u, 0x44u
};

/*! SHA3_256 digest of the dummy message for ML-DSA */
static uint8_t messageHashSHA3_256[] = {
    0x0Bu, 0xA5u, 0x99u, 0xA6u, 0x83u, 0x8Au, 0x0Eu, 0xC6u,
    0x41u, 0xDCu, 0x9Du, 0x46u, 0x32u, 0x2Fu, 0xFCu, 0xCBu,
    0xEDu, 0xFFu, 0xE2u, 0x50u, 0xC2u, 0x01u, 0xCBu, 0xFBu,
    0x58u, 0x47u, 0x80u, 0xBCu, 0x61u, 0x5Bu, 0x2Du, 0x58u
};

/*! Dummy context for ML-DSA */
static uint8_t mldsaContext[] = {
    0xDFu, 0xD4u, 0x89u, 0xE3u, 0xF5u, 0x7Fu, 0xBDu, 0x62u,
    0x70u, 0xB6u, 0xA1u, 0x49u, 0xE9u, 0x70u, 0xBDu, 0x32u,
    0x2Du, 0x44u, 0x5Cu, 0xFDu, 0x44u, 0x85u, 0xB6u, 0xF2u,
    0xF9u, 0x59u, 0x45u, 0x17u, 0xA7u, 0x6Du, 0xC8u, 0x35u,
    0xBDu, 0x42u, 0xFCu, 0xF3u, 0xDBu, 0xC3u, 0xF3u, 0x90u,
    0x4Fu, 0x90u, 0xC2u, 0xCDu, 0x3Fu, 0xDAu, 0x6Du, 0x50u,
    0x31u, 0xB2u, 0x85u, 0x48u, 0x9Au, 0xE0u, 0xA3u, 0xDBu,
    0xC7u, 0x65u, 0xA7u, 0xE8u, 0x0Fu, 0x0Du, 0xF3u, 0x32u,
    0x83u, 0xE0u, 0x23u, 0x57u, 0xC1u, 0xEEu, 0xE5u, 0xB2u,
    0x8Cu, 0xF6u, 0xDDu, 0xE7u, 0x61u, 0x7Eu, 0x57u, 0xA0u,
    0x42u, 0x54u, 0x24u, 0x9Au, 0xA0u, 0x4Cu, 0x16u, 0xE1u,
    0x9Au, 0xBEu, 0xE2u, 0xC0u, 0x76u, 0x7Fu, 0x86u, 0x2Du,
    0x78u, 0x24u, 0x4Cu, 0x96u, 0x7Au, 0xAFu, 0xA5u, 0x55u,
    0x4Du, 0x25u, 0x94u, 0xB0u, 0xC1u, 0x3Du, 0x99u, 0xAAu,
    0x26u, 0x1Du, 0x68u, 0x95u, 0x3Au, 0x6Bu, 0x57u, 0xD0u,
    0x77u, 0x9Eu, 0x07u, 0x67u, 0x44u, 0x32u, 0xD8u, 0x20u,
    0xA7u, 0x9Fu, 0x25u, 0xFBu, 0x29u, 0x7Du, 0xBEu, 0xAAu,
    0xEAu, 0x5Fu, 0x05u, 0xF7u, 0x1Cu, 0x8Du, 0x80u, 0xA1u,
    0x25u, 0x27u, 0xF3u, 0x4Eu, 0xA0u, 0x2Du, 0xCDu, 0x9Du,
    0x1Fu, 0x75u, 0x14u, 0x39u, 0x5Au, 0x8Fu, 0xB4u, 0x53u,
    0x1Du, 0xB1u, 0x3Du, 0x42u, 0xDFu, 0xBFu, 0xD1u, 0x19u,
    0x6Bu, 0x08u, 0xC3u, 0x07u, 0x08u, 0x77u, 0x46u, 0x8Bu,
    0xD8u, 0x61u, 0xF6u, 0xF9u, 0x85u
};

/*! Will hold ML-DSA signatures */
static uint8_t signature[4627] = {0u};

/*! External work area for ML-DSA-87 due to memory limitations on the S200 */
static uint8_t workAreaBuffer[ELE_MLDSA_WORK_AREA_BUFFER_SIZE] = {0u};

/*! ML-KEM cipher text buffer and length */
static uint8_t mlkemCipherText[1568] = {0u};
static size_t mlkemCipherTextLen     = sizeof(mlkemCipherText);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Show usage of SSSAPI PQC ML-KEM extensions for establishing a shared
 *        secret.
 */
status_t mlkem1024(void)
{
    status_t status                     = kStatus_Fail;
    sss_sscp_object_t keyPair           = {0};
    sss_sscp_object_t sharedSecretAlice = {0};
    sss_sscp_object_t sharedSecretBob   = {0};
    sss_sscp_derive_key_t deriveCtx     = {0};

    uint8_t sharedKeyDataAlice[32]    = {0u};
    size_t sharedKeyDataAliceSize     = sizeof(sharedKeyDataAlice);
    size_t sharedKeyDataAliceSizeBits = sizeof(sharedKeyDataAlice) * 8u;

    uint8_t sharedKeyDataBob[32]    = {0u};
    size_t sharedKeyDataBobSize     = sizeof(sharedKeyDataBob);
    size_t sharedKeyDataBobSizeBits = sizeof(sharedKeyDataBob) * 8u;

    do
    {
        PRINTF("\r\n**** ML-KEM-1024 Establish Shared Secret ****\r\n");

        PRINTF("Initialize keyPair key object...");
        status = sss_sscp_key_object_init(&keyPair, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Initialize Alice's shared secret key object...");
        status = sss_sscp_key_object_init(&sharedSecretAlice, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Initialize Bob's shared secret key object...");
        status = sss_sscp_key_object_init(&sharedSecretBob, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate keyPair key object...");
        status = sss_sscp_key_object_allocate_handle(&keyPair, 0u,
                                                     kSSS_KeyPart_Pair,
                                                     kSSS_CipherType_MLKEM1024,
                                                     4736u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate Alice's shared secret key object...");
        status = sss_sscp_key_object_allocate_handle(&sharedSecretAlice, 0u,
                                                     kSSS_KeyPart_Default,
                                                     kSSS_CipherType_SYMMETRIC,
                                                     32u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate Bob's shared secret key object...");
        status = sss_sscp_key_object_allocate_handle(&sharedSecretBob, 0u,
                                                     kSSS_KeyPart_Default,
                                                     kSSS_CipherType_SYMMETRIC,
                                                     32u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Generate ML-KEM key pair...");
        status = sss_sscp_key_store_generate_key(&keyStore, &keyPair, 0u, NULL);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Initialize ML-KEM key derivation context...");
        status = sss_sscp_derive_key_context_init(&deriveCtx,
                                                  &sssSession,
                                                  &keyPair,
                                                  kAlgorithm_SSS_PQC_MLKEM,
                                                  kMode_SSS_ComputeSharedSecret);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("ML-KEM Encapsulate on Bob's side first...");
        status = sss_sscp_asymmetric_mlkem_derive_key(&deriveCtx,
                                                      mlkemCipherText,
                                                      &mlkemCipherTextLen,
                                                      &sharedSecretBob,
                                                      kMode_SSS_MlkemEncapsulate);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("ML-KEM Decapsulate on Alice's side...");
        status = sss_sscp_asymmetric_mlkem_derive_key(&deriveCtx,
                                                      mlkemCipherText,
                                                      &mlkemCipherTextLen,
                                                      &sharedSecretAlice,
                                                      kMode_SSS_MlkemDecapsulate);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the shared secret key K (Bob's)...");
        status = sss_sscp_key_store_get_key(&keyStore, &sharedSecretBob,
                                            sharedKeyDataBob,
                                            &sharedKeyDataBobSize,
                                            &sharedKeyDataBobSizeBits,
                                            kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the shared secret key K' (Alice's)...");
        status = sss_sscp_key_store_get_key(&keyStore, &sharedSecretAlice,
                                            sharedKeyDataAlice,
                                            &sharedKeyDataAliceSize,
                                            &sharedKeyDataAliceSizeBits,
                                            kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Verify that K' == K...");
        if (memcmp(sharedKeyDataBob, sharedKeyDataAlice, sizeof(sharedKeyDataBob)) != 0)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Clean up if all went well */
        PRINTF("Clean up...");
        if (sss_sscp_derive_key_context_free(&deriveCtx) != kStatus_SSS_Success ||
            sss_sscp_key_object_free(&keyPair, 1u) != kStatus_SSS_Success ||
            sss_sscp_key_object_free(&sharedSecretAlice, 1u) != kStatus_SSS_Success ||
            sss_sscp_key_object_free(&sharedSecretBob, 1u) != kStatus_SSS_Success)
        {
            status = kStatus_Fail;
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
 * @brief Show usage of SSSAPI PQC ML-DSA extension for signing and verification
 *        using the ML-DSA-87 (Pure variant) algorithm.
 *
 * This function showcases usage of the SSS API for leveraging signature
 * generation and verification with the ML-DSA-87 Pure algorithm, with input
 * context, and with specifying the work area (see note below).
 *
 * NOTE: The ML-DSA-87 implementation requires an external work area. Its size
 *       MUST be at least ELE_MLDSA_WORK_AREA_BUFFER_SIZE Bytes and
 *       it MUST be word-aligned.
 */
status_t mldsa87(void)
{
    status_t status               = kStatus_Fail;
    sss_sscp_object_t keyPair     = {0};
    sss_sscp_asymmetric_t asymCtx = {0};
    size_t signatureSize          = sizeof(signature);

    do
    {
        PRINTF("\r\n****    ML-DSA-87 Pure Sign and Verify   ****\r\n");
        PRINTF("**** with Context and External Work Area ****\r\n");

        PRINTF("Initialize keyPair key object...");
        status = sss_sscp_key_object_init(&keyPair, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate keyPair key object...");
        status = sss_sscp_key_object_allocate_handle(&keyPair,
                                                     0u,
                                                     kSSS_KeyPart_Pair,
                                                     kSSS_CipherType_MLDSA87,
                                                     4896u+2592u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Generate key...");
        status = sss_sscp_key_store_generate_key(&keyStore, &keyPair, 0u, NULL);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Initialize asymmetric signature context...");
        status = sss_sscp_asymmetric_context_init(&asymCtx,
                                                  &sssSession,
                                                  &keyPair,
                                                  kAlgorithm_SSS_PQC_MLDSA,
                                                  kMode_SSS_Sign);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* NOTE: The ML-DSA-87 implementation requires the user to provide
         * a word-aligned work area buffer with a size of at least
         * ELE_MLDSA_WORK_AREA_BUFFER_SIZE Bytes.
         * Though the other ML-DSA algorithms do not require this external work
         * area buffer, if it is provided, a word-alignment check will be done
         * regardless.
         */
        PRINTF("Sign message...");
        status = sss_sscp_asymmetric_sign_pqc(&asymCtx,
                                              (uint8_t*)message,
                                              sizeof(message),
                                              (uint8_t*)signature,
                                              &signatureSize,
                                              mldsaContext,
                                              sizeof(mldsaContext),
                                              kAlgorithm_SSS_MLDSA_Pure,
                                              workAreaBuffer,
                                              sizeof(workAreaBuffer));
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Clear the signature context...");
        status = sss_sscp_asymmetric_context_free(&asymCtx);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Initialize asymmetric verification context...");
        status = sss_sscp_asymmetric_context_init(&asymCtx,
                                                  &sssSession,
                                                  &keyPair,
                                                  kAlgorithm_SSS_PQC_MLDSA,
                                                  kMode_SSS_Verify);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Verify message...");
        status = sss_sscp_asymmetric_verify_pqc(&asymCtx,
                                                (uint8_t*)message,
                                                sizeof(message),
                                                (uint8_t*)signature,
                                                signatureSize,
                                                mldsaContext,
                                                sizeof(mldsaContext),
                                                kAlgorithm_SSS_MLDSA_Pure);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Clean up if all went well */
        PRINTF("Clean up...");
        if (sss_sscp_asymmetric_context_free(&asymCtx) != kStatus_SSS_Success ||
            sss_sscp_key_object_free(&keyPair, 1u) != kStatus_SSS_Success)
        {
            status = kStatus_Fail;
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
 * @brief Show usage of SSSAPI PQC ML-DSA extension for signing and verification
 *        using the ML-DSA-65 (Pre-Hash variant) algorithm.
 *
 * This function showcases usage of the SSS API for leveraging signature
 * generation and verification with the ML-DSA-65 algorithm with a Pre-Hashed
 * input (with SHA3-256), no input context, and without specifying the work
 * area, which is only required by the ML-DSA-87 algorithm.
 */
status_t mldsa65(void)
{
    status_t status               = kStatus_Fail;
    sss_sscp_object_t keyPair     = {0};
    sss_sscp_asymmetric_t asymCtx = {0};
    size_t signatureSize          = sizeof(signature);

    do
    {
        PRINTF("\r\n**** ML-DSA-65 SHA3-256 Pre-Hashed Sign and Verify ****\r\n");
        PRINTF("****     without Context and External Work Area    ****\r\n");

        PRINTF("Initialize keyPair key object...");
        status = sss_sscp_key_object_init(&keyPair, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate keyPair key object...");
        status = sss_sscp_key_object_allocate_handle(&keyPair,
                                                     0u,
                                                     kSSS_KeyPart_Pair,
                                                     kSSS_CipherType_MLDSA65,
                                                     4032u+1952u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Generate key...");
        status = sss_sscp_key_store_generate_key(&keyStore, &keyPair, 0u, NULL);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Initialize asymmetric signature context...");
        status = sss_sscp_asymmetric_context_init(&asymCtx,
                                                  &sssSession,
                                                  &keyPair,
                                                  kAlgorithm_SSS_PQC_MLDSA,
                                                  kMode_SSS_Sign);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* NOTE: This call has three changes compared to the ML-DSA-87 showcase.
         * 1. The message is Pre-Hashed; such support is enabled for SHA3-xxx,
         *    SHAKE128, and SHAKE256. When using this Pre-Hashed flow, the
         *    `preHashAlg` parameter must be set to the correct hash algorithm.
         * 2. The context is NULL; the context is not required by ML-DSA.
         * 3. The work area may be NULL and its size 0 when NOT using ML-DSA-87.
         *    This work area is required only by ML-DSA-87 due to a memory
         *    size limitation in the S200 subsystem.
         */
        PRINTF("Sign pre-hashed message...");
        status = sss_sscp_asymmetric_sign_pqc(&asymCtx,
                                              (uint8_t*)messageHashSHA3_256,
                                              sizeof(messageHashSHA3_256),
                                              (uint8_t*)signature,
                                              &signatureSize,
                                              NULL,
                                              0u,
                                              kAlgorithm_SSS_SHA3_256,
                                              NULL,
                                              0u);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Clear the signature context...");
        status = sss_sscp_asymmetric_context_free(&asymCtx);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Initialize asymmetric verification context...");
        status = sss_sscp_asymmetric_context_init(&asymCtx,
                                                  &sssSession,
                                                  &keyPair,
                                                  kAlgorithm_SSS_PQC_MLDSA,
                                                  kMode_SSS_Verify);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Verify pre-hashed message...");
        status = sss_sscp_asymmetric_verify_pqc(&asymCtx,
                                                (uint8_t*)messageHashSHA3_256,
                                                sizeof(messageHashSHA3_256),
                                                (uint8_t*)signature,
                                                signatureSize,
                                                NULL,
                                                0u,
                                                kAlgorithm_SSS_SHA3_256);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Clean up if all went well */
        PRINTF("Clean up...");
        if (sss_sscp_asymmetric_context_free(&asymCtx) != kStatus_SSS_Success ||
            sss_sscp_key_object_free(&keyPair, 1u) != kStatus_SSS_Success)
        {
            status = kStatus_Fail;
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

    PRINTF("ELE PQC via SSSAPI Example\r\n\r\n");
    /*
     * This code example demonstrates EdgeLock usage of the PQC SSSAPI.
     * The example is performed in following steps:
     * 1.  Load ELE firmware
     * 2.  Open an EdgeLock session
     * 3.  Create a key store
     * 4.  Prepare contexts and keys and run ML-DSA-65 Pre-Hash signing and verification
     * 5.  Clean up contexts and key objects
     * 6.  Prepare contexts and keys and run ML-DSA-85 Pure signing and verification
     * 7.  Clean up contexts and key objects
     * 8.  Prepare contexts and keys and run ML-KEM-1024 encapsulation and decapsulation
     * 9.  Clean up contexts and key objects
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

        status = ELEMU_loadFw((ELEMU_Type *)(uintptr_t)ELEMUA, (uint32_t *)fw);
        if (status != kStatus_Success)
        {
            break;
        }

        status = sss_sscp_open_session(&sssSession, 0u, ELE_SUBSYSTEM, &sscpContext);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        status = sss_sscp_key_store_init(&keyStore, &sssSession);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        status = mldsa65();
        if (status != kStatus_Success)
        {
            break;
        }

        status = mldsa87();
        if (status != kStatus_Success)
        {
            break;
        }

        status = mlkem1024();
        if (status != kStatus_Success)
        {
            break;
        }

        status = sss_sscp_key_store_free(&keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

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

    while (true)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
