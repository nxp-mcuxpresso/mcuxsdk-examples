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
 * in flash, it streams the raw Zephyr coredump byte stream to the application
 * console (the Serial Manager interface gSerMgrIf, the same port that carries
 * the application logs and the NBU HCI logger output) as Base64-encoded text
 * lines, framed so a host tool can extract them from a mixed console capture:
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
 * therefore writes via the Serial Manager blocking write, whose blocking mode
 * is a polling loop (it pumps the TX ISR / spins, it does not wait on a
 * semaphore), so it is safe to call with the scheduler frozen. The backend uses
 * its own Serial Manager write handle opened on gSerMgrIf; the Serial Manager
 * serializes writes across handles, so this does not conflict with the app or
 * NBU logger handles on the same interface.
 */

/*!
 * @}  end of FWK_Board addtogroup
 */
/*!
 * @}  end of FWK_Board_module addtogroup
 */

#endif /* _BOARD_DEBUG_COREDUMP_H_ */
