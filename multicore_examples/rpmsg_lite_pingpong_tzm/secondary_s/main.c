/*
 * Copyright 2018-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if CONFIG_SECONDARY_CORE_USES_TZM == 1
#if (__ARM_FEATURE_CMSE & 1) == 0
#error "Need ARMv8-M security extensions"
#elif (__ARM_FEATURE_CMSE & 2) == 0
#error "Compile with --cmse"
#endif
#endif /* CONFIG_SECONDARY_CORE_USES_TZM == 1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpmsg_lite.h"
#if CONFIG_SECONDARY_CORE_USES_TZM == 1
#include "veneer_table.h"
#include "tzm_config.h"
#include "tzm_api.h"
#endif /* CONFIG_SECONDARY_CORE_USES_TZM == 1 */
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LOCAL_EPT_ADDR                (30U)
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

THE_MESSAGE volatile msg    = {0};
static uint32_t remote_addr = 0U;
#if CONFIG_SECONDARY_CORE_USES_TZM == 1
extern struct rpmsg_lite_instance *rpmsg_lite_instance_s;
#else
struct rpmsg_lite_instance *rpmsg_lite_instance_s = NULL;
#endif
struct rpmsg_lite_instance rpmsg_lite_ctxt_s = {0};

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
    volatile int32_t has_received                       = 0;
    struct rpmsg_lite_ept_static_context my_ept_context = {0};
    struct rpmsg_lite_endpoint *my_ept                  = NULL;

    /* Initialize standard SDK demo application pins */
    BOARD_InitHardware();

#ifdef MCMGR_USED
    uint32_t startupData;
    mcmgr_status_t status;

    /* Initialize MCMGR before calling its API */
    (void)MCMGR_Init();

    /* Get the startup data */
    do
    {
        status = MCMGR_GetStartupData(kMCMGR_Core0, &startupData);
    } while (status != kStatus_MCMGR_Success);

    rpmsg_lite_instance_s = rpmsg_lite_remote_init((void *)(char *)startupData, RPMSG_LITE_LINK_ID, RL_NO_FLAGS, &rpmsg_lite_ctxt_s);
    if (rpmsg_lite_instance_s == NULL)
    {
        goto cleanup;
    }

    /* Signal the other core we are ready by triggering the event and passing the APP_RPMSG_READY_EVENT_DATA */
    (void)MCMGR_TriggerEvent(kMCMGR_Core0, kMCMGR_RemoteApplicationEvent, APP_RPMSG_READY_EVENT_DATA);
#else
    rpmsg_lite_instance_s = rpmsg_lite_remote_init((void *)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS, &rpmsg_lite_ctxt_s);
    if (rpmsg_lite_instance_s == NULL)
    {
        goto cleanup;
    }
#endif /* MCMGR_USED */

    rpmsg_lite_wait_for_link_up(rpmsg_lite_instance_s, RL_BLOCK);

    my_ept = rpmsg_lite_create_ept(rpmsg_lite_instance_s, LOCAL_EPT_ADDR, my_ept_read_cb, (void *)&has_received, &my_ept_context);
    if (my_ept == NULL)
    {
        goto cleanup;
    }

#ifdef MCMGR_USED
    /* Signal the other core the endpoint has been created by triggering the event and passing the
     * APP_RPMSG_READY_EP_EVENT_DATA */
    (void)MCMGR_TriggerEvent(kMCMGR_Core0, kMCMGR_RemoteApplicationEvent, APP_RPMSG_EP_READY_EVENT_DATA);
#endif

#ifdef RPMSG_LITE_NS_USED
    (void)rpmsg_ns_announce(rpmsg_lite_instance_s, my_ept, RPMSG_LITE_NS_ANNOUNCE_STRING, (uint32_t)RL_NS_CREATE);
#endif /*RPMSG_LITE_NS_USED*/
    has_received = 0;

#if CONFIG_SECONDARY_CORE_USES_TZM == 1
    while (msg.DATA <= 50U)
#else
    while (msg.DATA <= 1050U)
#endif /* CONFIG_SECONDARY_CORE_USES_TZM == 1 */
    {
        if (1 == has_received)
        {
            has_received = 0;
            msg.DATA++;
            (void)rpmsg_lite_send(rpmsg_lite_instance_s, my_ept, remote_addr, (char *)&msg, sizeof(THE_MESSAGE), RL_DONT_BLOCK);
        }
    }

#if CONFIG_SECONDARY_CORE_USES_TZM == 1
    /* jump to the non-secure domain */
    TZM_JumpToNormalWorld(NON_SECURE_START);
    for (;;)
    {
        /* This point should never be reached */
    }
#endif /* CONFIG_SECONDARY_CORE_USES_TZM == 1 */
cleanup:
    if (my_ept)
    {
        (void)rpmsg_lite_destroy_ept(rpmsg_lite_instance_s, my_ept);
        my_ept = ((void *)0);
    }

    if (rpmsg_lite_instance_s)
    {
        (void)rpmsg_lite_deinit(rpmsg_lite_instance_s);
        rpmsg_lite_instance_s = ((void *)0);
    }

    msg.DATA = 0U;

    /* End of the example */
    for (;;)
    {
    }
}
