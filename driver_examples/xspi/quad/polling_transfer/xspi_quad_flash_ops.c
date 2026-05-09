/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_xspi.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
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
    qpiLUT[1] = XSPI_LUT_SEQ(kXSPI_Command_MODE_SDR, kXSPI_4PAD, 0xFF, kXSPI_Command_DUMMY_SDR, kXSPI_4PAD, 0x0A);
    qpiLUT[2] = XSPI_LUT_SEQ(kXSPI_Command_READ_SDR, kXSPI_4PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x0);

    XSPI_UpdateLUT(base, 5U * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI, qpiLUT, 5U);
    XSPI_ClearAhbAccessSeqPointer(base);
    XSPI_ClearAhbBuffer(base);
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

    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType         = kXSPI_Read;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_QPI_READ_STATUS;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = readValue;
    flashXfer.dataSize        = 8UL; /* ERR052528: min 8 bytes */
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
 * Enter 4-Byte Address Mode (SPI mode, command 0xB7).
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

status_t xspi_quad_flash_program(XSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t length)
{
    status_t status;
    xspi_transfer_t flashXfer;
    uint32_t chunkSize;
    uint32_t offset = 0U;

    while (offset < length)
    {
        chunkSize = length - offset;
        if (chunkSize > FLASH_PAGE_SIZE)
        {
            chunkSize = FLASH_PAGE_SIZE;
        }

        status = xspi_quad_wait_bus_busy(base);
        if (status != kStatus_Success)
        {
            return status;
        }

        status = xspi_quad_write_enable(base, dstAddr + offset);
        if (status != kStatus_Success)
        {
            return status;
        }

        assert((dstAddr + offset) <= (UINT32_MAX - EXAMPLE_XSPI_AMBA_BASE)); /* CERT-C INT30-C */
        flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE + dstAddr + offset;
        flashXfer.cmdType         = kXSPI_Write;
        flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_QPI_PAGE_PROGRAM;
        flashXfer.targetGroup     = kXSPI_TargetGroup0;
        flashXfer.data            = (uint32_t *)((uint8_t *)src + offset);
        flashXfer.dataSize        = chunkSize;
        flashXfer.lockArbitration = false;

        status = XSPI_TransferBlocking(base, &flashXfer);
        if (status != kStatus_Success)
        {
            return status;
        }

        status = xspi_quad_wait_bus_busy(base);
        if (status != kStatus_Success)
        {
            return status;
        }

        offset += chunkSize;
    }

    return kStatus_Success;
}

status_t xspi_quad_flash_page_program(XSPI_Type *base, uint32_t dstAddr, const uint32_t *src)
{
    return xspi_quad_flash_program(base, dstAddr, src, FLASH_PAGE_SIZE);
}

/*
 * Read a flash status register using SPI mode (1-pad).
 * Temporarily patches LUT[9] to issue the desired RDSR opcode, reads the
 * register, then restores LUT[9] to its original SPI RDSR2 (0x35) sequence.
 *
 * opcode: 0x05 (SR1), 0x35 (SR2), 0x15 (SR3)
 *
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

    /* Restore LUT[9] to SPI RDSR2 (0x35). */
    lutSeq[0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x35, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08);
    XSPI_UpdateLUT(base, 5U * NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR2, lutSeq, 5U);

    return status;
}

/*
 * Switch LUT[0] from SDR to DTR QPI read and enable DDR in the XSPI controller.
 *
 * DTR Fast Read Quad I/O with 4-Byte Address (0xEEh) in QPI mode:
 *   CMD:   0xEE on 4 pads DDR  (1 clock = 8 bits)
 *   ADDR:  32b  on 4 pads DDR  (4 clocks = 32 bits)
 *   MODE:  0xFF on 4 pads DDR  (1 clock = 8 bits)
 *   DUMMY: 7 clocks            (mode+dummy = 8 total, P[6:4]=000 default, 104 MHz max)
 *   DATA:  on 4 pads DDR
 *
 * The flash Set Read Parameters P[6:4]=000 already configures 8 dummy clocks
 * for DTR (and 6 for SDR) — no additional flash-side command is needed.
 *
 * Caller MUST lower the XSPI clock to ≤104 MHz before calling this function.
 */
void xspi_switch_to_dtr_read(XSPI_Type *base)
{
    uint32_t dtrLUT[5] = {0U};
    xspi_dll_config_t dllCfg;

    /* W25Q25PW QPI DTR Fast Read (0xEE): SDR for the instruction byte.
     * kXSPI_Command_DDR produces all-zero read data on this XSPI controller
     * with quad flash (confirmed across multiple DLL/DQS configurations).
     * Using kXSPI_Command_SDR for the instruction (2 QPI clocks instead of
     * 1 DDR clock) works because the flash in QPI DDR mode samples both
     * edges and sees the held nibble 0xE as valid DDR data.  The resulting
     * data has nibble-swapped bytes, corrected in software by the caller. */
    dtrLUT[0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0xEE, kXSPI_Command_RADDR_DDR, kXSPI_4PAD, 0x20);
    dtrLUT[1] = XSPI_LUT_SEQ(kXSPI_Command_MODE_DDR, kXSPI_4PAD, 0xFF, kXSPI_Command_DUMMY_SDR, kXSPI_4PAD, 0x07);
    dtrLUT[2] = XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_4PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x0);

    XSPI_UpdateLUT(base, 5U * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI, dtrLUT, 5U);

    /* Disable the XSPI module (MDIS=1) before reconfiguring MCR, FLSHCR,
     * and SMPR fields.  Many XSPI register fields are only writable when
     * the module is disabled — mirror the pattern used by XSPI_SetDeviceConfig. */
    XSPI_EnableModule(base, false);

    /* Set FLSHCR.TDH for DDR data alignment with 2x internal reference clock. */
    base->FLSHCR = (base->FLSHCR & ~XSPI_FLSHCR_TDH_MASK) | XSPI_FLSHCR_TDH(kXSPI_DDRDataAlignedWith2xInternalRefClk);

    /* Enable DDR mode.  Keep DQS pad loopback (FA_SEL=2, DQS_EN=1) which
     * self-aligns to actual data transitions and avoids the 1-byte shift
     * seen with dummy pad loopback (FA_SEL=1).  DQS_LAT_EN=1 adds one cycle
     * of DQS latency, producing a clean nibble-swap pattern (each byte has
     * its high and low nibbles reversed).  Without DQS_LAT_EN the pattern
     * is less predictable. */
    base->MCR = (base->MCR & ~(XSPI_MCR_DQS_FA_SEL_MASK)) | XSPI_MCR_DDR_EN_MASK | XSPI_MCR_DQS_EN_MASK |
                XSPI_MCR_DQS_LAT_EN_MASK | XSPI_MCR_DQS_FA_SEL(2U); /* FA_SEL=2, DQS_EN=1, DQS_LAT_EN=1 */

    /* Clear FSPHS and FSDLY — with DQS pad loopback, these do not control
     * the sampling phase (FSPHS only applies to dummy pad loopback FA_SEL=1). */
    base->SMPR &= ~(XSPI_SMPR_FSPHS_MASK | XSPI_SMPR_FSDLY_MASK);

    /* Clear DLLCR[0] to ensure clean state before reconfiguring the DLL.
     * Previous auto-update mode bits (DLLEN, FREQEN, etc.) must be cleared
     * to avoid conflicts when the new configuration is applied. */
    base->DLLCR[0] = 0U;

    /* Re-enable the module before calling XSPI_UpdateDllValue, which does
     * its own MDIS bracket internally. */
    XSPI_EnableModule(base, true);

    /* Use auto-update DLL mode for DDR.  Bypass mode with fixed reference
     * delay taps produced all-zero data — the fixed delay is insufficient
     * for DDR data eye centering at 99 MHz.  Auto-update mode actively
     * tracks the optimal delay.  At 99 MHz (below the 130 MHz threshold),
     * DLLEN is set without FREQEN. */
    dllCfg.dllMode     = kXSPI_AutoUpdateMode;
    dllCfg.useRefValue = true;
    (void)memset(&dllCfg.dllCustomPara, 0, sizeof(dllCfg.dllCustomPara));
    dllCfg.dllCustomDelayTapNum = 0U;
    dllCfg.enableCdl8           = false;
    XSPI_UpdateDllValue(base, &dllCfg, true, false, 99000000U);

    XSPI_ClearAhbBuffer(base);
}

/*
 * Restore LUT[0] to SDR QPI read (0xEC) and disable DDR mode in the controller.
 * Caller should restore the XSPI clock to the SDR frequency afterward.
 */
void xspi_switch_to_sdr_read(XSPI_Type *base)
{
    uint32_t sdrLUT[5] = {0U};
    xspi_dll_config_t dllCfg;

    sdrLUT[0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0xEC, kXSPI_Command_RADDR_SDR, kXSPI_4PAD, 0x20);
    sdrLUT[1] = XSPI_LUT_SEQ(kXSPI_Command_MODE_SDR, kXSPI_4PAD, 0xFF, kXSPI_Command_DUMMY_SDR, kXSPI_4PAD, 0x0A);
    sdrLUT[2] = XSPI_LUT_SEQ(kXSPI_Command_READ_SDR, kXSPI_4PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x0);

    XSPI_UpdateLUT(base, 5U * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI, sdrLUT, 5U);

    /* Disable module before reconfiguring MCR/FLSHCR/SMPR for SDR. */
    XSPI_EnableModule(base, false);

    /* Restore DQS pad loopback (FA_SEL=2, DQS_EN=1), disable DDR + DQS_LAT_EN. */
    base->MCR = (base->MCR & ~(XSPI_MCR_DDR_EN_MASK | XSPI_MCR_DQS_FA_SEL_MASK | XSPI_MCR_DQS_LAT_EN_MASK)) |
                XSPI_MCR_DQS_EN_MASK | XSPI_MCR_DQS_FA_SEL(2U);

    /* Restore FLSHCR.TDH=0 for SDR. */
    base->FLSHCR &= ~XSPI_FLSHCR_TDH_MASK;

    /* Clear FSPHS/FSDLY for SDR. */
    base->SMPR &= ~(XSPI_SMPR_FSPHS_MASK | XSPI_SMPR_FSDLY_MASK);

    XSPI_EnableModule(base, true);

    /* Restore DLL to auto-update mode for the SDR root clock. */
    dllCfg.dllMode     = kXSPI_AutoUpdateMode;
    dllCfg.useRefValue = true;
    (void)memset(&dllCfg.dllCustomPara, 0, sizeof(dllCfg.dllCustomPara));
    dllCfg.dllCustomDelayTapNum = 0U;
    dllCfg.enableCdl8           = false;
    XSPI_UpdateDllValue(base, &dllCfg, false, false, EXAMPLE_XSPI_ROOT_CLOCK_FREQ);

    XSPI_ClearAhbBuffer(base);
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
     * AHB access configuration — explicit values for clarity.
     *
     * ARDSeqIndex = 0 → LUT[0]. During init this is a SPI fast-read sequence
     * so XIP remains valid while the flash is still in SPI mode. After the
     * flash enters QPI mode, xspi_quad_enable_quad_mode() patches LUT[0] to
     * Fast Read Quad I/O (0xEC, QPI 4-4-4) before returning to flash code.
     * enableAHBPrefetch = true  → pre-fetch next cache line on AHB read.
     * enableAHBBufferWriteFlush = true → auto-flush AHB write buffer.
     * ahbErrorPayload = 0x5A5A5A5A → bus-error pattern on AHB timeout.
     *
     * Defaults from XSPI_GetDefaultConfig:
     *   ahbAlignment = kXSPI_AhbAlignmentNoLimit
     *   ahbSplitSize = kXSPI_AhbSplitSizeDisabled
     *   buffer[0..2] = masterId 0..2, 0x80 each, no priority
     *   buffer[3]    = masterId 3, 0x80, allMaster=true
     *   ptrAhbWriteConfig = NULL (no AHB write in this demo)
     */
    config.ptrAhbAccessConfig->ARDSeqIndex                 = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI; /* LUT[0] */
    config.ptrAhbAccessConfig->enableAHBPrefetch           = true; /* default=true, explicit for clarity */
    config.ptrAhbAccessConfig->enableAHBBufferWriteFlush   = true; /* default=false, override */
    config.ptrAhbAccessConfig->ahbSplitSize                = kXSPI_AhbSplitSizeDisabled;
    config.ptrAhbAccessConfig->ahbAlignment                = kXSPI_AhbAlignment256BLimit;
    config.ptrAhbAccessConfig->ptrAhbWriteConfig           = NULL;
    config.ptrAhbAccessConfig->ahbErrorPayload.highPayload = 0x5A5A5A5AUL;
    config.ptrAhbAccessConfig->ahbErrorPayload.lowPayload  = 0x5A5A5A5AUL;

    /*
     * IP access configuration — explicit timeout values.
     *
     * ipAccessTimeoutValue = 0xFFFFFFFF → max IP cmd timeout (default).
     * sfpArbitrationLockTimeoutValue = 0xFFFFFF → max SFP lock timeout.
     * SFP (FRAD/MDAD) = NULL → disabled; no secure-flash access checks.
     */
    config.ptrIpAccessConfig->ipAccessTimeoutValue           = 0xFFFFFFFFUL; /* default */
    config.ptrIpAccessConfig->sfpArbitrationLockTimeoutValue = 0xFFFFFFUL;

    XSPI_Init(base, &config);

    XSPI_SetDeviceConfig(base, &deviceconfig);
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
 *   3. Send Set Read Parameters (0xC0, QPI 4-pad) with 0x50:
 *      P[6:4]=101 → 12 dummy clocks, max 166MHz. This matches the LUT
 *      entry [0] which uses MODE(FFh, 2 clk) + DUMMY(10 clk) = 12 total.
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
     * Write P[7:0] = 0x50 → P[6:4]=101: SDR(0xEC) 12 dummy / 166MHz max. */
    writeValue         = 0x50U;
    flashXfer.cmdType  = kXSPI_Write;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_QPI_SET_READ_PARAM;
    flashXfer.data     = &writeValue;
    flashXfer.dataSize = 1UL;
    status             = XSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Step 4: switch the AHB/XIP read path to QPI before returning to flash. */
    xspi_quad_set_ahb_read_qpi(base);

    return kStatus_Success;
}

/*
 * Read flash data via IP command (not AHB).
 * Uses LUT[0] = Fast Read Quad I/O (0xEC, QPI 4-4-4, 4-byte addr).
 *
 * address: flash-relative byte offset (e.g. 0x100000), NOT AHB-mapped.
 */
status_t xspi_quad_flash_read(XSPI_Type *base, uint32_t address, uint32_t *dst, uint32_t length)
{
    xspi_transfer_t flashXfer;
    status_t status;
    uint32_t offset = 0U;
    uint32_t chunkSize;

    /* The XSPI IP RX buffer on this device is 256 bytes (RBDR[64]).
     * Reading more than 256 bytes in a single IP access causes data corruption.
     * Split into 256-byte chunks. */
    while (offset < length)
    {
        chunkSize = length - offset;
        if (chunkSize > (XSPI_RBDR_ARRAY_COUNT * 4U))
        {
            chunkSize = (XSPI_RBDR_ARRAY_COUNT * 4U);
        }

        assert((address + offset) <= (UINT32_MAX - EXAMPLE_XSPI_AMBA_BASE)); /* CERT-C INT30-C */
        flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE + address + offset;
        flashXfer.cmdType         = kXSPI_Read;
        flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI;
        flashXfer.targetGroup     = kXSPI_TargetGroup0;
        flashXfer.data            = (uint32_t *)((uint8_t *)dst + offset);
        flashXfer.dataSize        = chunkSize;
        flashXfer.lockArbitration = false;

        status = XSPI_TransferBlocking(base, &flashXfer);
        if (status != kStatus_Success)
        {
            return status;
        }

        offset += chunkSize;
    }

    return kStatus_Success;
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

void xspi_quad_reset_and_halt(XSPI_Type *base)
{
    (void)xspi_quad_reset_flash(base);
    for (;;)
    {
    }
}
