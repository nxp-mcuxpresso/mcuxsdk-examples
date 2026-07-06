/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
 * Include
 ************************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <zephyr_headers/debug/coredump.h>

#include "board_debug_utils.h"
#include "board_debug_coredump.h"
#include "board_dbg_logger.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Line prefix for every coredump line. This is the upstream Zephyr coredump
 * marker (COREDUMP_PREFIX_STR == "#CD:"). The leading '#' is outside the Base64
 * alphabet (A-Z a-z 0-9 + / =) and differs from the NBU HCI logger prefix '@',
 * so the host can separate coredump lines from HCI lines and console text. */
#define BOARD_DBG_COREDUMP_PREFIX_STR COREDUMP_PREFIX_STR

/* Frame markers, reusing the upstream strings so the host parser stays in sync
 * with the Zephyr logging backend ("#CD:BEGIN#" ... "#CD:END#"). */
#define BOARD_DBG_COREDUMP_BEGIN_STR COREDUMP_PREFIX_STR COREDUMP_BEGIN_STR
#define BOARD_DBG_COREDUMP_END_STR   COREDUMP_PREFIX_STR COREDUMP_END_STR

/* Number of raw dump bytes encoded per emitted Base64 line. Must be a multiple
 * of 3 so that, except for the very last line emitted by end(), each line is a
 * whole number of Base64 quanta and never contains '=' padding. 48 raw bytes
 * map to 64 Base64 chars per line. */
#define BOARD_DBG_COREDUMP_RAW_PER_LINE 48U

/* Line buffer: prefix + Base64 of one full raw line + CRLF + NUL terminator. */
#define BOARD_DBG_COREDUMP_LINE_BUF_SIZE                                          \
    (sizeof(BOARD_DBG_COREDUMP_PREFIX_STR) - 1U +                                \
     BOARD_DBG_BASE64_ENCODED_SIZE(BOARD_DBG_COREDUMP_RAW_PER_LINE) + 2U + 1U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void coredump_board_debug_backend_start(void);
static void coredump_board_debug_backend_end(void);
static void coredump_board_debug_backend_buffer_output(uint8_t *buf, size_t buflen);
static int  coredump_board_debug_backend_query(enum coredump_query_id query_id, void *arg);
static int  coredump_board_debug_backend_cmd(enum coredump_cmd_id cmd_id, void *arg);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Streaming Base64 state. The raw dump arrives in arbitrary-length chunks; we
 * buffer up to BOARD_DBG_COREDUMP_RAW_PER_LINE raw bytes and emit one Base64
 * line at a time. Any 0..(RAW_PER_LINE-1) trailing bytes are carried in
 * s_rawAccum across buffer_output() calls; the final partial group (0..2 bytes)
 * is flushed with '=' padding only once, by end(). */
static uint8_t  s_rawAccum[BOARD_DBG_COREDUMP_RAW_PER_LINE];
static uint32_t s_rawAccumLen = 0U;

/* Backend error latch, returned by COREDUMP_QUERY_GET_ERROR. */
static int s_coredumpError = 0;

/*******************************************************************************
 * Private functions
 ******************************************************************************/

/*
 * Write a string to the shared debug log engine using the IMMEDIATE (synchronous)
 * path. The dump runs in fault context with the scheduler frozen, so the engine
 * writes inline over its selected port; the engine is brought up at boot by
 * BOARD_DbgLoggerInit() so no port setup happens here. The engine is
 * port-agnostic: the output goes to whatever port its Kconfig choice selects.
 */
static void coredump_board_debug_write(const char *str, uint32_t len)
{
    if ((str != NULL) && (len != 0U))
    {
        if (BOARD_DbgLoggerLogImmediate((const uint8_t *)str, (uint16_t)len) < 0)
        {
            s_coredumpError = -EIO;
        }
    }
}

/*
 * Encode exactly raw_len raw bytes (raw_len must be a multiple of 3, except for
 * the single final group emitted by end()) and emit them as one framed line
 * "#CD:<base64>\r\n".
 */
static void coredump_board_debug_emit_line(const uint8_t *raw, uint32_t raw_len)
{
    char     line_buf[BOARD_DBG_COREDUMP_LINE_BUF_SIZE];
    uint32_t line_len = 0U;

    if (raw_len != 0U)
    {
        /* Prefix */
        (void)memcpy(&line_buf[line_len], BOARD_DBG_COREDUMP_PREFIX_STR,
                     sizeof(BOARD_DBG_COREDUMP_PREFIX_STR) - 1U);
        line_len += (uint32_t)(sizeof(BOARD_DBG_COREDUMP_PREFIX_STR) - 1U);

        /* Base64 payload */
        line_len += BOARD_DBG_Base64Encode(raw, raw_len, &line_buf[line_len]);

        /* CRLF terminator */
        line_buf[line_len] = '\r';
        line_len++;
        line_buf[line_len] = '\n';
        line_len++;

        coredump_board_debug_write(line_buf, line_len);
    }
}

/*******************************************************************************
 * Backend API
 ******************************************************************************/

static void coredump_board_debug_backend_start(void)
{
    s_coredumpError = 0;
    s_rawAccumLen   = 0U;

    /* Open the framed coredump block. */
    coredump_board_debug_write("\r\n" BOARD_DBG_COREDUMP_BEGIN_STR "\r\n",
                               (uint32_t)(2U + (sizeof(BOARD_DBG_COREDUMP_BEGIN_STR) - 1U) + 2U));
}

static void coredump_board_debug_backend_end(void)
{
    /* Flush the final 0..(RAW_PER_LINE-1) carried raw bytes. This is the only
     * place a non-multiple-of-3 group is encoded, so it is the only line that
     * may carry '=' padding. */
    if (s_rawAccumLen > 0U)
    {
        coredump_board_debug_emit_line(s_rawAccum, s_rawAccumLen);
        s_rawAccumLen = 0U;
    }

    /* Close the framed coredump block. */
    coredump_board_debug_write(BOARD_DBG_COREDUMP_END_STR "\r\n",
                               (uint32_t)((sizeof(BOARD_DBG_COREDUMP_END_STR) - 1U) + 2U));
}

static void coredump_board_debug_backend_buffer_output(uint8_t *buf, size_t buflen)
{
    const uint8_t *ptr       = buf;
    size_t         remaining = buflen;

    if ((buf != NULL) && (buflen != 0U))
    {
        /* Top up the accumulator first so we always emit full RAW_PER_LINE groups
         * (a multiple of 3) until the very end of the stream. */
        if (s_rawAccumLen > 0U)
        {
            uint32_t space = BOARD_DBG_COREDUMP_RAW_PER_LINE - s_rawAccumLen;
            uint32_t take  = (remaining < (size_t)space) ? (uint32_t)remaining : space;

            (void)memcpy(&s_rawAccum[s_rawAccumLen], ptr, take);
            s_rawAccumLen += take;
            ptr += take;
            remaining -= take;

            if (s_rawAccumLen == BOARD_DBG_COREDUMP_RAW_PER_LINE)
            {
                coredump_board_debug_emit_line(s_rawAccum, s_rawAccumLen);
                s_rawAccumLen = 0U;
            }
        }

        /* Emit as many full RAW_PER_LINE lines directly from the input as possible. */
        while (remaining >= (size_t)BOARD_DBG_COREDUMP_RAW_PER_LINE)
        {
            coredump_board_debug_emit_line(ptr, BOARD_DBG_COREDUMP_RAW_PER_LINE);
            ptr += BOARD_DBG_COREDUMP_RAW_PER_LINE;
            remaining -= BOARD_DBG_COREDUMP_RAW_PER_LINE;
        }

        /* Carry the trailing 0..(RAW_PER_LINE-1) bytes to the next call / end(). */
        if (remaining > 0U)
        {
            (void)memcpy(&s_rawAccum[0], ptr, remaining);
            s_rawAccumLen = (uint32_t)remaining;
        }
    }
}

static int coredump_board_debug_backend_query(enum coredump_query_id query_id, void *arg)
{
    int ret;

    (void)arg;

    switch (query_id)
    {
        case COREDUMP_QUERY_GET_ERROR:
        {
            ret = s_coredumpError;
            break;
        }
        /* This backend streams the dump live to the console and keeps no stored
         * copy, so stored-dump queries do not apply. */
        case COREDUMP_QUERY_HAS_STORED_DUMP:
        case COREDUMP_QUERY_GET_STORED_DUMP_SIZE:
        default:
        {
            ret = -ENOTSUP;
            break;
        }
    }

    return ret;
}

static int coredump_board_debug_backend_cmd(enum coredump_cmd_id cmd_id, void *arg)
{
    int ret;

    (void)arg;

    switch (cmd_id)
    {
        case COREDUMP_CMD_CLEAR_ERROR:
        {
            s_coredumpError = 0;
            ret             = 0;
            break;
        }
        /* No stored dump is kept by this streaming backend; verify/erase/copy/
         * invalidate of a stored dump are not supported. */
        case COREDUMP_CMD_VERIFY_STORED_DUMP:
        case COREDUMP_CMD_ERASE_STORED_DUMP:
        case COREDUMP_CMD_COPY_STORED_DUMP:
        case COREDUMP_CMD_INVALIDATE_STORED_DUMP:
        default:
        {
            ret = -ENOTSUP;
            break;
        }
    }

    return ret;
}

/*******************************************************************************
 * Public backend instance
 ******************************************************************************/

/* The Zephyr coredump dispatcher (coredump_core.c) binds this symbol when
 * CONFIG_DEBUG_COREDUMP_BACKEND_OTHER is selected. Exactly one BACKEND_OTHER
 * implementation is ever compiled/linked into a given build (this one is
 * selected by the wireless prjseg; the mflash one by the coredump component
 * Kconfig), so there is no duplicate-symbol conflict. */
struct coredump_backend_api coredump_backend_other = {
    .start         = coredump_board_debug_backend_start,
    .end           = coredump_board_debug_backend_end,
    .buffer_output = coredump_board_debug_backend_buffer_output,
    .query         = coredump_board_debug_backend_query,
    .cmd           = coredump_board_debug_backend_cmd,
};

/*******************************************************************************
 * EOF
 ******************************************************************************/
