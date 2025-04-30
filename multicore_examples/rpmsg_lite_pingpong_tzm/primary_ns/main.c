/*
 * Copyright 2018-2020, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpmsg_lite.h"
#include "veneer_table.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PRINTF_NSE DbgConsole_Printf_NSE

#if CONFIG_SECONDARY_CORE_USES_TZM == 1
#define REMOTE_EPT_ADDR   (31U)
#else
#define REMOTE_EPT_ADDR   (30U)
#endif /* CONFIG_SECONDARY_CORE_USES_TZM == 1 */
#define LOCAL_NS_EPT_ADDR (41U)

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
THE_MESSAGE volatile msg = {0};

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
    volatile int32_t has_received                                      = 0;
    struct rpmsg_lite_endpoint *my_ept_ns                              = NULL;
    struct rpmsg_lite_endpoint_callback_data_descr_ns callback_data_ns = {(void *)&has_received, (void *)&msg};
    struct rpmsg_lite_endpoint_callback_descr_ns callback_descr_ns     = {(rl_ept_rx_cb_ns_t)my_ept_read_cb_ns,
                                                                          &callback_data_ns};
    bool error_occurred                                                = false;
    char str[50];
    struct rpmsg_lite_send_params_ns msg_params                        = {REMOTE_EPT_ADDR, (char *)&msg, sizeof(THE_MESSAGE)};

    /* Print the initial banner */
    PRINTF_NSE("\r\nNon-secure portion of the application started!\r\n");
    PRINTF_NSE("\r\nData exchange in non-secure domain\r\n");

    /* Create non-secure endpoint */
    my_ept_ns = rpmsg_lite_create_ept_nse(LOCAL_NS_EPT_ADDR, &callback_descr_ns);
    if (my_ept_ns == NULL)
    {
        PRINTF_NSE("Failed to create non-secure endpoint\r\n");
        error_occurred = true;
        goto cleanup;
    }

    has_received = 0;

    /* Send the first message from the non-secure domain to the remoteproc */
    msg.DATA                                    = 1000U;
    (void)rpmsg_lite_send_nse(my_ept_ns, &msg_params, RL_DONT_BLOCK);

    while (msg.DATA <= 1050U)
    {
        if (1 == has_received)
        {
            PRINTF_NSE("Primary core received a msg\r\n");
            int result = sprintf(str, "Message: DATA = %i\r\n", (int)msg.DATA);
            if (result < 0)
            {
                /* Handle error - use a default message */
                PRINTF_NSE("Error formatting message\r\n");
            }
            else
            {
                /* Use the formatted string as intended */
                PRINTF_NSE(str);
            }
            has_received = 0;
            msg.DATA++;
            (void)rpmsg_lite_send_nse(my_ept_ns, &msg_params, RL_DONT_BLOCK);
        }
    }

cleanup:
    if (my_ept_ns != NULL)
    {
        rpmsg_lite_destroy_ept_nse(my_ept_ns);
    }

    /* Print the ending banner */
    if (error_occurred)
    {
        PRINTF_NSE("\r\nRPMsg demo ends with errors\r\n");
    }
    else
    {
        PRINTF_NSE("\r\nRPMsg demo ends\r\n");
    }

    for (;;)
    {
    }
}
