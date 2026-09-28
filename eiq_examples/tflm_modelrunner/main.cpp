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
#include "sys_info/sys_info.h"

#ifdef EXTERNAL_MEM
#include <string.h>
#include "fsl_debug_console.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

#ifdef EXTERNAL_MEM
/* Defined in hardware_init.c (C linkage): the eDMA completion flag and the
 * prefetch-start helper that programs and kicks off one transfer. */
extern "C" volatile bool g_Transfer_Done;
extern "C" void NPU_PrefetchDmaStart(void *dst, void *src, uint32_t size);

namespace tflite {

extern uint8_t scratchWeightsBuffer[];

/* Below this size the eDMA setup + IRQ (~1.6 us CPU) costs more than a CPU
 * memcpy, so small fetches are copied synchronously. */
#define NPU_PREFETCH_DMA_MIN_BYTES 128U

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

    /* Non-blocking eDMA prefetch; the CH4 completion IRQ sets g_Transfer_Done. */
    g_Transfer_Done = false;
    NPU_PrefetchDmaStart(dst, src, size);
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

    SYS_INFO_PrintAll();

    PRINTF("\r\n*************************************************");
    PRINTF("\r\n               TFLite Modelrunner");
    PRINTF("\r\n*************************************************\r\n");

    modelrunner();

}
