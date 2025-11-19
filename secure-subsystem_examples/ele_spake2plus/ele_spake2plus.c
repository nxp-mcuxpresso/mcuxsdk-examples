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

status_t test_spake2plus_matter(void)
{
    status_t status                        = kStatus_Fail;
    sss_sscp_derive_key_t deriveKeyContext = {0};
    sss_sscp_object_t mainKey, w0, L, pA, pB, cA, cB, Ke;

    uint8_t outBuf[96] = {0};
    size_t outBufSize, outBufSizeBits;

    do
    {
        PRINTF("\r\n**** RUNNING SPAKE2+ (Matter) ****\r\n");

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
        PRINTF("\r\n**** Prepare context and run SPAKE2+ (Matter) ****\r\n");

        PRINTF("Derive Key Context Init...");
        status = sss_sscp_derive_key_context_init(&deriveKeyContext, &sssSession, &mainKey, kAlgorithm_SSS_SPAKE2PLUS,
                                                  kMode_SSS_ComputeSharedSecret);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Run SPAKE2+ (Matter)...");
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


/*
 * The S200 SPAKE2+ CCC service is aligned with the CCC SPAKE2+ specification
 * based on Digital Key Technical Specification v4.0.0 CCC-TS-101
 * Test data are located in "Appendix D. Owner Pairing Test Vectors"
 */

/*
 * Context data are used as derivation data for
 * Info: "ConfirmationKeys" (ASCII, 16 bytes)
 * 5b020101 (Agreed SPAKE2+ protocol version, 4 bytes)
 * 5c0401010100 (Tag 5C of the SPAKE2+ REQUEST command, 6 bytes)
 * 01 (HKDF counter value, 1 byte)
 */
static uint8_t contextData_ccc[] = {
    0x43u,0x6fu,0x6eu,0x66u,0x69u,0x72u,0x6du,0x61u,0x74u,0x69u,0x6fu,0x6eu,0x4bu,0x65u,0x79u,0x73u,
        0x5bu,0x02u,0x01u,0x01u,0x5cu,0x04u,0x01u,0x01u,0x01u,0x00u, 0x01u};

/* 'w0' and 'L' */
static uint8_t w0Data_ccc[] = {0xe4u,0x33u,0xabu,0x43u,0x42u,0x83u,0x20u,0xb2u,0x4fu,0xabu,0x82u,0xf9u,0x15u,0xd1u,0xdbu,0x11u,
        0x4au,0xcdu,0x72u,0xf8u,0xa4u,0xbfu,0x4fu,0xbfu,0x3cu,0x71u,0x2bu,0x94u,0xbcu,0xc2u,0xf0u,0x13u};

static uint8_t LData_ccc[]  = {0xffu,0x69u,0xebu,0x60u,0x86u,0x93u,0x8bu,0x3cu,0xceu,0x2cu,0x9eu,0x64u,0xdcu,0xacu,0xeau,0x1au,
        0x92u,0x59u,0x18u,0xe7u,0x5eu,0x8cu,0x17u,0x94u,0x8du,0x31u,0x63u,0x22u,0xd3u,0x70u,0x12u,0x3fu,
        0x69u,0x13u,0x2au,0xedu,0x73u,0x98u,0x91u,0x9eu,0x6eu,0x66u,0x14u,0xf7u,0x62u,0x7bu,0x0au,0x54u,
        0x06u,0x0cu,0x5au,0x8cu,0x0du,0x93u,0xd2u,0x75u,0x41u,0x66u,0xabu,0x10u,0xfeu,0xa6u,0xa8u,0xffu};

/*
 * The B sides main key pair. Last 32 Bytes are the private key 'y',
 * the first 64 Bytes are the public key 'y*P', where 'P' is the generator
 * point of curve P-256. The public key is not part of the RFC test vector and
 * was calculated for the purposes of this example.
 */
static uint8_t mainKeyData_ccc[] = {0xd0u,0x5du,0x3fu,0x33u,0xd7u,0xa6u,0x77u,0x10u,0xf7u,0x02u,0x75u,0x60u,0x0eu,0x29u,0x05u,0xe4u,
        0x4fu,0x43u,0x6eu,0x33u,0x31u,0xa1u,0xf4u,0x79u,0xf7u,0xdeu,0x0bu,0x65u,0x0fu,0x67u,0x9du,0x12u,
        0x1cu,0x40u,0x4au,0xa8u,0x4eu,0x6fu,0x75u,0xfbu,0x75u,0xd0u,0x52u,0x6fu,0xf4u,0x8eu,0x00u,0x4bu,
        0x4eu,0xa6u,0x89u,0xd7u,0xfcu,0x61u,0x9au,0x05u,0xe0u,0xbau,0x37u,0xdfu,0xbeu,0xf3u,0x40u,0xbfu,
        0xf1u,0xe1u,0xd1u,0xc1u,0xb1u,0xa1u,0x91u,0x81u,0x71u,0x61u,0x51u,0x41u,0x31u,0x21u,0x11u,0x01u,
        0xf0u,0xe0u,0xd0u,0xc0u,0xb0u,0xa0u,0x90u,0x80u,0x70u,0x60u,0x50u,0x40u,0x30u,0x20u,0x10u,0x00u};

/* 'pA' (also noted as 'X') */
static uint8_t pAData_ccc[] = {0xf4u,0x45u,0x55u,0x20u,0x7au,0x61u,0x7fu,0xd9u,0x09u,0x00u,0xdbu,0xa5u,0xc8u,0xe6u,0xf8u,0x1eu,
        0xddu,0xbdu,0x87u,0x59u,0x08u,0x73u,0xa6u,0x3bu,0x90u,0x57u,0xddu,0xa9u,0xf1u,0x38u,0xdbu,0xc1u,
        0x6fu,0x45u,0x31u,0x95u,0xf6u,0x45u,0x2cu,0xe7u,0x1du,0x39u,0x90u,0x52u,0x43u,0x59u,0x52u,0xb8u,
        0x9au,0x10u,0xb9u,0x27u,0x43u,0x55u,0x74u,0xf5u,0xe3u,0x70u,0x7eu,0xaeu,0x03u,0x1cu,0x40u,0xe0u };

/*
 * Expected reference results for:
 * 'pB' (also noted as 'Y')
 * 'Ke' (in CCC moted 'SK')
 * 'cA' (in CCC noted as 'M1')
 * 'cB' (in CCC noted as 'M2')
 *  CCC System Keys ('Kenc', 'Kmac', 'Krmac', 'LONG_TERM_SHARED_SECRET')
 */
static const uint8_t pBRef_ccc[] = {0xb6u,0xfdu,0xafu,0x3fu,0x69u,0x49u,0x86u,0x9du,0x68u,0xf6u,0x67u,0x10u,0x8bu,0x75u,0xe4u,0xceu,
        0x74u,0x84u,0x7eu,0x89u,0x53u,0xd1u,0xe3u,0xc6u,0xaau,0xe2u,0x16u,0x99u,0xe8u,0x02u,0x72u,0x11u,
        0xc2u,0xd9u,0xb2u,0xb2u,0xa9u,0x06u,0xccu,0x7eu,0xa7u,0x02u,0x07u,0x15u,0xdeu,0xc4u,0x4eu,0x95u,
        0x65u,0x9eu,0x3fu,0xc8u,0x99u,0x4fu,0x63u,0x5bu,0x95u,0xe7u,0xc9u,0xeau,0x5cu,0x36u,0x2cu,0xbeu};
static const uint8_t KeRef_ccc[] = {0x9fu,0x69u,0x50u,0x17u,0xbau,0x5eu,0x1du,0xf7u,0x4au,0x1bu,0x8bu,0xbfu,0x27u,0xfeu,0x1eu,0x0du};
static const uint8_t cARef_ccc[] = {0x11u,0x0du,0x49u,0xf8u,0xc5u,0xa8u,0x96u,0xe1u,0x1du,0x4du,0xdeu,0x4cu,0x3bu,0x97u,0x04u,0xd2u};
static const uint8_t cBRef_ccc[] = {0x23u,0xd1u,0xa6u,0x18u,0xadu,0x3au,0xcbu,0xfdu,0x7au,0x9bu,0xd1u,0x9fu,0xd1u,0x73u,0x71u,0x07u};
static const uint8_t kencRef_ccc[] = {0x16u,0x18u,0x86u,0xcbu,0x9au,0xe7u,0x40u,0x3du,0x8du,0xbcu,0xcfu,0xe3u,0x6bu,0x8au,0x04u,0x26u};
static const uint8_t kmacRef_ccc[] = {0x63u,0x87u,0xbau,0x65u,0x47u,0x9cu,0xb7u,0xebu,0x9du,0xf9u,0x7bu,0xd4u,0x8au,0xc3u,0x31u,0x59u};
static const uint8_t krmacRef_ccc[] = {0x41u,0x67u,0x7fu,0xb6u,0x39u,0x84u,0x59u,0x19u,0x9fu,0x1eu,0x56u,0x97u,0x60u,0xdfu,0x91u,0xc1u};
static const uint8_t ltssRef_ccc[] = {0x5cu,0x4eu,0x19u,0xdau,0x55u,0x35u,0x24u,0xe3u,0x86u,0xfau,0x1eu,0xcau,0x91u,0xe8u,0xadu,0x0eu};
static const uint8_t kBleIntrRef_ccc[] = {0x47u,0xaeu,0x79u,0x07u,0x2bu,0x47u,0xd9u,0x8bu,0xd2u,0x81u,0x69u,0xd2u,0xd8u,0x03u,0xa0u,0x7fu};
static const uint8_t kBleOobRef_ccc[] = {0x3cu,0x00u,0x9du,0x26u,0xe5u,0xe2u,0xd9u,0x69u,0x79u,0x99u,0xa9u,0x18u,0xdcu,0x2fu,0x8fu,0x08u};

status_t test_spake2plus_ccc(void)
{
    status_t status                        = kStatus_Fail;
    sss_sscp_derive_key_t deriveKeyContext = {0};
    sss_sscp_object_t mainKey, w0, L, pA, pB, cA, cB; /* input keys */
    sss_sscp_object_t Ke, kenc, kmac, krmac, ltss, kBleIntr, kBleOob; /* output keys */

    uint8_t outBuf[96] = {0};
    size_t outBufSize, outBufSizeBits;

    do
    {
        PRINTF("\r\n**** RUNNING SPAKE2+ (CCC) ****\r\n");

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

        PRINTF("Init kenc key object...");
        status = sss_sscp_key_object_init(&kenc, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init kmac key object...");
        status = sss_sscp_key_object_init(&kmac, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init krmac key object...");
        status = sss_sscp_key_object_init(&krmac, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init ltss key object...");
        status = sss_sscp_key_object_init(&ltss, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init kBleIntr key object...");
        status = sss_sscp_key_object_init(&kBleIntr, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Init kBleOob key object...");
        status = sss_sscp_key_object_init(&kBleOob, &keyStore);
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

        PRINTF("Allocate kenc handle...");
        status = sss_sscp_key_object_allocate_handle(&kenc, 0u, kSSS_KeyPart_Default, kSSS_CipherType_AES, 16u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate kmac handle...");
        status = sss_sscp_key_object_allocate_handle(&kmac, 0u, kSSS_KeyPart_Default, kSSS_CipherType_AES, 16u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate krmac handle...");
        status = sss_sscp_key_object_allocate_handle(&krmac, 0u, kSSS_KeyPart_Default, kSSS_CipherType_AES, 16u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate ltss handle...");
        status = sss_sscp_key_object_allocate_handle(&ltss, 0u, kSSS_KeyPart_Default, kSSS_CipherType_AES, 16u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate kBleIntr handle...");
        status = sss_sscp_key_object_allocate_handle(&kBleIntr, 0u, kSSS_KeyPart_Default, kSSS_CipherType_AES, 16u,
                                                     (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate kBleOob handle...");
        status = sss_sscp_key_object_allocate_handle(&kBleOob, 0u, kSSS_KeyPart_Default, kSSS_CipherType_AES, 16u,
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
            sss_sscp_key_store_set_key(&keyStore, &mainKey, mainKeyData_ccc, sizeof(mainKeyData_ccc), 256u, kSSS_KeyPart_Pair);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Set the w0 value...");
        status = sss_sscp_key_store_set_key(&keyStore, &w0, w0Data_ccc, sizeof(w0Data_ccc), sizeof(w0Data_ccc) * 8u,
                                            kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Set the L registration record...");
        status = sss_sscp_key_store_set_key(&keyStore, &L, LData_ccc, sizeof(LData_ccc), 256u, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Set the pA public share...");
        status = sss_sscp_key_store_set_key(&keyStore, &pA, pAData_ccc, sizeof(pAData_ccc), 256u, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        /* Run SPAKE2+ */
        PRINTF("\r\n**** Prepare context and run SPAKE2+ (CCC)****\r\n");

        PRINTF("Derive Key Context Init...");
        status = sss_sscp_derive_key_context_init(&deriveKeyContext, &sssSession, &mainKey, kAlgorithm_SSS_SPAKE2PLUS_CCC,
                                                  kMode_SSS_ComputeSharedSecret);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Run SPAKE2+ (CCC)...");
        status = sss_sscp_asymmetric_spake2plus_derive_key_ccc(&deriveKeyContext, &pA, &w0, &L, contextData_ccc,
                                                               sizeof(contextData_ccc), &pB, &cA, &cB, &Ke, &kenc,
                                                               &kmac, &krmac, &ltss, &kBleIntr, &kBleOob);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        /* Check reference keys for correctness */
        PRINTF("\r\n**** Check against expected values ****\r\n");

        PRINTF("Get the Ke (SK) key...");
        outBufSize = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &Ke, outBuf, &outBufSize, &outBufSizeBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Check against KeRef...");
        if (memcmp(outBuf, KeRef_ccc, sizeof(KeRef_ccc)))
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the pB (Y) key...");
        outBufSize = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &pB, outBuf, &outBufSize, &outBufSizeBits, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Check against pBRef...");
        if (memcmp(outBuf, pBRef_ccc, sizeof(pBRef_ccc)))
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the cA (M1) key...");
        outBufSize = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &cA, outBuf, &outBufSize, &outBufSizeBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Check against cARef...");
        if (memcmp(outBuf, cARef_ccc, sizeof(cARef_ccc)))
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the cB (M2) key...");
        outBufSize = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &cB, outBuf, &outBufSize, &outBufSizeBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Check against cBRef...");
        if (memcmp(outBuf, cBRef_ccc, sizeof(cBRef_ccc)))
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the kenc key...");
        outBufSize = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &kenc, outBuf, &outBufSize, &outBufSizeBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Check against kencRef...");
        if (memcmp(outBuf, kencRef_ccc, sizeof(kencRef_ccc)))
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the kmac key...");
        outBufSize = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &kmac, outBuf, &outBufSize, &outBufSizeBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Check against kmacRef...");
        if (memcmp(outBuf, kmacRef_ccc, sizeof(kmacRef_ccc)))
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the krmac key...");
        outBufSize = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &krmac, outBuf, &outBufSize, &outBufSizeBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Check against krmacRef...");
        if (memcmp(outBuf, krmacRef_ccc, sizeof(krmacRef_ccc)))
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the ltss key...");
        outBufSize = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &ltss, outBuf, &outBufSize, &outBufSizeBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Check against ltssRef...");
        if (memcmp(outBuf, ltssRef_ccc, sizeof(ltssRef_ccc)))
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the kBleIntr key...");
        outBufSize = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &kBleIntr, outBuf, &outBufSize, &outBufSizeBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Check against kBleIntrRef...");
        if (memcmp(outBuf, kBleIntrRef_ccc, sizeof(kBleIntrRef_ccc)))
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the kBleOob key...");
        outBufSize = sizeof(outBuf);
        status = sss_sscp_key_store_get_key(&keyStore, &kBleOob, outBuf, &outBufSize, &outBufSizeBits, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            PRINTF(ERROR_STRING);
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Check against kBleOobRef...");
        if (memcmp(outBuf, kBleOobRef_ccc, sizeof(kBleOobRef_ccc)))
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
            (sss_sscp_key_object_free(&Ke, 1u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&kenc, 1u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&kmac, 1u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&krmac, 1u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&ltss, 1u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&kBleIntr, 1u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&kBleOob, 1u) != kStatus_SSS_Success))
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
     * 1.  Load ELE firmware.
     * 2.  Open an EdgeLock session
     * 3.  Create a key store
     * 4.  Initialize key objects and allocate handles
     * 5.  Set the input data for SPAKE2+ Matter
     * 6.  Initialize a key derivation context and run SPAKE2+ Matter
     * 7.  Check if derived keys match the reference keys
     * 8.  Free contexts and key objects
     * 9.  Do steps 3. through 7. again, but for SPAKE2+ CCC
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

        /* Load firmware (only required for the CCC specification SPAKE2+) */
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

        status = test_spake2plus_matter();
        if (status != kStatus_Success)
        {
            break;
        }

        status = test_spake2plus_ccc();
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
