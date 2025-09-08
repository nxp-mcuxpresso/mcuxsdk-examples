/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpmsg_lite.h"
#include "veneer_table.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define REMOTE_EPT_ADDR   (41U)
#define LOCAL_NS_EPT_ADDR (31U)

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

/* This is the read callback for the non-secure endpoint */
static int32_t my_ept_read_cb_ns(void *payload, int32_t payload_len, uint32_t src, void *priv)
{
    int32_t *has_received = priv;

    if (payload_len <= sizeof(THE_MESSAGE))
    {
        *has_received = 1;
    }
    return RL_RELEASE;
}

/*!
 * @brief Main function
 */
int main(void)
{
    volatile int32_t has_received;
    struct rpmsg_lite_endpoint *my_ept_ns;
    struct rpmsg_lite_endpoint_callback_data_descr_ns callback_data_ns = {(void *)&has_received, (void *)&msg};
    struct rpmsg_lite_endpoint_callback_descr_ns callback_descr_ns     = {(rl_ept_rx_cb_ns_t)my_ept_read_cb_ns,
                                                                          &callback_data_ns};
    /* Create non-secure endpoint */
    my_ept_ns = rpmsg_lite_create_ept_nse(LOCAL_NS_EPT_ADDR, &callback_descr_ns);

    has_received = 0;

    struct rpmsg_lite_send_params_ns msg_params = {REMOTE_EPT_ADDR, (char *)&msg, sizeof(THE_MESSAGE)};
    while (msg.DATA <= 1050U)
    {
        if (1 == has_received)
        {
            has_received = 0;
            msg.DATA++;
            (void)rpmsg_lite_send_nse(my_ept_ns, &msg_params, RL_DONT_BLOCK);
        }
    }

    rpmsg_lite_destroy_ept_nse(my_ept_ns);

    for (;;)
    {
    }
}
