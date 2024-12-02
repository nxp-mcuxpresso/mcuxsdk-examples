/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"

#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
#include "fsl_intmux.h"
#endif

/*${header:end}*/

/*${variable:start}*/

#if defined(RELOCATE_VECTOR_TABLE)
/* RAM Vector Table */
SDK_ALIGN(static uint32_t s_ivt_ram[VECTOR_TABLE_SIZE / sizeof(uint32_t)], 256);
#endif

/*${variable:end}*/

/*${function:start}*/

/* Relocate Vector Table to RAM */
void BOARD_RelocateVectorTable(void)
{
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t __Vectors[];
#elif defined(__MCUXPRESSO)
    extern uint32_t __Vectors[];
#elif defined(__ICCARM__)
    extern uint32_t __Vectors[];
#elif defined(__GNUC__)
    extern uint32_t __Vectors[];
#endif

    uint32_t n;
    uint32_t irqMaskValue;
    irqMaskValue = DisableGlobalIRQ();

    /* Copy the vector table from ROM to RAM */
    for (n = 0; n < (VECTOR_TABLE_SIZE / sizeof(uint32_t)); n++)
    {
        s_ivt_ram[n] = __Vectors[n];
    }
    /* Point the VTOR to the position of vector table */
    SCB->VTOR = (uint32_t)s_ivt_ram;

    EnableGlobalIRQ(irqMaskValue);
    SDK_ISR_EXIT_BARRIER;
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

#if defined(RELOCATE_VECTOR_TABLE)
    BOARD_RelocateVectorTable();
#endif

    /*Set clock source for LPSPI*/
    CLOCK_SetMux(kCLOCK_LpspiMux, EXAMPLE_LPSPI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_LpspiDiv, EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER);

    NVIC_SetPriority(EXAMPLE_LPSPI_SLAVE_IRQN, 2);
}
/*${function:end}*/
