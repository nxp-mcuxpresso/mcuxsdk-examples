/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_DEBUG_NBU_PORT_H_
#define _BOARD_DEBUG_NBU_PORT_H_

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * \brief Initialize HCI logger port
 *
 * \return 0 Success, otherwise failure
 */
int BOARD_DbgNbuPortInit(void);

/*!
 * \brief Write data to HCI logger port
 *
 * \param[in] data pointer to data buffer
 * \param[in] len length of data
 *
 * \return 0 Success, otherwise failure
 */
int BOARD_DbgNbuPortWrite(const uint8_t *data, uint16_t len);

/*!
 * \brief Reinitialize HCI logger port - Needed on power down exit
 *
 * \return 0 Success, otherwise failure
 */
int BOARD_DbgNbuPortReinit(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_DEBUG_NBU_PORT_H_ */
