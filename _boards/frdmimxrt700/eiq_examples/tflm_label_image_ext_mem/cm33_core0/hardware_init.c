/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "board_init.h"
#include "fsl_debug_console.h"
#include "fsl_edma.h"
#include "fsl_edma_soc.h"
#include "fsl_xspi.h"
#include "fsl_xspi_edma.h"
#include "pin_mux.h"
#include "pmic_support.h"

extern void xspi_callback(XSPI_Type *base, xspi_edma_handle_t *handle, status_t status, void *userData);
extern status_t xspi_quad_full_init(XSPI_Type *base, uint8_t *vendorId);
extern void xspi_quad_invalidate_caches(void);

/*!
 * @brief Configuration for W25Q25PWZEIM Quad SPI NOR flash (QPI 4-4-4 SDR mode).
 *
 * xspiRootClk: MAIN_PLL_PFD1 / 2 = 200 MHz root clock.
 *   The serial clock driven to the flash is half of the root clock, so the
 *   flash sees 100 MHz in QPI SDR mode.
 *
 * sampleClkSource: kXSPI_SampleClkFromDqsPadLoopback routes the XSPI TX
 *   clock through the DQS pad circuitry internally and back to the RX
 *   sampler — no DQS pin on the flash side is required.
 *
 * addrMode: kXSPI_Device4ByteAddressable keeps CS# low for an extra clock
 *   so the 4th address byte is driven. The flash exits 4-byte mode on
 *   every software reset, so xspi_quad_enter_4byte_mode() must be called
 *   after every xspi_quad_reset_flash().
 */
xspi_device_config_t deviceconfig = {
    .xspiRootClk                                             = EXAMPLE_XSPI_ROOT_CLOCK_FREQ,
    .enableCknPad                                            = false,
    .deviceInterface                                         = kXSPI_StrandardExtendedSPI,
    .interfaceSettings.strandardExtendedSPISettings.pageSize = FLASH_PAGE_SIZE,
    .CSHoldTime                                              = 3U,
    .CSSetupTime                                             = 3U,
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
    .ptrDeviceDdrConfig    = NULL, /*!< SDR mode; no DDR configuration. */
    .addrMode              = kXSPI_Device4ByteAddressable,
    .columnAddrWidth       = 0U,
    .enableCASInterleaving = false,
    .deviceSize[0]         = 32U * 1024U, /*!< 32 MB (W25Q25PWZEIM = 256 Mbit) in KB units. */
    .deviceSize[1]         = 32U * 1024U, /*!< Must equal deviceSize[0] so SFA2AD >= SFA1AD. */
    .ptrDeviceRegInfo      = NULL,
};

/*
 * Errata ERR052528: Limitation on LUT-Data Size < 8byte in xspi.
 * Description: Read command including RDSR command can't work if LUT data size in read status is less than 8.
 * Workaround: Use LUT data size of minimum 8 byte for read commands including RDSR.
 *
 * W25Q25PWZEIM @ XSPI0 / SDR / QPI (4-4-4) / 100MHz flash clock / 4-byte address
 *
 * Indices 0-6   QPI operational commands (4-pad for cmd/addr/data).
 *               Index 0 is the 4-4-4 Fast Read Quad I/O (0xEC).
 * Indices 7-13  SPI init commands (1-pad) used during flash initialization
 *               before entering QPI mode.
 * Index  14     QPI Set Read Parameters (0xC0, 4-pad).
 * Index  15     QPI Read JEDEC ID (0x9F, 4-pad): verifies flash identity
 *               after QPI entry.
 */
const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
    /* [0] Fast Read Quad I/O, QPI mode (4-4-4, 4-byte address, cmd 0xECh).
     *     M7-M0 = 0xFF (non-continuous), 6 total dummy clocks (P[6:4]=000, 133 MHz max). */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI +
     0]    = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0xEC, kXSPI_Command_RADDR_SDR, kXSPI_4PAD, 0x20),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI +
        1] = XSPI_LUT_SEQ(kXSPI_Command_MODE_SDR, kXSPI_4PAD, 0xFF, kXSPI_Command_DUMMY_SDR, kXSPI_4PAD, 0x04),
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

    /* [7] Enable Reset, SPI mode (0x66 on 1 pad) - used before entering QPI */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_RESET_ENABLE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x66, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [8] Reset Memory, SPI mode (0x99 on 1 pad) */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_RESET_MEMORY +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x99, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [9] Read Status Register-2, SPI mode (0x35 on 1 pad) - to check/set QE bit */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR2 +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x35, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x08),

    /* [10] Write Enable, SPI mode (0x06 on 1 pad) - before SR2 write */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_ENABLE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x06, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [11] Write Status Register-2, SPI mode (0x31 on 1 pad) - to set QE bit */
    [5 * NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_SR2 +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x31, kXSPI_Command_WRITE_SDR, kXSPI_1PAD, 0x01),

    /* [12] Enter 4-Byte Address Mode (0xB7 on 1 pad) */
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_4BYTE +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0xB7, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [13] Enter QPI Mode (0x38 on 1 pad) - switches flash from SPI to QPI (4-4-4) */
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_QPI +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x38, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* [14] Set Read Parameters, QPI mode (0xC0 on 4 pads, 1 byte: P7-P0).
     *      P[7:0] = 0x00 -> P[6:4]=000: SDR(0xEC) 6 dummy / 133MHz max;
     *                                   DTR(0xEE) 8 dummy / 104MHz max. */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_SET_READ_PARAM +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0xC0, kXSPI_Command_WRITE_SDR, kXSPI_4PAD, 0x01),

    /* [15] Read JEDEC ID, QPI mode (0x9F on 4 pads) - returns EF/60/18 for W25Q25PWZEIM */
    [5 * NOR_CMD_LUT_SEQ_IDX_QPI_READ_ID +
        0] = XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_4PAD, 0x9F, kXSPI_Command_READ_SDR, kXSPI_4PAD, 0x08),
};

edma_handle_t dmaTxHandle;
edma_handle_t dmaRxHandle;
xspi_edma_handle_t xspiHandle;

void cleanCache(void)
{
    XCACHE_CleanCache(XCACHE0);
}

void invalidateCache(void)
{
    XCACHE_CleanCache(XCACHE0);
    XCACHE_InvalidateCache(XCACHE0);
}

void BOARD_Init()
{
    edma_config_t userConfig;

    BOARD_InitBootPins();
    BOARD_InitXSPI0Pins();

    // Disable LDO
    POWER_SetVddnSupplySrc(kVddSrc_PMIC);
    POWER_SetVdd1SupplySrc(kVddSrc_PMIC);
    POWER_SetVdd2SupplySrc(kVddSrc_PMIC);
    POWER_ApplyPD();

    BOARD_InitPMICPins();
    BOARD_InitPmic();
    BOARD_SetPmicVdd2Voltage(1100000U); /* 1.1v for 325MHz clock. */

    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();

    BOARD_InitAHBSC();
    BOARD_ConfigMPU();

    /* Attach MAIN_PLL_PFD1 (400 MHz) to XSPI0 — matches the FCB-established
     * clock setup at boot. Required for EDMA read paths. */
    CLOCK_AttachClk(kMAIN_PLL_PFD1_to_XSPI0);
    CLOCK_SetClkDiv(kCLOCK_DivXspi0Clk, 1u);     /* 400MHz */

    POWER_DisablePD(kPDRUNCFG_APD_NPU);
    POWER_DisablePD(kPDRUNCFG_PPD_NPU);
    POWER_ApplyPD();

    RESET_ClearPeripheralReset(kDMA0_RST_SHIFT_RSTn);
    EDMA_EnableRequest(EXAMPLE_XSPI_DMA, XSPI_TX_DMA_REQUEST_SOURCE);
    EDMA_EnableRequest(EXAMPLE_XSPI_DMA, XSPI_RX_DMA_REQUEST_SOURCE);

#if defined(ENABLE_RAM_VECTOR_TABLE)
    InstallIRQHandler(XSPI_TX_DMA_IRQn, (uint32_t)XSPI_TX_DMA_ISR);
    InstallIRQHandler(XSPI_RX_DMA_IRQn, (uint32_t)XSPI_RX_DMA_ISR);
#endif

    EDMA_GetDefaultConfig(&userConfig);
    /* To unlock TBDR, configure the master ID of EDMA to be the same as the cm33_core0 */
    userConfig.enableMasterIdReplication = true;
    EDMA_Init(EXAMPLE_XSPI_DMA, &userConfig);
    /* To unlock TBDR, configure the master ID of EDMA to be the same as the cm33_core0 */
    EDMA_EnableChannelMasterIDReplication(EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL, true);
    /* Create the EDMA channel handles */
    EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL);
    EDMA_CreateHandle(&dmaRxHandle, EXAMPLE_XSPI_DMA, XSPI_RX_DMA_CHANNEL);

    /* Create handle for xspi. */
    XSPI_TransferCreateHandleEDMA(EXAMPLE_XSPI, &xspiHandle, xspi_callback, NULL, &dmaTxHandle, &dmaRxHandle);

    /* Why we don't call xspi_quad_full_init() here:
     * The W25Q25PWZEIM on frdmimxrt700 is already configured for Quad SDR XIP
     * by the Boot ROM via flash_config.c (sflashPadType=4, sampleClk=Loopback,
     * serialClkFreq=166MHz, Read=0xEC Fast Read Quad I/O 1-4-4 4B). NPU weight
     * "fetch-on-demand" goes through ordinary memcpy() against the AHB-XIP
     * window at 0x28200000+ — no XSPI controller command-path is needed at
     * runtime, so we leave the Boot-ROM-established XIP config untouched.
     * Calling xspi_quad_full_init() here has been empirically observed to leave
     * the target with no serial output on frdmimxrt700; re-enable only if a
     * future XSPI command-path transfer pattern is added to this example. */
    (void)xspi_quad_full_init; /* suppresses unused-function warning if the
                                * driver is linked but not called at boot */
}
