/*
 * Copyright 2025 NXP
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

#include "ele_200_fw.h" /* ELE FW */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ELE_MAX_SUBSYSTEM_WAIT (0xFFFFFFFFu)
#define ELE_SUBSYSTEM          (kType_SSS_Ele200)
#define OK_STRING              ("OK\r\n")
#define ERROR_STRING           ("ERROR\r\n")
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/

/* A 256bit EC private key */
static uint8_t priv[32u] = {0xb2u,0x30u,0x44u,0xa4u,0xccu,0xb2u,0x46u,0x52u,
                            0x3du,0x85u,0x55u,0xcdu,0xe3u,0x67u,0xfau,0x04u,
                            0xd3u,0xfeu,0x05u,0x7fu,0x3fu,0x8au,0xd6u,0x70u,
                            0x3au,0xdbu,0xceu,0xf3u,0x49u,0x2bu,0x8bu,0xc8u};

/* Blob to be imported = y1 || y2 */
uint8_t blob[96u] = {/* y1 part of the blob, of length 0x40 Bytes */
                     0x98u,0x6eu,0xb7u,0xf2u,0xf5u,0xccu,0x0du,0x29u,
                     0x8cu,0xbeu,0x21u,0x90u,0x9eu,0xa0u,0x2eu,0x80u,
                     0x86u,0x90u,0x5au,0x1cu,0x24u,0x2bu,0xb4u,0xf2u,
                     0xbdu,0x8bu,0x49u,0xd6u,0x02u,0x7bu,0x51u,0x59u,
                     0x6eu,0xddu,0xd5u,0x91u,0x9eu,0x1cu,0x50u,0xf8u,
                     0xfau,0x20u,0xb3u,0x75u,0x35u,0xceu,0x0cu,0x71u,
                     0x50u,0xa9u,0xdau,0xbcu,0x61u,0xbau,0x2au,0xe7u,
                     0x9cu,0x60u,0x57u,0x9au,0xfau,0xbdu,0x98u,0x1au,
                     /* y2 part of the blob, of length 0x20 Bytes */
                     0x31u,0x60u,0xc4u,0xd3u,0x7bu,0xb1u,0x83u,0x36u,
                     0x00u,0x61u,0xb6u,0x47u,0xd5u,0xccu,0x7eu,0xa8u,
                     0x11u,0xf6u,0x84u,0x6du,0x8au,0x11u,0x2cu,0xbfu,
                     0x70u,0x87u,0x9cu,0xe8u,0xcau,0xb6u,0xbau,0xcau};

const uint8_t shared_secret_expected[16u] = {0x99, 0x99, 0x96, 0xa7, 0x89, 0xa3, 0xbc, 0x00,
                                             0x45, 0xc8, 0xa5, 0xfb, 0x42, 0xc7, 0xd1, 0xbd};

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t get_ele_fw_version(ELEMU_Type *mu, uint8_t *ele_fw_version)
{
    sscp_context_t sscpContext    = {0u};
    sss_mgmt_t mgmtContext        = {0u};
    sss_sscp_session_t sssSession = {0u};
    status_t status               = kStatus_Fail;
    size_t datalen                = 8u;

    /* PropertyId of Edgelock Firmware version */
    uint32_t propertyId = 0x51u;

    do
    {
        if (sscp_mu_init(&sscpContext, mu) != kStatus_SSCP_Success)
        {
            break;
        }

        if (sss_sscp_open_session(&sssSession, 0u, kType_SSS_Ele200, &sscpContext) != kStatus_SSS_Success)
        {
            break;
        }

        if (sss_mgmt_context_init(&mgmtContext, &sssSession) != kStatus_SSS_Success)
        {
            break;
        }

        if (sss_mgmt_get_property(&mgmtContext, propertyId, ele_fw_version, &datalen) != kStatus_SSS_Success)
        {
            break;
        }

        /* If all steps before passes without break, then consider it as success*/
        status = kStatus_Success;

    } while (false);

    /* FREE MGMT CONTEXT */
    sss_mgmt_context_free(&mgmtContext);
    /* CLOSE SESSION */
    sss_sscp_close_session(&sssSession);

    return status;
}


/*!
 * @brief Main function
 */
int main(void)
{
    char ch;
    status_t status = kStatus_Fail;
    uint32_t ele_version[2];

    sscp_context_t sscpContext    = {0};
    sss_sscp_session_t sssSession = {0};
    sss_sscp_key_store_t keyStore = {0};

    /* ELE will respond with 0xFFFFFFFFFFFFFFFF if no FW is loaded*/
    static const uint32_t no_fw_loaded[2] = {0xFFFFFFFF, 0xFFFFFFFF};

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("ELE Elgamal example\r\n\r\n");

    do
    {
        status = ELEMU_mu_wait_for_ready(ELEMUA, ELE_MAX_SUBSYSTEM_WAIT);
        if (status != kStatus_Success)
        {
            break;
        }

        status = sscp_mu_init(&sscpContext, (ELEMU_Type *)(uintptr_t)ELEMUA);
        if (status != kStatus_SSCP_Success)
        {
            break;
        }

        /* First we load the FW and check if it loaded properly */
        PRINTF("Load ELE FW...");
        status = ELEMU_loadFw(ELEMUA, (uint32_t *)fw);
        (void)get_ele_fw_version(ELEMUA, (uint8_t *)ele_version);
        if (memcmp(no_fw_loaded, ele_version, sizeof(ele_version)) == 0 ||
            status != kStatus_Success)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

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

        /* Initialize the private key key object and set the private EC key */
        sss_sscp_object_t privkey = {0};
        size_t privkeysize        = sizeof(priv);

        PRINTF("Initialize and set the local private key...");
        status = sss_sscp_key_object_init(&privkey, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        status = sss_sscp_key_object_allocate_handle(&privkey, 0u, /* key id */
                                                     kSSS_KeyPart_Private,
                                                     kSSS_CipherType_EC_NIST_P,
                                                     privkeysize,
                                                     0x1f);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&privkey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }

        /* Set key into key object*/
        status = sss_sscp_key_store_set_key(&keyStore, &privkey, priv, privkeysize,
                                            (privkeysize * 8U), kSSS_KeyPart_Private);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&privkey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }
        PRINTF(OK_STRING);

        PRINTF("Initialize key object for the shared secret...");
        /* Create object and allocate handle for storing resulting key after importing blob */
        sss_sscp_object_t secretkey = {0};
        size_t secretkeysize        = 16u;

        status = sss_sscp_key_object_init(&secretkey, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        status = sss_sscp_key_object_allocate_handle(&secretkey, 0u, /* key id */
                                                     kSSS_KeyPart_Default,
                                                     kSSS_CipherType_AES,
                                                     secretkeysize,
                                                     0x1f);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&secretkey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }
        PRINTF(OK_STRING);

        /* Import blob into ELE at previously allocated handle */
        PRINTF("Import the blob into ELE...");
        status = sss_sscp_key_store_import_key_ext(&keyStore, &secretkey, blob, sizeof(blob), secretkeysize * 8U,
                                                   kSSS_blobType_ELGAMAL_blob, &privkey);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&secretkey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }
        PRINTF(OK_STRING);

        /* Get key from key object into plain buffer */
        uint8_t shared_secret_buf[16u] = {0u};
        size_t buf_len                 = sizeof(shared_secret_buf);
        size_t outKeySize              = 0u;

        PRINTF("Retrieve the shared secret into plaintext buffer...");
        status = sss_sscp_key_store_get_key(&keyStore, &secretkey, shared_secret_buf, &buf_len,
                                            &outKeySize, kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&privkey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }
        PRINTF(OK_STRING);

        size_t outKeySizeInBytes = outKeySize / 8u;
        PRINTF("The shared secret is\r\n    ");
        for (int i = 0; i < outKeySizeInBytes; i++)
        {
            PRINTF("%02X ", shared_secret_buf[i]);
        }

        PRINTF("\r\nComputed shared secret matches expected value...");
        if (memcmp(shared_secret_expected, shared_secret_buf, sizeof(shared_secret_expected)) != 0)
        {
            status = kStatus_Fail;
            break;
        }
        PRINTF(OK_STRING);

        /* Close session */
        status = sss_sscp_close_session(&sssSession);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        status = kStatus_Success;
    } while (0);

    if (status != kStatus_Success)
    {
        PRINTF(ERROR_STRING);
    }

    PRINTF("\r\nExample end\r\n");

    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
