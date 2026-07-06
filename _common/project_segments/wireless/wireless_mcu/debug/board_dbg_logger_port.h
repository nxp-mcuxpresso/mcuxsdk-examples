/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_DBG_LOGGER_PORT_H_
#define _BOARD_DBG_LOGGER_PORT_H_

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * \brief Initialize the log output port.
 *
 * \return 0 Success, negative for failure.
 */
int BOARD_DbgLoggerPortInit(void);

/*!
 * \brief Write data to the log output port.
 *
 * \param[in] data pointer to data buffer
 * \param[in] len  length of data
 *
 * \return 0 Success, negative for failure.
 */
int BOARD_DbgLoggerPortWrite(const uint8_t *data, uint16_t len);

/*!
 * \brief Reinitialize the log output port (needed on power down exit).
 *
 * \return 0 Success, negative for failure.
 */
int BOARD_DbgLoggerPortReinit(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_DBG_LOGGER_PORT_H_ */
