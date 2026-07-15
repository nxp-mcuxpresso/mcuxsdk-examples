/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Example Application
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "pin_mux.h"
#include "fsl_common.h"
#include "board.h"

#include "freemaster.h"
#include "freemaster_example.h"

#include "fsl_debug_console.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! Note: All global variables accessed by FreeMASTER are defined in a shared
//! freemaster_example.c file

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////


/* PDBDM buffer should be located in a non-cacheable section
  (see the FMSTR_PDBDM_USER_BUFFER also defined in freemaster_cfg.h) */
AT_NONCACHEABLE_SECTION(FMSTR_PDBDM_COMBUFF _pdbdm);

int main(void)
{
#if (defined(MIMXRT798S_cm33_core0_SERIES) || defined(MIMXRT798S_hifi4_SERIES) || defined(MIMXRT798S_ezhv_SERIES))
     /* Board initialization core0 */
    BOARD_ConfigMPU();
#else
    /* Board initialization core1 */
    BOARD_InitAHBSC();
#endif

    /* Common board initialization */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* This example uses shared code from FreeMASTER generic example application */
    FMSTR_Example_Init();

    PRINTF(
        "\nFreeMASTER PDBDM Example.\n"
        "Connect using FreeMASTER Packet Driven JTAG/BDM Communication Plug-in.\n"
        "This communication cannot be used along with existing debugger session.\n\n");

    while(1)
    {
        /* FreeMASTER example increments several variables periodically,
           use the FreeMASTER PC Host tool to visualize the variables */
        FMSTR_Example_Poll();
    }
}
////////////////////////////////////////////////////////////////////////////////
// EOF
/////////////////////////////////////////////////////////////////////////////////
