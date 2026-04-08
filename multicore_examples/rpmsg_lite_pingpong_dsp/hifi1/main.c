/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <xtensa/config/core.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpmsg_lite.h"
#include "board.h"
#include "app.h"
#ifdef MCMGR_USED
#include "mcmgr.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_RPMSG_READY_EVENT_DATA    (1U)
#define APP_RPMSG_EP_READY_EVENT_DATA (2U)

#if defined(CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM) && (CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM == 1)
#define NO_MAX_DATA    199U
#define LOCAL_EPT_ADDR (31U)
#else
#define NO_MAX_DATA    299U
#define LOCAL_EPT_ADDR (42U)
#endif

typedef struct the_message
{
    uint32_t DATA;
} THE_MESSAGE, *THE_MESSAGE_PTR;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static THE_MESSAGE volatile msg = {0};
static uint32_t remote_addr     = 0U;

/* Internal functions */
static int32_t my_ept_read_cb(void *payload, uint32_t payload_len, uint32_t src, void *priv)
{
    int32_t *has_received = priv;

    if (payload_len <= sizeof(THE_MESSAGE))
    {
        (void)memcpy((void *)&msg, payload, payload_len);
        remote_addr   = src;
        *has_received = 1;
    }
    return RL_RELEASE;
}

/*!
 * @brief Main function
 */
int main(void)
{
    xos_start_main("main", 7, 0);

    volatile int32_t has_received                       = 0;
    struct rpmsg_lite_ept_static_context my_ept_context = {0};
    struct rpmsg_lite_endpoint *my_ept                  = NULL;
    struct rpmsg_lite_instance rpmsg_ctxt               = {0};
    struct rpmsg_lite_instance *my_rpmsg                = NULL;

    /* Initialize standard SDK demo application pins */
    BOARD_InitHardware();

    (void)PRINTF("RPMsg demo starts DSP core '%s'.\r\n", XCHAL_CORE_ID);

#ifdef MCMGR_USED
    uint32_t startupData;
    mcmgr_status_t status;

    /* Initialize MCMGR before calling its API */
    (void)MCMGR_Init();

    /* Get the startup data */
    do
    {
        status = MCMGR_GetStartupData(APP_CM_CORE, &startupData);
    } while (status != kStatus_MCMGR_Success);

    /* In case of MCMGR is used we will get shared mem address from secondary core in startup data. */
    my_rpmsg = rpmsg_lite_remote_init((void *)(char *)(platform_patova(startupData)), RPMSG_LITE_LINK_ID, RL_NO_FLAGS,
                                      &rpmsg_ctxt);
    if (my_rpmsg == NULL)
    {
        goto cleanup;
    }

    /* Signal the cm33 core1 we are ready by triggering the event and passing the APP_RPMSG_READY_EVENT_DATA */
    (void)MCMGR_TriggerEvent(APP_COMM_CORE, kMCMGR_RemoteApplicationEvent, APP_RPMSG_READY_EVENT_DATA);
#else
    /* Otherwise the shared mem address is declared in app.h file. */
    my_rpmsg = rpmsg_lite_remote_init((void *)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS, &rpmsg_ctxt);
    if (my_rpmsg == NULL)
    {
        goto cleanup;
    }
#endif /* MCMGR_USED */

    rpmsg_lite_wait_for_link_up(my_rpmsg, RL_BLOCK);

    my_ept = rpmsg_lite_create_ept(my_rpmsg, LOCAL_EPT_ADDR, my_ept_read_cb, (void *)&has_received, &my_ept_context);
    if (my_ept == NULL)
    {
        goto cleanup;
    }

#ifdef MCMGR_USED
    /* Signal the cm33 core1 the endpoint has been created by triggering the event and passing the
     * APP_RPMSG_READY_EP_EVENT_DATA */
    (void)MCMGR_TriggerEvent(APP_COMM_CORE, kMCMGR_RemoteApplicationEvent, APP_RPMSG_EP_READY_EVENT_DATA);
#endif /* MCMGR_USED */

#ifdef RPMSG_LITE_NS_USED
    (void)rpmsg_ns_announce(my_rpmsg, my_ept, RPMSG_LITE_NS_ANNOUNCE_STRING, (uint32_t)RL_NS_CREATE);
#endif /*RPMSG_LITE_NS_USED*/

    has_received = 0;

    while (true)
    {
        if (1 == has_received)
        {
            has_received = 0;
            msg.DATA++;
            (void)rpmsg_lite_send(my_rpmsg, my_ept, remote_addr, (char *)&msg, sizeof(THE_MESSAGE), RL_DONT_BLOCK);

            if (msg.DATA >= NO_MAX_DATA)
            {
                break;
            }
        }
    }

cleanup:
    if (my_ept)
    {
        (void)rpmsg_lite_destroy_ept(my_rpmsg, my_ept);
        my_ept = ((void *)0);
    }

    if (my_rpmsg)
    {
        (void)rpmsg_lite_deinit(my_rpmsg);
        my_rpmsg = ((void *)0);
    }

    msg.DATA = 0U;

    /* End of the example */
    for (;;)
    {
    }
}
