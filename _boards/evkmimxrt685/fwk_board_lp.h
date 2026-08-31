/* -------------------------------------------------------------------------- */
/*                           Copyright 2026 NXP                               */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

#ifndef _FWK_BOARD_LP_H_
#define _FWK_BOARD_LP_H_

/*!
 * \brief Initialize board specific low power resources.
 *        Registers a power manager notification callback that saves/restores
 *        board-level pin configurations around deep sleep transitions.
 */
void BOARD_LowPowerInit(void);

#endif /* _FWK_BOARD_LP_H_ */
