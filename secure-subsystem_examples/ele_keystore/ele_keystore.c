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

/*!
 * @brief Plaintext for examples using the generic AES API.
 * 16-byte multiple, last '\0' is not used.
 */
SDK_ALIGN(static uint8_t s_GenericAesPlain[], 8u) =
    "Be that word our sign of parting, bird or fiend! I shrieked upstarting"
    "Get thee back into the tempest and the Nights Plutonian shore!"
    "Leave no black plume as a token of that lie thy soul hath spoken!"
    "Leave my loneliness unbroken! quit the bust above my door!"
    "Take thy beak from out my heart, and take thy form from off my door!"
    "Quoth the raven, Nevermore.  ";

/*! @brief Initialization vector for CBC method: 16 bytes: "mysecretpassword". */
SDK_ALIGN(static uint8_t s_CbcIv[16u], 8u) = {0x6d, 0x79, 0x73, 0x65, 0x63, 0x72, 0x65, 0x74,
                                              0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64};

/*! @brief Encrypted ciphertext from CBC method goes here. */
SDK_ALIGN(uint8_t s_CbcCipher[sizeof(s_GenericAesPlain) - 1U], 8u);

/*! @brief Decrypted plaintext from CBC method goes here. */
SDK_ALIGN(uint8_t s_CbcPlainDecrypted[sizeof(s_GenericAesPlain) - 1U], 8u);
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */

int main(void)
{
    char ch;
    status_t status          = kStatus_Fail;
    sss_sscp_object_t sssKey = {0};
    sss_sscp_rng_t ctx_rng   = {0};
    sss_sscp_symmetric_t ctx = {0};
    size_t keysize           = 16u;

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("ELE Keystore via SSSAPI Example\r\n\r\n");

    /*
     * This code example demonstrates EdgeLock usage for Exporting key as a blob and import it back.
     * The example is performed in following steps:
     * 1. Open EdgeLock session
     * 2. Create key store
     * 3. Create and allocate key object
     * 4. Initialize RNG before key generation
     * 4. Generate opaque key
     * 5. Perform AES CBC encryption operation with opaque key
     * 6. Export opaque key as a blob
     * 7. Erase key from keystore, free keyobject ad close keystore and session
     * 8. Open session, keystore and init keyobject
     * 9. Allocate key handle
     * 10. Import key blob into ELE
     * 10. Perform AES CBC decryption with key from blob
     * 10. Compare decrypted message with plaintext, if matches that means imported and exported key is the same
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

        /* Open session to specific security subsystem */
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

        /* Init key object  */
        status = sss_sscp_key_object_init(&sssKey, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        /* Allocate keystore handle */
        status = sss_sscp_key_object_allocate_handle(&sssKey, 1u, /* key id */
                                                     kSSS_KeyPart_Default, kSSS_CipherType_AES, keysize,
                                                     kSSS_KeyProp_CryptoAlgo_AES);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }

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

        PRINTF("Generate opaque key...");
        /* Generate opaque key */
        status = sss_sscp_key_store_generate_key(&keyStore, &sssKey, keysize * 8U, NULL);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }
        PRINTF(OK_STRING);

        /* Encrypt message by AES CBC using opaque key */

        /* Init symmetric context */
        status = sss_sscp_symmetric_context_init(&ctx, &sssSession, &sssKey, kAlgorithm_SSS_AES_CBC, kMode_SSS_Encrypt);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }

        /* RUN AES-CBC Encryption */
        PRINTF("Encrypt message by AES CBC using opaque key...");
        status = sss_sscp_cipher_one_go(&ctx, s_CbcIv, sizeof(s_CbcIv), s_GenericAesPlain, s_CbcCipher,
                                        sizeof(s_GenericAesPlain) - 1U);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_symmetric_context_free(&ctx);
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }
        PRINTF(OK_STRING);

        /* Close AES context*/
        status = sss_sscp_symmetric_context_free(&ctx);

        /* Export key into blob */

        uint8_t key_blob[100] = {0};
        size_t blob_size      = keysize + 24u;
        /* Export key  as ELE die unique blob */
        PRINTF("Export opaque key as blob...");
        status = sss_sscp_key_store_export_key(&keyStore, &sssKey, key_blob, &blob_size, kSSS_blobType_ELKE_blob);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }
        PRINTF(OK_STRING);

        /* Erase key from keystore */
        PRINTF("Delete key from keystore...");
        status = sss_sscp_key_store_erase_key(&keyStore, &sssKey);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }
        PRINTF(OK_STRING);

        /* Free keyobject */
        (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);

        PRINTF("Free all object, close keystore and session...");
        /* Close keystore */
        status = sss_sscp_key_store_free(&keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
        PRINTF(OK_STRING);

        /* Close session */
        status = sss_sscp_close_session(&sssSession);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        /* Recreate session, keystore, import blob and then decrypt ciphertext, check if decrypted plaintext is same as
         * on start which means that the key is still same and imported blob is correct*/

        PRINTF("Open session and keystore...");
        /* Open session to specific security subsystem */
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
        PRINTF(OK_STRING);

        /* Init key object  */
        status = sss_sscp_key_object_init(&sssKey, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        /* Allocate keystore handle which will be used to hold key from blob */
        /* Key properties will be set from imported blob later, during allocating handle it can be anything expect
         * kSSS_KeyProp_NoImportExport */
        status = sss_sscp_key_object_allocate_handle(&sssKey, 1u, /* key id */
                                                     kSSS_KeyPart_Default, kSSS_CipherType_AES, keysize,
                                                     kSSS_KeyProp_CryptoAlgo_AES);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }

        /* Import blob into ELE at previously allocated handle */
        PRINTF("Import blob into ELE...");
        status = sss_sscp_key_store_import_key(&keyStore, &sssKey, key_blob, blob_size, keysize * 8U,
                                               kSSS_blobType_ELKE_blob);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }
        PRINTF(OK_STRING);

        /* Init symmetric cipher context for decryption previously encrypted message */
        status = sss_sscp_symmetric_context_init(&ctx, &sssSession, &sssKey, kAlgorithm_SSS_AES_CBC, kMode_SSS_Decrypt);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }

        /* RUN AES-CBC Decryption */
        PRINTF("Decryption previously encrypted message...");
        status = sss_sscp_cipher_one_go(&ctx, s_CbcIv, sizeof(s_CbcIv), s_CbcCipher, s_CbcPlainDecrypted,
                                        sizeof(s_GenericAesPlain) - 1U);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_symmetric_context_free(&ctx);
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }
        PRINTF(OK_STRING);

        /* Close AES context*/
        status = sss_sscp_symmetric_context_free(&ctx);

        if (status == kStatus_SSS_Success)
        {
            if (memcmp(s_CbcPlainDecrypted, s_GenericAesPlain, sizeof(s_CbcCipher)) == 0)
            {
                PRINTF(
                    "Decrypted message matches with plaintext, that means imported and exported key is the "
                    "same.\r\n\r\n");
                status = kStatus_Success;
            }
            else
            {
                status = kStatus_Fail;
            }
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

    PRINTF("Example end\r\n");

    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
