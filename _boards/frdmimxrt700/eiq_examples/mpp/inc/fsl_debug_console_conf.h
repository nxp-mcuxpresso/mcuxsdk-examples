/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_DEBUG_CONSOLE_CONF_H_
#define _FSL_DEBUG_CONSOLE_CONF_H_

/****************Debug console configuration********************/

/*! @brief If Non-blocking mode is needed, please define it at project setting,
 * otherwise blocking mode is the default transfer mode.
 * Warning: If you want to use non-blocking transfer,please make sure the corresponding
 * IO is not occupied by DMA, otherwise there will be conflict.
 */
#ifndef DEBUGCONSOLE_TRANSFER_NON_BLOCKING
/* #define DEBUGCONSOLE_TRANSFER_NON_BLOCKING */
#endif

/*! @brief define the transmit buffer length which is used in non-blocking transfer,
 * the buffer is maintained by debug console driver.
 * Note: This value should not be 0.
 */
#define DEBUG_CONSOLE_TRANSMIT_BUFFER_LEN 512U

/*! @brief define the receive buffer length which is used in non-blocking transfer,
 * the buffer is maintained by debug console driver.
 * Note: This value should not be 0.
 */
#define DEBUG_CONSOLE_RECEIVE_BUFFER_LEN 1024U

/*! @brief Whether enable the reliable TX function
 * If the macro is defined, the TX will block until all data is sent out.
 */
#define DEBUG_CONSOLE_TX_RELIABLE_ENABLE 1U

#endif /* _FSL_DEBUG_CONSOLE_CONF_H_ */
