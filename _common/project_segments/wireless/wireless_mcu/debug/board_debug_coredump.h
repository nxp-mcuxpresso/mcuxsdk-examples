/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_DEBUG_COREDUMP_H_
#define _BOARD_DEBUG_COREDUMP_H_

/*!
 * @addtogroup FWK_Board_module
 * @{
 */
/*!
 * @addtogroup FWK_Board
 * @{
 */

/*
 * This is the wireless Board Debug Layer implementation of the Zephyr coredump
 * "other" backend (struct coredump_backend_other). Instead of storing the dump
 * in flash, it streams the raw Zephyr coredump byte stream through the shared
 * debug log engine (board_dbg_logger), over whatever output port the engine's
 * Kconfig choice selects, as Base64-encoded text lines, framed so a host tool
 * can extract them from a mixed console capture:
 *
 *   #CD:BEGIN#
 *   #CD:<base64 chars>
 *   #CD:<base64 chars>
 *   ...
 *   #CD:END#
 *
 * The "#CD:" prefix is the upstream Zephyr coredump marker (COREDUMP_PREFIX_STR)
 * and "#" is outside the Base64 alphabet (A-Z a-z 0-9 + / =), so coredump lines
 * never collide with the NBU HCI logger lines (which use a "@" prefix) or with
 * human-readable console text.
 *
 * Base64 is streamed: the raw dump arrives in arbitrary-length chunks via
 * buffer_output(), and 0..2 leftover bytes are carried across chunk boundaries
 * so that "=" padding is only ever emitted once, by end(), at the very end of
 * the stream. The host therefore concatenates all "#CD:" payload lines (between
 * BEGIN# and END#) into a single Base64 string and decodes it once.
 *
 * This backend is invoked from the connectivity DBG fault handler, i.e. in
 * Cortex-M exception (HardFault) context with the RTOS scheduler frozen. It
 * therefore writes via the engine's IMMEDIATE (synchronous) path, which emits
 * inline in the caller context without waiting on a semaphore, so it is safe to
 * call with the scheduler frozen. The engine is brought up at boot by
 * BOARD_DbgLoggerInit(), so no port is opened from fault context.
 */

/*!
 * @}  end of FWK_Board addtogroup
 */
/*!
 * @}  end of FWK_Board_module addtogroup
 */

#endif /* _BOARD_DEBUG_COREDUMP_H_ */
