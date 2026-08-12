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

/* V2X/FCE message unit base used by this demo.
 * app.h (board layer) is expected to provide V2X_FCE_MU. If the board header does
 * not define it yet, define V2X_FCE_MU in your app.h to the V2X/FCE MU instance.
 * On i.MX943 the Linux DTS places this MU (v2x_mu2) at 0x47320000. */
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

/* AES key slot used to load the plain AES-256 key (0..7). */
#define V2X_FCE_KEY_SLOT (0u)

/*
 * NIST SP 800-38D / GCM AES-256 known-answer test vector.
 * (gcmEncryptExtIV256, [Keylen=256, IVlen=96, PTlen=128, AADlen=128, Taglen=128])
 */
static const uint8_t s_key[32] = {
    0x92, 0xe1, 0x1d, 0xcd, 0xaa, 0x86, 0x6f, 0x5c, 0xe7, 0x90, 0xfd, 0x24, 0x50, 0x1f, 0x92, 0x50,
    0x9a, 0xac, 0xf4, 0xcb, 0x8b, 0x13, 0x39, 0xd5, 0x0c, 0x9c, 0x12, 0x40, 0x93, 0x5d, 0xd0, 0x8b,
};

SDK_ALIGN(static const uint8_t s_iv[12], 8u) = {
    0xac, 0x93, 0xa1, 0xa6, 0x14, 0x52, 0x99, 0xbd, 0xe9, 0x02, 0xf2, 0x1a,
};

SDK_ALIGN(static const uint8_t s_aad[16], 8u) = {
    0x1e, 0x08, 0x89, 0x01, 0x6f, 0x67, 0x60, 0x1c, 0x8e, 0xbe, 0xa4, 0x94, 0x3b, 0xc2, 0x3a, 0xd6,
};

SDK_ALIGN(static const uint8_t s_plain[16], 8u) = {
    0x2d, 0x71, 0xbc, 0xfa, 0x91, 0x4e, 0x4a, 0xc0, 0x45, 0xb2, 0xaa, 0x60, 0x95, 0x5f, 0xad, 0x24,
};

static const uint8_t s_expectedCipher[16] = {
    0x89, 0x95, 0xae, 0x2e, 0x6d, 0xf3, 0xdb, 0xf9, 0x6f, 0xac, 0x7b, 0x71, 0x37, 0xba, 0xe6, 0x7f,
};

static const uint8_t s_expectedTag[16] = {
    0xec, 0xa5, 0xaa, 0x77, 0xd5, 0x1d, 0x4a, 0x0a, 0x14, 0xd9, 0xc5, 0x1e, 0x1d, 0xa4, 0x74, 0xab,
};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* V2X/FCE DMA descriptors reference these buffers; keep them 64-bit aligned. */
SDK_ALIGN(static uint8_t s_cipher[sizeof(s_plain)], 8u);
SDK_ALIGN(static uint8_t s_tag[16], 8u);
SDK_ALIGN(static uint8_t s_decrypted[sizeof(s_plain)], 8u);
SDK_ALIGN(static uint8_t s_info[64], 8u);

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
    v2x_fce_gcm_op_t op      = {0};
    status_t status;

    /* Board and pin/clock initialization. */
    BOARD_InitHardware();

    PRINTF("\r\n*************** EdgeLock V2X/FCE AES-GCM demo **********\r\n\r\n");

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
    /*
     * Mirror the base V2X demo: read and print the V2X firmware version over
     * the V2X debug MU. This confirms the firmware brought up above is alive
     * before exercising the FCE crypto engine.
     */
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
    PRINTF("****************** Ping V2X/FCE service ****************\r\n");
    status = V2X_FCE_Ping(V2X_FCE_MU);
    if (status != kStatus_Success)
    {
        PRINTF("Ping V2X/FCE service failed (status=%d).\r\n", (int)status);
    }
    else
    {
        PRINTF("Ping V2X/FCE service successfully.\r\n");
    }

    PRINTF("****************** Open V2X/FCE service ****************\r\n");
    status = V2X_FCE_Open(V2X_FCE_MU, &handle);
    if (status != kStatus_Success)
    {
        PRINTF("Open V2X/FCE service failed.\r\n");
        goto error;
    }
    PRINTF("Open V2X/FCE service successfully.\r\n\r\n");

    /****************** Query FCE service info *****************/
    PRINTF("****************** Get V2X/FCE info *******************\r\n");
    status    = V2X_FCE_GetInfo(&handle, s_info, sizeof(s_info));
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


    /****************** Load the plain AES-256 key *******************/
    /* Mirror the reference test_aes_keyslot(): exercise every AES key slot
     * by loading the plain key into each slot in turn, not just one slot. */
    PRINTF("****************** Load AES-256 plain key ****************\r\n");
    for (uint8_t slot = 0u; slot < V2X_FCE_AES_KEY_SLOTS; slot++)
    {
        status = V2X_FCE_LoadAesPlainKey(&handle, slot, s_key, sizeof(s_key));
        if (status != kStatus_Success)
        {
            PRINTF("Load AES plain key into slot %u failed.\r\n", (unsigned int)slot);
            goto close;
        }
        PRINTF("Loaded AES-256 key into slot %u.\r\n", (unsigned int)slot);
    }
    PRINTF("\r\n");

    /****************** AES-GCM authenticated encrypt ****************/
    PRINTF("****************** AES-GCM encrypt ***********************\r\n");
    op.keySlot   = V2X_FCE_KEY_SLOT;
    op.iv        = s_iv;
    op.ivSize    = sizeof(s_iv);
    op.aad       = s_aad;
    op.aadSize   = sizeof(s_aad);
    op.input     = s_plain;
    op.inputSize = sizeof(s_plain);
    op.output    = s_cipher;
    op.tag       = s_tag;
    op.tagSize   = sizeof(s_tag);

    status = V2X_FCE_AesGcmEncrypt(&handle, &op);
    if (status != kStatus_Success)
    {
        PRINTF("AES-GCM encrypt failed (status=%d).\r\n", (int)status);
        goto close;
    }
    PRINTF("Ciphertext:\r\n");
    V2X_FCE_DumpHex(s_cipher, sizeof(s_cipher));
    PRINTF("Tag:\r\n");
    V2X_FCE_DumpHex(s_tag, sizeof(s_tag));

    if (!V2X_FCE_Compare(s_cipher, s_expectedCipher, sizeof(s_cipher)) ||
        !V2X_FCE_Compare(s_tag, s_expectedTag, sizeof(s_tag)))
    {
        PRINTF("Ciphertext/tag mismatch against the NIST KAT vector.\r\n");
        status = kStatus_Fail;
        goto close;
    }
    PRINTF("Ciphertext and tag match the NIST KAT vector.\r\n\r\n");

    /****************** AES-GCM authenticated decrypt ****************/
    PRINTF("****************** AES-GCM decrypt ***********************\r\n");
    op.input     = s_cipher;
    op.inputSize = sizeof(s_cipher);
    op.output    = s_decrypted;
    op.tag       = s_tag;
    op.tagSize   = sizeof(s_tag);

    status = V2X_FCE_AesGcmDecrypt(&handle, &op);
    if (status != kStatus_Success)
    {
        PRINTF("AES-GCM decrypt/authentication failed.\r\n");
        goto close;
    }
    PRINTF("Decrypted plaintext:\r\n");
    V2X_FCE_DumpHex(s_decrypted, sizeof(s_decrypted));

    if (!V2X_FCE_Compare(s_decrypted, s_plain, sizeof(s_plain)))
    {
        PRINTF("Decrypted plaintext mismatch.\r\n");
        status = kStatus_Fail;
        goto close;
    }
    PRINTF("Decrypted plaintext matches the original.\r\n\r\n");

close:
    /****************** Close the V2X/FCE service ******************/
    PRINTF("****************** Close V2X/FCE service ***************\r\n");
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
        PRINTF("*************** EdgeLock V2X/FCE demo END **************\r\n");
    }
    else
    {
        PRINTF("*************** EdgeLock V2X/FCE demo FAILED ***********\r\n");
    }

    while (1)
    {
    }
}
