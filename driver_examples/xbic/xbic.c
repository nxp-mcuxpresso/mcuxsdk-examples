/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_xbic.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t address, master, slave, syndrome;
    volatile uint32_t *xbicParaInSlavePort;

    /* Define a parameter located in slave port. */
    xbicParaInSlavePort = (uint32_t *)DEMO_XBIC_ERROR_ADDRESS;

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\n\rXBIC Example started !\n\r");

    /* Enable master port error detection. */
    XBIC_EnableMasterPortEDC(DEMO_XBIC, DEMO_XBIC_MASTER_PORT);

    /* Enable slave port error detection. */
    XBIC_EnableSlavePortEDC(DEMO_XBIC, DEMO_XBIC_SLAVE_PORT);

    /* Inject errors with cconfiguration. */
    XBIC_ErrorInjectionConfig(DEMO_XBIC, DEMO_XBIC_MASTER_PORT, DEMO_XBIC_SLAVE_PORT, DEMO_XBIC_SYNDROMES);

    /* Enable the error inject. */
    XBIC_EnableErrorInjection(DEMO_XBIC, true);

    /* Generate transfer error by read operation on variable in slave port. */
    (void)*xbicParaInSlavePort;

    /* Disable the error injection. */
    XBIC_EnableErrorInjection(DEMO_XBIC, false);

    /* Check if error status is valid . */
    if(XBIC_GetErrorValidFlag(DEMO_XBIC) == true)
    {
        PRINTF("\n\rXBIC error Injected successfully !\n\r");

        /* Get the most recent transfer error master port ID. */
        master = XBIC_GetErrorMasterPorts(DEMO_XBIC);
        /* Get the most recent transfer error slave port ID. */
        slave = XBIC_GetErrorSlavePorts(DEMO_XBIC);
        /* Get the most recent transfer error address. */
        address = XBIC_GetErrorAddress(DEMO_XBIC);
        /* Get the most recent transfer error syndrome. */
        syndrome = XBIC_GetErrorSyndrome(DEMO_XBIC);

        /* Check if the latest transfer error address, master port, slave port, syndrome. */
        if((xbicParaInSlavePort == (uint32_t*)address) && \
           (master == DEMO_XBIC_MASTER_PORT) &&        \
           (slave == DEMO_XBIC_SLAVE_PORT) &&          \
           (syndrome == DEMO_XBIC_SYNDROMES))
        {
            PRINTF("\n\rXBIC error data received successfully !\n\r");
            PRINTF("\n\rXBIC Example finished !\n\r");
        }
        else
        {
            PRINTF("\n\rXBIC error data received failed !\n\r");
        }
    }
    else
    {
        PRINTF("\n\rXBIC error Injected failed !\n\r");
    }

    while (true)
    {
    }
}
