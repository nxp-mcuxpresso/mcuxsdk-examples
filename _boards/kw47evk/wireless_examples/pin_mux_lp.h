/* -------------------------------------------------------------------------- */
/*                             Copyright 2024 NXP                             */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

#ifndef _PIN_MUX_LP_H_
#define _PIN_MUX_LP_H_

/*!
 * @addtogroup FWK_Pinmux_module
 * @{
 */
/*!
 * @addtogroup FWK_Pinmux_LP
 * The FWK_Pinmux_LP module
 *
 * FWK_Pinmux_LP module provides APIs to manage wakeup source muxing.
 * @{
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "fsl_pm_device.h"

/* -------------------------------------------------------------------------- */
/*                                Public macros                               */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Defines the wake up source for the button0
 *        Can be an external pin or an internal module mapped to WUU
 *        Based on supported wake up sources from SDK power manager (see fsl_pm_device.h)
 *        Set by default to GPIOD interrupt as SW2 button is mapped to PTD1 on EVK boards
 *
 */
#define BOARD_WAKEUP_SOURCE_BUTTON0 (uint32_t) PM_WSID_GPIOD_LOW

/*!
 * \brief Defines the wake up source for the button1
 *        Can be an external pin or an internal module mapped to WUU
 *        Based on supported wake up sources from SDK power manager (see fsl_pm_device.h)
 *        Set by default to PTC6 interrupt as SW3 button is mapped to PTC6 on EVK boards
 *
 */
#define BOARD_WAKEUP_SOURCE_BUTTON1 (uint32_t) PM_WSID_PTC6_FALLING_EDGE

/*!
 * @}  end of FWK_Pinmux_LP addtogroup
 */
/*!
 * @}  end of FWK_Pinmux_module addtogroup
 */

#endif /* _BOARD_LP_H_ */