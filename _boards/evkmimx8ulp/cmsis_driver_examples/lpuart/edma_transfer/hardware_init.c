/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_reset.h"
#include "fsl_upower.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_BootClockRUN();
    UPOWER_PowerOnMemPart(0U, (uint32_t)kUPOWER_MP1_DMA0);
    CLOCK_EnableClock(kCLOCK_Dma0Ch0);
    CLOCK_EnableClock(kCLOCK_Dma0Ch1);
    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(EXAMPLE_USART_DMA_BASEADDR, &edmaConfig);
    EDMA_SetChannelMux(EXAMPLE_USART_DMA_BASEADDR, RTE_USART1_DMA_TX_CH, RTE_USART1_DMA_TX_PERI_SEL);
    EDMA_SetChannelMux(EXAMPLE_USART_DMA_BASEADDR, RTE_USART1_DMA_RX_CH, RTE_USART1_DMA_RX_PERI_SEL);
    CLOCK_SetIpSrc(kCLOCK_Lpuart1, kCLOCK_Pcc1BusIpSrcCm33Bus);
    RESET_PeripheralReset(kRESET_Lpuart1);
    if (BOARD_IsLowPowerBootType() != true) /* not low power boot type */
    {
        BOARD_HandshakeWithUboot(); /* Must handshake with uboot, unless will get issues(such as: SoC reset all the
                                       time) */
    }
    else                            /* low power boot type */
    {
        BOARD_SetTrdcGlobalConfig();
    }
}

uint32_t LPUART1_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpuart1);
}

/*${function:end}*/
