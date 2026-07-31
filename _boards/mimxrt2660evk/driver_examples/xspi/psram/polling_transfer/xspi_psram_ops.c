/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Board-specific replacement for the shared xspi_psram_ops.c (swapped in by
 * reconfig.cmake): the MIMXRT2660-EVK PSRAM is an AP Memory APS512XXN-OBx9
 * DDR OPI/HPI Xccela device, not a HyperBus one. The Xccela command set
 * (1-byte instruction + 32-bit DDR address, dedicated mode-register read /
 * write instructions) and the X8-power-up -> X16 switch sequence have
 * nothing in common with the HyperBus CA framing the shared file implements,
 * so all device handling lives here. The exported function names are kept so
 * the shared example main links unchanged.
 *
 * The bus runs at the device maximum of 250 MHz (boot clocks already place
 * XSPI1 at SYSPLLDIV4 / SND_DIV2 = 250 MHz SCK). At the 250 MHz latency code
 * the device does not support RBX (row-boundary-crossing reads), so every
 * burst is kept inside the 2 KB device row by the controller instead: AHB
 * read prefetch is bounded to 1 KB alignment, and every transaction is kept
 * at the recommended 512-byte transfer size (write page split, read
 * prefetch amount - which also keeps CE# low time well below the 1 us tCEM
 * extended-temperature limit); IP commands are issued 1 KB aligned by the
 * example.
 *
 * On PSRAM-resident targets (psram/psram_txt/xspi_nor_psram) this file, the
 * XSPI driver, and hardware_init.c are pinned into ITCM/DTCM by the
 * example-private linker scripts (../linker/), so the controller can be
 * re-programmed while the image lives behind it; Global Reset is skipped
 * there because the datasheet does not guarantee memory content across it.
 */

#include "fsl_xspi.h"
#include "fsl_llc.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* APS512XXN mode register map (datasheet section 7.7, Table 3). */
#define APS512XXN_MR0 0x00U /* Latency type / read latency / drive strength. */
#define APS512XXN_MR1 0x01U /* Vendor ID (read-only). */
#define APS512XXN_MR2 0x02U /* Good-die / device ID / density (read-only). */
#define APS512XXN_MR4 0x04U /* Write latency / refresh rate / PASR. */
#define APS512XXN_MR8 0x08U /* X8-X16 mode / RBX / burst type / burst length. */

/* MR0: read latency type (bit 5, Table 4). */
#define APS512XXN_MR0_LATENCY_VARIABLE 0x00U
#define APS512XXN_MR0_LATENCY_FIXED    0x20U
/* MR0: read latency code (bits 4:2, Table 5). */
#define APS512XXN_MR0_READ_LATENCY_5    (0x02U << 2) /* Up to 133 MHz (power-up default). */
#define APS512XXN_MR0_READ_LATENCY_7    (0x04U << 2) /* Up to 200 MHz. */
#define APS512XXN_MR0_READ_LATENCY_10   (0x06U << 2) /* Up to 250 MHz. */
#define APS512XXN_MR0_READ_LATENCY_MASK ((0x07U << 2) | APS512XXN_MR0_LATENCY_FIXED)
/* MR0: output drive strength (bits 1:0, Table 7). */
#define APS512XXN_MR0_DRIVE_FULL    0x00U /* 25 Ohm (default). */
#define APS512XXN_MR0_DRIVE_HALF    0x01U /* 50 Ohm. */
#define APS512XXN_MR0_DRIVE_QUARTER 0x02U /* 100 Ohm. */
#define APS512XXN_MR0_DRIVE_EIGHTH  0x03U /* 200 Ohm. */
#define APS512XXN_MR0_DRIVE_MASK    0x03U

/* MR1: vendor ID (bits 4:0, Table 9). */
#define APS512XXN_MR1_VENDOR_MASK 0x1FU
#define APS512XXN_MR1_VENDOR_APM  0x0DU

/* MR4: write latency code (bits 7:5, Table 15). */
#define APS512XXN_MR4_WRITE_LATENCY_5    (0x02U << 5) /* Up to 133 MHz (power-up default). */
#define APS512XXN_MR4_WRITE_LATENCY_7    (0x01U << 5) /* Up to 200 MHz. */
#define APS512XXN_MR4_WRITE_LATENCY_9    (0x03U << 5) /* Up to 250 MHz. */
#define APS512XXN_MR4_WRITE_LATENCY_MASK (0x07U << 5)
/* MR4: refresh frequency (bits 4:3): temperature-compensated slow refresh. */
#define APS512XXN_MR4_REFRESH_TC_SLOW (0x01U << 3)
/* MR4: partial-array self refresh (bits 2:0): full array. */
#define APS512XXN_MR4_PASR_FULL 0x00U

/* MR8: IO mode (bit 6, Table 19). */
#define APS512XXN_MR8_IO_X16 0x40U

/*
 * LUT dummy cycles:
 *  - reads: data capture is strobed by the device DQS (armed from the dummy
 *    phase on, and SCK runs until the byte count is received), so the frame
 *    timing is set by the device latency, not by this operand - any value
 *    from the bus-turnaround minimum up works (verified on silicon with a
 *    2..19 sweep). The nominal latency counts are used for readability:
 *    memory read LC = 10 (MR0, Table 5, 250 MHz), register read LC - 1 = 9
 *    (Table 6, above 200 MHz).
 *  - memory write: operand 8 for WLC = 9 clocks at 250 MHz (swept on
 *    silicon; the operand has a one-cycle offset against the device WLC
 *    count); writes have no strobe feedback, so this must track the device
 *    MR4 setting exactly.
 */
#define APS512XXN_LUT_DUMMY_READ     10U
#define APS512XXN_LUT_DUMMY_WRITE    8U
#define APS512XXN_LUT_DUMMY_REG_READ 9U

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*
 * On RT2660 the XSPI SFP checks are enabled and locked in MGC
 * (GVLD | GVLDMDAD | GVLDFRAD, MGC is write-protected), so every IP command
 * requires valid MDAD/FRAD descriptors - otherwise the SFAR request is
 * rejected with ERRSTAT[IPS_ERR] and IPSERROR reports
 * MDADPROG/FRADPROG = "no descriptors programmed" (observed 0xC001), and the
 * following IPCR write escalates to a bus fault. Program permissive
 * descriptors: both target groups accept any master, one FRAD region grants
 * write access to the whole PSRAM window (reads are never FRAD-restricted).
 */
static xspi_sfp_mdad_config_t s_psramMdadConfig = {
    .tgMdad[0] =
        {
            .assignIsValid        = true,
            .enableDescriptorLock = false,
            .maskType             = 0U, /* ANDed mask ... */
            .mask                 = 0U, /* ... of 0: any master ID matches. */
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

static xspi_sfp_frad_config_t s_psramFradConfig = {
    .fradConfig[0] =
        {
            /* Whole PSRAM space, covering the direct (0x8000_0000) and the
             * LLC-cached (0x8800_0000) AHB paths; 64 KB granularity. */
            .startAddress        = 0x80000000UL,
            .endAddress          = 0x8FFF0000UL,
            .tg0MasterAccess     = 0x7U, /* MDnACP: secure/privileged/user writes allowed. */
            .tg1MasterAccess     = 0x7U,
            .assignIsValid       = true,
            .descriptorLock      = kXSPI_DescriptorLockDisabled,
            .exclusiveAccessLock = kXSPI_ExclusiveAccessLockDisabled,
        },
    /* Remaining FRAD entries stay invalid (zero-initialized). */
};

static xspi_device_ddr_config_t s_psramDdrConfig = {
    .ddrDataAlignedClk         = kXSPI_DDRDataAlignedWith2xInternalRefClk,
    .enableByteSwapInOctalMode = false,
    .enableDdr                 = true,
};

static xspi_device_config_t s_psramConfig = {
    .xspiRootClk  = 500000000, /* SYSPLLDIV4 (BOARD_InitBootClocks); SCK = 250 MHz (SND_DIV /2). */
    .enableCknPad = false,
    /* The Xccela HPI interface is not HyperBus, but it needs the same
     * controller services this path enables: DQS_OUT_EN (host drives DQS/DM
     * as the write data mask) and the X16 data-bus controls. The command
     * sequences themselves are fully defined by the LUT below. */
    .deviceInterface = kXSPI_HyperBus,
    /* Controller X16 must stay DISABLED while the device is in X8 mode:
     * measured on silicon, MCR[X16_EN] also gates the IP-command RX capture
     * (with it set and only DQS0 toggling, register reads return zero
     * bytes). APS512XXN_EnterX16Mode() switches device and controller
     * together after the mode registers are programmed. */
    .interfaceSettings.hyperBusSettings.x16Mode = kXSPI_x16ModeDisable,
    /* VAR_LAT_EN implements the HyperBus RWDS-signaled 2x latency; Xccela
     * signals read pushout through the DQS preamble instead, which the
     * external-DQS sample path absorbs. */
    .interfaceSettings.hyperBusSettings.enableVariableLatency = false,
    .interfaceSettings.hyperBusSettings.forceBit10To1         = false,
    /* 512-byte transaction split (vendor-recommended transfer size at
     * 250 MHz): keeps CE# low inside tCEM and write bursts inside the 2 KB
     * device row (no RBX at 250 MHz). Applies to the AHB write engine
     * (PPWB) and the IP write page split alike. */
    .interfaceSettings.hyperBusSettings.pageSize = 512,
    .CSHoldTime                                  = 3,
    .CSSetupTime                                 = 3,
    .sampleClkConfig.sampleClkSource             = kXSPI_SampleClkFromExternalDQS,
    .sampleClkConfig.dllConfig.dllMode           = kXSPI_AutoUpdateMode,
    .sampleClkConfig.dllConfig.useRefValue       = true,
    .sampleClkConfig.dllConfig.enableCdl8        = true,
    /* Byte addressable while the device is in X8 mode (keeps the MR address
     * mapping 1:1); switched to 2-byte (word) addressable together with X16
     * entry - the device is word addressable in X16. */
    .addrMode              = kXSPI_DeviceByteAddressable,
    .columnAddrWidth       = 0U,
    .enableCASInterleaving = false,
    /* 512 Mb = 64 MB total, single CE# (the two internal 256 Mb dies are
     * selected by AX[14] transparently). Unit is KB, split across the two
     * SFAD entries. */
    .deviceSize[0]      = 0x8000U,
    .deviceSize[1]      = 0x8000U,
    .ptrDeviceRegInfo   = NULL,
    .ptrDeviceDdrConfig = &s_psramDdrConfig,
};

/*
 * APS512XXN Xccela command set. The instruction occupies the whole first
 * clock: the device latches it on the first rising edge, so the byte is
 * driven on both DDR half-cycles (two kXSPI_Command_DDR entries with the
 * same opcode - a single half-cycle instruction shifts the entire address
 * phase by half a clock and the device decodes a wrong address). The 32-bit
 * DDR address follows; its LSB doubles as the mode-register address (MA)
 * for the register commands. Sequence shape matches the silicon-validated
 * boot-stage configuration.
 *
 * Errata ERR052528: read sequences (including register reads) must use a
 * LUT data size of at least 8 bytes.
 */
static const uint32_t s_psramLUT[CUSTOM_LUT_LENGTH] = {
    /* Linear Burst Read, 20h (ignores the MR8 burst configuration; without
     * RBX it wraps at the 2 KB row boundary - the controller keeps bursts
     * inside the row, see the file header). */
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ + 0] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_DDR, kXSPI_8PAD, 0x20),
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ + 1] = XSPI_LUT_SEQ(
        kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, APS512XXN_LUT_DUMMY_READ),
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ + 2] =
        XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x00),

    /* Linear Burst Write, A0h. Write data is masked per byte lane by DQS/DM
     * driven from the host (DQS_OUT_EN). */
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 0] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xA0, kXSPI_Command_DDR, kXSPI_8PAD, 0xA0),
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 1] = XSPI_LUT_SEQ(
        kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, APS512XXN_LUT_DUMMY_WRITE),
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 2] =
        XSPI_LUT_SEQ(kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x00),

    /* Mode Register Read, 40h: MA in the address LSB, data on DQ[7:0]. */
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ + 0] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x40, kXSPI_Command_DDR, kXSPI_8PAD, 0x40),
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ + 1] = XSPI_LUT_SEQ(
        kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, APS512XXN_LUT_DUMMY_REG_READ),
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ + 2] =
        XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x00),

    /* Mode Register Write, C0h: write latency 1, no dummy phase - the MR
     * value is driven as TWO DDR bytes right after the address (the device
     * latches the second half-cycle; a single byte ends the frame before
     * the latch point and the write is dropped - measured on silicon and
     * matching the validated boot-stage sequence). */
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 0] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xC0, kXSPI_Command_DDR, kXSPI_8PAD, 0xC0),
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x02),
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 2] =
        XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_1PAD, 0x00, kXSPI_Command_STOP, kXSPI_1PAD, 0x00),

    /* Global Reset, FFh: instruction-only frame, CE# low for >= 4 clocks. */
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_RESET + 0] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xFF, kXSPI_Command_DDR, kXSPI_8PAD, 0xFF),
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_RESET + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 6, kXSPI_Command_STOP, kXSPI_1PAD, 0x00),
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * APS512XXN mode register access.
 *
 * All register commands transfer on DQ[7:0] only (in X8 and X16 mode alike;
 * the controller X16 setting only affects AHB accesses). They address the
 * device through BOARD_XSPI1_AMBA_BASE_DIRECT (SFAR addresses live on the
 * direct map) so the MA byte is not disturbed by the test-window offset or
 * word-address conversion.
 */
static status_t APS512XXN_WriteModeReg(XSPI_Type *base, uint8_t regAddr, uint8_t value)
{
    /* The MR value is transferred as two identical DDR bytes (one full
     * clock) - see the register-write LUT sequence comment. */
    uint32_t regVal           = (uint32_t)value | ((uint32_t)value << 8);
    xspi_transfer_t flashXfer = {
        .deviceAddress   = BOARD_XSPI1_AMBA_BASE_DIRECT + regAddr,
        .cmdType         = kXSPI_Write,
        .seqIndex        = HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE,
        .targetGroup     = kXSPI_TargetGroup0,
        .data            = &regVal,
        .dataSize        = 2,
        .lockArbitration = false,
    };

    return XSPI_TransferBlocking(base, &flashXfer);
}

static status_t APS512XXN_ReadModeReg(XSPI_Type *base, uint8_t regAddr, uint8_t *value)
{
    uint32_t readBuffer[2]    = {0U, 0U}; /* Minimum read size: ERR052528 + external-DQS 8-byte rule. */
    xspi_transfer_t flashXfer = {
        /* The controller floors the IP address to an even value, so always
         * read the even register-pair window [MRn, MRn+1] and pick the
         * requested byte out of it (measured on silicon: an odd MA reaches
         * the device as MA-1). */
        .deviceAddress   = BOARD_XSPI1_AMBA_BASE_DIRECT + ((uint32_t)regAddr & ~1UL),
        .cmdType         = kXSPI_Read,
        .seqIndex        = HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ,
        .targetGroup     = kXSPI_TargetGroup0,
        .data            = readBuffer,
        .dataSize        = 8,
        .lockArbitration = false,
    };
    status_t status;

    status = XSPI_TransferBlocking(base, &flashXfer);
    *value = (uint8_t)((readBuffer[0] >> (((uint32_t)regAddr & 1UL) * 8UL)) & 0xFFU);

    return status;
}

/* Program the read latency type/code (MR0) and write latency / refresh /
 * PASR (MR4). Both are written blind: the bring-up sequence runs them at
 * low speed before the device can be read back reliably, and every field
 * is defined explicitly (vendor-recommended values). Drive strength is
 * reset to full here; use APS512XXN_SetDriveStrength() afterwards to
 * change it. */
static status_t APS512XXN_SetLatency(XSPI_Type *base, uint8_t latencyType, uint8_t readLatency, uint8_t writeLatency)
{
    status_t status;

    status = APS512XXN_WriteModeReg(base, APS512XXN_MR0, latencyType | readLatency | APS512XXN_MR0_DRIVE_FULL);
    if (status != kStatus_Success)
    {
        return status;
    }

    return APS512XXN_WriteModeReg(base, APS512XXN_MR4,
                                  writeLatency | APS512XXN_MR4_REFRESH_TC_SLOW | APS512XXN_MR4_PASR_FULL);
}

/* Set the output drive strength (MR0[1:0]), preserving the latency bits. */
static status_t APS512XXN_SetDriveStrength(XSPI_Type *base, uint8_t strength)
{
    status_t status;
    uint8_t mr0 = 0U;

    status = APS512XXN_ReadModeReg(base, APS512XXN_MR0, &mr0);
    if (status != kStatus_Success)
    {
        return status;
    }
    mr0 = (uint8_t)((mr0 & ~APS512XXN_MR0_DRIVE_MASK) | strength);

    return APS512XXN_WriteModeReg(base, APS512XXN_MR0, mr0);
}

/* Check that a device answers with the AP Memory vendor ID (MR1[4:0]). */
static status_t APS512XXN_VerifyVendorId(XSPI_Type *base)
{
    status_t status;
    uint8_t mr1 = 0U;

    status = APS512XXN_ReadModeReg(base, APS512XXN_MR1, &mr1);
    if (status != kStatus_Success)
    {
        return status;
    }

    return ((mr1 & APS512XXN_MR1_VENDOR_MASK) == APS512XXN_MR1_VENDOR_APM) ? kStatus_Success : kStatus_Fail;
}

/* Verify the device reached X16 mode (MR8[6], written blind during the
 * low-speed bring-up), then switch the controller to match. Mode register
 * data stays on DQ[7:0], so the MR8 read-back works with the controller
 * still in X8 mode; afterwards the controller data bus is widened to 16
 * bits (command/address stay on DQ[7:0], HPI style) and the device
 * addressing switches to 2-byte (word) units. */
static status_t APS512XXN_VerifyAndEnterX16Mode(XSPI_Type *base)
{
    status_t status;
    uint8_t mr8 = 0U;

    status = APS512XXN_ReadModeReg(base, APS512XXN_MR8, &mr8);
    if ((status != kStatus_Success) || ((mr8 & APS512XXN_MR8_IO_X16) == 0U))
    {
        return kStatus_Fail;
    }

    /* Device is in X16: widen the controller data bus and switch to word
     * (2-byte) addressing. */
    XSPI_SetHyperBusX16Mode(base, kXSPI_x16ModeEnabledOnlyData);

    return XSPI_UpdateDeviceAddrMode(base, kXSPI_Device2ByteAddressable);
}

/* Serial-clock ratio switch (CLK_CFG). CSINCFG (chip-select inactive width)
 * is kept at 8 SCK in both settings: at 250 MHz that is 32 ns >= the 28 ns
 * tCPH the device needs between back-to-back transactions. */
static void XSPI1_SetSckRatio(XSPI_Type *base, uint32_t ratio)
{
    XSPI_EnableModule(base, false);
    base->CLK_CFG = XSPI_CLK_CFG_CLK_RTO(ratio) | XSPI_CLK_CFG_CSINCFG(8U);
    XSPI_EnableModule(base, true);
}

void xspi_hyper_ram_init(XSPI_Type *base)
{
    xspi_ip_access_config_t psramIpAccessConfig;
    xspi_ahb_access_config_t psramAhbAccessConfig;
    xspi_ahb_write_config_t psramAhbWriteConfig = {
        .AWRSeqIndex        = HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE,
        .blockRead          = false,
        .blockSequenceWrite = false,
    };
    xspi_config_t config;

    /* To store the LUT in local (stack) memory: the LUT update below must
     * not fetch its source from behind the controller being re-programmed.
     * Copied by hand - libc memcpy is not pinned to ITCM and must not be
     * executed on PSRAM-resident targets from here on. */
    uint32_t tempLUT[CUSTOM_LUT_LENGTH];
    uint32_t lutIdx;

    for (lutIdx = 0U; lutIdx < (uint32_t)CUSTOM_LUT_LENGTH; lutIdx++)
    {
        tempLUT[lutIdx] = s_psramLUT[lutIdx];
    }

    /* XSPI1 is usable out of reset (module clock and root clock are already
     * set up by BOARD_InitBootClocks, 250 MHz SCK): no module reset or clock
     * handling is needed before re-programming the controller - XSPI_Init
     * and XSPI_SetDeviceConfig rewrite every controller register anyway.
     * On PSRAM-resident targets, from here until the X16 switch completes
     * nothing may execute from or access the PSRAM (this file is pinned to
     * ITCM/DTCM). */

    /* No PSRAM traffic may hit the controller while it is re-programmed and
     * the device is switched to 250 MHz/X16: push every cached PSRAM byte
     * back through the still-working boot-stage configuration and keep the
     * CPU caches off (no speculative line fills) until the bring-up is
     * complete. Disabling the D-cache cleans + invalidates it; the LLC is
     * flushed through the same working path right after. */
    SCB_DisableDCache();
    SCB_DisableICache();
    (void)LLC_CleanInvalidateCache(CMPT__LLC);

    config.ptrAhbAccessConfig = &psramAhbAccessConfig;
    config.ptrIpAccessConfig  = &psramIpAccessConfig;

    XSPI_GetDefaultConfig(&config);

    config.ptrAhbAccessConfig->ahbErrorPayload.highPayload = 0x5A5A5A5AUL;
    config.ptrAhbAccessConfig->ahbErrorPayload.lowPayload  = 0x5A5A5A5AUL;
    config.ptrAhbAccessConfig->ARDSeqIndex                 = HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ;
    config.ptrAhbAccessConfig->enableAHBBufferWriteFlush   = true;
    config.ptrAhbAccessConfig->enableAHBPrefetch           = true;
    /* No RBX at 250 MHz: bound AHB read prefetch bursts to 1 KB alignment
     * so a linear-burst read can never wrap inside the 2 KB device row. */
    /* No RBX at 250 MHz: bound AHB read prefetch bursts to 1 KB alignment
     * so a linear-burst read can never wrap inside the 2 KB device row. */
    config.ptrAhbAccessConfig->ahbAlignment      = kXSPI_AhbAlignment1KBLimit;
    config.ptrAhbAccessConfig->ptrAhbWriteConfig = &psramAhbWriteConfig;

    config.ptrIpAccessConfig->ipAccessTimeoutValue           = 0xFFFFFFFFUL;
    config.ptrIpAccessConfig->ptrSfpFradConfig               = &s_psramFradConfig;
    config.ptrIpAccessConfig->ptrSfpMdadConfig               = &s_psramMdadConfig;
    config.ptrIpAccessConfig->sfpArbitrationLockTimeoutValue = 0xFFFFFFUL;

    XSPI_Init(base, &config);
    XSPI_SetDeviceConfig(base, &s_psramConfig);

    /* Match the boot-stage (validated) SFACR TX-path settings, which the
     * driver never programs: TX-buffer residue flushing, TBDR gating, and a
     * read-side wait boundary (PRWB) mirroring the 512-byte write split. */
    base->SFACR |= XSPI_SFACR_PRWB(9U) | XSPI_SFACR_TBDRGTEN_MASK | XSPI_SFACR_TX_FLUSH_MASK;

    /* Bound the AHB read prefetch transaction to the same 512 bytes
     * (ADATSZ unit: 8 bytes). The driver sets the prefetch amount equal to
     * the 1 KB buffer partition size, which exceeds the recommended
     * transfer size at 250 MHz. */
    for (uint32_t bufIdx = 0U; bufIdx < XSPI_BUFCR_COUNT; bufIdx++)
    {
        base->BUFCR[bufIdx] = (base->BUFCR[bufIdx] & ~XSPI_BUFCR_ADATSZ_MASK) | XSPI_BUFCR_ADATSZ(0x40U);
    }

    XSPI_UpdateLUT(base, 0, tempLUT, CUSTOM_LUT_LENGTH);

    /* Device bring-up per the vendor-recommended sequence: the three
     * mode-register writes are performed below 133 MHz (all blind writes -
     * register writes always use write latency 1), then the bus is
     * switched to 250 MHz for the read-back verification and the X16
     * switch. No Global Reset - it would clear the array on PSRAM-resident
     * targets and is not needed to bring the device into our configuration.
     * Retry forever on failure - there is no console access here
     * (PSRAM-resident targets must not call into flash/PSRAM library code
     * at this point). */
    for (;;)
    {
        /* 500 MHz root / 4 = 125 MHz SCK for the bring-up writes. */
        XSPI1_SetSckRatio(base, 3U);

        if (APS512XXN_SetLatency(base, APS512XXN_MR0_LATENCY_VARIABLE, APS512XXN_MR0_READ_LATENCY_10,
                                 APS512XXN_MR4_WRITE_LATENCY_9) != kStatus_Success)
        {
            continue;
        }

        if (APS512XXN_WriteModeReg(base, APS512XXN_MR8, APS512XXN_MR8_IO_X16) != kStatus_Success)
        {
            continue;
        }

        /* Device fully configured: back to 250 MHz (root / 2). */
        XSPI1_SetSckRatio(base, 1U);

        /* Re-arm the read DLL with the silicon-validated working value
         * (DLLEN | FREQEN | REFCNTR=2 | RES=6 | CDL8 | AUTO_UPD | SLV_EN):
         * XSPI_UpdateDllValue() cannot produce it - in auto-update mode it
         * never sets CDL8 and uses RES=4 - and with the driver value the
         * read sampling margin at 250 MHz is not sufficient (intermittent
         * data corruption). Same limitation and value as documented in
         * BOARD_InitBootClocks. */
        base->DLLCR[0] = 0U;
        base->DLLCR[0] = 0xC260001CU;
        while ((base->DLLSR & XSPI_DLLSR_SLVA_LOCK_MASK) == 0U)
        {
        }

        if (APS512XXN_VerifyVendorId(base) != kStatus_Success)
        {
            continue;
        }

        /* Exercise the drive-strength read-modify-write path at speed. */
        if (APS512XXN_SetDriveStrength(base, APS512XXN_MR0_DRIVE_FULL) != kStatus_Success)
        {
            continue;
        }

        if (APS512XXN_VerifyAndEnterX16Mode(base) == kStatus_Success)
        {
            break;
        }
    }

    /* PSRAM path fully operational: both enables start from an invalidated
     * cache (CMSIS invalidates before setting the enable bit). */
    SCB_EnableICache();
    SCB_EnableDCache();
}

status_t xspi_hyper_ram_ipcommand_write_data(XSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    xspi_transfer_t flashXfer;
    status_t status;

    /* The IP engine accesses the device directly: clean+invalidate any LLC
     * lines the AHB path holds for this range first, so no stale dirty line
     * can write back over the IP-written data (and later AHB reads re-fill
     * from the device). */
    (void)LLC_CleanInvalidateCacheByRange(EXAMPLE_XSPI_AMBA_BASE + address, length);

    /* Write data. The driver splits the transfer into 256-byte (pageSize)
     * frames; start address and length must keep each frame inside a 2 KB
     * device row (the example uses 1 KB-aligned accesses). */
    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE_DIRECT + address;
    flashXfer.cmdType         = kXSPI_Write;
    flashXfer.seqIndex        = HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = buffer;
    flashXfer.dataSize        = length;
    flashXfer.lockArbitration = false;

    status = XSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t xspi_hyper_ram_ipcommand_read_data(XSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    xspi_transfer_t flashXfer;
    status_t status;

    /* Same LLC discipline as the IP write path. */
    (void)LLC_CleanInvalidateCacheByRange(EXAMPLE_XSPI_AMBA_BASE + address, length);

    /* Read data (split into 512-byte chunks by the driver; same row-bound
     * alignment rule as the write path). */
    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE_DIRECT + address;
    flashXfer.cmdType         = kXSPI_Read;
    flashXfer.seqIndex        = HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = buffer;
    flashXfer.dataSize        = length;
    flashXfer.lockArbitration = false;

    status = XSPI_TransferBlocking(base, &flashXfer);

    return status;
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
