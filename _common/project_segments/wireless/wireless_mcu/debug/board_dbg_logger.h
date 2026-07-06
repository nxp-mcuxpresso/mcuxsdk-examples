/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_DBG_LOGGER_H_
#define _BOARD_DBG_LOGGER_H_

#include <stdint.h>

/*!
 * \file board_dbg_logger.h
 * \brief Reusable, content-agnostic debug log engine.
 *
 * The engine streams an opaque (buf, len) blob to a shared output port. It has
 * no knowledge of the blob content or how it was allocated. Producers (HCI
 * tracing, NBU coredump, future clients) serialize their own bytes and hand
 * them to the engine for a synchronous write.
 *
 * The write is immediate: it happens in the caller context and returns once the
 * bytes have been pushed to the port. The engine does no buffering, queueing or
 * locking; a producer that shares the port from more than one context must
 * serialize its own calls.
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * \brief Initialize the global log engine (idempotent, first-call-wins).
 *
 * Sets up the output port. A second call after the engine is initialized is a
 * no-op that returns 0.
 *
 * \return 0 on success, negative on error.
 */
int BOARD_DbgLoggerInit(void);

/*!
 * \brief Write an opaque blob synchronously in the caller context.
 *
 * \param[in] buf blob to write
 * \param[in] len number of bytes (must be > 0)
 *
 * \return 0 on success, negative on error.
 */
int BOARD_DbgLoggerLogImmediate(const uint8_t *buf, uint16_t len);

#if defined(__cplusplus)
}
#endif

#endif /* _BOARD_DBG_LOGGER_H_ */
