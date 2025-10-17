/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_DEBUG_NBU_H_
#define _BOARD_DEBUG_NBU_H_

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * @brief Initialize the NBU debug system
 *
 * This function initializes the NBU debug framework and registers
 * a system error notification callback. The callback will be invoked when an NBU fault
 * or system error is detected.
 *
 * @return 0 Success, otherwise failure
 *
 */
int BOARD_DbgNbuInit(void);

/**
 * @brief Process NBU debug state and handle fault detection
 *
 * This function should be called periodically (typically from the idle hook)
 * to check the NBU state and detect any fault/warning conditions.
 * When a fault/warning is detected, the previously registered system error callback will be invoked.
 *
 * @note If a fault/warning is detected, the registered callback will be executed during this call
 */
void BOARD_DbgNbuProcess(void);

/*!
 * @}  end of FWK_Board addtogroup
 */
/*!
 * @}  end of FWK_Board_module addtogroup
 */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_DEBUG_NBU_H_ */
