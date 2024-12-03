/*!
 * Copyright 2024 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file board_lp.h
 * \brief Collection of BOARD helpers dedicated to low power support
 *
 */

#ifndef _BOARD_LP_H_
#define _BOARD_LP_H_

/*!
 * @addtogroup FWK_Board_module
 * @{
 */
/*!
 * @addtogroup FWK_Board_LP
 * The FWK_Board_LP module
 *
 * FWK_Board_LP module provides APIs to manage low power state.
 * @{
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                Public macros                               */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                              Public prototypes                             */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Initialize board ressources related to low power, likely to register
 *        low power entry/exit callbacks for optional peripherals
 *
 */
void BOARD_LowPowerInit(void);

/*!
 * @}  end of FWK_Board_LP addtogroup
 */
/*!
 * @}  end of FWK_Board_module addtogroup
 */

#endif /* _BOARD_LP_H_ */
