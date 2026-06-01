/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "app.h"
#include "fsl_xspi.h"

/*${function:start}*/
xspi_device_ddr_config_t flashDDrConfig =
{
    .ddrDataAlignedClk = kXSPI_DDRDataAlignedWith2xInternalRefClk,
    .enableDdr = true,
    .enableByteSwapInOctalMode = false,
};

xspi_device_config_t deviceConfig =
{
    .xspiRootClk = 400000000,  /*!< 400MHz */
    .enableCknPad = false,  /*!< Do not support differential clock */      
    .deviceInterface = kXSPI_StrandardExtendedSPI,  /*!< Support Single IO and Octal IO */
    .interfaceSettings.strandardExtendedSPISettings.pageSize = FLASH_PAGE_SIZE,   /*!< 256 byte page buffer. */
    .CSHoldTime = 3,
    .CSSetupTime = 3,
    .sampleClkConfig.sampleClkSource = kXSPI_SampleClkFromExternalDQS,  /*!< Device support Data strobe signal.  */
    .sampleClkConfig.enableDQSLatency = false,
    .sampleClkConfig.dllConfig.dllMode = kXSPI_AutoUpdateMode,
    .sampleClkConfig.dllConfig.useRefValue = true,
    .sampleClkConfig.dllConfig.enableCdl8 = true,
    .addrMode = kXSPI_DeviceByteAddressable,
    .columnAddrWidth = 0U,
    .enableCASInterleaving = false,
    .deviceSize[0] = FLASH_SIZE,
    .deviceSize[1] = FLASH_SIZE,   /*!< Single DIE flash, so deviceSize1 should equal to deviceSize0. */
    .ptrDeviceRegInfo = NULL,      /*!< Not used in this example. */
    .ptrDeviceDdrConfig = &flashDDrConfig,
};

#if defined(ENABLE_SFP_CONFIG) && (ENABLE_SFP_CONFIG)
xspi_sfp_mdad_config_t *pSfpMdadConfig;
xspi_sfp_frad_config_t *pSfpFradConfig;

xspi_sfp_mdad_config_t sfpMdadConfig =
    {
        .tgMdad[kXSPI_TargetGroup0] =
        {
            .assignIsValid = true,
            .enableDescriptorLock = false,
            .maskType = kXSPI_MdadMaskTypeAnd,
            .mask = SOC_CPU_DOMAIN_ID,
            .masterIdReference = SOC_CPU_DOMAIN_ID,
            .secureAttribute = kXSPI_AttributeMasterNonsecureSecureBoth,
        },
    };

static xspi_sfp_frad_config_t sfpFradConfig =
    {
        .fradConfig =
        {
            [0] = {
                .startAddress = 0,
                .endAddress = 0xFFFFFFFF,
                .tgConfig = {
                    [kXSPI_TargetGroup0] = {
                        .tgMasterAccess[0] = 3,
                        .assignIsValid = true,
                        .descriptorLock = kXSPI_DescriptorLockDisabled,
                        .exclusiveAccessLock = kXSPI_ExclusiveAccessLockDisabled,
                    },
                },
            }
        },
    };
#endif



const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
#if (defined(BOARD_USE_EMU) && BOARD_USE_EMU)
    /*  OPI DDR read */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x03, kXSPI_Command_RADDR_SDR, kXSPI_1PAD, 0x18),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        XSPI_LUT_SEQ(kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x04, kXSPI_Command_STOP, kXSPI_1PAD, 0),

    /* Read ID */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_ID_OPI] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x9F, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x04),

    /*  Write Enable */
    [5 * NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE_OPI] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x06, kXSPI_Command_STOP, kXSPI_1PAD, 0),

    /*  Erase Sector */
    [5 * NOR_CMD_LUT_SEQ_IDX_ERASE_SECTOR] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x20, kXSPI_Command_RADDR_SDR, kXSPI_1PAD, 0x18),

    /*  Erase Chip */
    [5 * NOR_CMD_LUT_SEQ_IDX_ERASE_CHIP] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xC4, kXSPI_Command_DDR, kXSPI_8PAD, 0xC4),
    [5 * NOR_CMD_LUT_SEQ_IDX_ERASE_CHIP + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_STOP, kXSPI_8PAD, 0),

    /*  Program */
    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x02, kXSPI_Command_RADDR_SDR, kXSPI_1PAD, 0x18),
    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL + 1] = 
        XSPI_LUT_SEQ(kXSPI_Command_WRITE_SDR, kXSPI_1PAD, 0x01, kXSPI_Command_STOP, kXSPI_1PAD, 0),

    /*  Dummy write, do nothing when AHB write command is triggered. */
    [5 * NOR_CMD_LUT_SEQ_IDX_WRITE] =
        XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_1PAD, 0x0, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /*  Read status register using Octal DDR read */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x05, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x04),


#else
    /*  OCTAL I/O FAST READ */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR,       kXSPI_8PAD, 0xFD, kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20),
    // Don't care the dummy value setting here, as it uses external DQS
    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x03, kXSPI_Command_READ_DDR,  kXSPI_8PAD, 0x04),

    /* Read ID */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_ID_OPI] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR,       kXSPI_8PAD, 0x9F, kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x08),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_ID_OPI + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_READ_DDR,  kXSPI_8PAD, 0x01, kXSPI_Command_STOP,      kXSPI_1PAD, 0x00),

    /*  Write Enable */
    [5 * NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR,       kXSPI_1PAD, 0x06, kXSPI_Command_STOP,      kXSPI_1PAD, 0x00),

    /*  Write Enable */
    [5 * NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE_OPI] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR,       kXSPI_8PAD, 0x06, kXSPI_Command_STOP,      kXSPI_1PAD, 0x00),

    /*  Erase Sector */
    [5 * NOR_CMD_LUT_SEQ_IDX_ERASE_SECTOR] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR,       kXSPI_8PAD, 0x21, kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20),

    /*  Erase Chip */
    [5 * NOR_CMD_LUT_SEQ_IDX_ERASE_CHIP] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR,       kXSPI_8PAD, 0xC4, kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20),

    /* EXTENDED OCTAL INPUT FAST Program */
    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR,       kXSPI_8PAD, 0x12, kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20),
    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x04, kXSPI_Command_STOP,      kXSPI_1PAD, 0x00),

    /*  Dummy write, do nothing when AHB write command is triggered. */
    [5 * NOR_CMD_LUT_SEQ_IDX_WRITE] =
        XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_1PAD, 0x0, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /*  Read status register using Octal DDR read */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR,       kXSPI_8PAD, 0x05, kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x08),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_READ_DDR,  kXSPI_8PAD, 0x01, kXSPI_Command_STOP,      kXSPI_1PAD, 0x00),

    // Volatile Configuration Register address 000000h - I/O mode
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_OPI] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR,       kXSPI_1PAD, 0x81, kXSPI_Command_SDR,       kXSPI_1PAD, 0x00),
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_OPI + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR,       kXSPI_1PAD, 0x00, kXSPI_Command_SDR,       kXSPI_1PAD, 0x00),
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_OPI + 2] =
        XSPI_LUT_SEQ(kXSPI_Command_WRITE_SDR, kXSPI_1PAD, 0x01, kXSPI_Command_STOP,      kXSPI_1PAD, 0x00),
#endif /* BOARD_USE_EMU */
};

void BOARD_InitHardware(void)
{
    SystemPlatformInit();
    BOARD_ConfigMPU();
    BOARD_InitDebugConsolePins();
    BOARD_InitXspiPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    xspi_clock_init();

#if defined(ENABLE_SFP_CONFIG) && (ENABLE_SFP_CONFIG)
    pSfpMdadConfig = &sfpMdadConfig;
    pSfpFradConfig = &sfpFradConfig;
#endif
}

/*${function:end}*/
