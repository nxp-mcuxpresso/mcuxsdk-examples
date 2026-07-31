/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Board-specific replacement for the shared xspi_quad_flash_edma_ops.c
 * (swapped in by reconfig.cmake). The shared file targets the RT700 (XCACHE/
 * CACHE64 cache pair, stateful QPI 4-4-4 mode); this board drives the
 * W25H512NWEAM in plain Quad I/O (1-4-4) with verified 4-byte addressing,
 * LLC + CM85 L1 cache maintenance, and the paced-retry/recovery ladder shared
 * with the polling example. EDMA feeds the XSPI TX/RX buffers for the page
 * program and read paths.
 */

#include "fsl_xspi.h"
#include "fsl_xspi_edma.h"
#include "fsl_edma.h"
#include "fsl_common.h"
#include "fsl_llc.h"
#include "fsl_debug_console.h"
#include "app.h"

#include "fsl_trdc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* QE bit is bit 1 of Status Register-2 for the W25H512NWEAM. */
#define FLASH_QUAD_ENABLE_BIT (1U << 1U)

/*******************************************************************************
 * Variables
 *****************************************************************************/
extern xspi_device_config_t deviceconfig;
extern const uint32_t customLUT[CUSTOM_LUT_LENGTH];
extern edma_handle_t dmaTxHandle;
extern edma_handle_t dmaRxHandle;
extern xspi_edma_handle_t xspiHandle;
static volatile bool g_completionFlag       = false;
static volatile status_t g_completionStatus = kStatus_Success;

/*
 * TX (channel 0) is used only by page program. If a previous program attempt
 * latches an EDMA channel/global error, clean that sticky state before arming
 * the next TX transfer so RX (channel 1, read path) remains untouched.
 */
static void xspi_quad_prepare_tx_dma_channel(void)
{
    EDMA_Type *edmaBase = (EDMA_Type *)(void *)EXAMPLE_XSPI_DMA;

    XSPI_EnableTxDMA(EXAMPLE_XSPI, false);
    EDMA_AbortTransfer(&dmaTxHandle);

    /* Clear sticky CH_INT/CH_ES/DONE bits on the TX channel only. */
    EDMA_CHANNEL_BASE(edmaBase, XSPI_TX_DMA_CHANNEL)->CH_INT = DMA_CH_INT_INT_MASK;
    EDMA_CHANNEL_BASE(edmaBase, XSPI_TX_DMA_CHANNEL)->CH_ES |= DMA_CH_ES_ERR_MASK;
    EDMA_CHANNEL_BASE(edmaBase, XSPI_TX_DMA_CHANNEL)->CH_CSR |= DMA_CH_CSR_DONE_MASK;

    /* Resume service requests in case a previous error forced HALT. */
    EDMA_MP_BASE(edmaBase)->MP_CSR &= ~DMA_CORE_MP_CSR_HALT_MASK;
}

/*******************************************************************************
 * Code
 * NOTE: on the flash-XIP targets this file is placed in ITCM (see the board
 * linker) because it reconfigures/erases the same XSPI0 NOR it would otherwise
 * fetch from. The fsl_xspi driver is already RAMFUNC, so the only remaining
 * flash-resident dependency in the reconfig path is the delay helper -- hence
 * the RAM-resident xspi_quad_delay_us() below instead of SDK_DelayAtLeastUs().
 ******************************************************************************/

/*
 * RAM-resident microsecond busy-wait. Must not be the flash-resident
 * SDK_DelayAtLeastUs: it is called while the XSPI0 NOR is busy (erase/program)
 * or mid-reconfigure, when a flash code-fetch would stall. Implemented as a
 * plain finite loop (NOT the DWT cycle counter -- CYCCNT is optional on
 * Cortex-M85 and may read 0, which would spin forever). One loop pass is a few
 * core cycles, so this over-waits slightly, which is fine for "at least"
 * delays. Lives in this ITCM-resident file (see the board linker).
 */
static void xspi_quad_delay_us(volatile uint32_t delayTime_us)
{
    /* ~ (core clock / 1 MHz) passes per microsecond; each pass is several
     * cycles, so the real delay is >= requested. */
    volatile uint32_t count = delayTime_us * (SystemCoreClock / 1000000UL);
    while (count != 0UL)
    {
        count--;
    }
}

/*
 * Drop every stale copy of the flash data between the device and the CPU before
 * an AHB (memory-mapped) read. Two cache levels shadow the 0x60000000 flash
 * window on this SoC -- the CM85 L1 (I-/D-cache) and the shared last-level cache
 * (LLC) -- plus the XSPI AHB prefetch buffer. Erase/program go through XSPI IP
 * commands, which bypass all of these, so each must be invalidated. Order is
 * far-to-near (XSPI buffer -> LLC -> L1) so an L1 refill pulls through freshly
 * invalidated levels down to the flash.
 */
void xspi_quad_invalidate_caches(void)
{
    XSPI_ClearAhbBuffer(EXAMPLE_XSPI); /* XSPI AHB prefetch buffer */

    /* L2: clean+invalidate the shared LLC via its driver (bounded internally;
     * a no-op-fast path when the LLC is disabled). */
    (void)LLC_CleanInvalidateCache(CMPT__LLC);

    /* L1: clean+invalidate D-cache (clean so unrelated dirty lines survive),
     * invalidate I-cache. */
    SCB_CleanInvalidateDCache();
#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    SCB_InvalidateICache();
#endif

    __DSB();
    __ISB();
}

/*
 * EDMA completion callback. Runs from the EDMA channel interrupt, so on the
 * flash-XIP targets the whole interrupt path (vector table, fsl_edma,
 * fsl_edma_soc, fsl_xspi_edma, this file) is placed in ITCM by the board
 * linker -- it can fire while the flash array is busy programming and no
 * flash fetch is possible.
 */
void xspi_callback(XSPI_Type *base, xspi_edma_handle_t *handle, status_t status, void *userData)
{
    g_completionStatus = status;
    g_completionFlag   = true;
}

/*
 * Strong overrides of the startup weak EDMA3 channel-interrupt thunks. The
 * startup copies live in flash, but these thunks run from the EDMA completion
 * interrupt while the flash array may be busy programming, so they must live
 * in this ITCM-resident file. Instance 0 selects MAIN__EDMA3 (see the device
 * EDMA_BASE_PTRS ordering).
 */
/* Direct RAMFUNC IRQ handlers for EDMA TX/RX channels. These avoid calling
 * into flash-resident EDMA_DriverIRQHandler (fsl_edma.c is not RAMFUNC) while
 * the NOR flash array is busy programming -- a flash fetch during that window
 * stalls indefinitely. The handlers clear the channel done flag and call the
 * xspi_callback directly from RAM. */

extern xspi_edma_handle_t xspiHandle;

static void xspi_quad_set_ahb_read_spi(XSPI_Type *base)
{
    uint32_t spiLUT[5] = {0U};

    /* During controller init the flash is still in default SPI mode. Keep
     * LUT[0] usable for AHB reads until xspi_quad_set_ahb_read_qpi() installs
     * the Quad I/O read sequence after quad mode is enabled. */
    spiLUT[0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x0B, kXSPI_Command_RADDR_SDR, kXSPI_1PAD, 0x20);
    spiLUT[1] = XSPI_LUT_SEQ(kXSPI_Command_DUMMY_SDR, kXSPI_1PAD, 0x08, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08);

    XSPI_UpdateLUT(base, 5U * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI, spiLUT, 5U);
    XSPI_ClearAhbAccessSeqPointer(base);
    XSPI_ClearAhbBuffer(base);
}

static void xspi_quad_set_ahb_read_qpi(XSPI_Type *base)
{
    uint32_t qpiLUT[5] = {0U};

    /* Quad I/O (1-4-4): command 0xEC on 1 pad, address/mode/data on 4 pads.
     * Installed into LUT[0] after quad mode is enabled so AHB memory-mapped
     * reads use the Quad-I/O read. */
    qpiLUT[0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0xEC, kXSPI_Command_RADDR_SDR, kXSPI_4PAD, 0x20);
    /* Mode phase (2 clocks) + 4 dummy = 6-clock read latency (validated on this
     * part). MUST match the IP-read LUT[0] in hardware_init.c. */
    qpiLUT[1] = XSPI_LUT_SEQ(kXSPI_Command_MODE_SDR, kXSPI_4PAD, 0xFF, kXSPI_Command_DUMMY_SDR, kXSPI_4PAD, 0x04);
    qpiLUT[2] = XSPI_LUT_SEQ(kXSPI_Command_READ_SDR, kXSPI_4PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x0);

    XSPI_UpdateLUT(base, 5U * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI, qpiLUT, 5U);
    XSPI_ClearAhbAccessSeqPointer(base);
    XSPI_ClearAhbBuffer(base);
}

static status_t xspi_quad_transfer_paced(XSPI_Type *base, xspi_transfer_t *flashXfer);

/*
 * Write Enable (0x06). Must be issued before every erase, program, or
 * status-register write.
 */
static status_t xspi_quad_write_enable(XSPI_Type *base, uint32_t baseAddr)
{
    xspi_transfer_t flashXfer;
    status_t status;

    assert(baseAddr <= (UINT32_MAX - EXAMPLE_XSPI_AMBA_BASE)); /* CERT-C INT30-C */
    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE + baseAddr;
    flashXfer.cmdType         = kXSPI_Command;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_QPI_WRITE_ENABLE;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = NULL;
    flashXfer.dataSize        = 0UL;
    flashXfer.lockArbitration = false;

    status = xspi_quad_transfer_paced(base, &flashXfer);

    return status;
}

/*
 * Software reset: QPI rescue reset (0x66+0x99 on 4 pads, covers a flash left
 * in QPI mode by a previous session) followed by the normal SPI reset
 * (0x66+0x99 on 1 pad). After this call the flash is in its default SDR /
 * 3-byte-address / SPI state; call xspi_quad_enter_4byte_mode() and
 * xspi_quad_enable_quad_mode() after.
 */
status_t xspi_quad_reset_flash(XSPI_Type *base)
{
    status_t status;
    xspi_transfer_t flashXfer;

    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType         = kXSPI_Command;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = NULL;
    flashXfer.dataSize        = 0UL;
    flashXfer.lockArbitration = false;

    /* QPI rescue: 0x66 + 0x99 on 4 pads -- exits QPI if flash was left in it.
     * All four resets go through the paced path: the reset commands are
     * idempotent, so re-issuing one after a transient launch failure is safe. */
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_QPI_RESET_ENABLE;
    status             = xspi_quad_transfer_paced(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_QPI_RESET_MEMORY;
    status             = xspi_quad_transfer_paced(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }
    xspi_quad_delay_us(100U);

    /* SPI reset: 0x66 + 0x99 on 1 pad. */
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_SPI_RESET_ENABLE;
    status             = xspi_quad_transfer_paced(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_SPI_RESET_MEMORY;
    status             = xspi_quad_transfer_paced(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Wait tRST (30 us max) then flush the XSPI AHB prefetch buffer.
     * NOTE: use XSPI_ClearAhbBuffer here, NOT XSPI_SoftwareReset.
     * XSPI_SoftwareReset performs a heavy SFM+AHB host-domain reset that
     * un-arms the AHB read engine configured by XSPI_Init, which then makes
     * subsequent AHB memory-mapped reads of the flash window fault. */
    xspi_quad_delay_us(100U);
    XSPI_ClearAhbBuffer(base);

    return status;
}

/*
 * Issue one IP transaction with paced retry. A transaction issued right after
 * a preceding command transiently returns a target-group queue-writing or
 * sequence error on this XSPI (same class the status poll in
 * xspi_quad_wait_bus_busy handles); the interface clears the condition when
 * given a short gap, so wait for bus idle, try, and on error pace and retry a
 * bounded number of times. Every IP transaction in this file that is not
 * already inside the xspi_quad_wait_bus_busy() poll loop goes through this
 * helper (resets, write-enable, erase/program dispatch, register reads and
 * writes): all of those commands are idempotent or fail before launching, so
 * re-issuing after a transient launch failure is safe.
 */
static status_t xspi_quad_transfer_paced(XSPI_Type *base, xspi_transfer_t *flashXfer)
{
    status_t status;
    uint32_t tries = 0U;

    do
    {
        {
            uint32_t idleWait = 0U;
            while (!XSPI_GetBusIdleStatus(base))
            {
                if (++idleWait > 1000000U)
                {
                    break;
                }
            }
        }
        status = XSPI_TransferBlocking(base, flashXfer);
        if (status == kStatus_Success)
        {
            break;
        }
        /* Escalating recovery. Silicon observation (no-XIP layouts): the
         * sequencer FSM occasionally parks (FSMSTAT.STATE stuck, VLD never
         * asserting for new queue entries) and refuses IP transactions for a
         * stretch that ranges from a few ms to beyond 300 ms. Paced re-tries
         * alone cover the short stalls; for the long ones escalate: pulse the
         * TG queue reset first, then toggle the module (MDIS) -- both leave
         * the AHB read engine armed, unlike a full SFM/AHB domain reset. */
        if (tries == 50U)
        {
            XSPI_ResetTgQueue(base);
        }
        if (tries == 80U)
        {
            XSPI_EnableModule(base, false);
            xspi_quad_delay_us(10U);
            XSPI_EnableModule(base, true);
        }
        xspi_quad_delay_us((tries < 50U) ? 200U : 2000U);
    } while (++tries < 120U);

    return status;
}

/* Poll SR1 BUSY (0x05). */
static status_t xspi_quad_wait_bus_busy(XSPI_Type *base)
{
    bool isBusy;
    uint32_t readValue[2] = {0U};
    status_t status;
    xspi_transfer_t flashXfer;
    uint32_t errCount = 0U;

    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType         = kXSPI_Read;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_QPI_READ_STATUS;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = readValue;
    flashXfer.dataSize        = 8UL; /* ERR052528: min 8 bytes */
    flashXfer.lockArbitration = false;

    do
    {
        /* Pace the polls. A status read issued back-to-back right after a
         * long-running erase/program command transiently fails on this XSPI,
         * and hammering it keeps the interface in that error state; a short gap
         * between polls lets it settle (silicon-observed: paced polls clear the
         * error within tens of ms, un-paced polls never do). ~200 us is far
         * below any real erase/program time so it does not slow completion. */
        xspi_quad_delay_us(200U);

        status = XSPI_TransferBlocking(base, &flashXfer);
        if (status != kStatus_Success)
        {
            /* Still busy from the caller's point of view - keep polling rather
             * than aborting; bail only if the error persists far beyond any
             * real program/erase time (~200 us * 100000 = 20 s). */
            if (++errCount > 100000U)
            {
                return status;
            }
            isBusy = true;
            continue;
        }
        errCount = 0U;
        if (FLASH_BUSY_STATUS_POL)
        {
            isBusy = ((readValue[0] & (1U << FLASH_BUSY_STATUS_OFFSET)) != 0U);
        }
        else
        {
            isBusy = ((readValue[0] & (1U << FLASH_BUSY_STATUS_OFFSET)) == 0U);
        }
    } while (isBusy);

    return kStatus_Success;
}

/*
 * Enter 4-Byte Address Mode (SPI mode, command 0xB7) -> SR3.ADS = 1 (4-byte).
 * The W25H512NWEAM is 64 MB; 3-byte addressing only reaches 16 MB, so 4-byte
 * mode is required to access the whole device. The flash reverts to 3-byte mode
 * on a software reset, so this must be re-issued after every reset sequence.
 */
status_t xspi_quad_enter_4byte_mode(XSPI_Type *base)
{
    xspi_transfer_t flashXfer;

    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType         = kXSPI_Command;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_ENTER_4BYTE;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = NULL;
    flashXfer.dataSize        = 0UL;
    flashXfer.lockArbitration = false;

    return xspi_quad_transfer_paced(base, &flashXfer);
}

/*
 * Confirm 4-byte address mode is active by reading Status Register-3 (0x15) and
 * testing ADS (bit 0, "current address mode": 1 = 4-byte). Returns
 * kStatus_Success only when ADS is set; otherwise kStatus_Fail so the caller
 * can abort before addressing the flash beyond 16 MB.
 */
status_t xspi_quad_check_4byte_mode(XSPI_Type *base)
{
    xspi_transfer_t flashXfer;
    uint32_t sr3[2] = {0U};
    status_t status;

    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType         = kXSPI_Read;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR3;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = sr3;
    flashXfer.dataSize        = 8UL; /* ERR052528: min 8 bytes */
    flashXfer.lockArbitration = false;

    status = xspi_quad_transfer_paced(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* SR3.ADS = bit 0. */
    return ((sr3[0] & 0x01U) != 0U) ? kStatus_Success : kStatus_Fail;
}

/*
 * Read JEDEC ID (0x9F). Returns the manufacturer ID byte (byte 0 of the
 * 3-byte response). W25H512NWEAM: manufacturer = 0xEF (Winbond).
 */
status_t xspi_quad_get_vendor_id(XSPI_Type *base, uint8_t *vendorId)
{
    uint32_t temp[2] = {0U};
    xspi_transfer_t flashXfer;

    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType         = kXSPI_Read;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_QPI_READ_ID;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = temp;
    flashXfer.dataSize        = 8UL; /* ERR052528: min 8 bytes */
    flashXfer.lockArbitration = false;

    status_t status = xspi_quad_transfer_paced(base, &flashXfer);
    if (status == kStatus_Success)
    {
        *vendorId = (uint8_t)(temp[0] & 0xFFUL);
    }

    return status;
}

status_t xspi_quad_flash_erase_sector(XSPI_Type *base, uint32_t address)
{
    status_t status;
    xspi_transfer_t flashXfer;

    /* Make sure external flash is not in busy status. */
    status = xspi_quad_wait_bus_busy(base);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = xspi_quad_write_enable(base, address);
    if (status != kStatus_Success)
    {
        return status;
    }

    assert(address <= (UINT32_MAX - EXAMPLE_XSPI_AMBA_BASE)); /* CERT-C INT30-C */
    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE + address;
    flashXfer.cmdType         = kXSPI_Command;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_QPI_ERASE_SECTOR;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = NULL;
    flashXfer.dataSize        = 0UL;
    flashXfer.lockArbitration = false;
    status                    = xspi_quad_transfer_paced(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = xspi_quad_wait_bus_busy(base);

    return status;
}

/*
 * Program one page (FLASH_PAGE_SIZE bytes) with the data fed to the XSPI TX
 * buffer by EDMA. The completion callback sets g_completionFlag from the EDMA
 * channel interrupt; the wait loop below uses the RAM-resident delay because
 * the flash array goes busy as soon as the program command finishes on the
 * bus, so no flash-resident code may run until wait_bus_busy() confirms
 * completion.
 */
status_t xspi_quad_flash_page_program(XSPI_Type *base, uint32_t dstAddr, const uint32_t *src)
{
    status_t status;
    xspi_transfer_t flashXfer;
    EDMA_Type *edmaBase = (EDMA_Type *)(void *)EXAMPLE_XSPI_DMA;

    status = xspi_quad_wait_bus_busy(base);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = xspi_quad_write_enable(base, dstAddr);
    if (status != kStatus_Success)
    {
        return status;
    }

    xspi_quad_prepare_tx_dma_channel();

    assert(dstAddr <= (UINT32_MAX - EXAMPLE_XSPI_AMBA_BASE)); /* CERT-C INT30-C */
    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE + dstAddr;
    flashXfer.cmdType         = kXSPI_Write;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_QPI_PAGE_PROGRAM;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = (uint32_t *)(uintptr_t)src;
    flashXfer.dataSize        = FLASH_PAGE_SIZE;
    flashXfer.lockArbitration = false;

    g_completionFlag   = false;
    g_completionStatus = kStatus_Success;

    status = XSPI_TransferEDMA(base, &xspiHandle, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Wait for the EDMA TX major-loop completion callback. The custom
     * MAIN_EDMA3_CH0_IRQHandler fires when EDMA finishes, clears CH_INT,
     * disables TX DMA, resets the handle state, and calls xspi_callback()
     * which sets g_completionFlag. Polling the CH_CSR DONE bit directly
     * is racy with the IRQ clearing EARQ/DONE before the main thread
     * observes it; using g_completionFlag (same approach as xspi_quad_flash_read)
     * avoids that race. */
    {
        uint32_t waitCount = 0U;
        while (!g_completionFlag)
        {
            xspi_quad_delay_us(1U);
            if (++waitCount > 2000000U)
            {
                PRINTF("TX DMA timeout waiting for g_completionFlag\r\n");
                XSPI_EnableTxDMA(base, false);
                xspiHandle.state = 0U;
                PRINTF("TX timeout CH_ES=0x%08X MP_ES=0x%08X MP_CSR=0x%08X\r\n",
                       (unsigned)EDMA_CHANNEL_BASE(edmaBase, XSPI_TX_DMA_CHANNEL)->CH_ES,
                       (unsigned)EDMA_MP_BASE(edmaBase)->MP_ES, (unsigned)EDMA_MP_BASE(edmaBase)->MP_CSR);
                return kStatus_Timeout;
            }
        }
        if (g_completionStatus != kStatus_Success)
        {
            return g_completionStatus;
        }
    }

    /* Wait for the XSPI bus to go idle (IP command fully transmitted). */
    while (!XSPI_GetBusIdleStatus(base))
    {
    }

    /* Wait for the NOR flash array to finish programming. */
    status = xspi_quad_wait_bus_busy(base);
    if (status != kStatus_Success)
    {
        PRINTF("wait_bus_busy after program failed: 0x%08X\r\n", (unsigned)status);
    }

    return status;
}

void xspi_quad_flash_init(XSPI_Type *base)
{
    xspi_ip_access_config_t xspiIpAccessConfig;
    xspi_ahb_access_config_t xspiAhbAccessConfig;
    xspi_config_t config;

    EXAMPLE_XSPI_CLOCK_INIT();

    while (!XSPI_GetBusIdleStatus(base))
    {
    }

    config.ptrAhbAccessConfig = &xspiAhbAccessConfig;
    config.ptrIpAccessConfig  = &xspiIpAccessConfig;
    XSPI_GetDefaultConfig(&config);

    /*
     * ARDSeqIndex = 0 -> LUT[0]. During init this is a SPI fast-read sequence
     * so AHB reads remain valid while the flash is still in SPI mode; after
     * quad mode is enabled, xspi_quad_set_ahb_read_qpi() installs the Quad
     * I/O read sequence.
     */
    config.ptrAhbAccessConfig->ARDSeqIndex                 = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI; /* LUT[0] */
    config.ptrAhbAccessConfig->enableAHBPrefetch           = true;
    config.ptrAhbAccessConfig->enableAHBBufferWriteFlush   = true;
    config.ptrAhbAccessConfig->ahbSplitSize                = kXSPI_AhbSplitSizeDisabled;
    config.ptrAhbAccessConfig->ahbAlignment                = kXSPI_AhbAlignment256BLimit;
    config.ptrAhbAccessConfig->ptrAhbWriteConfig           = NULL;
    config.ptrAhbAccessConfig->ahbErrorPayload.highPayload = 0x5A5A5A5AUL;
    config.ptrAhbAccessConfig->ahbErrorPayload.lowPayload  = 0x5A5A5A5AUL;

    /* Finite IP-access timeout (MTO). An IP transaction on this XSPI can
     * transiently never complete; with the reset default (0xFFFFFFFF) the
     * blocking read/write helpers spin on it forever. A finite count makes the
     * controller raise TO_ERR so XSPI_TransferBlocking returns kStatus_Timeout
     * and the paced-retry path can recover. ~10.5M IPS cycles (tens of ms) is
     * far beyond any real IP command here (status/ID reads, one-page program
     * are all in the microsecond range). */
    config.ptrIpAccessConfig->ipAccessTimeoutValue           = 0x00A00000UL;
    config.ptrIpAccessConfig->sfpArbitrationLockTimeoutValue = 0xFFFFFFUL;

    /*
     * MDAD/FRAD: only CSSI (the secure subsystem, initiator ID 0b001010) can
     * write MGC/MRC/MTO and SFP_ARB_TIMEOUT after reset. For MDAD and FRAD the
     * access check uses the domain ID, not the initiator ID, so the CM85 core
     * running from M33 domain can configure them here. Provide open MDAD
     * entries (any master accepted) and a single FRAD covering the full XSPI0
     * AHB window so that both AHB and IP transactions are allowed to proceed.
     */
    XSPI_Init(base, &config);

    /*
     * MDAD: allow any master from either target group (TG0 = AHB, TG1 = IP) to
     * access XSPI flash. mask=0 / maskType=0 means the reference ID is ANDed with
     * all-zeros, so every initiator matches.
     */
    xspi_sfp_mdad_config_t s_quadMdadConfig = {
        .tgMdad[0] =
            {
                .assignIsValid        = true,
                .enableDescriptorLock = false,
                .maskType             = 0U, /* ANDed mask */
                .mask                 = 0U, /* of 0: any master ID matches. */
                .masterIdReference    = 0U,
                .secureAttribute      = kXSPI_AttributeMasterNonsecureSecureBoth,
            },
        .tgMdad[1] =
            {
                .assignIsValid        = true,
                .enableDescriptorLock = false,
                .maskType             = 0U,
                .mask                 = 0U,
                .masterIdReference    = 0U,
                .secureAttribute      = kXSPI_AttributeMasterNonsecureSecureBoth,
            },
    };

    /*
     * FRAD: cover the full 256 MB XSPI0 AHB window (0x60000000-0x6FFF0000) with
     * read+write access for both target groups. Remaining FRAD entries are
     * zero-initialized (invalid).
     */
    xspi_sfp_frad_config_t s_quadFradConfig = {
        .fradConfig[0] =
            {
                .startAddress        = 0x60000000UL,
                .endAddress          = 0x6FFF0000UL,
                .tg0MasterAccess     = 0x7U, /* MDnACP: secure/privileged/user writes allowed. */
                .tg1MasterAccess     = 0x7U,
                .assignIsValid       = true,
                .descriptorLock      = kXSPI_DescriptorLockDisabled,
                .exclusiveAccessLock = kXSPI_ExclusiveAccessLockDisabled,
            },
        /* Remaining FRAD entries stay invalid (zero-initialized). */
    };

    /* Configure MDAD/FRAD after init via dedicated API so the struct pointer
     * fields do not need to be present in the compiled header version. */
    XSPI_UpdateSFPConfig(base, &s_quadMdadConfig, &s_quadFradConfig);

    XSPI_SetDeviceConfig(base, &deviceconfig);
    XSPI_UpdateLUT(base, 0, customLUT, CUSTOM_LUT_LENGTH);
    xspi_quad_set_ahb_read_spi(base);
}

/*
 * Set the SR2 Quad Enable (QE) bit so the flash will accept 4-pad transfers.
 * Reads SR2 (0x35) and, only if QE is clear, writes it back with QE set
 * (0x06 + 0x31). Must run in SPI mode before xspi_quad_enter_qpi_mode().
 */
status_t xspi_quad_enable_quad_mode(XSPI_Type *base)
{
    status_t status;
    xspi_transfer_t flashXfer;
    uint32_t readValue[2] = {0U};
    uint32_t writeValue;

    /* Read SR2 (SPI, 1-pad) to check the QE bit. */
    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType         = kXSPI_Read;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR2;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = readValue;
    flashXfer.dataSize        = 8UL; /* ERR052528: min 8 bytes */
    flashXfer.lockArbitration = false;

    status = xspi_quad_transfer_paced(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    if ((readValue[0] & FLASH_QUAD_ENABLE_BIT) == 0U)
    {
        /* Set QE bit: Write Enable (SPI) then Write SR2 (SPI). */
        flashXfer.cmdType  = kXSPI_Command;
        flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_ENABLE;
        flashXfer.data     = NULL;
        flashXfer.dataSize = 0UL;
        status             = xspi_quad_transfer_paced(base, &flashXfer);
        if (status != kStatus_Success)
        {
            return status;
        }

        writeValue         = readValue[0] | FLASH_QUAD_ENABLE_BIT;
        flashXfer.cmdType  = kXSPI_Write;
        flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_SR2;
        flashXfer.data     = &writeValue;
        flashXfer.dataSize = 1UL;
        status             = xspi_quad_transfer_paced(base, &flashXfer);
        if (status != kStatus_Success)
        {
            return status;
        }
        /* tW (Write Status Register) = 15 ms max -- fixed delay avoids
         * needing a separate SPI-mode RDSR1 LUT entry during init. */
        xspi_quad_delay_us(15000U);
        XSPI_ClearAhbBuffer(base);
    }

    /* Quad mode is enabled; install the Quad-I/O read sequence into LUT[0] so
     * AHB memory-mapped reads use it. The flash stays in SPI command mode
     * (Quad I/O 1-4-4) -- it is never put into QPI (4-4-4). */
    xspi_quad_set_ahb_read_qpi(base);

    return kStatus_Success;
}

/*
 * Read flash data via IP command (not AHB).
 * address: flash-relative byte offset, NOT AHB-mapped.
 */
/*
 * Read flash data via the Quad-I/O read sequence (LUT[0]) using EDMA.
 *
 * RBCT.WMRK must be set to (length/4 - 1) before TransferEDMA so the RX
 * buffer watermark matches the full transfer size. The SR1 status polls (8
 * bytes = 2 words) leave WMRK=1; if not re-set here the DMA request fires on
 * 8 bytes while the whole read is issued as one IP transaction, the RX
 * buffer fills before DMA drains it, and the transfer never completes.
 *
 * The destination buffer must be 4-byte aligned and non-cacheable.
 */
status_t xspi_quad_flash_read(XSPI_Type *base, uint32_t address, uint32_t *dst, uint32_t length)
{
    xspi_transfer_t flashXfer;
    status_t status;

    assert(address <= (UINT32_MAX - EXAMPLE_XSPI_AMBA_BASE)); /* CERT-C INT30-C */
    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE + address;
    flashXfer.cmdType         = kXSPI_Read;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = dst;
    flashXfer.dataSize        = length;
    flashXfer.lockArbitration = false;

    assert(length >= 4U && (length % 4U) == 0U && length <= 256U);

    g_completionFlag   = false;
    g_completionStatus = kStatus_Success;

    /* Set WMRK to the full transfer size so DMA fires once for the entire
     * read. */
    base->RBCT = XSPI_RBCT_WMRK((length / 4UL) - 1UL);

    status = XSPI_TransferEDMA(base, &xspiHandle, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    uint32_t waitCount = 0U;
    while (!g_completionFlag)
    {
        xspi_quad_delay_us(100U);
        if (++waitCount > 100000U)
        {
            return kStatus_Timeout;
        }
    }

    return g_completionStatus;
}

status_t xspi_quad_full_init(XSPI_Type *base, uint8_t *vendorId)
{
    status_t status;

    xspi_quad_flash_init(base);

    status = xspi_quad_reset_flash(base);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Enter 4-byte address mode (SR3.ADS=1). The device is 64 MB, so the whole
     * array is only reachable with 4-byte addresses. reset_flash() above left
     * the flash in its power-up 3-byte mode, so 0xB7 must be issued here. */
    status = xspi_quad_enter_4byte_mode(base);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Confirm the flash actually accepted 4-byte mode before addressing beyond
     * 16 MB; abort if SR3.ADS did not latch. */
    status = xspi_quad_check_4byte_mode(base);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = xspi_quad_enable_quad_mode(base);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = xspi_quad_get_vendor_id(base, vendorId);

    XSPI_ClearAhbBuffer(base);
    __DSB();
    __ISB();

    return status;
}

void xspi_quad_reset_and_halt(XSPI_Type *base)
{
    (void)xspi_quad_reset_flash(base);
    for (;;)
    {
    }
}
