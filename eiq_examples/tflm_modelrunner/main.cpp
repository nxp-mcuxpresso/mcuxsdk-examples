/*
 * Copyright 2021-2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/


#include "modelrunner.h"
#include "app.h"

#ifdef EXTERNAL_MEM
#include <string.h>
#include "fsl_edma.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * NPU weight-prefetch copy()/wait() overrides (memory-to-memory via DMA0 ch4).
 * Firmware calls copy() to start a non-blocking fetch of the next layer's
 * weights from XSPI2 flash into SRAM scratch, then wait() before the kernel.
 * copy() programs the TCD (Prepare/Submit/Start); the CH4 completion IRQ sets
 * g_Transfer_Done. The eDMA controller/channel/IRQ bring-up lives in
 * hardware_init.c (BOARD_InitHardware); g_DMA_NPU_Handle and g_Transfer_Done
 * are defined there and referenced here via extern "C".
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Defined in hardware_init.c (C linkage) and shared with the eDMA init/IRQ. */
extern "C" edma_handle_t g_DMA_NPU_Handle;
extern "C" volatile bool g_Transfer_Done;

namespace tflite {

extern uint8_t scratchWeightsBuffer[];

edma_transfer_config_t transferConfig;

/* Below this size the eDMA setup + IRQ (~1.6 us CPU) costs more than a CPU
 * memcpy, so small fetches are copied synchronously. */
#define NPU_PREFETCH_DMA_MIN_BYTES 128U

/* Widest eDMA transfer element (SSIZE/DSIZE). */
#define NPU_PREFETCH_DMA_MAX_WIDTH 64U

/* Strong override of the weak copy() in neutron.cpp. */
void copy(void *dst, void *src, uint32_t size, uint32_t channel) {
    if (((uintptr_t)dst + size - (uintptr_t)scratchWeightsBuffer) > SCRATCH_WEIGHTS_SRAM_SIZE)
    {
        PRINTF("[copy] ERROR scratchWeightsBuffer overflow: end=%x limit=%x\r\n",
               (unsigned)((uintptr_t)dst + size - (uintptr_t)scratchWeightsBuffer),
               (unsigned)SCRATCH_WEIGHTS_SRAM_SIZE);
        while (1);
        return;
    }

    /* Small transfer: setup+IRQ dominates, so memcpy on the CPU and mark done
     * immediately (wait() then returns without spinning). */
    if (size < NPU_PREFETCH_DMA_MIN_BYTES)
    {
        memcpy(dst, src, size);
        g_Transfer_Done = true;
        return;
    }

    /* Pick the widest element that keeps src, dst and size aligned; step down
     * 64->32->...->4 until it fits (eDMA requires alignment to the beat). */
    uint32_t width = NPU_PREFETCH_DMA_MAX_WIDTH;
    while (width > sizeof(uint32_t) &&
           (((uintptr_t)src | (uintptr_t)dst | (uintptr_t)size) & (width - 1U)) != 0U)
    {
        width >>= 1;
    }

    g_Transfer_Done = false;
    EDMA_PrepareTransfer(&transferConfig,
                         src, width,
                         dst, width,
                         size, size,
                         kEDMA_MemoryToMemory);
    EDMA_SubmitTransfer(&g_DMA_NPU_Handle, &transferConfig);
    EDMA_StartTransfer(&g_DMA_NPU_Handle);
}

/* Strong override of the weak wait() in neutron.cpp. Spins until the EDMA
 * callback sets g_Transfer_Done, then clears it for the next transfer. */
void wait(uint32_t channel) {
    while (g_Transfer_Done != true)
    {
    }
    g_Transfer_Done = false;
}
}  // namespace tflite

#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function.
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("\r\n*************************************************");
    PRINTF("\r\n               TFLite Modelrunner");
    PRINTF("\r\n*************************************************\r\n");

    modelrunner();

}
