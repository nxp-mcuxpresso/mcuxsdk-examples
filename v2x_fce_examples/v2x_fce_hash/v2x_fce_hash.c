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
#include <string.h>
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

/*
 * NIST FIPS 180-4 SHA-256 one-block known-answer test: message "abc".
 */
static const uint8_t s_msg[3] = {
    0x61, 0x62, 0x63, /* "abc" */
};

static const uint8_t s_expectedDigest[32] = {
    0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23,
    0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c, 0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad,
};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* FCE DMA descriptors reference these buffers; keep them 64-bit aligned. */
SDK_ALIGN(static uint8_t s_digest[32], 8u);

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
    v2x_fce_hash_op_t op;
    status_t status;

    /*
     * shaAlgo is an enum, so a {0} initializer would mix an integer with an
     * enumerated type (IAR Pe188). Clear the request with memset instead.
     */
    (void)memset(&op, 0, sizeof(op));

    /* FCE_GET_INFO response buffer (firmware requires an open FCE session first). */
    SDK_ALIGN(static uint8_t s_info[16], 8u);

    /* Board and pin/clock initialization. */
    BOARD_InitHardware();

    PRINTF("\r\n*************** EdgeLock V2X/FCE HASH demo ***************\r\n\r\n");

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

    /****************** HASH generate *****************************/
    /* HASH is keyless: no key slot is loaded before the request. */
    PRINTF("****************** SHA-256 hash generate *****************\r\n");
    op.shaAlgo    = kV2X_FCE_Sha256;
    op.input      = s_msg;
    op.inputSize  = sizeof(s_msg);
    op.digest     = s_digest;
    op.digestSize = sizeof(s_digest);

    status = V2X_FCE_HashGenerate(&handle, &op);
    if (status != kStatus_Success)
    {
        PRINTF("HASH generate failed.\r\n");
        goto close;
    }
    PRINTF("Digest:\r\n");
    V2X_FCE_DumpHex(s_digest, sizeof(s_digest));

    if (!V2X_FCE_Compare(s_digest, s_expectedDigest, sizeof(s_digest)))
    {
        PRINTF("Digest mismatch against the FIPS 180-4 KAT vector.\r\n");
        status = kStatus_Fail;
        goto close;
    }
    PRINTF("Digest matches the FIPS 180-4 KAT vector.\r\n\r\n");

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
