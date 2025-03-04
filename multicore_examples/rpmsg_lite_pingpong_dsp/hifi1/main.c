/*
 * Copyright 2025 NXP
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
#include "mcmgr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LOCAL_EPT_ADDR                (42U)
#define APP_RPMSG_READY_EVENT_DATA    (1U)
#define APP_RPMSG_EP_READY_EVENT_DATA (2U)

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
    /* Usually for Cortex the SystemInit() is called in startup.s file.
     * For DSP there is not such a file, so call it here.
     * This is required because it will call SystemInitHook() in which there is
     * prepared hook function for MCMGR to call MCMGR_EarlyInit() in hardware_init.c file.
     */
    SystemInit();

    xos_start_main("main", 7, 0);

    volatile int32_t has_received = 0;
    struct rpmsg_lite_ept_static_context my_ept_context;
    struct rpmsg_lite_endpoint *my_ept;
    struct rpmsg_lite_instance rpmsg_ctxt;
    struct rpmsg_lite_instance *my_rpmsg;

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
        status = MCMGR_GetStartupData(&startupData, APP_CM_CORE);
    } while (status != kStatus_MCMGR_Success);

    /* In case of MCMGR is used we will get shared mem address from secondary core in startup data. */
    my_rpmsg = rpmsg_lite_remote_init((void *)(char *)(platform_patova(startupData)), RPMSG_LITE_LINK_ID, RL_NO_FLAGS, &rpmsg_ctxt);

    /* Signal the cm33 core1 we are ready by triggering the event and passing the APP_RPMSG_READY_EVENT_DATA */
    (void)MCMGR_TriggerEvent(kMCMGR_RemoteApplicationEvent, APP_RPMSG_READY_EVENT_DATA, APP_CM_CORE);
#else
    /* Otherwise the shared mem address is declared in app.h file. */
    my_rpmsg = rpmsg_lite_remote_init((void *)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS, &rpmsg_ctxt);
#endif /* MCMGR_USED */

    rpmsg_lite_wait_for_link_up(my_rpmsg, RL_BLOCK);

    my_ept = rpmsg_lite_create_ept(my_rpmsg, LOCAL_EPT_ADDR, my_ept_read_cb, (void *)&has_received, &my_ept_context);

#ifdef MCMGR_USED
    /* Signal the cm33 core1 the endpoint has been created by triggering the event and passing the
     * APP_RPMSG_READY_EP_EVENT_DATA */
    (void)MCMGR_TriggerEvent(kMCMGR_RemoteApplicationEvent, APP_RPMSG_EP_READY_EVENT_DATA, APP_CM_CORE);
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

            if (msg.DATA >= 299U)
            {
                break;
            }
        }
    }

    (void)rpmsg_lite_destroy_ept(my_rpmsg, my_ept);
    my_ept = ((void *)0);
    (void)rpmsg_lite_deinit(my_rpmsg);
    msg.DATA = 0U;

    /* End of the example */
    for (;;)
    {
    }
}
