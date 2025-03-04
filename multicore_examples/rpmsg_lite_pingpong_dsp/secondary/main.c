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
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LOCAL_EPT_ADDR_CM             (30U)
#define LOCAL_EPT_ADDR_DSP            (32U)
#define REMOTE_EPT_ADDR_DSP           (42U)

#define APP_RPMSG_READY_EVENT_DATA    (1U)
#define APP_RPMSG_EP_READY_EVENT_DATA (2U)

typedef struct the_message
{
    uint32_t DATA;
} THE_MESSAGE, *THE_MESSAGE_PTR;

#define SH_MEM_TOTAL_SIZE (8192U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static THE_MESSAGE volatile cm_msg = {0};
static THE_MESSAGE volatile dsp_msg = {0};
static uint32_t cm_remote_addr     = 0U;

/* Internal functions */
static int32_t cm_ept_read_cb(void *payload, uint32_t payload_len, uint32_t src, void *priv)
{
    int32_t *has_received = priv;

    if (payload_len <= sizeof(THE_MESSAGE))
    {
        (void)memcpy((void *)&cm_msg, payload, payload_len);
        cm_remote_addr   = src;
        *has_received = 1;
    }
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
    (void)PRINTF("Secondary core received a dsp_msg. Message: Size=%x, DATA = %i\r\n", payload_len, dsp_msg.DATA);
    return RL_RELEASE;
}

static void RPMsgRemoteReadyEventHandler(uint16_t eventData, void *context, mcmgr_core_t coreNum)
{
    uint16_t *data = &((uint16_t *)context)[coreNum];

    *data = eventData;
}

/*!
 * @brief Main function
 */
int main(void)
{
    volatile int32_t cm_has_received = 0;
    volatile int32_t dsp_has_received = 0;
    volatile uint16_t RPMsgRemoteReadyEventData[APP_EVENT_SIZE] = {0};
    struct rpmsg_lite_ept_static_context cm_ept_context;
    struct rpmsg_lite_ept_static_context dsp_ept_context;
    struct rpmsg_lite_endpoint *cm_ept;
    struct rpmsg_lite_endpoint *dsp_ept;
    struct rpmsg_lite_instance cm_rpmsg_ctxt;
    struct rpmsg_lite_instance dsp_rpmsg_ctxt;
    struct rpmsg_lite_instance *cm_rpmsg;
    struct rpmsg_lite_instance *dsp_rpmsg;

    /* Initialize standard SDK demo application pins */
    BOARD_InitHardware();

    (void)PRINTF("RPMsg demo starts Secondary core.\r\n");

    uint32_t startupData;
    mcmgr_status_t status;

    /* Initialize MCMGR before calling its API */
    (void)MCMGR_Init();

    /* Register the application event before starting any core */
    (void)MCMGR_RegisterEvent(kMCMGR_RemoteApplicationEvent, RPMsgRemoteReadyEventHandler,
        (void *)RPMsgRemoteReadyEventData);

    /* Boot DSP Hifi1 core application */
    (void)MCMGR_StartCore(APP_DSP_CORE, (void *)(char *)NULL, (uint32_t)RPMSG_LITE_SHMEM_BASE_DSP,
                          kMCMGR_Start_Synchronous);

    /* Get the startup data from primary core */
    do
    {
        status = MCMGR_GetStartupData(&startupData, APP_CM_CORE);
    } while (status != kStatus_MCMGR_Success);

    /* We will get shared mem address from primary core in startup data. */
    cm_rpmsg = rpmsg_lite_remote_init((void *)(char *)(platform_patova(startupData)), RPMSG_LITE_LINK_ID_CM, RL_NO_FLAGS, &cm_rpmsg_ctxt);

    /* Signal the other core we are ready by triggering the event and passing the APP_RPMSG_READY_EVENT_DATA */
    (void)MCMGR_TriggerEvent(kMCMGR_RemoteApplicationEvent, APP_RPMSG_READY_EVENT_DATA, APP_CM_CORE);

    rpmsg_lite_wait_for_link_up(cm_rpmsg, RL_BLOCK);

    cm_ept = rpmsg_lite_create_ept(cm_rpmsg, LOCAL_EPT_ADDR_CM, cm_ept_read_cb, (void *)&cm_has_received, &cm_ept_context);

    /* Signal the other core the endpoint has been created by triggering the event and passing the
     * APP_RPMSG_READY_EP_EVENT_DATA */
    (void)MCMGR_TriggerEvent(kMCMGR_RemoteApplicationEvent, APP_RPMSG_EP_READY_EVENT_DATA, APP_CM_CORE);

#ifdef RPMSG_LITE_NS_USED
    (void)rpmsg_ns_announce(cm_rpmsg, cm_ept, RPMSG_LITE_NS_ANNOUNCE_STRING, (uint32_t)RL_NS_CREATE);
#endif /* RPMSG_LITE_NS_USED */
    cm_has_received = 0;

    while (true)
    {
        if (1 == cm_has_received)
        {
            cm_has_received = 0;

            cm_msg.DATA++;
            (void)rpmsg_lite_send(cm_rpmsg, cm_ept, cm_remote_addr, (char *)&cm_msg, sizeof(THE_MESSAGE), RL_DONT_BLOCK);

            if (cm_msg.DATA >= 99U)
            {
                break;
            }
        }
    }

    /* Wait until the DSP Hifi1 application signals the rpmsg remote has been initialized and is ready to
     * communicate. */
    while (APP_RPMSG_READY_EVENT_DATA != RPMsgRemoteReadyEventData[APP_DSP_CORE])
    {
    };

    dsp_rpmsg = rpmsg_lite_master_init(RPMSG_LITE_SHMEM_BASE_DSP, SH_MEM_TOTAL_SIZE, RPMSG_LITE_LINK_ID_DSP, RL_NO_FLAGS, &dsp_rpmsg_ctxt);

    dsp_ept = rpmsg_lite_create_ept(dsp_rpmsg, LOCAL_EPT_ADDR_DSP, dsp_ept_read_cb, (void *)&dsp_has_received, &dsp_ept_context);

    dsp_has_received = 0;

    /* Wait until the DSP Hifi1 application signals the rpmsg remote endpoint has been created. */
    while (APP_RPMSG_EP_READY_EVENT_DATA != RPMsgRemoteReadyEventData[APP_DSP_CORE])
    {
    };

    (void)PRINTF("Sending data to HIFI1 core...\r\n");
    /* Send the first message to the dsp core */
    dsp_msg.DATA = 200U;
    (void)PRINTF("Secondary core sending  a dsp_msg. Message: Size=%x, DATA = %i\r\n", sizeof(THE_MESSAGE), dsp_msg.DATA);
    (void)rpmsg_lite_send(dsp_rpmsg, dsp_ept, REMOTE_EPT_ADDR_DSP, (char *)&dsp_msg, sizeof(THE_MESSAGE), RL_DONT_BLOCK);

    while (true)
    {
        if (1 == dsp_has_received)
        {
            dsp_has_received = 0;
            if (dsp_msg.DATA >= 299U)
            {
                break;
            }

            dsp_msg.DATA++;
            (void)PRINTF("Secondary core sending  a dsp_msg. Message: Size=%x, DATA = %i\r\n", sizeof(THE_MESSAGE), dsp_msg.DATA);
            (void)rpmsg_lite_send(dsp_rpmsg, dsp_ept, REMOTE_EPT_ADDR_DSP, (char *)&dsp_msg, sizeof(THE_MESSAGE), RL_DONT_BLOCK);
        }
    }

    (void)rpmsg_lite_destroy_ept(cm_rpmsg, cm_ept);
    cm_ept = ((void *)0);
    (void)rpmsg_lite_deinit(cm_rpmsg);
    cm_msg.DATA = 0U;

    (void)rpmsg_lite_destroy_ept(dsp_rpmsg, dsp_ept);
    dsp_ept = ((void *)0);
    (void)rpmsg_lite_deinit(dsp_rpmsg);
    dsp_msg.DATA = 0U;

    /* Print the ending banner */
    (void)PRINTF("\r\nRPMsg demo ends\r\n");

    /* End of the example */
    for (;;)
    {
    }
}
