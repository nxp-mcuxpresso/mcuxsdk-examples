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
#if !(defined(XIP_BOOT_HEADER_ENABLE) && (XIP_BOOT_HEADER_ENABLE == 1))
#include "fsl_iomuxc.h"
#endif
/*${header:end}*/

/*${variable:start}*/
/*
 * W25H512NWEAM (64 MB) on XSPI0, driven in Quad I/O (1-4-4) SDR mode: the read
 * (0xEC) and quad page program (0x34) use the 4 data pads, with the command on
 * 1 pad; all other commands are 1-pad SPI. The device runs in 4-byte address
 * mode (entered with 0xB7, verified via SR3.ADS) because 3-byte addressing
 * cannot reach beyond 16 MB of the 64 MB array. The flash is never put into QPI
 * (4-4-4) mode, so a run can never leave it in a state the ROM/flash-loader
 * (which speak plain SPI) cannot recover from.
 *
 * sampleClkSource is DQS pad loopback -- the part has no DQS pin, so the
 * loopback through the pad ring is the source that tracks the real pad
 * timing (same choice the boot ROM makes for this flash).
 *
 * dllConfig uses auto-update mode with the NXP reference values
 * (useRefValue = true) rather than a hand-picked bypass delay: bypass mode
 * needs its own FREQEN decision (XSPI_UpdateDllValue derives it from
 * xspiRootClk only when useRefValue is true), and this example's boot path
 * (see BOARD_InitHardware) never runs the boot ROM init that would otherwise
 * leave a known DLLCR state behind.
 */
xspi_device_config_t deviceconfig = {
    .xspiRootClk     = EXAMPLE_XSPI_ROOT_CLOCK_FREQ,
    .enableCknPad    = false,
    .deviceInterface = kXSPI_StrandardExtendedSPI,
    .interfaceSettings.strandardExtendedSPISettings.pageSize = FLASH_PAGE_SIZE,
    .CSHoldTime  = 3U,
    .CSSetupTime = 3U,
    .sampleClkConfig =
        {
            .sampleClkSource = kXSPI_SampleClkFromDqsPadLoopback,
            .dllConfig =
                {
                    .dllMode     = kXSPI_AutoUpdateMode,
                    .useRefValue = true,
                },
        },
    .ptrDeviceDdrConfig    = NULL,
    .addrMode              = kXSPI_DeviceByteAddressable,
    .columnAddrWidth       = 0U,
    .enableCASInterleaving = false,
    .deviceSize[0]         = 64U * 1024U, /* 64 MB (W25H512 = 512 Mbit) in KB units. */
    .deviceSize[1]         = 64U * 1024U, /* Must equal deviceSize[0] so SFA2AD >= SFA1AD. */
    .ptrDeviceRegInfo      = NULL,
};

/*
 * ERR052528 workaround: the XSPI controller cannot complete a read of fewer
 * than 8 bytes, so every READ_SDR phase below uses operand 0x08.
 */
AT_QUICKACCESS_SECTION_DATA(const uint32_t customLUT[CUSTOM_LUT_LENGTH]) = {
    /* [0] Fast Read Quad I/O (1-4-4, 0xEC), 4-byte address: command on 1 pad,
     * address/mode/data on 4 pads. M7-M0 = 0xFF (non-continuous) is a 2-clock
     * phase; MODE(2) + DUMMY(4) = 6-clock read latency (validated on this part). */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI +
     0]    = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0xEC, kXSPI_Command_RADDR_SDR, kXSPI_4PAD, 0x20),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI +
        1] = XSPI_LUT_SEQ(kXSPI_Command_MODE_SDR, kXSPI_4PAD, 0xFF, kXSPI_Command_DUMMY_SDR, kXSPI_4PAD, 0x04),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI +
        2] = XSPI_LUT_SEQ(kXSPI_Command_READ_SDR, kXSPI_4PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [1] Read Status Register-1 (BUSY), SPI (0x05 on 1 pad). */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_READ_STATUS +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x05, kXSPI_Command_DUMMY_SDR, kXSPI_4PAD, 0x08),
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_READ_STATUS +
        1] = XSPI_LUT_SEQ(kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [2] QPI rescue: Enable Reset (0x66 on 4 pads). Exits QPI if the flash was
     * left in QPI mode by a previous session; ignored harmlessly in SPI mode. */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_RESET_ENABLE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0x66, kXSPI_Command_STOP, kXSPI_4PAD, 0x0),

    /* [3] QPI rescue: Reset Memory (0x99 on 4 pads). */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_RESET_MEMORY +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0x99, kXSPI_Command_STOP, kXSPI_4PAD, 0x0),

    /* [4] Write Enable (0x06 on 1 pad). */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_WRITE_ENABLE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x06, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [5] Sector Erase 4KB (0x21 on 1 pad, 4-byte address). */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_ERASE_SECTOR +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x21, kXSPI_Command_RADDR_SDR, kXSPI_1PAD, 0x20),

    /* [6] Quad Input Page Program (1-1-4, 0x34, 4-byte address): command and
     * address on 1 pad, data on 4 pads. */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_PAGE_PROGRAM +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x34, kXSPI_Command_RADDR_SDR, kXSPI_1PAD, 0x20),
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_PAGE_PROGRAM +
        1] = XSPI_LUT_SEQ(kXSPI_Command_WRITE_SDR, kXSPI_4PAD, 0x04, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [7] Enable Reset, SPI (0x66 on 1 pad) -- init before QPI entry. */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_RESET_ENABLE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x66, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [8] Reset Memory, SPI (0x99 on 1 pad). */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_RESET_MEMORY +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x99, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [9] Read Status Register-2, SPI (0x35 on 1 pad) -- check/set QE bit. */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR2 +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x35, kXSPI_Command_DUMMY_SDR, kXSPI_1PAD, 0x8),
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR2 +
        1] = XSPI_LUT_SEQ(kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [10] Write Enable, SPI (0x06 on 1 pad) -- before SR2 write / 4-byte enter. */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_ENABLE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x06, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [11] Write Status Register-2, SPI (0x31 on 1 pad) -- set QE bit. */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_SR2 +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x31, kXSPI_Command_WRITE_SDR, kXSPI_1PAD, 0x01),

    /* [12] Enter 4-Byte Address Mode, SPI (0xB7 on 1 pad). The 64 MB device
     * needs 4-byte addressing; reverts to 3-byte on software reset so it is
     * re-issued after every reset sequence. */
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_4BYTE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0xB7, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [13] Enter QPI Mode (0x38) -- unused in this Quad-I/O (1-4-4) example;
     * kept only so the LUT index numbering is stable. */
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_QPI +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x38, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [14] Read Status Register-3, SPI (0x15 on 1 pad) -- confirm 4-byte mode
     * via SR3.ADS (bit 0, 1 = 4-byte / current address mode), read after 0xB7.
     * An 8-clock dummy phase is inserted between the command and the data phase
     * to prevent IP-access timeout on this silicon (observed: without the gap
     * the XSPI sequencer can stall on SR3 reads and never return). */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR3 +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x15, kXSPI_Command_DUMMY_SDR, kXSPI_1PAD, 0x8),
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR3 +
        1] = XSPI_LUT_SEQ(kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [15] Read JEDEC ID (0x9F on 1 pad) -- manufacturer 0xEF (Winbond). */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_READ_ID +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x9F, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08),
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_READ_ID +
        1] = XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_1PAD, 0x0, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),
};
/*${variable:end}*/

/*${function:start}*/
#if !(defined(XIP_BOOT_HEADER_ENABLE) && (XIP_BOOT_HEADER_ENABLE == 1))
/*
 * Mux the XSPI0 pins, including DQS0 (sampleClkSource is DQS pad loopback, so
 * DQS0 must be muxed to the XSPI0 function for reads to sample correctly).
 * On the xspi_nor* (XIP) targets the boot ROM already muxes these pins
 * before jumping to the image executing from that same flash -- re-muxing
 * there would disturb the live XIP interface for no benefit, so this runs
 * only on targets without a boot header (the ROM does not run their init).
 */
static void BOARD_InitXspi0QuadPins(void)
{
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO6_0_MAIN_XSPI0_SCLK0, 0x3000U); /* Drive Strength: 33 ohm */
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO6_1_MAIN_XSPI0_SS0_N, 0x3000U); /* Drive Strength: 33 ohm */
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO6_2_MAIN_XSPI0_DATA0,
                              0x3080U); /* Input Buffer Enable: Enables, Drive Strength: 33 ohm */
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO6_3_MAIN_XSPI0_DATA1,
                              0x3080U); /* Input Buffer Enable: Enables, Drive Strength: 33 ohm */
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO6_4_MAIN_XSPI0_DATA2,
                              0x3080U); /* Input Buffer Enable: Enables, Drive Strength: 33 ohm */
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO6_5_MAIN_XSPI0_DATA3,
                              0x3080U); /* Input Buffer Enable: Enables, Drive Strength: 33 ohm */
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO6_6_MAIN_XSPI0_DQS0,
                              0x3090U); /* Pulldown Enable, Input Buffer Enable: Enables, Drive Strength: 33 ohm */
}
#endif

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
#if !(defined(XIP_BOOT_HEADER_ENABLE) && (XIP_BOOT_HEADER_ENABLE == 1))
    BOARD_InitXspi0QuadPins();
#endif

    /* In this case, DMA is being used to fill the TBDR buffer, so it should be made sure that the DMA
     * should have same domain ID as the CPU.
     * In default, the CPU is in domain 1, so the DMA3 channel 0,1 should be assigned to domain 1 as well.
     */
    trdc_non_processor_domain_assignment_t eDma3Ch0_1_assignment;

    TRDC_GetDefaultNonProcessorDomainAssignment(&eDma3Ch0_1_assignment);
    eDma3Ch0_1_assignment.domainId = 1U;
    TRDC_SetNonProcessorDomainAssignment(MAIN__TRDC, kTRDC_MAIN_MasterMEDMA3Ch0_1, &eDma3Ch0_1_assignment);
}
/*${function:end}*/
