/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpmsg_lite.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "app.h"
#include "mcmgr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define REMOTE_EPT_ADDR_CM            (30U)
#define REMOTE_EPT_ADDR_DSP           (31U)
#define LOCAL_EPT_ADDR_CM             (40U)
#define LOCAL_EPT_ADDR_DSP            (41U)
#define APP_RPMSG_READY_EVENT_DATA    (1U)
#define APP_RPMSG_EP_READY_EVENT_DATA (2U)

typedef struct the_message
{
    uint32_t DATA;
} THE_MESSAGE, *THE_MESSAGE_PTR;

#define SH_MEM_TOTAL_SIZE (4096U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static THE_MESSAGE volatile cm_msg  = {0};
static THE_MESSAGE volatile dsp_msg = {0};

/* This is the read callback, note we are in a task context when this callback
is invoked, so kernel primitives can be used freely */
static int32_t cm_ept_read_cb(void *payload, uint32_t payload_len, uint32_t src, void *priv)
{
    int32_t *has_received = priv;

    if (payload_len <= sizeof(THE_MESSAGE))
    {
        (void)memcpy((void *)&cm_msg, payload, payload_len);
        *has_received = 1;
    }
    (void)PRINTF("Primary core received a cm_msg. Message: Size=%x, DATA = %i\r\n", payload_len, cm_msg.DATA);
    return RL_RELEASE;
}

static int32_t dsp_ept_read_cb(void *payload, uint32_t payload_len, uint32_t src, void *priv)
{
    int32_t *has_received = priv;

    if (payload_len <= sizeof(THE_MESSAGE))
    {
        (void)memcpy((void *)&dsp_msg, payload, payload_len);
        *has_received = 1;
    }
    (void)PRINTF("Primary core received a dsp_msg. Message: Size=%x, DATA = %i\r\n", payload_len, dsp_msg.DATA);
    return RL_RELEASE;
}

static void RPMsgRemoteReadyEventHandler(mcmgr_core_t coreNum, uint16_t eventData, void *context)
{
    uint16_t *data = &((uint16_t *)context)[coreNum];

    *data = eventData;
}

/*!
 * @brief Main function
 */
int main(void)
{
    volatile int32_t cm_has_received                            = 0;
    volatile int32_t dsp_has_received                           = 0;
    volatile uint16_t RPMsgRemoteReadyEventData[APP_EVENT_SIZE] = {0};
    struct rpmsg_lite_ept_static_context cm_ept_context         = {0};
    struct rpmsg_lite_endpoint *cm_ept                          = NULL;
    struct rpmsg_lite_instance cm_rpmsg_ctxt                    = {0};
    struct rpmsg_lite_instance *cm_rpmsg                        = NULL;

    struct rpmsg_lite_ept_static_context dsp_ept_context = {0};
    struct rpmsg_lite_endpoint *dsp_ept                  = NULL;
    struct rpmsg_lite_instance dsp_rpmsg_ctxt            = {0};
    struct rpmsg_lite_instance *dsp_rpmsg                = NULL;

    bool error_occurred = false;

    /* Initialize standard SDK demo application pins */
    BOARD_InitHardware();

    (void)PRINTF("RPMsg demo starts Primary core.\r\n");

#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* This section ensures the primary core` image is copied from flash location to the target RAM memory.
       It consists of several steps: image size calculation and image copying.
       These steps are not required on MCUXpresso IDE which copies the primary core` image to the target memory during
       startup automatically. */
    uint32_t core1_image_size;
    core1_image_size = get_core1_image_size();
    (void)PRINTF("Copy CORE1 image to address: 0x%x, size: %d\r\n", (void *)(char *)CORE1_BOOT_ADDRESS,
                 core1_image_size);

    /* Copy application from FLASH to RAM */
    (void)memcpy((void *)(char *)CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
    invalidate_cache_for_core1_image_memory(CORE1_BOOT_ADDRESS, core1_image_size);
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY */

#endif /* CORE1_IMAGE_COPY_TO_RAM */

    /* Initialize MCMGR before calling its API */
    (void)MCMGR_Init();

    /* Register the application event before starting any core */
    (void)MCMGR_RegisterEvent(kMCMGR_RemoteApplicationEvent, RPMsgRemoteReadyEventHandler,
                              (void *)RPMsgRemoteReadyEventData);

    /* Boot secondary core application */
    (void)MCMGR_StartCore(APP_CM_CORE, (void *)(char *)CORE1_BOOT_ADDRESS, (uint32_t)RPMSG_LITE_SHMEM_BASE_CM,
                          kMCMGR_Start_Synchronous);

    /* Boot DSP Hifi4 core application */
    (void)MCMGR_StartCore(APP_DSP_CORE, (void *)(char *)CORE1_BOOT_ADDRESS, (uint32_t)RPMSG_LITE_SHMEM_BASE_DSP,
                          kMCMGR_Start_Synchronous);

    /* Print the initial banner */
    (void)PRINTF("\r\nRPMsg demo starts\r\n");

    /* Wait until the secondary core application signals the rpmsg remote has been initialized and is ready to
     * communicate. */
    while (APP_RPMSG_READY_EVENT_DATA != RPMsgRemoteReadyEventData[APP_CM_CORE])
    {
    };

    cm_rpmsg = rpmsg_lite_master_init(RPMSG_LITE_SHMEM_BASE_CM, SH_MEM_TOTAL_SIZE, RPMSG_LITE_LINK_ID_CM, RL_NO_FLAGS,
                                      &cm_rpmsg_ctxt);
    if (cm_rpmsg == NULL)
    {
        (void)PRINTF("Failed to initialize CM rpmsg\r\n");
        error_occurred = true;
        goto cleanup;
    }

    cm_ept =
        rpmsg_lite_create_ept(cm_rpmsg, LOCAL_EPT_ADDR_CM, cm_ept_read_cb, (void *)&cm_has_received, &cm_ept_context);
    if (cm_ept == NULL)
    {
        (void)PRINTF("Failed to create CM endpoint...\r\n");
        error_occurred = true;
        goto cleanup;
    }

    cm_has_received = 0;

    /* Wait until the secondary core application signals the rpmsg remote endpoint has been created. */
    while (APP_RPMSG_EP_READY_EVENT_DATA != RPMsgRemoteReadyEventData[APP_CM_CORE])
    {
    };

    (void)PRINTF("Sending data to Secondary core...\r\n");
    /* Send the first message to the cm core */
    cm_msg.DATA = 0U;
    (void)PRINTF("Primary core sending  a cm_msg. Message: Size=%x, DATA = %i\r\n", sizeof(THE_MESSAGE), cm_msg.DATA);
    (void)rpmsg_lite_send(cm_rpmsg, cm_ept, REMOTE_EPT_ADDR_CM, (char *)&cm_msg, sizeof(THE_MESSAGE), RL_DONT_BLOCK);

    while (true)
    {
        if (1 == cm_has_received)
        {
            cm_has_received = 0;
            if (cm_msg.DATA >= 99U)
            {
                break;
            }

            cm_msg.DATA++;
            (void)PRINTF("Primary core sending  a cm_msg. Message: Size=%x, DATA = %i\r\n", sizeof(THE_MESSAGE),
                         cm_msg.DATA);
            (void)rpmsg_lite_send(cm_rpmsg, cm_ept, REMOTE_EPT_ADDR_CM, (char *)&cm_msg, sizeof(THE_MESSAGE),
                                  RL_DONT_BLOCK);
        }
    }

    /* Wait until the DSP Hifi4 application signals the rpmsg remote has been initialized and is ready to
     * communicate. */
    while (APP_RPMSG_READY_EVENT_DATA != RPMsgRemoteReadyEventData[APP_DSP_CORE])
    {
    };

    dsp_rpmsg = rpmsg_lite_master_init(RPMSG_LITE_SHMEM_BASE_DSP, SH_MEM_TOTAL_SIZE, RPMSG_LITE_LINK_ID_DSP,
                                       RL_NO_FLAGS, &dsp_rpmsg_ctxt);
    if (dsp_rpmsg == NULL)
    {
        (void)PRINTF("Failed to initialize DSP rpmsg\r\n");
        error_occurred = true;
        goto cleanup;
    }

    dsp_ept = rpmsg_lite_create_ept(dsp_rpmsg, LOCAL_EPT_ADDR_DSP, dsp_ept_read_cb, (void *)&dsp_has_received,
                                    &dsp_ept_context);
    if (dsp_ept == NULL)
    {
        (void)PRINTF("Failed to create DSP endpoint...\r\n");
        error_occurred = true;
        goto cleanup;
    }

    dsp_has_received = 0;

    /* Wait until the DSP Hifi4 application signals the rpmsg remote endpoint has been created. */
    while (APP_RPMSG_EP_READY_EVENT_DATA != RPMsgRemoteReadyEventData[APP_DSP_CORE])
    {
    };

    (void)PRINTF("Sending data to HIFI4 core...\r\n");
    /* Send the first message to the dsp core */
    dsp_msg.DATA = 100U;
    (void)PRINTF("Primary core sending  a dsp_msg. Message: Size=%x, DATA = %i\r\n", sizeof(THE_MESSAGE), dsp_msg.DATA);
    (void)rpmsg_lite_send(dsp_rpmsg, dsp_ept, REMOTE_EPT_ADDR_DSP, (char *)&dsp_msg, sizeof(THE_MESSAGE),
                          RL_DONT_BLOCK);

    while (true)
    {
        if (1 == dsp_has_received)
        {
            dsp_has_received = 0;
            if (dsp_msg.DATA >= 199U)
            {
                break;
            }

            dsp_msg.DATA++;
            (void)PRINTF("Primary core sending  a dsp_msg. Message: Size=%x, DATA = %i\r\n", sizeof(THE_MESSAGE),
                         dsp_msg.DATA);
            (void)rpmsg_lite_send(dsp_rpmsg, dsp_ept, REMOTE_EPT_ADDR_DSP, (char *)&dsp_msg, sizeof(THE_MESSAGE),
                                  RL_DONT_BLOCK);
        }
    }

cleanup:
    if (cm_ept != NULL)
    {
        (void)rpmsg_lite_destroy_ept(cm_rpmsg, cm_ept);
        cm_ept = NULL;
    }

    if (cm_rpmsg != NULL)
    {
        (void)rpmsg_lite_deinit(cm_rpmsg);
        cm_rpmsg = NULL;
    }

    cm_msg.DATA = 0U;

    if (dsp_ept != NULL)
    {
        (void)rpmsg_lite_destroy_ept(dsp_rpmsg, dsp_ept);
        dsp_ept = NULL;
    }

    if (dsp_rpmsg != NULL)
    {
        (void)rpmsg_lite_deinit(dsp_rpmsg);
        dsp_rpmsg = NULL;
    }

    dsp_msg.DATA = 0U;

    /* Print the ending banner */
    if (error_occurred)
    {
        (void)PRINTF("\r\nRPMsg demo ends with Errors\r\n");
    }
    else
    {
        (void)PRINTF("\r\nRPMsg demo ends\r\n");
    }

    for (;;)
    {
    }
}
