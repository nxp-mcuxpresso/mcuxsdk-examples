/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "timer.h"
/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/

/*${function:start}*/
#include "fsl_debug_console.h"


#include "fsl_inputmux_connections.h"
#include "fsl_inputmux.h"

#include <xtensa/config/core.h>
#include <xtensa/xos.h>
#include <xtensa/xtruntime.h>

#define CORE_DCACHE_LINESIZE 64

extern void xthal_dcache_region_invalidate(void *addr, size_t size);
extern void xthal_dcache_region_writeback(void *addr, size_t size);
extern void xthal_dcache_region_writeback_inv(void *addr, size_t size);

void cleanCacheByRange(uint32_t start_addr, uint32_t size) {
    xthal_dcache_region_writeback((void *)start_addr, size);
}

void invalidateCacheByRange(uint32_t start_addr, uint32_t size) {
    xthal_dcache_region_invalidate((void *)start_addr, size);
}


volatile uint32_t * nrCTRL = (volatile uint32_t *)0x40175000U;
volatile uint32_t * nrINTR = (volatile uint32_t *)0x40175040U;

extern void Neutron_IRQHandler(void * arg);

void BOARD_InitHardware(void)
{

    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* Note: need tell clock driver the frequency of OSC. */

    TIMER_Init();
    INPUTMUX_Init(INPUTMUX0);
    CLOCK_EnableClock(kCLOCK_InputMux);
    INPUTMUX_AttachSignal(INPUTMUX0, 17U, kINPUTMUX_Neutron64IrqToDspInterrupt);
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();

    xos_register_interrupt_handler(DSP_INT0_SEL17_IRQn, Neutron_IRQHandler, NULL);
    xos_interrupt_enable(DSP_INT0_SEL17_IRQn);

    PRINTF("DSP Init Susccessfully\r\n");
}
/*${function:end}*/
