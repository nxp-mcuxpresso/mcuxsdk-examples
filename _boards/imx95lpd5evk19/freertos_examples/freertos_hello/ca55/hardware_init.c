/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "mmu.h"
#include "irq.h"
#include "sm_platform.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    IRQn_Type const s_muIrqs[] = MU_IRQS;
    IRQn_Type irq = s_muIrqs[SM_PLATFORM_MU_INST];

    /* Init board cpu and hardware. */
    MMU_init();
    /* Enable GIC before register any interrupt handler*/
    GIC_Enable();
    irq_register(irq, SM_platform_MU_IRQHandler, NULL, portLOWEST_USABLE_INTERRUPT_PRIORITY - 1);
    SM_Platform_Init();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
