/*!
 * Copyright 2024 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file pin_mux.c
 * \brief Collection of helpers to configure pinmux
 *
 */

#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_io_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    IO_MUX_SetPinMux(IO_MUX_FC3_USART_DATA);
}

void BOARD_InitPinButton0(void)
{
    IO_MUX_SetPinMux(IO_MUX_GPIO11);
}

void BOARD_InitPinLEDRGB(void)
{
    /* BLUE LED */
    IO_MUX_SetPinMux(IO_MUX_GPIO0);
    /* RED LED */
    IO_MUX_SetPinMux(IO_MUX_GPIO1);
    /* GREEN LED */
    IO_MUX_SetPinMux(IO_MUX_GPIO12);
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
