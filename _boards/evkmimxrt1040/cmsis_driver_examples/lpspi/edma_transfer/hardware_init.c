/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Set clock source for LPSPI*/
    CLOCK_SetMux(kCLOCK_LpspiMux, EXAMPLE_LPSPI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_LpspiDiv, EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER);

    /* DMAMux init and EDMA init */
    DMAMUX_Init(EXAMPLE_LPSPI_DMAMUX_BASEADDR);
    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(EXAMPLE_LPSPI_DMA_BASEADDR, &edmaConfig);
}

uint32_t LPSPI3_GetFreq(void)
{
    return EXAMPLE_LPSPI_MASTER_CLOCK_FREQ;
}

uint32_t LPSPI1_GetFreq(void)
{
    return EXAMPLE_LPSPI_SLAVE_CLOCK_FREQ;
}

/*${function:end}*/
