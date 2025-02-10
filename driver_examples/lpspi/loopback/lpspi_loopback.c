/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TRANSFER_SIZE     (512U)    /*! Transfer dataSize.*/
#define TRANSFER_BAUDRATE (500000U) /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/
void EXAMPLE_LPSPI_IRQHANDLER(void)
{
    LPSPI_MasterTransferHandleIRQ(EXMPALE_LPSPI_BASE, &g_sLpspiHandle);
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("LPSPI polling example start.\r\n");
    PRINTF("This example use one lpspi instance as master and another as slave on one board.\r\n");
    PRINTF("Master uses polling way and slave uses interrupt way.\r\n");
    PRINTF(
        "Note that some LPSPI instances interrupt is in INTMUX ,"
        "you should set the intmux when you porting this example accordingly \r\n");

    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("LPSPI_master -- LPSPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS \r\n");
    PRINTF("   SOUT     --    SIN  \r\n");
    PRINTF("   SIN      --    SOUT \r\n");

    EnableIRQ(EXAMPLE_LPSPI_IRQ);
    LPSPI_MasterInit();
    LPSPI_MasterTransferCreateHandle();
    LPSPI_MasterTranserSendReceive();

    while (1)
    {
    }
}
