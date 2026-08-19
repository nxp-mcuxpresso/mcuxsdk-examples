/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_clock.h"
#include "app.h"
#include "timer.h"
#include "pmic_support.h"
#ifdef EXTERNAL_MEM
#include <stdbool.h>
#include "fsl_edma.h"

/*
 * NPU weight-prefetch eDMA controller bring-up (memory-to-memory, DMA0 ch4).
 * copy()/wait() in main.cpp drive the transfers; this file owns the eDMA init
 * and the CH4 completion IRQ. Use EDMA_Init() (not RESET_ClearPeripheralReset,
 * which breaks XSPI); CH_MUX must be 0 so the channel takes no hardware request.
 */
/* DMA0 = the eDMA4 instance in the COMPUTE domain owned by this CM33 core0. */
#define NPU_PREFETCH_DMA     DMA0
#define NPU_PREFETCH_DMA_CH  4

/* Shared with copy()/wait() in main.cpp (referenced there via extern "C"). */
edma_handle_t g_DMA_NPU_Handle;
volatile bool g_Transfer_Done = false;

/* Callback invoked by the EDMA driver ISR. */
static void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    (void)handle;
    (void)param;
    (void)tcds;
    if (transferDone)
    {
        g_Transfer_Done = true;
    }
}

/* CH4 completion IRQ. Dispatch to EDMA_HandleIRQ, which runs EDMA_Callback. */
void EDMA0_CH4_DriverIRQHandler(void)
{
    EDMA_HandleIRQ(&g_DMA_NPU_Handle);
    SDK_ISR_EXIT_BARRIER;
}
#endif

/*${function:start}*/
int64_t os_clock_now(){
    return TIMER_GetTimeInUS();
}
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_InitAHBSC();
    BOARD_InitBootClocks();
    BOARD_ConfigMPU();

    // Disable LDO
    POWER_SetVddnSupplySrc(kVddSrc_PMIC);
    POWER_SetVdd1SupplySrc(kVddSrc_PMIC);
    POWER_SetVdd2SupplySrc(kVddSrc_PMIC);
    POWER_ApplyPD();

    BOARD_InitPmicPins();
    BOARD_InitPmic();
    BOARD_SetPmicVdd2Voltage(1100000U); /* 1.1v for 325MHz clock. */
    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();

    POWER_DisablePD(kPDRUNCFG_APD_NPU);
    POWER_DisablePD(kPDRUNCFG_PPD_NPU);
    POWER_ApplyPD();
    TIMER_Init();
#if defined(DEMO_USE_XSPI2) && DEMO_USE_XSPI2
    BOARD_InitPsRamPins_Xspi2();
    CLOCK_AttachClk(kMAIN_PLL_PFD3_to_XSPI2);
    CLOCK_SetClkDiv(kCLOCK_DivXspi2Clk, 1u);     /*500MHz*/
#elif defined(DEMO_USE_XSPI1) && DEMO_USE_XSPI1
    BOARD_InitPsRamPins_Xspi1();
    CLOCK_AttachClk(kAUDIO_PLL_PFD1_to_XSPI1);
    CLOCK_SetClkDiv(kCLOCK_DivXspi1Clk, 1u);     /*400MHz*/
#endif
#ifdef EXTERNAL_MEM
    /* Init the eDMA controller (writes MP_CSR) for the NPU weight prefetch. */
    {
        edma_config_t dmaConfig;
        EDMA_GetDefaultConfig(&dmaConfig);
        EDMA_Init(NPU_PREFETCH_DMA, &dmaConfig);
    }
    EDMA_SetChannelMux(NPU_PREFETCH_DMA, NPU_PREFETCH_DMA_CH, 0U);
    EDMA_CreateHandle(&g_DMA_NPU_Handle, NPU_PREFETCH_DMA, NPU_PREFETCH_DMA_CH);
    EDMA_SetCallback(&g_DMA_NPU_Handle, EDMA_Callback, NULL);
#endif
}
/*${function:end}*/
