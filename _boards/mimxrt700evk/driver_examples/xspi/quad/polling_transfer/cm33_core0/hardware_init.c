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
 * W25Q512NW on XSPI0, Quad I/O (1-4-4) SDR (the part has no QPI mode): only the
 * 0xEC read and 0x34 page program use quad pads; all other commands are 1-pad.
 *
 * sampleClkSource must be DQS pad loopback — the part has no DQS pin and XSPI0
 * does not support dummy-pad loopback, so it is the only source that returns
 * correct read data.
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
                    .enableCdl8  = true,
                },
        },
    .ptrDeviceDdrConfig    = NULL,
    .addrMode              = kXSPI_DeviceByteAddressable,
    .columnAddrWidth       = 0U,
    .enableCASInterleaving = false,
    .deviceSize[0]         = 64U * 1024U, /* 64 MB (W25Q512 = 512 Mbit) in KB units. */
    .deviceSize[1]         = 64U * 1024U, /* Must equal deviceSize[0] so SFA2AD >= SFA1AD. */
    .ptrDeviceRegInfo      = NULL,
};

/*
 * ERR052528 workaround: the XSPI controller cannot complete a read of fewer than
 * 8 bytes, so every READ_SDR phase below uses operand 0x08.
 */
AT_QUICKACCESS_SECTION_DATA(const uint32_t customLUT[CUSTOM_LUT_LENGTH]) = {
    /* Fast Read Quad I/O (1-4-4, 0xEC): M7-M0 = 0xFF (non-continuous), 12 dummy clocks. */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI +
     0]    = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0xEC, kXSPI_Command_RADDR_SDR, kXSPI_4PAD, 0x20),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI +
        1] = XSPI_LUT_SEQ(kXSPI_Command_MODE_SDR, kXSPI_4PAD, 0xFF, kXSPI_Command_DUMMY_SDR, kXSPI_4PAD, 0x0A),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI +
        2] = XSPI_LUT_SEQ(kXSPI_Command_READ_SDR, kXSPI_4PAD, 0x08, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_READ_STATUS +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x05, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08),

    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_RESET_ENABLE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x66, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_RESET_MEMORY +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x99, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_WRITE_ENABLE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x06, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_ERASE_SECTOR +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x21, kXSPI_Command_RADDR_SDR, kXSPI_1PAD, 0x20),

    /* Quad Input Page Program (1-1-4, 0x34): cmd/addr on 1 pad, data on 4 pads. */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_PAGE_PROGRAM +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x34, kXSPI_Command_RADDR_SDR, kXSPI_1PAD, 0x20),
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_PAGE_PROGRAM +
        1] = XSPI_LUT_SEQ(kXSPI_Command_WRITE_SDR, kXSPI_4PAD, 0x04, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_RESET_ENABLE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x66, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_RESET_MEMORY +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x99, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR2 +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x35, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08),

    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_ENABLE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x06, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_SR2 +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x31, kXSPI_Command_WRITE_SDR, kXSPI_1PAD, 0x01),

    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_4BYTE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0xB7, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* Enter QPI (0x38) — unused; the W25Q512NW has no QPI mode. */
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_QPI +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x38, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* Set Read Parameters (0xC0): P[7:0] = 0x50 → 12 read dummy clocks. */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_SET_READ_PARAM +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0xC0, kXSPI_Command_WRITE_SDR, kXSPI_1PAD, 0x01),

    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_READ_ID +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x9F, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08),
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitNorFlashPins();
}
/*${function:end}*/
