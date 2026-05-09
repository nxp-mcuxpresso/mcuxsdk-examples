/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_xspi.h"
/*${header:end}*/

/*${variable:start}*/
/*
 * Device configuration for W25Q25PWZEIM NOR flash on XSPI0.
 *
 * Interface:  QPI 4-4-4 SDR — command, address, mode, and data all on
 *             4 pads.  The flash enters QPI mode via 0x38 after QE is
 *             set in SR2.  All operational LUT entries (indices 0-6)
 *             use 4-pad for every phase.
 *
 * Clock:      Audio PLL PFD0 feeds XSPI0 with divider 1 to make a
 *             330.5 MHz XSPI0 root clock. The serial clock driven to the
 *             flash is half of the root clock, so the flash sees ~165 MHz.
 *             xspiRootClk must be the root clock so the driver computes
 *             CS timing and timeout counts correctly.
 *
 * Sample clock: DQS pad loopback — the XSPI controller feeds its own
 *             output clock back through the DQS pad to sample incoming
 *             data.  No flash-generated DQS strobe is required for SDR
 *             mode at ≤133 MHz.
 *
 * CS timing:  CSSetupTime / CSHoldTime = 3 serial-clock cycles each,
 *             satisfying the W25Q25PWZEIM tCSS and tCSH requirements
 *             with margin at 165 MHz.
 *
 * Address:    4-byte mode (kXSPI_Device4ByteAddressable) matches the
 *             0xB7 Enter-4-Byte-Address command issued during init.
 *             The flash reverts to 3-byte mode after a software reset,
 *             so 0xB7 must be re-issued after every reset sequence.
 *
 * Size:       32 MB (W25Q256 = 256 Mbit) expressed in KB as required
 *             by the xspi_device_config_t API.
 */
xspi_device_config_t deviceconfig = {
    /*!< Root clock = Audio PLL PFD0 / 1 = 330.5 MHz; flash serial clock = root / 2 ≈ 165 MHz. */
    .xspiRootClk = EXAMPLE_XSPI_ROOT_CLOCK_FREQ,
    /*!< CKN (differential clock negative) pad not used in SDR mode. */
    .enableCknPad = false,
    /*!< Standard/Extended SPI interface covers 1/2/4-pad transfers.
     *   QPI (4-4-4) is achieved by selecting kXSPI_4PAD for every
     *   cmd/addr/data phase in the LUT entries below. */
    .deviceInterface = kXSPI_StrandardExtendedSPI,
    /*!< Page size for the page-program command (0x12 in QPI mode).
     *   256 bytes for W25Q25PWZEIM. */
    .interfaceSettings.strandardExtendedSPISettings.pageSize = FLASH_PAGE_SIZE,
    /*!< CS hold time: 3 serial-clock cycles after the last clock edge. */
    .CSHoldTime = 3U,
    /*!< CS setup time: 3 serial-clock cycles before the first clock edge. */
    .CSSetupTime = 3U,
    .sampleClkConfig =
        {
            /*!< DQS pad loopback: the XSPI output read clock is fed back
             *   through the DQS pad to sample incoming read data. This is the
             *   sampling source the boot ROM uses for XIP reads from this flash
             *   and the only source that returns correct read data here. */
            .sampleClkSource = kXSPI_SampleClkFromDqsPadLoopback,
            .dllConfig =
                {
                    .dllMode     = kXSPI_AutoUpdateMode,
                    .useRefValue = true,
                    .enableCdl8  = true,
                },
        },
    .ptrDeviceDdrConfig = NULL, /*!< SDR mode; no DDR configuration. */
    /*!< 4-byte address mode: all LUT RADDR phases encode 0x20 (32-bit).
     *   The flash enters this mode via 0xB7 during init and exits it
     *   on software reset, requiring 0xB7 to be resent after each reset. */
    .addrMode              = kXSPI_DeviceByteAddressable,
    .columnAddrWidth       = 0U,          /*!< No column address (NOR flash, not HyperFlash). */
    .enableCASInterleaving = false,       /*!< Not applicable for NOR flash. */
    .deviceSize[0]         = 32U * 1024U, /*!< 32 MB (W25Q25PWZEIM = 256 Mbit) in KB units. */
    .deviceSize[1]         = 32U * 1024U, /*!< Must equal deviceSize[0] so SFA2AD >= SFA1AD. */
    .ptrDeviceRegInfo      = NULL,
};

/*
 * Errata ERR052528 workaround — minimum 8-byte LUT data size for all reads.
 * The XSPI controller cannot complete a read transaction (including RDSR)
 * if the LUT DATA phase specifies fewer than 8 bytes.  Every READ_SDR phase
 * below therefore uses operand 0x08 (8 bytes) even when only 1–3 bytes of
 * the returned data are meaningful to the application.
 *
 * LUT layout — W25Q25PWZEIM at XSPI0, SDR, QPI 4-4-4, 165 MHz flash clock, 4-byte addr
 * ─────────────────────────────────────────────────────────────────────────────
 * Indices 0–6   QPI operational commands (4-pad for cmd/addr/data).
 *               Index 0 is the 4-4-4 Fast Read Quad I/O (0xEC).
 *
 * Indices 7–13  SPI init commands (1-pad) used during flash initialization
 *               before entering QPI mode.
 *
 * Index  14     QPI Set Read Parameters (0xC0, 4-pad).
 *
 * Index  15     QPI Read JEDEC ID (0x9F, 4-pad): verifies flash identity
 *               after QPI entry.
 *
 * Fast Read Quad I/O (0xEC) dummy-clock breakdown at ~165 MHz flash clock, QPI 4-4-4:
 *   CMD    :  0xEC, 2 SDR clocks on 4 pads (1 byte)
 *   ADDR   :  4 bytes, 8 SDR clocks on 4 pads
 *   M7–M0  :  0xFF, 2 SDR clocks on 4 pads (non-continuous read)
 *   DUMMY  :  10 SDR clocks
 *   DATA   :  N bytes on 4 pads
 *   Total dummy = MODE (2) + DUMMY (10) = 12 clocks, P[6:4]=101, max 166 MHz.
 */
AT_QUICKACCESS_SECTION_DATA(const uint32_t customLUT[CUSTOM_LUT_LENGTH]) = {
    /* [0] Fast Read Quad I/O, QPI mode (4-4-4, 4-byte address, cmd 0xECh).
     *     M7-M0 = 0xFF (non-continuous), 12 total dummy clocks (P[6:4]=101, max 166 MHz). */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI +
     0]    = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0xEC, kXSPI_Command_RADDR_SDR, kXSPI_4PAD, 0x20),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI +
        1] = XSPI_LUT_SEQ(kXSPI_Command_MODE_SDR, kXSPI_4PAD, 0xFF, kXSPI_Command_DUMMY_SDR, kXSPI_4PAD, 0x0A),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI +
        2] = XSPI_LUT_SEQ(kXSPI_Command_READ_SDR, kXSPI_4PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [1] Read Status Register-1, QPI mode (0x05 on 4 pads) */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_READ_STATUS +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0x05, kXSPI_Command_READ_SDR, kXSPI_4PAD, 0x08),

    /* [2] Enable Reset, QPI mode (0x66 on 4 pads) */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_RESET_ENABLE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0x66, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [3] Reset Memory, QPI mode (0x99 on 4 pads) */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_RESET_MEMORY +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0x99, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [4] Write Enable, QPI mode (0x06 on 4 pads) */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_WRITE_ENABLE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0x06, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [5] Sector Erase 4KB, QPI mode (0x21 on 4 pads, 4-byte address) */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_ERASE_SECTOR +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0x21, kXSPI_Command_RADDR_SDR, kXSPI_4PAD, 0x20),

    /* [6] Page Program, QPI mode (0x12 on 4 pads, 4-byte address, data on 4 pads) */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_PAGE_PROGRAM +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0x12, kXSPI_Command_RADDR_SDR, kXSPI_4PAD, 0x20),
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_PAGE_PROGRAM +
        1] = XSPI_LUT_SEQ(kXSPI_Command_WRITE_SDR, kXSPI_4PAD, 0x04, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [7] Enable Reset, SPI mode (0x66 on 1 pad) — used before entering QPI */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_RESET_ENABLE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x66, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [8] Reset Memory, SPI mode (0x99 on 1 pad) */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_RESET_MEMORY +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x99, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [9] Read Status Register-2, SPI mode (0x35 on 1 pad) — to check/set QE bit */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR2 +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x35, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08),

    /* [10] Write Enable, SPI mode (0x06 on 1 pad) — before SR2 write */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_ENABLE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x06, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [11] Write Status Register-2, SPI mode (0x31 on 1 pad) — to set QE bit */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_SR2 +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x31, kXSPI_Command_WRITE_SDR, kXSPI_1PAD, 0x01),

    /* [12] Enter 4-Byte Address Mode (0xB7 on 1 pad) */
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_4BYTE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0xB7, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [13] Enter QPI Mode (0x38 on 1 pad) — switches flash from SPI to QPI (4-4-4) */
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_QPI +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x38, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [14] Set Read Parameters, QPI mode (0xC0 on 4 pads, 1 byte: P7-P0).
     *      P[7:0] = 0x50 → P[6:4]=101: SDR(0xEC) 12 dummy / 166MHz max. */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_SET_READ_PARAM +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0xC0, kXSPI_Command_WRITE_SDR, kXSPI_4PAD, 0x01),

    /* [15] Read JEDEC ID, QPI mode (0x9F on 4 pads) — returns EF/60/18 for W25Q25PWZEIM */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_READ_ID +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0x9F, kXSPI_Command_READ_SDR, kXSPI_4PAD, 0x08),
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* XSPI0 clock setup is performed inside xspi_quad_flash_init(), before
     * XSPI_Init() and XSPI_SetDeviceConfig(). Those operations run from RAM
     * for flash targets because the CPU may be fetching instructions from
     * the same XSPI0 flash. */
}
/*${function:end}*/
