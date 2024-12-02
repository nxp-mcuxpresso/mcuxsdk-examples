/*
 * Copyright 2021-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "fsl_common.h"
#include "fsl_io_mux.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
    BOARD_InitQuadSpiFlashPins();
}

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {                                /*!< Function assigned for the core: Cortex-M33[cm33] */
   IO_MUX_SetPinMux(IO_MUX_FC3_USART_DATA);
   IO_MUX_SetPinMux(IO_MUX_FC0_USART_DATA);
}

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitFlexSPI0BPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitQuadSpiFlashPins(void)
{
    IO_MUX_SetPinMux(IO_MUX_QUAD_SPI_FLASH);
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
