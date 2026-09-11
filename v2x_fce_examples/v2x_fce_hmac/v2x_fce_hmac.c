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

/* HMAC key slot used to load the plain key (0..7). */
#define V2X_FCE_HMAC_KEY_SLOT (0u)

/*
 * RFC 4231 HMAC-SHA-256 Test Case 2 ("Jefe" / "what do ya want for nothing?").
 */
static const uint8_t s_key[4] = {
    0x4a, 0x65, 0x66, 0x65, /* "Jefe" */
};

static const uint8_t s_msg[28] = {
    0x77, 0x68, 0x61, 0x74, 0x20, 0x64, 0x6f, 0x20, 0x79, 0x61, 0x20, 0x77, 0x61, 0x6e,
    0x74, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x6e, 0x6f, 0x74, 0x68, 0x69, 0x6e, 0x67, 0x3f,
};

static const uint8_t s_expectedMac[32] = {
    0x5b, 0xdc, 0xc1, 0x46, 0xbf, 0x60, 0x75, 0x4e, 0x6a, 0x04, 0x24, 0x26, 0x08, 0x95, 0x75, 0xc7,
    0x5a, 0x00, 0x3f, 0x08, 0x9d, 0x27, 0x39, 0x83, 0x9d, 0xec, 0x58, 0xb9, 0x64, 0xec, 0x38, 0x43,
};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* FCE DMA descriptors reference these buffers; keep them 64-bit aligned. */
SDK_ALIGN(static uint8_t s_mac[32], 8u);

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
    v2x_fce_hmac_op_t op    = {0};
    status_t status;

    /* FCE_GET_INFO response buffer (firmware requires an open FCE session first). */
    SDK_ALIGN(static uint8_t s_info[16], 8u);

    /* Board and pin/clock initialization. */
    BOARD_InitHardware();

    PRINTF("\r\n*************** EdgeLock V2X/FCE HMAC demo ***************\r\n\r\n");

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

    /****************** Load the plain HMAC key *********************/
    PRINTF("****************** Load HMAC plain key *******************\r\n");
    status = V2X_FCE_LoadHmacPlainKey(&handle, V2X_FCE_HMAC_KEY_SLOT, kV2X_FCE_Sha256, s_key, sizeof(s_key));
    if (status != kStatus_Success)
    {
        PRINTF("Load HMAC plain key failed.\r\n");
        goto close;
    }
    PRINTF("Loaded HMAC key into slot %u.\r\n\r\n", (unsigned int)V2X_FCE_HMAC_KEY_SLOT);

    /****************** HMAC generate ******************************/
    PRINTF("****************** HMAC-SHA-256 generate *****************\r\n");
    op.keySlot    = V2X_FCE_HMAC_KEY_SLOT;
    op.shaAlgo    = kV2X_FCE_Sha256;
    op.input      = s_msg;
    op.inputSize  = sizeof(s_msg);
    op.digest     = s_mac;
    op.digestSize = sizeof(s_mac);

    status = V2X_FCE_HmacGenerate(&handle, &op);
    if (status != kStatus_Success)
    {
        PRINTF("HMAC generate failed.\r\n");
        goto close;
    }
    PRINTF("HMAC:\r\n");
    V2X_FCE_DumpHex(s_mac, sizeof(s_mac));

    if (!V2X_FCE_Compare(s_mac, s_expectedMac, sizeof(s_mac)))
    {
        PRINTF("HMAC mismatch against the RFC 4231 KAT vector.\r\n");
        status = kStatus_Fail;
        goto close;
    }
    PRINTF("HMAC matches the RFC 4231 KAT vector.\r\n\r\n");

    /****************** HMAC verify ********************************/
    PRINTF("****************** HMAC-SHA-256 verify *******************\r\n");
    op.digest     = s_mac;
    op.digestSize = sizeof(s_mac);

    status = V2X_FCE_HmacVerify(&handle, &op);
    if (status != kStatus_Success)
    {
        PRINTF("HMAC verify failed.\r\n");
        goto close;
    }
    PRINTF("HMAC verify successful.\r\n\r\n");

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
