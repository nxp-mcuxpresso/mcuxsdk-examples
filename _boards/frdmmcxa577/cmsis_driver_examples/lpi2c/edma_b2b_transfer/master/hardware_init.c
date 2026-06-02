/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_edma_soc.h"
#include "RTE_Device.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
	
	/* Release peripheral RESET */
    RESET_PeripheralReset(kDMA0_RST_SHIFT_RSTn);

    /* Attach peripheral clock */
    CLOCK_SetupFRO12MClocking();
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPI2C0);
    CLOCK_SetClockDiv(kCLOCK_DivLPI2C0, 1u);

    /* Enable DMA clock. */
    CLOCK_EnableClock(kCLOCK_GateDMA0);
    
    /* Enable DMA0 request for LPI2C0 */
    EDMA_SocRequestEnable(RTE_I2C0_DMA_TX_DMA_BASE, RTE_I2C0_DMA_TX_PERI_SEL, true);
    EDMA_SocRequestEnable(RTE_I2C0_DMA_RX_DMA_BASE, RTE_I2C0_DMA_RX_PERI_SEL, true);
}

uint32_t LPI2C0_GetFreq(void)
{
    return LPI2C_CLOCK_FREQUENCY;
}
/*${function:end}*/