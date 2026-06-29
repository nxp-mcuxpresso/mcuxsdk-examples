/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_DEBUG_UTILS_H_
#define _BOARD_DEBUG_UTILS_H_

#include <stdint.h>

/*!
 * @addtogroup FWK_Board_module
 * @{
 */
/*!
 * @addtogroup FWK_Board
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * \brief Number of Base64 characters produced for n input bytes (RFC 4648,
 *        4 output chars per 3 input bytes, rounded up, with '=' padding).
 *
 * Does not include any line prefix or terminator the caller may add.
 */
#define BOARD_DBG_BASE64_ENCODED_SIZE(n) ((((n) + 2U) / 3U) * 4U)

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * \brief Encode a byte buffer into standard Base64 (RFC 4648, with '=' padding).
 *
 * \param[in]  src     pointer to the input bytes
 * \param[in]  src_len number of input bytes
 * \param[out] dst     output buffer, must hold at least
 *                     BOARD_DBG_BASE64_ENCODED_SIZE(src_len) chars
 *
 * \return number of Base64 characters written to dst
 */
uint32_t BOARD_DBG_Base64Encode(const uint8_t *src, uint32_t src_len, char *dst);

#if defined(__cplusplus)
}
#endif

/*!
 * @}  end of FWK_Board addtogroup
 */
/*!
 * @}  end of FWK_Board_module addtogroup
 */

#endif /* _BOARD_DEBUG_UTILS_H_ */
