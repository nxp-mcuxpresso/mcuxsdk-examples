/* -------------------------------------------------------------------------- */
/*                            Copyright 2025 NXP                              */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

#ifndef _BOARD_EXTFLASH_H_
#define _BOARD_EXTFLASH_H_

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */
#include "fsl_nor_flash.h"
#include "fsl_spifi_nor_flash.h"

/* -------------------------------------------------------------------------- */
/*                              Public prototypes                             */
/* -------------------------------------------------------------------------- */

/**
 * @brief Initialize external flash module
 *
 */
void BOARD_InitExternalFlash(void);

/**
 * @brief Retrieve pointer on external flash configuration description.
 *
 */
nor_config_t * BOARD_GetExtFlashConfig(void);

#endif /* _BOARD_EXTFLASH_H_ */
