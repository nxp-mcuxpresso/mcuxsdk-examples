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
#include <stdint.h>
#include "fsl_edma.h"


/*
 * NPU weight-prefetch eDMA controller bring-up (memory-to-memory, DMA0 ch4).
 * copy()/wait() in main.cpp drive the transfers; this file owns the eDMA init.
 * Use EDMA_Init() (not RESET_ClearPeripheralReset, which breaks XSPI); CH_MUX
 * must be 0 so the channel takes no hardware request. The CH4 IRQ is serviced
 * by the SDK's EDMA0_CH4_DriverIRQHandler in fsl_edma_soc.c, which dispatches
 * to the handle registered by EDMA_CreateHandle below and runs EDMA_Callback;
 * do NOT define EDMA0_CH4_DriverIRQHandler here (it would be a duplicate).
 */
/* DMA0 = the eDMA4 instance in the COMPUTE domain owned by this CM33 core0. */
#define NPU_PREFETCH_DMA     DMA0
#define NPU_PREFETCH_DMA_CH  4

/* Widest eDMA transfer element (SSIZE/DSIZE). */
#define NPU_PREFETCH_DMA_MAX_WIDTH 64U

/* Shared with copy()/wait() in main.cpp (referenced there via extern "C"). */
edma_handle_t g_DMA_NPU_Handle;
volatile bool g_Transfer_Done = false;

/* Reused by every prefetch; only copy() (single producer) programs it. */
static edma_transfer_config_t s_transferConfig;


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

/*
 * Program and kick off one memory-to-memory prefetch on the NPU channel.
 * copy() in main.cpp calls this after clearing g_Transfer_Done; the CH4
 * completion IRQ sets it back. Picks the widest transfer element that keeps
 * src, dst and size aligned, stepping 64->32->...->4 (eDMA needs the operands
 * aligned to the beat).
 */
void NPU_PrefetchDmaStart(void *dst, void *src, uint32_t size)
{
    uint32_t width = NPU_PREFETCH_DMA_MAX_WIDTH;
    while (width > sizeof(uint32_t) &&
           (((uintptr_t)src | (uintptr_t)dst | (uintptr_t)size) & (width - 1U)) != 0U)
    {
        width >>= 1;
    }

    EDMA_PrepareTransfer(&s_transferConfig,
                         src, width,
                         dst, width,
                         size, size,
                         kEDMA_MemoryToMemory);
    EDMA_SubmitTransfer(&g_DMA_NPU_Handle, &s_transferConfig);
    EDMA_StartTransfer(&g_DMA_NPU_Handle);
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
