/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_DEBUG_H_
#define _BOARD_DEBUG_H_

/*!
 * @addtogroup FWK_Board_module
 * The FWK_Board module
 *
 * FWK_Board module provides APIs to config platform functionalities and peripherals.
 * @{
 */
/*!
 * @addtogroup FWK_Board
 * The FWK_Board main module
 *
 * FWK_Board main module provides APIs to initialise hardware and manage serial consoles.
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * \brief Initializes SWO pins.
 *
 */
void BOARD_InitSWO(void);

/*!
 * \brief Deinitializes SWO pins.
 *
 * \details Prevents leakage in lowpower caused by SWO pins in pull down mode.
 *
 */
void BOARD_DeInitSWO(void);

/*!
 * @}  end of FWK_Board addtogroup
 */
/*!
 * @}  end of FWK_Board_module addtogroup
 */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
