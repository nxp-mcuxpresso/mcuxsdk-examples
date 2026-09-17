/*
 * Copyright 2018 - 2021, 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stdint.h"
#include <string.h>
#include <arm_cmse.h>
#include "tzm_api.h"
#include "rpmsg_lite.h"
#include "veneer_table.h"
#include "fsl_debug_console.h"

/* Enforce minimum GCC version for CMSE security checks.
 * GCC 10.0-10.2 has a known bug where cmse_check_address_range() and
 * cmse_check_pointed_object() always return NULL (GCC Bugzilla #99157).
 * The non-secure pointer validation in this file depends on these functions
 * being correct, so refuse to build on an affected compiler rather than
 * silently skipping the checks. Solved in GCC 10.3. */
#if defined(__GNUC__) && (__GNUC__ == 10) && (__GNUC_MINOR__ < 3)
#error "GCC 10.0-10.2 is not supported for this file due to GCC Bugzilla #99157 \
(cmse_check_address_range() always fails). Please upgrade to GCC 10.3 or later."
#endif

struct rpmsg_lite_instance *rpmsg_lite_instance_s;
#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
struct rpmsg_lite_ept_static_context ept_context_s;
#endif

/* Secure-world copies of the NS endpoint callback descriptor and its nested
 * data struct. rpmsg_lite_create_ept() retains the descriptor pointer and
 * ept_read_cb_s() uses it asynchronously at receive time, so the copy must have
 * static (not stack) lifetime. After validation, all secure-side dereferences
 * go through these secure copies and never through the original NS pointer, so
 * the non-secure world cannot repoint rx_cb_data/app_buffer between the
 * cmse_check_address_range() check and its use (anti-TOCTOU).
 * Note: a single descriptor pair supports one NS endpoint at a time, which
 * matches this example (and the single ept_context_s above). */
static struct rpmsg_lite_endpoint_callback_descr_ns      ept_callback_descr_s;
static struct rpmsg_lite_endpoint_callback_data_descr_ns ept_callback_data_s;

/* This is the endpoint receive callback that is executed in the secure domain. It ensures data is copied from secure
   shared memory to the non-secure memory and then it calls the application user-defined non-secure callback.  */
static int32_t ept_read_cb_s(void *payload, uint32_t payload_len, uint32_t src, void *priv)
{
    /* priv points at the secure descriptor copy (ept_callback_descr_s) that was
       populated and validated in rpmsg_lite_create_ept_nse(), so every field
       read below comes from secure memory, not from the NS pointer. */
    struct rpmsg_lite_endpoint_callback_descr_ns *callback_descr = priv;

    /* app_buffer is a non-secure pointer value held in the secure copy. Its
       value is trusted (snapshotted) but the accessed span depends on the
       runtime payload_len, so re-validate the exact [app_buffer, +payload_len)
       range is in non-secure memory before the secure world writes into it. */
    void *app_buffer = callback_descr->rx_cb_data->app_buffer;
    if (cmse_check_address_range(app_buffer, payload_len, CMSE_NONSECURE | CMSE_MPU_READWRITE) == NULL)
    {
        PRINTF("ept_read_cb_s: app_buffer not located in normal world!\r\n");
        for (;;)
            ;
    }

    env_memcpy(app_buffer, payload, payload_len);

    /* Define non-secure callback function pointer with cleared LSB */
    rl_ept_rx_cb_ns_t rx_cb_ns = (rl_ept_rx_cb_ns_t)cmse_nsfptr_create(callback_descr->rx_cb);

    /* Calls the application user-defined non-secure callback. app_data is passed
       by value to the non-secure callback (which dereferences it in NS world);
       the secure world never dereferences it, so no range check is required. */
    return rx_cb_ns(app_buffer, payload_len, src, callback_descr->rx_cb_data->app_data);
}

/* strnlen function implementation for arm compiler */
#if defined(__arm__)
size_t strnlen(const char *s, size_t maxLength)
{
    size_t length = 0;
    while ((length < maxLength) && (*s))
    {
        s++;
        length++;
    }
    return length;
}
#endif

TZM_IS_NOSECURE_ENTRY struct rpmsg_lite_endpoint *rpmsg_lite_create_ept_nse(
    uint32_t addr, struct rpmsg_lite_endpoint_callback_descr_ns *ept_callback_descr)
{
    /* Validate the descriptor struct itself is fully located in non-secure memory. */
    if (cmse_check_address_range(ept_callback_descr, sizeof(*ept_callback_descr),
                                 CMSE_NONSECURE | CMSE_MPU_READ) == NULL)
    {
        PRINTF("ept_callback_descr is not located in normal world!\r\n");
        for (;;)
            ;
    }

    /* Snapshot the descriptor into secure memory immediately after validation so
       the non-secure world cannot modify its fields between check and use. All
       subsequent accesses use the secure copy only (anti-TOCTOU). */
    ept_callback_descr_s = *ept_callback_descr;

    /* Validate the nested rx_cb_data pointer (taken from the secure snapshot).
       rx_cb is a non-secure function pointer that is later sanitized with
       cmse_nsfptr_create() before it is called. */
    if (cmse_check_address_range(ept_callback_descr_s.rx_cb_data, sizeof(*ept_callback_descr_s.rx_cb_data),
                                 CMSE_NONSECURE | CMSE_MPU_READ) == NULL)
    {
        PRINTF("ept_callback_descr->rx_cb_data is not located in normal world!\r\n");
        for (;;)
            ;
    }

    /* Snapshot the nested data struct into secure memory as well and re-point the
       secure descriptor at the secure data copy, so app_buffer/app_data are read
       from secure memory at receive time rather than re-read from NS memory. */
    ept_callback_data_s             = *ept_callback_descr_s.rx_cb_data;
    ept_callback_descr_s.rx_cb_data = &ept_callback_data_s;

#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
    return rpmsg_lite_create_ept(rpmsg_lite_instance_s, addr, ept_read_cb_s, &ept_callback_descr_s, &ept_context_s);
#else
    return rpmsg_lite_create_ept(rpmsg_lite_instance_s, addr, ept_read_cb_s, &ept_callback_descr_s);
#endif /* RL_USE_STATIC_API */
}

TZM_IS_NOSECURE_ENTRY int32_t rpmsg_lite_destroy_ept_nse(struct rpmsg_lite_endpoint *rl_ept)
{
    /* rl_ept is a secure-world-owned handle; do not validate with CMSE_NONSECURE. */
    return rpmsg_lite_destroy_ept(rpmsg_lite_instance_s, rl_ept);
}

TZM_IS_NOSECURE_ENTRY int32_t rpmsg_lite_send_nse(struct rpmsg_lite_endpoint *ept,
                                                  struct rpmsg_lite_send_params_ns *message_params,
                                                  uint32_t timeout)
{
    struct rpmsg_lite_send_params_ns local_params;

    /* 1. Validate the parameter struct itself is fully in non-secure memory. */
    if (cmse_check_address_range(message_params, sizeof(*message_params), CMSE_NONSECURE | CMSE_MPU_READ) == NULL)
    {
        PRINTF("message_params is not located in normal world!\r\n");
        for (;;)
            ;
    }

    /* 2. Snapshot fields to prevent TOCTOU attacks. */
    local_params = *message_params;

    /* 3. ept is a secure-world-owned handle; do not validate with CMSE_NONSECURE. */

    /* 4. Bound size and validate the full payload range is in non-secure memory. */
    if (local_params.size > (uint32_t)RL_BUFFER_PAYLOAD_SIZE)
    {
        for (;;)
            ;
    }
    if ((local_params.data == RL_NULL) ||
        (cmse_check_address_range(local_params.data, local_params.size, CMSE_NONSECURE | CMSE_MPU_READ) == NULL))
    {
        PRINTF("message_params->data is not located in normal world!\r\n");
        for (;;)
            ;
    }

    return rpmsg_lite_send(rpmsg_lite_instance_s, ept, local_params.dst, local_params.data, local_params.size,
                           timeout);
}

TZM_IS_NOSECURE_ENTRY void DbgConsole_Printf_NSE(char const *s, size_t length)
{
    char secure_buf[MAX_STRING_LENGTH + 1] = {'\0'};
    size_t verified_length                 = 0U;

    /* 1. Validate the caller-supplied length BEFORE any dereference of the
     *    non-secure pointer. This avoids reading attacker-controlled memory
     *    (e.g. via strnlen) before it has been range-checked. */
    if ((length == 0U) || (length > (size_t)MAX_STRING_LENGTH))
    {
        PRINTF("Input data error: Invalid string length!\r\n");
        for (;;)
            ;
    }

    /* 2. Verify that the entire string, including the null terminator, resides
     *    in non-secure memory. length + 1U covers the terminator. */
    if (cmse_check_address_range((void *)s, length + 1U, CMSE_NONSECURE | CMSE_MPU_READ) == NULL)
    {
        PRINTF("Input data error: String is not located in normal world!\r\n");
        for (;;)
            ;
    }

    /* 3. Copy into a secure buffer IMMEDIATELY after validation. All subsequent
     *    operations use only the immutable secure copy, so the non-secure world
     *    cannot tamper with the content afterwards (anti-TOCTOU). */
    memcpy(secure_buf, s, length);
    secure_buf[length] = '\0';

    /* 4. Verify the secure copy is properly terminated and that its length
     *    matches what the caller claimed. */
    verified_length = strnlen(secure_buf, length);
    if (verified_length != length)
    {
        PRINTF("Input data error: String too long or invalid string termination!\r\n");
        for (;;)
            ;
    }

    /* 5. Safe to print: operate on the validated secure copy only and never pass
     *    an untrusted non-secure string as the format argument. */
    PRINTF("%s", secure_buf);
}
