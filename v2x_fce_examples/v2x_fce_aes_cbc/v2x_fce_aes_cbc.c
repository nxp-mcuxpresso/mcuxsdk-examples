/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

#include "v2x_fce.h" /* EdgeLock V2X / FCE fast crypto engine service */
#include "fsl_ele_base_api.h" /* EdgeLock enclave base API: V2X FW bring-up */
#include "v2x_base.h" /* EdgeLock V2X base (debug) service: FW version query */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef V2X_FCE_MU
#define V2X_FCE_MU ((MU_Type *)EXAMPLE_V2X_FCE_MU_BASE)
#endif

/* V2X (debug) MU used to query the V2X firmware version (0x47350000). */
#ifndef V2X_MU
#define V2X_MU ((MU_Type *)EXAMPLE_V2X_MU_BASE)
#endif

/* Number of poll iterations to wait for the V2X firmware to reach running state. */
#ifndef V2X_FW_STATE_RETRIES
#define V2X_FW_STATE_RETRIES (1000u)
#endif

/* AES key slot used to load the plain key (0..7). */
#define V2X_FCE_AES_KEY_SLOT (0u)

/*
 * NIST SP 800-38A F.2.1 AES-128-CBC known-answer vector (first block).
 */
static const uint8_t s_key[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
};

static const uint8_t s_iv[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
};

static const uint8_t s_plaintext[16] = {
    0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
};

static const uint8_t s_expectedCipher[16] = {
    0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* FCE DMA descriptors reference these buffers; keep them 64-bit aligned. */
SDK_ALIGN(static uint8_t s_cipher[16], 8u);
SDK_ALIGN(static uint8_t s_decrypted[16], 8u);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void V2X_FCE_DumpHex(const uint8_t *data, uint32_t len)
{
    for (uint32_t i = 0u; i < len; i++)
    {
        PRINTF("%02x", data[i]);
        if (((i + 1u) % 16u) == 0u)
        {
            PRINTF("\r\n");
        }
    }
    if ((len % 16u) != 0u)
    {
        PRINTF("\r\n");
    }
}

static bool V2X_FCE_Compare(const uint8_t *a, const uint8_t *b, uint32_t len)
{
    for (uint32_t i = 0u; i < len; i++)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }
    return true;
}

/*!
 * @brief Main function.
 */
int main(void)
{
    v2x_fce_handle_t handle = {0};
    v2x_fce_cbc_op_t op     = {0};
    status_t status;

    /* FCE_GET_INFO response buffer (firmware requires an open FCE session first). */
    SDK_ALIGN(static uint8_t s_info[16], 8u);

    /* Board and pin/clock initialization. */
    BOARD_InitHardware();

    PRINTF("\r\n*************** EdgeLock V2X/FCE AES-CBC demo ***************\r\n\r\n");

    /****************** Bring up V2X firmware *****************/
    /*
     * On i.MX943 the V2X firmware core is not auto-started. Mirror the base
     * V2X demo (and Linux v2x_early_init()): if the firmware is not already
     * running, ask the ELE to authenticate/release the V2X FW image and poll
     * until it reports a running state before talking to the V2X/FCE MU.
     */
    PRINTF("****************** Bring up V2X firmware *****************\r\n");
    {
        uint32_t v2xState = 0u;
        uint32_t poll;

        status = ELE_BaseAPI_GetV2xFwState(ELE_MU, &v2xState);
        if ((status == kStatus_Success) && ELE_BaseAPI_IsV2xFwRunning(v2xState))
        {
            /* V2X firmware is already running; nothing to do. */
        }
        else
        {
            status = ELE_BaseAPI_V2xFwAuthenticate(ELE_MU, EXAMPLE_V2X_FW_IMG_DDR_ADDR);
            if (status != kStatus_Success)
            {
                PRINTF("Failed to authenticate V2X firmware (0x%x).\r\n", (unsigned int)status);
                goto error;
            }

            for (poll = 0u; poll < V2X_FW_STATE_RETRIES; poll++)
            {
                status = ELE_BaseAPI_GetV2xFwState(ELE_MU, &v2xState);
                if (status != kStatus_Success)
                {
                    PRINTF("Failed to read V2X firmware state (0x%x).\r\n", (unsigned int)status);
                    goto error;
                }
                if (ELE_BaseAPI_IsV2xFwRunning(v2xState))
                {
                    break;
                }
            }

            if (!ELE_BaseAPI_IsV2xFwRunning(v2xState))
            {
                status = kStatus_Timeout;
                PRINTF("Timed out waiting for V2X firmware to start.\r\n");
                goto error;
            }
            status = kStatus_Success;
        }
    }
    PRINTF("V2X firmware is running.\r\n\r\n");

    /****************** Get the V2X firmware version ***************/
    PRINTF("****************** Get V2X FW version ********************\r\n");
    {
        v2x_fw_version_t version = {0};

        status = V2X_Base_GetFwVersion(V2X_MU, &version);
        if (status != kStatus_Success)
        {
            PRINTF("Get V2X FW version failed (0x%x).\r\n", (unsigned int)status);
            goto error;
        }
        PRINTF("V2X FW version: %u.%u.%u  commit 0x%08x\r\n\r\n", (unsigned int)version.major,
               (unsigned int)version.minor, (unsigned int)version.patch, (unsigned int)version.commitId);
    }

    /****************** Open the V2X/FCE service ********************/
    PRINTF("****************** Open V2X/FCE service ******************\r\n");
    status = V2X_FCE_Open(V2X_FCE_MU, &handle);
    if (status != kStatus_Success)
    {
        PRINTF("Open V2X/FCE service failed.\r\n");
        goto error;
    }
    PRINTF("Open V2X/FCE service successfully.\r\n\r\n");

    /****************** Query V2X/FCE info (after Open) ************/
    PRINTF("****************** Get V2X/FCE info **********************\r\n");
    status = V2X_FCE_GetInfo(&handle, s_info, sizeof(s_info));
    if (status != kStatus_Success)
    {
        PRINTF("Get V2X/FCE info failed.\r\n");
        goto close;
    }
    PRINTF("V2X/FCE info:\r\n");
    PRINTF("V2X/FCE API version : %u.%u.%u\r\n", (unsigned)s_info[2], (unsigned)s_info[1],
           (unsigned)s_info[0]);
    PRINTF("Number of FIFO entries : %u\r\n",
           (unsigned)((uint32_t)s_info[4] | ((uint32_t)s_info[5] << 8) | ((uint32_t)s_info[6] << 16) |
                      ((uint32_t)s_info[7] << 24)));
    PRINTF("\r\n");

    /****************** Load the plain AES key *********************/
    PRINTF("****************** Load AES plain key ********************\r\n");
    status = V2X_FCE_LoadAesPlainKey(&handle, V2X_FCE_AES_KEY_SLOT, s_key, sizeof(s_key));
    if (status != kStatus_Success)
    {
        PRINTF("Load AES plain key failed.\r\n");
        goto close;
    }
    PRINTF("Loaded AES key into slot %u.\r\n\r\n", (unsigned int)V2X_FCE_AES_KEY_SLOT);

    /****************** AES-CBC encrypt ***************************/
    PRINTF("****************** AES-128-CBC encrypt *******************\r\n");
    op.keySlot   = V2X_FCE_AES_KEY_SLOT;
    op.iv        = s_iv;
    op.ivSize    = sizeof(s_iv);
    op.input     = s_plaintext;
    op.inputSize = sizeof(s_plaintext);
    op.output    = s_cipher;

    status = V2X_FCE_AesCbcEncrypt(&handle, &op);
    if (status != kStatus_Success)
    {
        PRINTF("AES-CBC encrypt failed.\r\n");
        goto close;
    }
    PRINTF("Ciphertext:\r\n");
    V2X_FCE_DumpHex(s_cipher, sizeof(s_cipher));

    if (!V2X_FCE_Compare(s_cipher, s_expectedCipher, sizeof(s_cipher)))
    {
        PRINTF("Ciphertext mismatch against the NIST SP 800-38A KAT vector.\r\n");
        status = kStatus_Fail;
        goto close;
    }
    PRINTF("Ciphertext matches the NIST SP 800-38A KAT vector.\r\n\r\n");

    /****************** AES-CBC decrypt ***************************/
    PRINTF("****************** AES-128-CBC decrypt *******************\r\n");
    op.input     = s_cipher;
    op.inputSize = sizeof(s_cipher);
    op.output    = s_decrypted;

    status = V2X_FCE_AesCbcDecrypt(&handle, &op);
    if (status != kStatus_Success)
    {
        PRINTF("AES-CBC decrypt failed.\r\n");
        goto close;
    }
    PRINTF("Decrypted plaintext:\r\n");
    V2X_FCE_DumpHex(s_decrypted, sizeof(s_decrypted));

    if (!V2X_FCE_Compare(s_decrypted, s_plaintext, sizeof(s_decrypted)))
    {
        PRINTF("Decrypted plaintext does not match the original.\r\n");
        status = kStatus_Fail;
        goto close;
    }
    PRINTF("Decrypted plaintext matches the original.\r\n\r\n");

close:
    /****************** Close the V2X/FCE service ******************/
    PRINTF("****************** Close V2X/FCE service *****************\r\n");
    if (V2X_FCE_Close(&handle) != kStatus_Success)
    {
        PRINTF("Close V2X/FCE service failed.\r\n");
    }
    else
    {
        PRINTF("Close V2X/FCE service successfully.\r\n\r\n");
    }

error:
    if (status == kStatus_Success)
    {
        PRINTF("*************** EdgeLock V2X/FCE demo END ****************\r\n");
    }
    else
    {
        PRINTF("*************** EdgeLock V2X/FCE demo FAILED *************\r\n");
    }

    while (1)
    {
    }
}
