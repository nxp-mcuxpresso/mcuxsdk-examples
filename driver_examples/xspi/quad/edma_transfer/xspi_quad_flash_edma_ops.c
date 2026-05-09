/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_xspi.h"
#include "fsl_xspi_edma.h"
#include "fsl_common.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* QE bit is bit 1 of Status Register-2 for W25Q25PWZEIM */
#define FLASH_QUAD_ENABLE_BIT (1U << 1U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 *****************************************************************************/
extern xspi_device_config_t deviceconfig;
extern const uint32_t customLUT[CUSTOM_LUT_LENGTH];
static volatile bool g_completionFlag = false;
extern edma_handle_t dmaTxHandle;
extern edma_handle_t dmaRxHandle;
extern xspi_edma_handle_t xspiHandle;

/*******************************************************************************
 * Code
 * NOTE: All functions in this file are placed in RAM by the linker script so
 * that flash erase/program operations can proceed without the XSPI controller
 * serving code fetches from the same flash.  Do NOT use AT_QUICKACCESS_SECTION_CODE
 * macros here; the linker handles placement of the entire object file.
 ******************************************************************************/

void xspi_quad_invalidate_caches(void)
{
    XCACHE1->CCR |= XCACHE_CCR_INVW0_MASK | XCACHE_CCR_INVW1_MASK | XCACHE_CCR_GO_MASK;
    while ((XCACHE1->CCR & XCACHE_CCR_GO_MASK) != 0U)
    {
    }
    XCACHE1->CCR &= ~(XCACHE_CCR_INVW0_MASK | XCACHE_CCR_INVW1_MASK);

    XSPI_Cache64_InvalidateCache(CACHE64_CTRL0);

    __DSB();
    __ISB();
}

static void xspi_quad_set_ahb_read_spi(XSPI_Type *base)
{
    uint32_t spiLUT[5] = {0U};

    /* During controller init the flash is still in default SPI mode. Keep
     * LUT[0] usable for AHB/XIP until xspi_quad_set_ahb_read_qpi() restores
     * the QPI 4-4-4 read sequence after Enter QPI. */
    spiLUT[0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x0B, kXSPI_Command_RADDR_SDR, kXSPI_1PAD, 0x20);
    spiLUT[1] = XSPI_LUT_SEQ(kXSPI_Command_DUMMY_SDR, kXSPI_1PAD, 0x08, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08);

    XSPI_UpdateLUT(base, 5U * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI, spiLUT, 5U);
    XSPI_ClearAhbAccessSeqPointer(base);
    XSPI_ClearAhbBuffer(base);
}

static void xspi_quad_set_ahb_read_qpi(XSPI_Type *base)
{
    uint32_t qpiLUT[5] = {0U};

    qpiLUT[0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0xEC, kXSPI_Command_RADDR_SDR, kXSPI_4PAD, 0x20);
    qpiLUT[1] = XSPI_LUT_SEQ(kXSPI_Command_MODE_SDR, kXSPI_4PAD, 0xFF, kXSPI_Command_DUMMY_SDR, kXSPI_4PAD, 0x04);
    qpiLUT[2] = XSPI_LUT_SEQ(kXSPI_Command_READ_SDR, kXSPI_4PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x0);

    XSPI_UpdateLUT(base, 5U * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI, qpiLUT, 5U);
    XSPI_ClearAhbAccessSeqPointer(base);
    XSPI_ClearAhbBuffer(base);
}

void xspi_callback(XSPI_Type *base, xspi_edma_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        g_completionFlag = true;
    }
}

/*
 * Write Enable in QPI mode (4-pad, 0x06).
 * Must be issued before every erase, program, or status-register write.
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

    status = XSPI_TransferBlocking(base, &flashXfer);

    return status;
}

/*
 * Software reset sequence for QPI → SPI transition:
 *   1. QPI rescue reset (0x66+0x99 on 4 pads) — covers the case where the
 *      flash is stuck in QPI mode from a previous session.
 *   2. SPI reset (0x66+0x99 on 1 pad) — normal SPI reset path.
 * After this call the flash is in its default SDR / 3-byte-address / SPI state.
 * Call xspi_quad_enter_4byte_mode() and xspi_quad_enable_quad_mode() after.
 * Note: deep power-down recovery (0xAB) is not supported in this example.
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

    /* QPI rescue: 0x66 + 0x99 on 4 pads — exits QPI if flash was left in it. */
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_QPI_RESET_ENABLE;
    status             = XSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_QPI_RESET_MEMORY;
    status             = XSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }
    SDK_DelayAtLeastUs(100U, SystemCoreClock);

    /* SPI reset: 0x66 + 0x99 on 1 pad. */
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_SPI_RESET_ENABLE;
    status             = XSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_SPI_RESET_MEMORY;
    status             = XSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Wait tRST (30 µs max) then flush the XSPI AHB prefetch buffer.
     * NOTE: use XSPI_ClearAhbBuffer here, NOT XSPI_SoftwareReset.
     * XSPI_SoftwareReset performs a heavy SFM+AHB host-domain reset that
     * un-arms the AHB read engine configured by XSPI_Init, which then makes
     * subsequent AHB memory-mapped reads of the flash window fault. The flash
     * is already returned to its default SPI state by the 0x66/0x99 commands
     * above, so only a prefetch-buffer flush is required here. */
    SDK_DelayAtLeastUs(100U, SystemCoreClock);
    XSPI_ClearAhbBuffer(base);

    return status;
}

/*
 * Poll SR1 BUSY bit in QPI mode (4-pad 0x05).
 * Must only be called after the flash has entered QPI mode.
 */
static status_t xspi_quad_wait_bus_busy(XSPI_Type *base)
{
    bool isBusy;
    uint32_t readValue[2] = {0U};
    status_t status;
    xspi_transfer_t flashXfer;

    flashXfer.deviceAddress = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType       = kXSPI_Read;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_QPI_READ_STATUS;
    flashXfer.targetGroup   = kXSPI_TargetGroup0;
    flashXfer.data          = readValue;
    /* ERR052528 workaround: read at least 8 bytes for RDSR commands. */
    flashXfer.dataSize        = 8UL;
    flashXfer.lockArbitration = false;

    do
    {
        status = XSPI_TransferBlocking(base, &flashXfer);
        if (status != kStatus_Success)
        {
            return status;
        }
        if (FLASH_BUSY_STATUS_POL)
        {
            isBusy = ((readValue[0] & (1U << FLASH_BUSY_STATUS_OFFSET)) != 0U);
        }
        else
        {
            isBusy = ((readValue[0] & (1U << FLASH_BUSY_STATUS_OFFSET)) == 0U);
        }
    } while (isBusy);

    return status;
}

/*
 * Enter 4-Byte Address Mode (command 0xB7).
 * The flash exits 4-byte mode on every software reset, so this must be called
 * after xspi_quad_reset_flash() when the device is kXSPI_Device4ByteAddressable.
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

    return XSPI_TransferBlocking(base, &flashXfer);
}

/*
 * Read JEDEC ID in QPI mode (4-pad, 0x9F).
 * Returns the manufacturer ID byte (byte 0 of the 3-byte response).
 * W25Q25PWZEIM: manufacturer=0xEF, memory type=0x60, capacity=0x18.
 * Must only be called after the flash has entered QPI mode.
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

    status_t status = XSPI_TransferBlocking(base, &flashXfer);
    *vendorId       = (uint8_t)(temp[0] & 0xFFUL);

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
    status                    = XSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = xspi_quad_wait_bus_busy(base);

    return status;
}

status_t xspi_quad_flash_page_program(XSPI_Type *base, uint32_t dstAddr, const uint32_t *src)
{
    status_t status;
    xspi_transfer_t flashXfer;

    /* Make sure external flash is not in busy status. */
    status = xspi_quad_wait_bus_busy(base);
    if (kStatus_Success != status)
    {
        return status;
    }

    status = xspi_quad_write_enable(base, dstAddr);
    if (status != kStatus_Success)
    {
        return status;
    }

    assert(dstAddr <= (UINT32_MAX - EXAMPLE_XSPI_AMBA_BASE)); /* CERT-C INT30-C */
    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE + dstAddr;
    flashXfer.cmdType         = kXSPI_Write;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_QPI_PAGE_PROGRAM;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = (uint32_t *)src;
    flashXfer.dataSize        = FLASH_PAGE_SIZE;
    flashXfer.lockArbitration = false;

    g_completionFlag = false;

    status = XSPI_TransferEDMA(base, &xspiHandle, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Wait for transfer completed. */
    while (!g_completionFlag)
    {
        SDK_DelayAtLeastUs(100U, SystemCoreClock);
    }

    status = xspi_quad_wait_bus_busy(base);

    return status;
}

/*
 * Read flash data via QPI SDR (command 0xECh) using EDMA.
 *
 * RBCT.WMRK must be set to (length/4 - 1) before TransferEDMA so that the
 * RX FIFO watermark matches the full transfer size.  RDSR polling reads
 * (8 bytes = 2 words) leave WMRK=1; if not reset here the DMA request fires
 * on 8 bytes but the entire 256-byte read is issued as one IP transaction
 * causing the XSPI RX FIFO to fill before DMA drains it — DMA never fires
 * the terminal count and the transfer hangs.
 *
 * The destination buffer must be 4-byte aligned and non-cacheable (placed in
 * AT_NONCACHEABLE_SECTION_ALIGN with alignment ≥4).
 */
status_t xspi_quad_flash_read(XSPI_Type *base, uint32_t srcAddr, uint32_t *dst, uint32_t length)
{
    status_t status;
    xspi_transfer_t flashXfer;

    assert(srcAddr <= (UINT32_MAX - EXAMPLE_XSPI_AMBA_BASE)); /* CERT-C INT30-C */

    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE + srcAddr;
    flashXfer.cmdType         = kXSPI_Read;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = dst;
    flashXfer.dataSize        = length;
    flashXfer.lockArbitration = false;

    g_completionFlag = false;

    /* Set WMRK to full transfer size so DMA fires once for the entire read. */
    base->RBCT = XSPI_RBCT_WMRK((length / 4UL) - 1UL);

    status = XSPI_TransferEDMA(base, &xspiHandle, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Wait for DMA transfer to complete. */
    while (!g_completionFlag)
    {
        SDK_DelayAtLeastUs(100U, SystemCoreClock);
    }

    return kStatus_Success;
}

/*
 * Read a flash status register using SPI mode (1-pad).
 * Temporarily patches LUT[9] with the desired opcode, then restores it.
 * opcode: 0x05 (SR1), 0x35 (SR2), 0x15 (SR3)
 * Must be called while the flash is in SPI mode (before entering QPI).
 */
status_t xspi_quad_read_spi_register(XSPI_Type *base, uint8_t opcode, uint8_t *regValue)
{
    uint32_t lutSeq[5]    = {0};
    uint32_t readValue[2] = {0U};
    xspi_transfer_t flashXfer;
    status_t status;

    lutSeq[0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, opcode, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08);
    XSPI_UpdateLUT(base, 5U * NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR2, lutSeq, 5U);

    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType         = kXSPI_Read;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR2;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = readValue;
    flashXfer.dataSize        = 8UL;
    flashXfer.lockArbitration = false;

    status    = XSPI_TransferBlocking(base, &flashXfer);
    *regValue = (uint8_t)(readValue[0] & 0xFFU);

    lutSeq[0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x35, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08);
    XSPI_UpdateLUT(base, 5U * NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR2, lutSeq, 5U);

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
    XSPI_Init(base, &config);

    /* Configure flash settings according to serial flash feature. */
    XSPI_SetDeviceConfig(base, &deviceconfig);

    /* Update LUT table. */
    XSPI_UpdateLUT(base, 0, customLUT, CUSTOM_LUT_LENGTH);
    xspi_quad_set_ahb_read_spi(base);
}

/*
 * Enter QPI (4-4-4) mode.
 *
 * Sequence (all SPI-mode commands until 0x38):
 *   1. Check QE bit in SR2 (0x35); write SR2 with QE=1 (0x31) if not set.
 *      QE must be 1 before the flash accepts Enter QPI (0x38).
 *   2. Send Enter QPI (0x38, SPI 1-pad). Flash switches to QPI mode.
 *   3. Send Set Read Parameters (0xC0, QPI 4-pad) with 0x00:
 *      P[6:4]=000 → 6 dummy clocks, max 133MHz. This matches the LUT
 *      entry [0] which uses MODE(FFh, 2 clk) + DUMMY(4 clk) = 6 total.
 *   4. Flush XSPI AHB buffer.
 *
 * After this function returns, all flash commands must use QPI entries
 * (indices 0-6 and 15 in the LUT).
 */
status_t xspi_quad_enable_quad_mode(XSPI_Type *base)
{
    status_t status;
    xspi_transfer_t flashXfer;
    uint32_t readValue[2] = {0U};
    uint32_t writeValue;

    /* Step 1: Read SR2 (SPI, 1-pad) to check QE bit. */
    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType         = kXSPI_Read;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR2;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = readValue;
    flashXfer.dataSize        = 8UL; /* ERR052528: min 8 bytes */
    flashXfer.lockArbitration = false;

    status = XSPI_TransferBlocking(base, &flashXfer);
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
        status             = XSPI_TransferBlocking(base, &flashXfer);
        if (status != kStatus_Success)
        {
            return status;
        }

        writeValue         = readValue[0] | FLASH_QUAD_ENABLE_BIT;
        flashXfer.cmdType  = kXSPI_Write;
        flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_SR2;
        flashXfer.data     = &writeValue;
        flashXfer.dataSize = 1UL;
        status             = XSPI_TransferBlocking(base, &flashXfer);
        if (status != kStatus_Success)
        {
            return status;
        }
        /* tW (Write Status Register) = 15ms max — use fixed delay to avoid
         * needing a separate SPI-mode RDSR1 LUT entry during this init phase. */
        SDK_DelayAtLeastUs(15000U, SystemCoreClock);
        /* Flush the AHB prefetch buffer only; do NOT XSPI_SoftwareReset, which
         * would un-arm the AHB read engine configured by XSPI_Init. */
        XSPI_ClearAhbBuffer(base);
    }

    /* Step 2: Enter QPI mode (0x38, SPI 1-pad).
     * After this the flash responds only to QPI (4-4-4) commands. */
    flashXfer.cmdType  = kXSPI_Command;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_ENTER_QPI;
    flashXfer.data     = NULL;
    flashXfer.dataSize = 0UL;
    status             = XSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Step 3: Set Read Parameters (0xC0, QPI 4-pad).
     * Write P[7:0] = 0x00 → P[6:4]=000: SDR(0xEC) 6 dummy / 133MHz max. */
    writeValue         = 0x00U;
    flashXfer.cmdType  = kXSPI_Write;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_QPI_SET_READ_PARAM;
    flashXfer.data     = &writeValue;
    flashXfer.dataSize = 1UL;
    status             = XSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    xspi_quad_set_ahb_read_qpi(base);

    return kStatus_Success;
}

/*
 * Combined initialization: XSPI controller init + flash reset + 4-byte address
 * mode + QPI (4-4-4) entry + JEDEC ID verification.
 *
 * This function runs entirely from RAM because XSPI_Init() reconfigures the
 * AHB read engine for QPI while the flash is still in SPI mode, breaking XIP
 * until xspi_quad_enable_quad_mode() completes.  No flash-resident code
 * (including PRINTF) may be called between these two points.
 *
 * On success the flash is in QPI 4-4-4 mode and XIP works via QPI — the
 * caller may safely use PRINTF and other flash-resident functions.
 *
 * On failure XIP may be broken; the caller should loop without calling
 * flash-resident code.
 */
status_t xspi_quad_full_init(XSPI_Type *base, uint8_t *vendorId)
{
    status_t status;

    xspi_quad_flash_init(base);

    status = xspi_quad_reset_flash(base);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = xspi_quad_enter_4byte_mode(base);
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

/*
 * Reset flash back to default SPI mode and loop forever.
 *
 * After xspi_quad_reset_flash() returns, the flash is in SPI mode but the
 * AHB read engine is still configured for QPI — XIP is permanently broken.
 * This function never returns so the caller (in flash) is never re-entered.
 * All PRINTFs must be done before calling this function.
 */
void xspi_quad_reset_and_halt(XSPI_Type *base)
{
    (void)xspi_quad_reset_flash(base);
    for (;;)
    {
    }
}
