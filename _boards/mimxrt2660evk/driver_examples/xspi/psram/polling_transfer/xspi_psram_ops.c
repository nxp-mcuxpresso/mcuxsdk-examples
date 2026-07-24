/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Board-specific replacement for the shared xspi_psram_ops.c (swapped in by
 * reconfig.cmake). On this chip the controller + device bring-up cannot be
 * expressed through XSPI_Init()/XSPI_SetDeviceConfig() alone: the serial
 * clock only runs after CLK_CFG is programmed (and it must be programmed
 * before DDR/DQS enable), the AHB buffers need an explicit catch-all
 * routing, and the APS256XXN powers up in X8 mode so X16 entry needs a
 * guarded MR8 write. All of that lives in the silicon-validated
 * BOARD_Init16bitsPsRam() (board.c), which this file reuses.
 *
 * The IP-command path is implemented with the raw target-group descriptor
 * sequence validated during bring-up instead of XSPI_TransferBlocking():
 * on this silicon a triggered write transfer moves the TG FSM straight to
 * "write transfer triggered" (FSMSTAT.STATE=2) and consumes TBDR data as
 * it arrives, while the driver's blocking write spins waiting for the
 * "TBDR lock open" state (1) that never appears - a deadlock. Data is
 * moved in 8-byte frames, the exact frame shape proven by the bring-up
 * register-read/write path (external-DQS reads must be multiples of 8
 * bytes, RM rule).
 */

#include "fsl_xspi.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Bytes per IP frame: multiple-of-8 (external-DQS read rule), small enough
 * that a frame never approaches the device tCEM (1 us) CE#-low limit. */
#define IP_CHUNK_BYTES 8U

/*******************************************************************************
 * Code
 ******************************************************************************/

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

/* Queue one IP frame with the TG descriptor discipline required for
 * back-to-back CPU-issued commands: queue empty -> SFAR -> descriptor
 * validated -> IPCR (which starts the frame). All waits bounded. */
static status_t xspi_ip_start_frame(XSPI_Type *base, uint32_t sfar, uint8_t seqIdx)
{
    uint32_t i;

    for (i = 0U; ((base->TGSFARS & XSPI_TGSFARS_VLD_MASK) != 0U) && (i < 100000U); i++)
    {
    }
    base->ERRSTAT     = XSPI_ERRSTAT_ARB_WIN_MASK; /* W1C stale grant */
    base->SFP_TG_SFAR = sfar;
    for (i = 0U;
         ((base->TGSFARS & (XSPI_TGSFARS_VLD_MASK | XSPI_TGSFARS_ERR_MASK)) == 0U) && (i < 100000U);
         i++)
    {
    }
    if ((base->TGSFARS & XSPI_TGSFARS_ERR_MASK) != 0U)
    {
        return kStatus_Fail;
    }
    base->SFP_TG_IPCR = ((uint32_t)seqIdx << XSPI_SFP_TG_IPCR_SEQID_SHIFT) |
                        ((uint32_t)IP_CHUNK_BYTES << XSPI_SFP_TG_IPCR_IDATSZ_SHIFT);
    return kStatus_Success;
}

/* Paced completion wait: tight back-to-back polls of an active frame raise
 * FR.ILLACC and abort the session, so sample roughly every 1 us. */
static status_t xspi_ip_wait_frame_done(XSPI_Type *base)
{
    uint32_t i;

    for (i = 0U; i < 10000U; i++)
    {
        if ((base->ERRSTAT & XSPI_ERRSTAT_TO_ERR_MASK) != 0U)
        {
            base->ERRSTAT = XSPI_ERRSTAT_TO_ERR_MASK; /* W1C */
            return kStatus_Fail;
        }
        if ((base->FR & XSPI_FR_TFF_MASK) != 0U)
        {
            base->FR = XSPI_FR_TFF_MASK; /* W1C */
            return kStatus_Success;
        }
        SDK_DelayAtLeastUs(1U, 1000000000U);
    }
    return kStatus_Timeout;
}

status_t xspi_hyper_ram_ipcommand_write_data(XSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    uint32_t chunk;
    status_t status;

    for (chunk = 0U; chunk < length; chunk += IP_CHUNK_BYTES)
    {
        base->MCR |= XSPI_MCR_CLR_TXF_MASK;

        status = xspi_ip_start_frame(base, EXAMPLE_XSPI_AMBA_BASE + address + chunk,
                                     HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE);
        if (status != kStatus_Success)
        {
            return status;
        }
        /* Feed the data right after IPCR: the triggered frame consumes
         * TBDR entries as they arrive (host-paced feeding loses the
         * data-phase window; CPU-paced stores keep ahead of the bus). */
        base->TBDR = buffer[chunk / 4U];
        base->TBDR = buffer[(chunk / 4U) + 1U];

        status = xspi_ip_wait_frame_done(base);
        if (status != kStatus_Success)
        {
            return status;
        }
    }
    return kStatus_Success;
}

status_t xspi_hyper_ram_ipcommand_read_data(XSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    uint32_t chunk;
    status_t status;

    for (chunk = 0U; chunk < length; chunk += IP_CHUNK_BYTES)
    {
        base->MCR |= XSPI_MCR_CLR_RXF_MASK;

        status = xspi_ip_start_frame(base, EXAMPLE_XSPI_AMBA_BASE + address + chunk,
                                     HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ);
        if (status != kStatus_Success)
        {
            return status;
        }
        status = xspi_ip_wait_frame_done(base);
        if (status != kStatus_Success)
        {
            return status;
        }
        buffer[chunk / 4U]        = base->RBDR[0];
        buffer[(chunk / 4U) + 1U] = base->RBDR[1];
        /* Drain + pop: RX residue keeps XSPI from reaching IDLE and
         * eventually wedges the fabric. */
        base->FR = 0x00010000U; /* RBDF W1C = pop */
        base->MCR |= XSPI_MCR_CLR_RXF_MASK | XSPI_MCR_CLR_TXF_MASK;
    }
    return kStatus_Success;
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
