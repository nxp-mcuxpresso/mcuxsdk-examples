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

#include "v2x_base.h" /* EdgeLock V2X base (debug) service */
#include "fsl_ele_base_api.h" /* EdgeLock enclave V2X FW bring-up */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* V2X message unit base used by this demo.
 * app.h (board layer) provides V2X_MU. */
#ifndef V2X_MU
#define V2X_MU ((MU_Type *)EXAMPLE_V2X_MU_BASE)
#endif

/* Number of start_rng retries while the TRNG initializes. */
#define V2X_START_RNG_RETRIES (1000u)

/* ELE MU base used only for the V2X firmware bring-up handshake.
 * app.h (board layer) provides ELE_MU / EXAMPLE_V2X_FW_IMG_DDR_ADDR. */
#ifndef ELE_MU
#define ELE_MU ((S3MU_Type *)EXAMPLE_ELE_MU_BASE)
#endif

/* Number of ELE get-state polls while the V2X firmware is authenticated. */
#define V2X_FW_STATE_RETRIES (1000u)

/* Debug switch: set to 1 (e.g. -DV2X_BASE_DEMO_DEBUG=1) to enable the
 * verbose V2X firmware bring-up trace prints. Default is quiet. */
#ifndef V2X_BASE_DEMO_DEBUG
#define V2X_BASE_DEMO_DEBUG (0)
#endif

#if V2X_BASE_DEMO_DEBUG
#define V2X_DBG(...) PRINTF(__VA_ARGS__)
#else
#define V2X_DBG(...) ((void)0)
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t s_dump[V2X_BASE_DBG_DUMP_MAX_WORDS];

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function.
 */
int main(void)
{
    v2x_fw_version_t version = {0};
    uint32_t dumpWords       = 0u;
    status_t status;
    uint32_t retries;

    /* Board and pin/clock initialization. */
    BOARD_InitHardware();

    PRINTF("\r\n*************** EdgeLock V2X base demo *******************\r\n\r\n");

    /****************** Bring up the V2X firmware via the ELE ********/
    /* On i.MX943 the V2X firmware core is not auto-started. The EdgeLock
     * enclave must first authenticate/release it (ELE_V2X_FW_AUTH_REQ) and the
     * V2X FW must reach a running state (ELE_GET_STATE) before the V2X MU below
     * will answer. Mirrors Linux v2x_early_init(). */
    PRINTF("****************** Bring up V2X firmware *****************\r\n");
    /* Debug: expand ELE_BaseAPI_BringUpV2xFw() into its sub-steps so the
     * failing stage and the raw ELE state value are visible on the console. */
    V2X_DBG("[dbg] ELE_MU=0x%08x V2X_MU=0x%08x fwAddr=0x%08x\r\n",
           (unsigned int)(uint32_t)ELE_MU, (unsigned int)(uint32_t)V2X_MU,
           (unsigned int)EXAMPLE_V2X_FW_IMG_DDR_ADDR);
    {
        uint32_t v2xState = 0u;
        uint32_t poll;

        status = ELE_BaseAPI_GetV2xFwState(ELE_MU, &v2xState);
        V2X_DBG("[dbg] initial ELE GetState: status=0x%x state=0x%x running=%d\r\n",
               (unsigned int)status, (unsigned int)v2xState,
               (int)ELE_BaseAPI_IsV2xFwRunning(v2xState));

        if ((status == kStatus_Success) && ELE_BaseAPI_IsV2xFwRunning(v2xState))
        {
            V2X_DBG("[dbg] V2X FW already running; skip authenticate.\r\n");
        }
        else
        {
            status = ELE_BaseAPI_V2xFwAuthenticate(ELE_MU, EXAMPLE_V2X_FW_IMG_DDR_ADDR);
            V2X_DBG("[dbg] ELE AuthenticateFw: status=0x%x\r\n", (unsigned int)status);
            if (status != kStatus_Success)
            {
                PRINTF("V2X firmware bring-up failed at authenticate (0x%x).\r\n",
                       (unsigned int)status);
                goto error;
            }

            for (poll = 0u; poll < V2X_FW_STATE_RETRIES; poll++)
            {
                status = ELE_BaseAPI_GetV2xFwState(ELE_MU, &v2xState);
                if (status != kStatus_Success)
                {
                    V2X_DBG("[dbg] poll[%u] GetState failed: status=0x%x\r\n",
                           (unsigned int)poll, (unsigned int)status);
                    PRINTF("V2X firmware bring-up failed at state poll (0x%x).\r\n",
                           (unsigned int)status);
                    goto error;
                }
                if (ELE_BaseAPI_IsV2xFwRunning(v2xState))
                {
                    break;
                }
            }
            V2X_DBG("[dbg] after poll: iters=%u last_state=0x%x running=%d\r\n",
                   (unsigned int)poll, (unsigned int)v2xState,
                   (int)ELE_BaseAPI_IsV2xFwRunning(v2xState));
            if (!ELE_BaseAPI_IsV2xFwRunning(v2xState))
            {
                status = kStatus_Timeout;
                PRINTF("V2X firmware bring-up timed out (0x%x).\r\n",
                       (unsigned int)status);
                goto error;
            }
            status = kStatus_Success;
        }
    }
    PRINTF("V2X firmware is running.\r\n\r\n");

    /****************** Get the V2X firmware version *****************/
    PRINTF("****************** Get V2X FW version ********************\r\n");
    status = V2X_Base_GetFwVersion(V2X_MU, &version);
    if (status != kStatus_Success)
    {
        PRINTF("Get V2X FW version failed (0x%x).\r\n", (unsigned int)status);
        goto error;
    }
    PRINTF("V2X FW version: %u.%u.%u  commit 0x%08x\r\n\r\n", (unsigned int)version.major,
           (unsigned int)version.minor, (unsigned int)version.patch, (unsigned int)version.commitId);

    /****************** Start the V2X TRNG **************************/
    PRINTF("****************** Start V2X TRNG ************************\r\n");
    for (retries = 0u; retries < V2X_START_RNG_RETRIES; retries++)
    {
        status = V2X_Base_StartRng(V2X_MU);
        if (status != kStatus_Busy)
        {
            break;
        }
    }
    if (status != kStatus_Success)
    {
        PRINTF("Start V2X TRNG failed (0x%x).\r\n", (unsigned int)status);
        goto error;
    }
    PRINTF("V2X TRNG started.\r\n\r\n");

    /****************** Set V2X power state on **********************/
    PRINTF("****************** Set V2X power state ON ****************\r\n");
    status = V2X_Base_SetPowerState(V2X_MU, kV2X_PowerOn);
    if (status != kStatus_Success)
    {
        PRINTF("Set V2X power state ON failed (0x%x).\r\n", (unsigned int)status);
        goto error;
    }
    PRINTF("V2X power state set to ON.\r\n\r\n");

    /****************** Read the V2X debug dump *********************/
    PRINTF("****************** V2X debug dump ************************\r\n");
    status = V2X_Base_DebugDump(V2X_MU, s_dump, V2X_BASE_DBG_DUMP_MAX_WORDS, &dumpWords);
    if (status != kStatus_Success)
    {
        PRINTF("V2X debug dump failed (0x%x).\r\n", (unsigned int)status);
        goto error;
    }
    PRINTF("V2X debug dump (%u words):\r\n", (unsigned int)dumpWords);
    for (uint32_t i = 0u; i < dumpWords; i++)
    {
        PRINTF("  [%02u] 0x%08x\r\n", (unsigned int)i, (unsigned int)s_dump[i]);
    }
    PRINTF("\r\n");

error:
    if (status == kStatus_Success)
    {
        PRINTF("*************** EdgeLock V2X base demo END ***************\r\n");
    }
    else
    {
        PRINTF("*************** EdgeLock V2X base demo FAILED ************\r\n");
    }

    while (1)
    {
    }
}
