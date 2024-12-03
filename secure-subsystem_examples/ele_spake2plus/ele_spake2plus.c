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

/*
 * The S200 SPAKE2+ service is aligned with the Matter SPAKE2+ specification
 * based on draft 02 of RFC9383. For this reason, the test vector values
 * showcased here are taken from the mentioned RFC draft.
 * https://datatracker.ietf.org/doc/draft-bar-cfrg-spake2plus/02/
 */

/*
 * Context data format follows SPAKE2+ transcript specification:
 * contextData = len(Context) || Context
 *            || len(A) || A
 *            || len(B) || B
 *            || len(M) || M
 *            || len(N) || N
 */
static uint8_t contextData[] = {
    0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x53, 0x50, 0x41, 0x4b, 0x45, 0x32, 0x2b, 0x2d, 0x50, 0x32,
    0x35, 0x36, 0x2d, 0x53, 0x48, 0x41, 0x32, 0x35, 0x36, 0x2d, 0x48, 0x4b, 0x44, 0x46, 0x20, 0x64, 0x72, 0x61,
    0x66, 0x74, 0x2d, 0x30, 0x31, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x6c, 0x69, 0x65, 0x6e,
    0x74, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72, 0x41, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x88, 0x6e, 0x2f, 0x97, 0xac, 0xe4, 0x6e, 0x55, 0xba, 0x9d, 0xd7, 0x24,
    0x25, 0x79, 0xf2, 0x99, 0x3b, 0x64, 0xe1, 0x6e, 0xf3, 0xdc, 0xab, 0x95, 0xaf, 0xd4, 0x97, 0x33, 0x3d, 0x8f,
    0xa1, 0x2f, 0x5f, 0xf3, 0x55, 0x16, 0x3e, 0x43, 0xce, 0x22, 0x4e, 0x0b, 0x0e, 0x65, 0xff, 0x02, 0xac, 0x8e,
    0x5c, 0x7b, 0xe0, 0x94, 0x19, 0xc7, 0x85, 0xe0, 0xca, 0x54, 0x7d, 0x55, 0xa1, 0x2e, 0x2d, 0x20, 0x41, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xd8, 0xbb, 0xd6, 0xc6, 0x39, 0xc6, 0x29, 0x37, 0xb0, 0x4d, 0x99,
    0x7f, 0x38, 0xc3, 0x77, 0x07, 0x19, 0xc6, 0x29, 0xd7, 0x01, 0x4d, 0x49, 0xa2, 0x4b, 0x4f, 0x98, 0xba, 0xa1,
    0x29, 0x2b, 0x49, 0x07, 0xd6, 0x0a, 0xa6, 0xbf, 0xad, 0xe4, 0x50, 0x08, 0xa6, 0x36, 0x33, 0x7f, 0x51, 0x68,
    0xc6, 0x4d, 0x9b, 0xd3, 0x60, 0x34, 0x80, 0x8c, 0xd5, 0x64, 0x49, 0x0b, 0x1e, 0x65, 0x6e, 0xdb, 0xe7};

/* 'w0' and 'L' */
static uint8_t w0Data[] = {0xe6, 0x88, 0x7c, 0xf9, 0xbd, 0xfb, 0x75, 0x79, 0xc6, 0x9b, 0xf4,
                           0x79, 0x28, 0xa8, 0x45, 0x14, 0xb5, 0xe3, 0x55, 0xac, 0x03, 0x48,
                           0x63, 0xf7, 0xff, 0xaf, 0x43, 0x90, 0xe6, 0x7d, 0x79, 0x8c};
static uint8_t LData[]  = {0x95, 0x64, 0x5c, 0xfb, 0x74, 0xdf, 0x6e, 0x58, 0xf9, 0x74, 0x8b, 0xb8, 0x3a,
                           0x86, 0x62, 0x0b, 0xab, 0x7c, 0x82, 0xe1, 0x07, 0xf5, 0x7d, 0x68, 0x70, 0xda,
                           0x8c, 0xbc, 0xb2, 0xff, 0x9f, 0x70, 0x63, 0xa1, 0x4b, 0x64, 0x02, 0xc6, 0x2f,
                           0x99, 0xaf, 0xcb, 0x97, 0x06, 0xa4, 0xd1, 0xa1, 0x43, 0x27, 0x32, 0x59, 0xfe,
                           0x76, 0xf1, 0xc6, 0x05, 0xa3, 0x63, 0x97, 0x45, 0xa9, 0x21, 0x54, 0xb9};

/*
 * The B sides main key pair. Last 32 Bytes are the private key 'y',
 * the first 64 Bytes are the public key 'y*P', where 'P' is the generator
 * point of curve P-256. The public key is not part of the RFC test vector and
 * was calculated for the purposes of this example.
 */
static uint8_t mainKeyData[] = {0x3f, 0x14, 0xff, 0xe4, 0xec, 0x29, 0xd0, 0xbd, 0x43, 0xbc, 0xb7, 0xaa, 0xc1, 0xb1,
                                0x2d, 0x78, 0xfe, 0xdf, 0xd4, 0xc5, 0x13, 0x19, 0xc4, 0x7c, 0xfe, 0x70, 0xbe, 0x04,
                                0x3e, 0x2b, 0x22, 0xa4, 0xca, 0x0a, 0xf5, 0x83, 0x39, 0xe7, 0x80, 0xc9, 0x22, 0xf6,
                                0x63, 0x74, 0xca, 0x9a, 0x21, 0xba, 0xb4, 0x1c, 0x15, 0x57, 0x71, 0x25, 0xbd, 0x1e,
                                0x52, 0x8a, 0xa0, 0xc1, 0xe7, 0xf3, 0xea, 0x94, 0x2e, 0x08, 0x95, 0xb0, 0xe7, 0x63,
                                0xd6, 0xd5, 0xa9, 0x56, 0x44, 0x33, 0xe6, 0x4a, 0xc3, 0xca, 0xc7, 0x4f, 0xf8, 0x97,
                                0xf6, 0xc3, 0x44, 0x52, 0x47, 0xba, 0x1b, 0xab, 0x40, 0x08, 0x2a, 0x91};

/* 'pA' (also noted as 'X') */
static uint8_t pAData[] = {0xaf, 0x09, 0x98, 0x7a, 0x59, 0x3d, 0x3b, 0xac, 0x86, 0x94, 0xb1, 0x23, 0x83,
                           0x94, 0x22, 0xc3, 0xcc, 0x87, 0xe3, 0x7d, 0x6b, 0x41, 0xc1, 0xd6, 0x30, 0xf0,
                           0x00, 0xdd, 0x64, 0x98, 0x0e, 0x53, 0x7a, 0xe7, 0x04, 0xbc, 0xed, 0xe0, 0x4e,
                           0xa3, 0xbe, 0xc9, 0xb7, 0x47, 0x5b, 0x32, 0xfa, 0x2c, 0xa3, 0xb6, 0x84, 0xbe,
                           0x14, 0xd1, 0x16, 0x45, 0xe3, 0x8e, 0xa6, 0x60, 0x9e, 0xb3, 0x9e, 0x7e};

/*
 * Expected reference results for:
 * 'pB' (also noted as 'Y')
 * 'Ke'
 * 'cA' (also noted as 'HMAC(KcA, Y)')
 * 'cB' (also noted as 'HMAC(KcB, X)')
 */
static const uint8_t pBRef[] = {0x41, 0x75, 0x92, 0x62, 0x0a, 0xeb, 0xf9, 0xfd, 0x20, 0x36, 0x16, 0xbb, 0xb9,
                                0xf1, 0x21, 0xb7, 0x30, 0xc2, 0x58, 0xb2, 0x86, 0xf8, 0x90, 0xc5, 0xf1, 0x9f,
                                0xea, 0x83, 0x3a, 0x9c, 0x90, 0x0c, 0xbe, 0x90, 0x57, 0xbc, 0x54, 0x9a, 0x3e,
                                0x19, 0x97, 0x5b, 0xe9, 0x92, 0x7f, 0x0e, 0x76, 0x14, 0xf0, 0x8d, 0x1f, 0x0a,
                                0x10, 0x8e, 0xed, 0xe5, 0xfd, 0x7e, 0xb5, 0x62, 0x45, 0x84, 0xa4, 0xf4};
static const uint8_t KeRef[] = {0x80, 0x1d, 0xb2, 0x97, 0x65, 0x48, 0x16, 0xeb,
                                0x4f, 0x02, 0x86, 0x81, 0x29, 0xb9, 0xdc, 0x89};
static const uint8_t cARef[] = {0xd4, 0x37, 0x6f, 0x2d, 0xa9, 0xc7, 0x22, 0x26, 0xdd, 0x15, 0x1b,
                                0x77, 0xc2, 0x91, 0x90, 0x71, 0x15, 0x5f, 0xc2, 0x2a, 0x20, 0x68,
                                0xd9, 0x0b, 0x5f, 0xaa, 0x6c, 0x78, 0xc1, 0x1e, 0x77, 0xdd};
static const uint8_t cBRef[] = {0x06, 0x60, 0xa6, 0x80, 0x66, 0x3e, 0x8c, 0x56, 0x95, 0x95, 0x6f,
                                0xb2, 0x2d, 0xff, 0x29, 0x8b, 0x1d, 0x07, 0xa5, 0x26, 0xcf, 0x3c,
                                0xc5, 0x91, 0xad, 0xfe, 0xcd, 0x1f, 0x6e, 0xf6, 0xe0, 0x2e};

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t test_spake2plus(void)
{
    status_t status                        = kStatus_Fail;
    sss_sscp_derive_key_t deriveKeyContext = {0};
    sss_sscp_object_t mainKey, w0, L, pA, pB, cA, cB, Ke;

    uint8_t outBuf[96] = {0};
    size_t outBufSize, outBufSizeBits;

    do
    {
        /* Init key objects */
        PRINTF("\r\n**** Initialize all key objects ****\r\n");

        PRINTF("Init mainKey key object...");
        status = sss_sscp_key_object_init(&mainKey, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init w0 key object...");
        status = sss_sscp_key_object_init(&w0, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init L key object...");
        status = sss_sscp_key_object_init(&L, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init pA key object...");
        status = sss_sscp_key_object_init(&pA, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init pB key object...");
        status = sss_sscp_key_object_init(&pB, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init cA key object...");
        status = sss_sscp_key_object_init(&cA, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init cB key object...");
        status = sss_sscp_key_object_init(&cB, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init Ke key object...");
        status = sss_sscp_key_object_init(&Ke, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        /* Allocate handles */
        PRINTF("\r\n**** Allocate handles ****\r\n");

        PRINTF("Allocate mainKey handle...");
        status = sss_sscp_key_object_allocate_handle(&mainKey, 0u, kSSS_KeyPart_Pair, kSSS_CipherType_EC_NIST_P, 96u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate w0 handle...");
        status = sss_sscp_key_object_allocate_handle(&w0, 0u, kSSS_KeyPart_Default, kSSS_CipherType_AES, 32u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate L handle...");
        status = sss_sscp_key_object_allocate_handle(&L, 0u, kSSS_KeyPart_Public, kSSS_CipherType_EC_NIST_P, 64u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate pA handle...");
        status = sss_sscp_key_object_allocate_handle(&pA, 0u, kSSS_KeyPart_Public, kSSS_CipherType_EC_NIST_P, 64u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate pB handle...");
        status = sss_sscp_key_object_allocate_handle(&pB, 0u, kSSS_KeyPart_Public, kSSS_CipherType_EC_NIST_P, 64u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate cA handle...");
        status = sss_sscp_key_object_allocate_handle(&cA, 0u, kSSS_KeyPart_Default, kSSS_CipherType_AES, 32u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate cB handle...");
        status = sss_sscp_key_object_allocate_handle(&cB, 0u, kSSS_KeyPart_Default, kSSS_CipherType_AES, 32u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate Ke handle...");
        status = sss_sscp_key_object_allocate_handle(&Ke, 0u, kSSS_KeyPart_Default, kSSS_CipherType_AES, 16u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        /* Set inputs */
        PRINTF("\r\n**** Set the inputs ****\r\n");

        PRINTF("Set mainKey key-pair...");
        status =
            sss_sscp_key_store_set_key(&keyStore, &mainKey, mainKeyData, sizeof(mainKeyData), 256u, kSSS_KeyPart_Pair);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Set the w0 value...");
        status = sss_sscp_key_store_set_key(&keyStore, &w0, w0Data, sizeof(w0Data), sizeof(w0Data) * 8u,
                                            kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Set the L registration record...");
        status = sss_sscp_key_store_set_key(&keyStore, &L, LData, sizeof(LData), 256u, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Set the pA public share...");
        status = sss_sscp_key_store_set_key(&keyStore, &pA, pAData, sizeof(pAData), 256u, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        /* Run SPAKE2+ */
        PRINTF("\r\n**** Prepare context and run SPAKE2+ ****\r\n");

        PRINTF("Derive Key Context Init...");
        status = sss_sscp_derive_key_context_init(&deriveKeyContext, &sssSession, &mainKey, kAlgorithm_SSS_SPAKE2PLUS,
                                                  kMode_SSS_ComputeSharedSecret);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Run SPAKE2+...");
        status = sss_sscp_asymmetric_spake2plus_derive_key(&deriveKeyContext, &pA, &w0, &L, contextData,
                                                           sizeof(contextData), &pB, &cA, &cB, &Ke);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        /* Check reference keys for correctness */
        PRINTF("\r\n**** Check against expected values ****\r\n");

        PRINTF("Get the Ke key...");
        outBufSize = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &Ke, outBuf, &outBufSize, &outBufSizeBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Check against KeRef...");
        if (memcmp(outBuf, KeRef, sizeof(KeRef)))
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the pB key...");
        outBufSize = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &pB, outBuf, &outBufSize, &outBufSizeBits, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Check against pBRef...");
        if (memcmp(outBuf, pBRef, sizeof(pBRef)))
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the cA key...");
        outBufSize = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &cA, outBuf, &outBufSize, &outBufSizeBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Check against cARef...");
        if (memcmp(outBuf, cARef, sizeof(cARef)))
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the cB key...");
        outBufSize = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &cB, outBuf, &outBufSize, &outBufSizeBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Check against cBRef...");
        if (memcmp(outBuf, cBRef, sizeof(cBRef)))
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        /* Clean up if all went well */
        PRINTF("Clean up...");

        if ((sss_sscp_derive_key_context_free(&deriveKeyContext) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&mainKey, 1u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&w0, 1u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&L, 1u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&pA, 1u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&pB, 1u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&cA, 1u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&cB, 1u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&Ke, 1u) != kStatus_SSS_Success))
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

    PRINTF("ELE SPAKE2+ via SSSAPI Example\r\n\r\n");

    /*
     * This code example demonstrates EdgeLock usage of the SPAKE2+ SSSAPI.
     * The example is performed in following steps:
     * 1. Open an EdgeLock session
     * 2. Create a key store
     * 3. Initialize key objects and allocate handles
     * 4. Set the input data for SPAKE2+
     * 5. Initialize a key derivation context and run SPAKE2+
     * 6. Check if derived keys match the reference keys
     * 7. Free contexts and key objects
     * 8. Free the key store
     * 9. Close the EdgeLock session
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

        status = test_spake2plus();
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
