/*
 * Copyright 2026 NXP
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

static uint8_t scalarK[]    = {0x44u,0x36u,0x3du,0x15u,0x7fu,0x47u,0x12u,0x21u,
                               0xb1u,0xe7u,0x5eu,0x59u,0x6fu,0xf4u,0x71u,0x4au,
                               0x71u,0x2bu,0x95u,0x78u,0x30u,0x16u,0x65u,0xd8u,
                               0x4eu,0xc1u,0x70u,0x04u,0x95u,0x25u,0x23u,0xa8u};

static uint8_t publicKeyG[] = {0x6bu,0x17u,0xd1u,0xf2u,0xe1u,0x2cu,0x42u,0x47u,
                               0xf8u,0xbcu,0xe6u,0xe5u,0x63u,0xa4u,0x40u,0xf2u,
                               0x77u,0x03u,0x7du,0x81u,0x2du,0xebu,0x33u,0xa0u,
                               0xf4u,0xa1u,0x39u,0x45u,0xd8u,0x98u,0xc2u,0x96u,
                               0x4fu,0xe3u,0x42u,0xe2u,0xfeu,0x1au,0x7fu,0x9bu,
                               0x8eu,0xe7u,0xebu,0x4au,0x7cu,0x0fu,0x9eu,0x16u,
                               0x2bu,0xceu,0x33u,0x57u,0x6bu,0x31u,0x5eu,0xceu,
                               0xcbu,0xb6u,0x40u,0x68u,0x37u,0xbfu,0x51u,0xf5u};

/* Same as publicKeyG with the last Byte changed to place the point outside
 * the elliptic curve.
 */
static uint8_t publicKeyGWrong[] = {
                               0x6bu,0x17u,0xd1u,0xf2u,0xe1u,0x2cu,0x42u,0x47u,
                               0xf8u,0xbcu,0xe6u,0xe5u,0x63u,0xa4u,0x40u,0xf2u,
                               0x77u,0x03u,0x7du,0x81u,0x2du,0xebu,0x33u,0xa0u,
                               0xf4u,0xa1u,0x39u,0x45u,0xd8u,0x98u,0xc2u,0x96u,
                               0x4fu,0xe3u,0x42u,0xe2u,0xfeu,0x1au,0x7fu,0x9bu,
                               0x8eu,0xe7u,0xebu,0x4au,0x7cu,0x0fu,0x9eu,0x16u,
                               0x2bu,0xceu,0x33u,0x57u,0x6bu,0x31u,0x5eu,0xceu,
                               0xcbu,0xb6u,0x40u,0x68u,0x37u,0xbfu,0x51u,0xf6u};

/* K x g*/
static uint8_t multResult[] = {0xffu,0x69u,0xebu,0x60u,0x86u,0x93u,0x8bu,0x3cu,
                               0xceu,0x2cu,0x9eu,0x64u,0xdcu,0xacu,0xeau,0x1au,
                               0x92u,0x59u,0x18u,0xe7u,0x5eu,0x8cu,0x17u,0x94u,
                               0x8du,0x31u,0x63u,0x22u,0xd3u,0x70u,0x12u,0x3fu,
                               0x69u,0x13u,0x2au,0xedu,0x73u,0x98u,0x91u,0x9eu,
                               0x6eu,0x66u,0x14u,0xf7u,0x62u,0x7bu,0x0au,0x54u,
                               0x06u,0x0cu,0x5au,0x8cu,0x0du,0x93u,0xd2u,0x75u,
                               0x41u,0x66u,0xabu,0x10u,0xfeu,0xa6u,0xa8u,0xffu};

/* 2G = G + G */
static uint8_t addResult[]  = {0x7cu,0xf2u,0x7bu,0x18u,0x8du,0x03u,0x4fu,0x7eu,
                               0x8au,0x52u,0x38u,0x03u,0x04u,0xb5u,0x1au,0xc3u,
                               0xc0u,0x89u,0x69u,0xe2u,0x77u,0xf2u,0x1bu,0x35u,
                               0xa6u,0x0bu,0x48u,0xfcu,0x47u,0x66u,0x99u,0x78u,
                               0x07u,0x77u,0x55u,0x10u,0xdbu,0x8eu,0xd0u,0x40u,
                               0x29u,0x3du,0x9au,0xc6u,0x9fu,0x74u,0x30u,0xdbu,
                               0xbau,0x7du,0xadu,0xe6u,0x3cu,0xe9u,0x82u,0x29u,
                               0x9eu,0x04u,0xb7u,0x9du,0x22u,0x78u,0x73u,0xd1u};

/*******************************************************************************
 * Code
 ******************************************************************************/

static status_t ecPointOperationSub()
{
    status_t status           = kStatus_Fail;
    uint8_t out_buf[64]       = {0u};
    size_t out_buf_len        = sizeof(out_buf);
    size_t out_buf_bitlen     = sizeof(out_buf) * 8u;
    sss_sscp_object_t kObjIn1 = {0u};
    sss_sscp_object_t kObjIn2 = {0u};
    sss_sscp_object_t kObjOut = {0u};

    do
    {
        PRINTF("**** Elliptic Curve operation SUB ****\r\n");

        PRINTF("Init kObjIn1...");
        status = sss_sscp_key_object_init(&kObjIn1, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init kObjIn2...");
        status = sss_sscp_key_object_init(&kObjIn2, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init kObjOut...");
        status = sss_sscp_key_object_init(&kObjOut, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate kObjIn1 key object handle...");
        status = sss_sscp_key_object_allocate_handle(&kObjIn1, 0u, kSSS_KeyPart_Public, kSSS_CipherType_EC_NIST_P,
                                                     64u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate kObjIn2 key object handle...");
        status = sss_sscp_key_object_allocate_handle(&kObjIn2, 0u, kSSS_KeyPart_Public, kSSS_CipherType_EC_NIST_P,
                                                     64u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate kObjOut key object handle...");
        status = sss_sscp_key_object_allocate_handle(&kObjOut, 0u, kSSS_KeyPart_Public, kSSS_CipherType_EC_NIST_P,
                                                     64u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Import a point (maps to a public key part) */
        PRINTF("Set point in kObjIn1...");
        status = sss_sscp_key_store_set_key(&keyStore, &kObjIn1, addResult, sizeof(addResult), 256u, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Import a point (maps to a public key part) */
        PRINTF("Set point in kObjIn2...");
        status = sss_sscp_key_store_set_key(&keyStore, &kObjIn2, publicKeyG, sizeof(publicKeyG), 256u, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("SUB operation...");
        status = sss_sscp_asymmetric_ec_point_operate(&sssSession, &kObjIn1, &kObjIn2, &kObjOut, kSSS_ecPointOp_SUB);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Check if value matches */
        PRINTF("Get result...");
        status = sss_sscp_key_store_get_key(&keyStore, &kObjOut, out_buf, &out_buf_len, &out_buf_bitlen, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("SUB result matches expected value...");
        if (memcmp(out_buf, publicKeyG, sizeof(publicKeyG)) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* Cleanup */
        PRINTF("Cleanup...");
        if ((sss_sscp_key_object_free(&kObjIn1, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&kObjIn2, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&kObjOut, 0u) != kStatus_SSS_Success))
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

static status_t ecPointOperationAdd()
{
    status_t status           = kStatus_Fail;
    uint8_t out_buf[64]       = {0u};
    size_t out_buf_len        = sizeof(out_buf);
    size_t out_buf_bitlen     = sizeof(out_buf) * 8u;
    sss_sscp_object_t kObjIn1 = {0u};
    sss_sscp_object_t kObjIn2 = {0u};
    sss_sscp_object_t kObjOut = {0u};

    do
    {
        PRINTF("**** Elliptic Curve operation ADD ****\r\n");

        PRINTF("Init kObjIn1...");
        status = sss_sscp_key_object_init(&kObjIn1, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init kObjIn2...");
        status = sss_sscp_key_object_init(&kObjIn2, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init kObjOut...");
        status = sss_sscp_key_object_init(&kObjOut, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate kObjIn1 key object handle...");
        status = sss_sscp_key_object_allocate_handle(&kObjIn1, 0u, kSSS_KeyPart_Public, kSSS_CipherType_EC_NIST_P,
                                                     64u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate kObjIn2 key object handle...");
        status = sss_sscp_key_object_allocate_handle(&kObjIn2, 0u, kSSS_KeyPart_Public, kSSS_CipherType_EC_NIST_P,
                                                     64u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate kObjOut key object handle...");
        status = sss_sscp_key_object_allocate_handle(&kObjOut, 0u, kSSS_KeyPart_Public, kSSS_CipherType_EC_NIST_P,
                                                     64u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Import a point (maps to a public key part) */
        PRINTF("Set point in kObjIn1...");
        status = sss_sscp_key_store_set_key(&keyStore, &kObjIn1, publicKeyG, sizeof(publicKeyG), 256u, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Import a point (maps to a public key part) */
        PRINTF("Set point in kObjIn2...");
        status = sss_sscp_key_store_set_key(&keyStore, &kObjIn2, publicKeyG, sizeof(publicKeyG), 256u, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("ADD operation...");
        status = sss_sscp_asymmetric_ec_point_operate(&sssSession, &kObjIn1, &kObjIn2, &kObjOut, kSSS_ecPointOp_ADD);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Check if value matches */
        PRINTF("Get result...");
        status = sss_sscp_key_store_get_key(&keyStore, &kObjOut, out_buf, &out_buf_len, &out_buf_bitlen, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("ADD result matched expected value...");
        if (memcmp(out_buf, addResult, sizeof(addResult)) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* Cleanup */
        PRINTF("Cleanup...");
        if ((sss_sscp_key_object_free(&kObjIn1, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&kObjIn2, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&kObjOut, 0u) != kStatus_SSS_Success))
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

    return status;}


static status_t ecPointOperationMult()
{
    status_t status           = kStatus_Fail;
    uint8_t out_buf[64]       = {0u};
    size_t out_buf_len        = sizeof(out_buf);
    size_t out_buf_bitlen     = sizeof(out_buf) * 8u;
    sss_sscp_object_t kObjIn1 = {0u};
    sss_sscp_object_t kObjIn2 = {0u};
    sss_sscp_object_t kObjOut = {0u};

    do
    {
        PRINTF("**** Elliptic Curve operation MULT ****\r\n");

        PRINTF("Init kObjIn1...");
        status = sss_sscp_key_object_init(&kObjIn1, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init kObjIn2...");
        status = sss_sscp_key_object_init(&kObjIn2, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init kObjOut...");
        status = sss_sscp_key_object_init(&kObjOut, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate kObjIn1 key object handle...");
        status = sss_sscp_key_object_allocate_handle(&kObjIn1, 0u, kSSS_KeyPart_Private, kSSS_CipherType_EC_NIST_P,
                                                     32u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate kObjIn2 key object handle...");
        status = sss_sscp_key_object_allocate_handle(&kObjIn2, 0u, kSSS_KeyPart_Public, kSSS_CipherType_EC_NIST_P,
                                                     64u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate kObjOut key object handle...");
        status = sss_sscp_key_object_allocate_handle(&kObjOut, 0u, kSSS_KeyPart_Public, kSSS_CipherType_EC_NIST_P,
                                                     64u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Import a scalar (maps to a private key part) */
        PRINTF("Set scalar in kObjIn1...");
        status = sss_sscp_key_store_set_key(&keyStore, &kObjIn1, scalarK, sizeof(scalarK), 256u, kSSS_KeyPart_Private);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Import a point (maps to a public key part) */
        PRINTF("Set point in kObjIn2...");
        status = sss_sscp_key_store_set_key(&keyStore, &kObjIn2, publicKeyG, sizeof(publicKeyG), 256u, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("MULT operation...");
        status = sss_sscp_asymmetric_ec_point_operate(&sssSession, &kObjIn1, &kObjIn2, &kObjOut, kSSS_ecPointOp_MULT);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Get the result...");
        status = sss_sscp_key_store_get_key(&keyStore, &kObjOut, out_buf, &out_buf_len, &out_buf_bitlen, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("MULT result matches expected value...");
        if (memcmp(out_buf, multResult, sizeof(multResult)) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* Cleanup */
        PRINTF("Cleanup...");
        if ((sss_sscp_key_object_free(&kObjIn1, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&kObjIn2, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&kObjOut, 0u) != kStatus_SSS_Success))
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

static status_t ecPointOperationTestPoint()
{
    status_t status           = kStatus_Fail;
    sss_sscp_object_t kObjIn1 = {0u};
    sss_sscp_object_t kObjIn2 = {0u};
    sss_sscp_object_t kObjOut = {0u};

    do
    {
        PRINTF("**** Elliptic Curve operation point on curve test ****\r\n");

        PRINTF("Init kObjIn1...");
        status = sss_sscp_key_object_init(&kObjIn1, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init kObjIn2...");
        status = sss_sscp_key_object_init(&kObjIn2, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Init kObjOut...");
        status = sss_sscp_key_object_init(&kObjOut, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Allocate kObjIn1 key object handle...");
        status = sss_sscp_key_object_allocate_handle(&kObjIn1, 0u, kSSS_KeyPart_Private, kSSS_CipherType_EC_NIST_P,
                                                     32u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate kObjIn2 key object handle...");
        status = sss_sscp_key_object_allocate_handle(&kObjIn2, 0u, kSSS_KeyPart_Public, kSSS_CipherType_EC_NIST_P,
                                                     64u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);
        PRINTF("Allocate kObjOut key object handle...");
        status = sss_sscp_key_object_allocate_handle(&kObjOut, 0u, kSSS_KeyPart_Public, kSSS_CipherType_EC_NIST_P,
                                                     64u, (sss_sscp_key_property_t)0x1Fu);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Set scalar in kObjIn1...");
        status = sss_sscp_key_store_set_key(&keyStore, &kObjIn1, scalarK, sizeof(scalarK), 256u, kSSS_KeyPart_Private);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Set valid curve point in kObjIn2...");
        status = sss_sscp_key_store_set_key(&keyStore, &kObjIn2, publicKeyG, sizeof(publicKeyG), 256u, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("MULT operation on a valid point on the curve...");
        status = sss_sscp_asymmetric_ec_point_operate(&sssSession, &kObjIn1, &kObjIn2, &kObjOut, kSSS_ecPointOp_MULT);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Set invalid curve point in kObjIn2...");
        status = sss_sscp_key_store_set_key(&keyStore, &kObjIn2, publicKeyGWrong, sizeof(publicKeyGWrong), 256u, kSSS_KeyPart_Public);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* The point publicKeyGWrong does not lie on the NIST P-256 curve,
         * so we expect failure here.
         */
        PRINTF("MULT operation on an invalid point outside the curve...");
        status = sss_sscp_asymmetric_ec_point_operate(&sssSession, &kObjIn1, &kObjIn2, &kObjOut, kSSS_ecPointOp_MULT);
        if (status == kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Cleanup */
        PRINTF("Cleanup...");
        if ((sss_sscp_key_object_free(&kObjIn1, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&kObjIn2, 0u) != kStatus_SSS_Success) ||
            (sss_sscp_key_object_free(&kObjOut, 0u) != kStatus_SSS_Success))
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

    PRINTF("ELE EC Point Operations SSSAPI Example\r\n\r\n");
    /*
     * This code example demonstrates EdgeLock usage of the SSSAPI for EC
     * arithmetic operations (ADD, SUB, and MULT).
     * The example is performed in following steps:
     * 1.  Open an EdgeLock session
     * 2.  Create a key store
     * 3.  Initialize key objects and allocate handles
     * 4.  Exercise MULT operation and check for correct result
     * 5.  Exercise ADD operation and check for correct result
     * 6.  Exercise SUB operation and check for correct result
     * 7.  Use the MULT operation to check if a point lies on a given curve
     * 8.  Free contexts and key objects
     * 9.  Free the key store
     * 10. Close the EdgeLock session
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

        status = ecPointOperationMult();
        if (status != kStatus_Success)
        {
            break;
        }

        status = ecPointOperationAdd();
        if (status != kStatus_Success)
        {
            break;
        }

        status = ecPointOperationSub();
        if (status != kStatus_Success)
        {
            break;
        }

        status = ecPointOperationTestPoint();
        if (status != kStatus_Success)
        {
            break;
        }

        /* Close keystore */
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

    while (true)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
