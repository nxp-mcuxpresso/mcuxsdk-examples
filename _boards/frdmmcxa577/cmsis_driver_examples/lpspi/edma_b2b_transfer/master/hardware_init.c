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
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPSPI1);
    CLOCK_SetClockDiv(kCLOCK_DivLPSPI1, 1u);

    /* Enable DMA0 request for LPSPI */
    EDMA_SocRequestEnable(RTE_SPI1_DMA_TX_DMA_BASE, RTE_SPI1_DMA_TX_PERI_SEL, true);
    EDMA_SocRequestEnable(RTE_SPI1_DMA_RX_DMA_BASE, RTE_SPI1_DMA_RX_PERI_SEL, true);
}

uint32_t LPSPI1_GetFreq()
{
    return EXAMPLE_LPSPI_CLOCK_FREQ;
}
/*${function:end}*/
