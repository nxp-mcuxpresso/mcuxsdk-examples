/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Board-specific replacement for the shared xspi_psram_edma_ops.c (swapped
 * in by reconfig.cmake). Initialization reuses the silicon-validated
 * BOARD_Init16bitsPsRam() - see the polling_transfer port for why the
 * bring-up cannot be expressed through XSPI_Init()/XSPI_SetDeviceConfig()
 * on this chip.
 *
 * The DMA write path moves data through the XSPI AHB window
 * (memory-to-memory eDMA) rather than feeding the IP-command TBDR by DMA:
 * on this silicon the data phase of a triggered IP write frame must be
 * served within a window that the DMA start latency already misses, and a
 * frame stalled waiting for data violates the device tCEM (CE# low, 1 us)
 * limit - see the comment in xspi_hyper_ram_dmacommand_write_data().
 * (The stock XSPI_TransferEDMA() additionally deadlocks in the same
 * "TBDR lock open" wait as XSPI_TransferBlocking(), see the
 * polling_transfer port.)
 */

#include "fsl_xspi.h"
#include "fsl_edma.h"
#include "fsl_xspi_edma.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Defined in the shared example main. */
extern edma_handle_t dmaTxHandle;
extern edma_handle_t dmaRxHandle;
extern xspi_edma_handle_t xspiHandle;

static volatile bool s_transferDone;

/*******************************************************************************
 * Code
 ******************************************************************************/

void xspi_callback(XSPI_Type *base, xspi_edma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        s_transferDone = true;
    }
}

void xspi_hyper_ram_init(XSPI_Type *base)
{
    /* Full controller + device bring-up: MODCON reset, CLK_CFG before DDR
     * enable, auto-DLL, Xccela LUT, Global Reset, MR8 X16 entry. Applies
     * the XSPI1 PSRAM pin mux internally and skips re-init when X16 is
     * already active (e.g. after a debugger-side init). */
    BOARD_Init16bitsPsRam(base);

    /* Console is up (BOARD_InitHardware ran first): report the identity
     * captured during init - MR1/MR2 expect 0x8D/0xDF. */
    BOARD_LogPsRamID();
}

status_t xspi_hyper_ram_dmacommand_write_data(XSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    edma_transfer_config_t xferConfig;
    uint32_t i;

    /* eDMA moves the data into the XSPI AHB window (memory-to-memory,
     * software triggered); the XSPI AHB write engine generates the serial
     * frames. Feeding the IP-command TBDR by DMA instead is not reliable
     * on this silicon: the data phase of a triggered frame must be served
     * within a window the DMA start latency already misses (silicon-
     * observed: all 64 words reach the TX buffer and TBSR.TRCTR counts
     * them out, but the frame has starved by then and the device stores
     * nothing - the same race that makes debugger-paced TBDR writes fail),
     * and a frame stalled waiting for data violates the device tCEM (CE#
     * low) limit of 1 us. */
    EDMA_PrepareTransfer(&xferConfig, buffer, 4U, (void *)(EXAMPLE_XSPI_AMBA_BASE + address), 4U, length, length,
                         kEDMA_MemoryToMemory);
    (void)EDMA_SubmitTransfer(&dmaTxHandle, &xferConfig);
    EDMA_TriggerChannelStart(EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL);

    /* Single software-triggered minor/major loop: poll the channel DONE
     * flag (no IRQ dependency). */
    for (i = 0U; i < 100000U; i++)
    {
        if ((EDMA_GetChannelStatusFlags(EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL) & (uint32_t)kEDMA_DoneFlag) != 0U)
        {
            EDMA_ClearChannelStatusFlags(EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL, (uint32_t)kEDMA_DoneFlag);
            return kStatus_Success;
        }
        SDK_DelayAtLeastUs(1U, 1000000000U);
    }
    return kStatus_Timeout;
}

void xspi_hyper_ram_ahbcommand_write_data(XSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    uint32_t *startAddr = (uint32_t *)(EXAMPLE_XSPI_AMBA_BASE + address);
    memcpy(startAddr, buffer, length);
}

void xspi_hyper_ram_ahbcommand_read_data(XSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    uint32_t *startAddr = (uint32_t *)(EXAMPLE_XSPI_AMBA_BASE + address);
    memcpy(buffer, startAddr, length);
}
