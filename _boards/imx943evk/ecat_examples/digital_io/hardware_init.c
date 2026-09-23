/*
 * Copyright 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_xbar.h"
#include "fsl_iomuxc.h"
#include "fsl_ecat.h"
#include "fsl_rgpio.h"

#include "ecat_def.h"
#include "ecatslv.h"
#include "ecat_hw.h"
#include "ecatappl.h"

#include "app.h"

#define ECAT_CHANNEL            SCMI_A2P
#define ECAT_DOMAIN_ID          24
#define ECAT_DISABLE_FLAGS      0x2
#define ECAT_ENABLE_FLAGS       0x0
#define ECAT_REST_STATE         0

UINT32 EcatTimerCnt;

#ifdef EEPROM_I2C_EMULATOR
volatile uint32_t g_systickCounter;

/* EEPROM image cached in RAM, served to the ESC over the emulated I2C target. */
uint8_t esc_eeprom_cache[ESC_EMULATOR_EEPROM_SIZE];

/* Default SII content used when the image stored in flash has an invalid CRC. */
static const uint8_t esc_eeprom_cache_default[] = {
    0x80, 0x0c, 0x84, 0xee, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0x00,
    0xc2, 0x0c, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x80, 0x00, 0x80, 0x10, 0x80, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
    0xff, 0xff
};

enum EEPROM_access_status
{
    memory_reading = 0,
    memory_writing,
    memory_addr_hi,
    memory_addr_lo,
};

static enum EEPROM_access_status EEPROM_status = memory_addr_hi;

static uint8_t eeprom_updated       = 0;
static uint16_t eeprom_updated_time = 0;
static uint8_t eeprom_verified       = 0;
static uint16_t eeprom_verified_time = 0;

/* Offset of the EEPROM image inside the XSPI NOR flash. */
static uint32_t s_escDataOffset = ESC_EEPROM_FLASH_OFFSET;

static xspi_device_ddr_config_t s_flashDdrConfig = {
    .ddrDataAlignedClk         = kXSPI_DDRDataAlignedWith2xInternalRefClk,
    .enableDdr                 = true,
    .enableByteSwapInOctalMode = false,
};

static xspi_device_config_t s_deviceConfig = {
    .xspiRootClk     = 400000000U, /* 400MHz */
    .enableCknPad    = false,
    .deviceInterface = kXSPI_StrandardExtendedSPI,
    .interfaceSettings.strandardExtendedSPISettings.pageSize = FLASH_PAGE_SIZE,
    .CSHoldTime                             = 3,
    .CSSetupTime                            = 3,
    .sampleClkConfig.sampleClkSource        = kXSPI_SampleClkFromExternalDQS,
    .sampleClkConfig.enableDQSLatency       = false,
    .sampleClkConfig.dllConfig.dllMode      = kXSPI_AutoUpdateMode,
    .sampleClkConfig.dllConfig.useRefValue  = true,
    .sampleClkConfig.dllConfig.enableCdl8   = true,
    .addrMode                               = kXSPI_DeviceByteAddressable,
    .columnAddrWidth                        = 0U,
    .enableCASInterleaving                  = false,
    .deviceSize[0]                          = FLASH_SIZE,
    .deviceSize[1]                          = FLASH_SIZE,
    .ptrDeviceRegInfo                       = NULL,
    .ptrDeviceDdrConfig                     = &s_flashDdrConfig,
};

#if defined(ENABLE_SFP_CONFIG) && (ENABLE_SFP_CONFIG)
static xspi_sfp_mdad_config_t s_sfpMdadConfig = {
    .tgMdad[kXSPI_TargetGroup0] =
        {
            .assignIsValid        = true,
            .enableDescriptorLock = false,
            .maskType             = kXSPI_MdadMaskTypeAnd,
            .mask                 = SOC_CPU_DOMAIN_ID,
            .masterIdReference    = SOC_CPU_DOMAIN_ID,
            .secureAttribute      = kXSPI_AttributeMasterNonsecureSecureBoth,
        },
};

static xspi_sfp_frad_config_t s_sfpFradConfig = {
    .fradConfig =
        {
            [0] = {
                .startAddress = 0,
                .endAddress   = 0xFFFFFFFF,
                .tgConfig = {
                    [kXSPI_TargetGroup0] = {
                        .tgMasterAccess[0]     = 3,
                        .assignIsValid         = true,
                        .descriptorLock        = kXSPI_DescriptorLockDisabled,
                        .exclusiveAccessLock   = kXSPI_ExclusiveAccessLockDisabled,
                    },
                },
            }
        },
};
#endif

static const uint32_t s_customLUT[CUSTOM_LUT_LENGTH] = {
    /* OCTAL I/O FAST READ */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_8PAD, 0xFD, kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x03, kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x04),

    /* Read ID */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_ID_OPI + 0] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_8PAD, 0x9F, kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x08),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_ID_OPI + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x01, kXSPI_Command_STOP, kXSPI_1PAD, 0x00),

    /* Write enable, single SPI mode */
    [5 * NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x06, kXSPI_Command_STOP, kXSPI_1PAD, 0x00),

    /* Write enable, octal mode */
    [5 * NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE_OPI] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_8PAD, 0x06, kXSPI_Command_STOP, kXSPI_1PAD, 0x00),

    /* Erase sector */
    [5 * NOR_CMD_LUT_SEQ_IDX_ERASE_SECTOR] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_8PAD, 0x21, kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20),

    /* Erase chip */
    [5 * NOR_CMD_LUT_SEQ_IDX_ERASE_CHIP] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_8PAD, 0xC4, kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20),

    /* Extended octal input fast program */
    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL + 0] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_8PAD, 0x12, kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20),
    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x04, kXSPI_Command_STOP, kXSPI_1PAD, 0x00),

    /* Dummy write, do nothing when AHB write command is triggered. */
    [5 * NOR_CMD_LUT_SEQ_IDX_WRITE] =
        XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_1PAD, 0x00, kXSPI_Command_STOP, kXSPI_1PAD, 0x00),

    /* Read status register using octal DDR read */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI + 0] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_8PAD, 0x05, kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x08),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x01, kXSPI_Command_STOP, kXSPI_1PAD, 0x00),

    /* Volatile configuration register address 000000h - I/O mode */
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_OPI + 0] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x81, kXSPI_Command_SDR, kXSPI_1PAD, 0x00),
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_OPI + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x00, kXSPI_Command_SDR, kXSPI_1PAD, 0x00),
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_OPI + 2] =
        XSPI_LUT_SEQ(kXSPI_Command_WRITE_SDR, kXSPI_1PAD, 0x01, kXSPI_Command_STOP, kXSPI_1PAD, 0x00),
};

static status_t xspi_nor_write_enable(XSPI_Type *base, uint32_t baseAddr, bool enableOctal)
{
    xspi_transfer_t flashXfer;

    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE + baseAddr;
    flashXfer.cmdType         = kXSPI_Command;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = NULL;
    flashXfer.dataSize        = 0UL;
    flashXfer.lockArbitration = false;
    flashXfer.seqIndex =
        enableOctal ? NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE_OPI : NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE;

    return XSPI_TransferBlocking(base, &flashXfer);
}

static status_t xspi_nor_wait_bus_busy(XSPI_Type *base)
{
    bool isBusy;
    uint32_t readValue;
    status_t status;
    xspi_transfer_t flashXfer;

    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType         = kXSPI_Read;
    flashXfer.data            = &readValue;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.dataSize        = 2U;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI;
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
            isBusy = (0U != (readValue & (1UL << FLASH_BUSY_STATUS_OFFSET)));
        }
        else
        {
            isBusy = (0U == (readValue & (1UL << FLASH_BUSY_STATUS_OFFSET)));
        }
    } while (isBusy);

    return status;
}

static status_t xspi_nor_enable_octal_mode(XSPI_Type *base)
{
    xspi_transfer_t flashXfer;
    status_t status;
    uint32_t writeValue = FLASH_ENABLE_OCTAL_CMD;

    status = xspi_nor_write_enable(base, 0U, false);
    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType         = kXSPI_Write;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_ENTER_OPI;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = &writeValue;
    flashXfer.dataSize        = 1U;
    flashXfer.lockArbitration = false;

    status = XSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    return xspi_nor_wait_bus_busy(base);
}

static status_t xspi_nor_flash_erase_sector(XSPI_Type *base, uint32_t address)
{
    status_t status;
    xspi_transfer_t flashXfer;

    status = xspi_nor_write_enable(base, 0U, true);
    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE + address;
    flashXfer.cmdType         = kXSPI_Command;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_ERASE_SECTOR;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.lockArbitration = false;
    flashXfer.dataSize        = 0UL;
    flashXfer.data            = NULL;

    status = XSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    return xspi_nor_wait_bus_busy(base);
}

static status_t xspi_nor_flash_page_program(XSPI_Type *base, uint32_t dstAddr, uint32_t *src)
{
    status_t status;
    xspi_transfer_t flashXfer;

    status = xspi_nor_write_enable(base, dstAddr, true);
    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress   = EXAMPLE_XSPI_AMBA_BASE + dstAddr;
    flashXfer.cmdType         = kXSPI_Write;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = src;
    flashXfer.dataSize        = FLASH_PAGE_SIZE;
    flashXfer.lockArbitration = false;

    status = XSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    return xspi_nor_wait_bus_busy(base);
}

static status_t flash_eeprom_emulator_init(void)
{
    xspi_config_t config;
    xspi_ahb_access_config_t ahbAccessConfig;
    xspi_ip_access_config_t ipAccessConfig;
    uint32_t tempLUT[CUSTOM_LUT_LENGTH];

    memcpy(tempLUT, s_customLUT, sizeof(tempLUT));

    BOARD_InitXspiPins();
    xspi_clock_init();

    config.ptrAhbAccessConfig = &ahbAccessConfig;
    config.ptrIpAccessConfig  = &ipAccessConfig;
    XSPI_GetDefaultConfig(&config);

#if (defined(FSL_FEATURE_XSPI_HAS_END_CFG) && FSL_FEATURE_XSPI_HAS_END_CFG)
    config.byteOrder = kXSPI_64BitLE;
#endif
    config.ptrAhbAccessConfig->ahbErrorPayload.highPayload = 0x5A5A5A5AUL;
    config.ptrAhbAccessConfig->ahbErrorPayload.lowPayload  = 0x5A5A5A5AUL;
    config.ptrAhbAccessConfig->ptrAhbWriteConfig           = NULL;
    config.ptrAhbAccessConfig->ARDSeqIndex                 = NOR_CMD_LUT_SEQ_IDX_READ;
    config.ptrAhbAccessConfig->enableAHBBufferWriteFlush   = true;
    config.ptrAhbAccessConfig->enableAHBPrefetch           = true;

#if defined(ENABLE_SFP_CONFIG) && (ENABLE_SFP_CONFIG)
    config.ptrIpAccessConfig->ptrSfpFradConfig = &s_sfpFradConfig;
    config.ptrIpAccessConfig->ptrSfpMdadConfig = &s_sfpMdadConfig;
#else
    config.ptrIpAccessConfig->ptrSfpFradConfig = NULL;
    config.ptrIpAccessConfig->ptrSfpMdadConfig = NULL;
#endif
    config.ptrIpAccessConfig->ipAccessTimeoutValue           = 0xFFFFFFFFUL;
    config.ptrIpAccessConfig->sfpArbitrationLockTimeoutValue = 0xFFFFFFUL;

    XSPI_Init(EXAMPLE_XSPI, &config);
    XSPI_SetDeviceConfig(EXAMPLE_XSPI, &s_deviceConfig);
    XSPI_UpdateLUT(EXAMPLE_XSPI, 0U, tempLUT, CUSTOM_LUT_LENGTH);

    s_escDataOffset = ESC_EEPROM_FLASH_OFFSET;

    return xspi_nor_enable_octal_mode(EXAMPLE_XSPI);
}

static void flash_eeprom_emulator_read(uint8_t *buf, uint32_t len)
{
#if defined(DEMO_INVALIDATE_CACHES)
    DEMO_INVALIDATE_CACHES;
#endif
    memcpy(buf, (void const *)(EXAMPLE_XSPI_AMBA_BASE + s_escDataOffset), len);
}

static status_t flash_eeprom_emulator_flash(uint8_t *buf, uint32_t len)
{
    status_t status;
    uint32_t sectorIndex;
    uint32_t pageIndex;
    uint32_t sectorNum = (len + SECTOR_SIZE - 1U) / SECTOR_SIZE;
    uint32_t pageNum   = (len + FLASH_PAGE_SIZE - 1U) / FLASH_PAGE_SIZE;

    for (sectorIndex = 0U; sectorIndex < sectorNum; sectorIndex++)
    {
        status = xspi_nor_flash_erase_sector(EXAMPLE_XSPI, s_escDataOffset + sectorIndex * SECTOR_SIZE);
        if (status != kStatus_Success)
        {
            PRINTF("\r\nEEPROM emulator: erase sector failure!\r\n");
            return status;
        }
    }

    for (pageIndex = 0U; pageIndex < pageNum; pageIndex++)
    {
        status = xspi_nor_flash_page_program(EXAMPLE_XSPI, s_escDataOffset + pageIndex * FLASH_PAGE_SIZE,
                                             (uint32_t *)(void *)(buf + pageIndex * FLASH_PAGE_SIZE));
        if (status != kStatus_Success)
        {
            PRINTF("\r\nEEPROM emulator: program page failure!\r\n");
            return status;
        }
    }

#if defined(DEMO_INVALIDATE_CACHES)
    DEMO_INVALIDATE_CACHES;
#endif

    return kStatus_Success;
}

static void eeprom_emulator_set_update_status(uint8_t status, uint16_t time)
{
    eeprom_updated      = status;
    eeprom_updated_time = time;
}

static void eeprom_emulator_set_verify_status(uint8_t status, uint16_t time)
{
    eeprom_verified      = status;
    eeprom_verified_time = time;
}

static uint16_t eeprom_emulator_get_verify_time(void)
{
    return eeprom_verified_time;
}

static int eeprom_emulator_init(void)
{
    uint16_t u16Crc = 0x00FF;
    uint16_t i;
    uint16_t j;
    int crcInvalid = 0;

    if (flash_eeprom_emulator_init() != kStatus_Success)
    {
        PRINTF("EEPROM emulator: failed to initialize the XSPI flash backend\r\n");
        return 1;
    }

    flash_eeprom_emulator_read(esc_eeprom_cache, ESC_EMULATOR_EEPROM_SIZE);

    /* The first 14 bytes of the SII area are protected by a CRC8 in byte 14. */
    for (i = 0U; i < 14U; i++)
    {
        u16Crc ^= esc_eeprom_cache[i];
        for (j = 0U; j < 8U; j++)
        {
            if (u16Crc & 0x80U)
            {
                u16Crc = (uint16_t)((u16Crc << 1) ^ 0x07U);
            }
            else
            {
                u16Crc <<= 1;
            }
        }
    }

    u16Crc &= 0x00FFU;
    if (u16Crc != esc_eeprom_cache[14])
    {
        crcInvalid = 1;
    }

    return crcInvalid;
}

static void LPI2C_Slave_Init(void)
{
    LPI2C_Type *base = I2C_SLAVE_DEV;
    lpi2c_slave_config_t slaveConfig;

    LPI2C_SlaveGetDefaultConfig(&slaveConfig);
    slaveConfig.address0 = I2C_SLAVE_ADDR;
    LPI2C_SlaveInit(base, &slaveConfig, I2C_SLAVE_CLOCK_FREQUENCY);

    /* Disable LPI2C IRQ sources while the peripheral is configured. */
    LPI2C_SlaveDisableInterrupts(base, (uint32_t)kLPI2C_SlaveIrqFlags);
    LPI2C_MasterEnable(base, false);
    LPI2C_SlaveEnable(base, true);
    LPI2C_SlaveClearStatusFlags(base, (uint32_t)kLPI2C_SlaveClearFlags);

    LPI2C_SlaveEnableInterrupts(base, (uint32_t)kLPI2C_SlaveIrqFlags);
    EnableIRQ(I2C_SLAVE_IRQn);
}

static void eeprom_emulator_flush(uint16_t curr_time_ms)
{
    if ((eeprom_updated == 1U) && (eeprom_verified == 1U))
    {
        if ((uint16_t)(curr_time_ms - eeprom_emulator_get_verify_time()) > EMULATOR_EEPROM_UPDATE_TIME_MS)
        {
            (void)flash_eeprom_emulator_flash(esc_eeprom_cache, ESC_EMULATOR_EEPROM_SIZE);
            eeprom_emulator_set_update_status(0U, curr_time_ms);
            eeprom_emulator_set_verify_status(0U, curr_time_ms);
        }
    }
}
#endif /* EEPROM_I2C_EMULATOR */

static void Ecat_KickOff(void)
{
    int32_t result = SCMI_ERR_SUCCESS;

	result = SCMI_Reset(ECAT_CHANNEL, ECAT_DOMAIN_ID, ECAT_DISABLE_FLAGS, ECAT_REST_STATE);
    if (result != SCMI_ERR_SUCCESS)
    {
        PRINTF("%s: %d, Failed to reset ecat\r\n", __func__, __LINE__);
    }
    SDK_DelayAtLeastUs(10000U, SystemCoreClock);

    /* EtherCAT port0 is in MII mode */
    BLK_CTRL_NETCMIX->CFG_ECAT &= ~(1 << BLK_CTRL_NETCMIX_CFG_ECAT_RMII_SEL0_SHIFT);
    /* EtherCAT port1 is in MII mode */
    BLK_CTRL_NETCMIX->CFG_ECAT &= ~(1 << BLK_CTRL_NETCMIX_CFG_ECAT_RMII_SEL1_SHIFT);

    /* ECAT_LINK_ACT[0] polarity control: Active LOW */
    BLK_CTRL_NETCMIX->CFG_ECAT &= ~(1 << BLK_CTRL_NETCMIX_CFG_ECAT_ECAT_LINK_ACT0_POL_SHIFT);
    /* ECAT_LINK_ACT[1] polarity control: Active LOW */
    BLK_CTRL_NETCMIX->CFG_ECAT &= ~(1 << BLK_CTRL_NETCMIX_CFG_ECAT_ECAT_LINK_ACT1_POL_SHIFT);

    /* 16 bit I2C memory address range */
    BLK_CTRL_NETCMIX->CFG_ECAT |= (1 << BLK_CTRL_NETCMIX_CFG_ECAT_PROM_SIZE_SHIFT);

    /* EtherCAT PHY_OFFSET_VEC */
    BLK_CTRL_NETCMIX->CFG_ECAT |= (BLK_CTRL_NETCMIX_CFG_ECAT_PHY_OFFSET_VEC(2));

    SDK_DelayAtLeastUs(10000U, SystemCoreClock);
    
    result = SCMI_Reset(ECAT_CHANNEL, ECAT_DOMAIN_ID, ECAT_ENABLE_FLAGS, ECAT_REST_STATE);
    if (result != SCMI_ERR_SUCCESS)
    {
        PRINTF("%s: %d, Failed to reset ecat\r\n", __func__, __LINE__);
    }
    SDK_DelayAtLeastUs(10000U, SystemCoreClock);
}

UINT16 HW_Init(void)
{
    UINT32 intMask;
	SystemPlatformInit();
    BOARD_InitDebugConsolePins();	

    clk_t ecatClk = {
        .clkId = kCLOCK_Ecat,
        .pclkId = kCLOCK_Syspll1dfs1div2, /* 400 MHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 100000000UL,
    };

    CLOCK_SetParent(&ecatClk);
    CLOCK_SetRate(&ecatClk);
    CLOCK_EnableClock(ecatClk.clkId);

    /* Init board hardware. */
    BOARD_InitEcatLinkPins();
#ifdef ECAT_RMII_PORT
    BOARD_InitEcatPortRmiiPins();
#else
    BOARD_InitEcatPortMiiPins();
#endif
    BOARD_InitEcatI2CPins();
    BOARD_InitEcatMDIOPins();
    BOARD_InitEcatCLKPins();
    BOARD_InitEcatResetPins();
    BOARD_InitI2C6Pins();
    BOARD_InitGPIOPins();
#ifdef EEPROM_I2C_EMULATOR
    /* LPI2C4 acts as the emulated EEPROM target, wired to ECAT_SDA/ECAT_SCL. */
    BOARD_InitI2C4Pins();
#endif
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    PRINTF("Start the SSC digital_io example...\r\n");

#ifdef EEPROM_I2C_EMULATOR
    /* clang-format off */
    clk_t lpi2cSlvClk = {
        .clkId       = I2C_SLAVE_CLOCK_ROOT,
        .pclkId      = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate        = 24000000UL,
    };
    /* clang-format on */

    CLOCK_SetParent(&lpi2cSlvClk);
    CLOCK_SetRate(&lpi2cSlvClk);
    CLOCK_EnableClock(lpi2cSlvClk.clkId);

    PRINTF("ecat eeprom emulator...\r\n");

    if (eeprom_emulator_init() != 0)
    {
        PRINTF("Initialize EEPROM Emulator with the default image...\r\n");
        memset(esc_eeprom_cache, 0U, sizeof(esc_eeprom_cache));
        memcpy(esc_eeprom_cache, esc_eeprom_cache_default, sizeof(esc_eeprom_cache_default));
    }
    LPI2C_Slave_Init();

    /*
     * GPIO_IO12..GPIO_IO15 (the SPI8 group, which carries ECAT_SDA/ECAT_SCL in
     * this mode) are not wired directly to the headers. They go through the
     * TMUX1574 analog switches U22/U19, selected by SPI8_SEL1 and SPI8_SEL3.
     * Both must be driven low to route the SoC pins to the arduino header,
     * otherwise the jumper wires see nothing at all.
     */
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL1);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL1);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL3);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL3);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#endif

    /*Select ECAT1_CLK25*/
#if USING_ECAT_CLK25
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A0);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A1);

    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A0);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A0);
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A1);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A1);
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#else
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A0);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A1);

    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A0);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A1);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#endif

	/* Select ECAT_MII_SEL */
	BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S1_ID, ECAT_MII_SEL);

    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S1_ID, ECAT_MII_SEL);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S1_ID, ECAT_MII_SEL);
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
 
    /*Open ECAT EEPROM*/
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, CAN2_SEL);

    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S3_ID, CAN2_SEL);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, CAN2_SEL);
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    Ecat_KickOff();

#if BOARD_IMX943_TYPE == BOARD_IMX943_EVK
    SDK_DelayAtLeastUs(90000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
 
	/*Select Extended Register Page 0*/
    ECAT_EscMdioWrite(ETHERCAT, 0x00, 0x13, 0x00);
   
	 /*Select Mode 1: LED_LINK = ON for Good Link, OFF for No Link*/
    ECAT_EscMdioWrite(ETHERCAT, 0x00, 0x19, (1 << 5));
	
	/*Select Extended Register Page 0*/
    ECAT_EscMdioWrite(ETHERCAT, 0x01, 0x13, 0x00);

    /*Select Mode 1: LED_LINK = ON for Good Link, OFF for No Link*/
    ECAT_EscMdioWrite(ETHERCAT, 0x01, 0x19, (1 << 5));
#endif

    intMask = 0x00;

    HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

    /* Set systick reload value to generate 1ms interrupt */
    SysTick_Config(SystemCoreClock / 1000U);

    /*Enable PDI IRQ*/
    EnableIRQ(Reserved192_IRQn);
    EnableIRQ(Reserved194_IRQn);
    EnableIRQ(Reserved195_IRQn);

    return 0;
}

void Reserved192_IRQHandler(void)
{
    PDI_Isr();

    SDK_ISR_EXIT_BARRIER;
}

/*config Sync0 IRQ*/
void Reserved194_IRQHandler(void)
{
	Sync0_Isr();
    SDK_ISR_EXIT_BARRIER;
}

/*config Sync1 IRQ*/
void Reserved195_IRQHandler(void)
{
	Sync1_Isr();
    SDK_ISR_EXIT_BARRIER;
}

void HW_Release(void)
{
}

void SysTick_Handler(void)
{
    /* Clear interrupt flag.*/
#if ECAT_TIMER_INT
    ECAT_CheckTimer();
#endif
    EcatTimerCnt++;
#ifdef EEPROM_I2C_EMULATOR
    g_systickCounter++;
    eeprom_emulator_flush((uint16_t)g_systickCounter);
#endif
    SDK_ISR_EXIT_BARRIER;
}

#ifdef EEPROM_I2C_EMULATOR
/*
 * Emulated EEPROM target handler. The ESC EEPROM host uses the 2-byte address
 * mode: a write access starts with the high and low address bytes, a read
 * access continues at the address latched by the preceding write.
 */
void I2C_SLAVE_IRQHandler(void)
{
    LPI2C_Type *base = I2C_SLAVE_DEV;
    static uint16_t eepromMemoryAddr = 0U;
    uint32_t primaskReg;
    uint32_t flags;
    uint8_t recv;
    uint8_t slaveAddr;

    primaskReg = DisableGlobalIRQ();
    LPI2C_SlaveDisableInterrupts(base, (uint32_t)kLPI2C_SlaveIrqFlags);

    while (true)
    {
        flags = LPI2C_SlaveGetStatusFlags(base);

        if (0U != (flags & ((uint32_t)kLPI2C_SlaveBitErrFlag | (uint32_t)kLPI2C_SlaveFifoErrFlag)))
        {
            LPI2C_SlaveClearStatusFlags(base, (uint32_t)kLPI2C_SlaveClearFlags);
            break;
        }

        if (0U != (flags & (uint32_t)kLPI2C_SlaveAddressValidFlag))
        {
            slaveAddr = (uint8_t)(base->SASR & 0xFFU);
            if (0U != (slaveAddr & 0x01U))
            {
                EEPROM_status = memory_reading;
            }
            else
            {
                EEPROM_status = memory_addr_hi;
            }
        }

        if (0U != (flags & (uint32_t)kLPI2C_SlaveRxReadyFlag))
        {
            recv = (uint8_t)(base->SRDR & LPI2C_SRDR_DATA_MASK);
            if (EEPROM_status == memory_addr_hi)
            {
                eepromMemoryAddr = (uint16_t)(recv << 8);
                EEPROM_status    = memory_addr_lo;
            }
            else if (EEPROM_status == memory_addr_lo)
            {
                eepromMemoryAddr += recv;
                EEPROM_status = memory_writing;
            }
            else if (EEPROM_status == memory_writing)
            {
                esc_eeprom_cache[eepromMemoryAddr++] = recv;
                if (eepromMemoryAddr >= ESC_EMULATOR_EEPROM_SIZE)
                {
                    eepromMemoryAddr = 0U;
                }
            }
            else
            {
                break;
            }
        }

        if (0U != (flags & (uint32_t)kLPI2C_SlaveRepeatedStartDetectFlag))
        {
            LPI2C_SlaveClearStatusFlags(base, (uint32_t)kLPI2C_SlaveRepeatedStartDetectFlag);
        }

        if (0U != (flags & (uint32_t)kLPI2C_SlaveTxReadyFlag))
        {
            if (EEPROM_status != memory_reading)
            {
                continue;
            }
            base->STDR = esc_eeprom_cache[eepromMemoryAddr++];
            if (eepromMemoryAddr >= ESC_EMULATOR_EEPROM_SIZE)
            {
                eepromMemoryAddr = 0U;
            }
        }

        if (0U != (flags & (uint32_t)kLPI2C_SlaveStopDetectFlag))
        {
            if (EEPROM_status == memory_writing)
            {
                eeprom_emulator_set_verify_status(0U, (uint16_t)g_systickCounter);
                eeprom_emulator_set_update_status(1U, (uint16_t)g_systickCounter);
            }
            else if (EEPROM_status == memory_reading)
            {
                eeprom_emulator_set_verify_status(1U, (uint16_t)g_systickCounter);
            }
            else
            {
                /* Nothing to do. */
            }
            EEPROM_status = memory_addr_hi;
            break;
        }
    }

    LPI2C_SlaveClearStatusFlags(base, (uint32_t)kLPI2C_SlaveClearFlags);
    LPI2C_SlaveEnableInterrupts(base, (uint32_t)kLPI2C_SlaveIrqFlags);

    EnableGlobalIRQ(primaskReg);

    SDK_ISR_EXIT_BARRIER;
}
#endif /* EEPROM_I2C_EMULATOR */

UINT16 HW_GetTimer(void)
{
    return EcatTimerCnt;
}

void HW_ClearTimer(void)
{
    EcatTimerCnt = 0;
}

void ENABLE_ESC_INT(void)
{
    NVIC_EnableIRQ(Reserved192_IRQn);
    NVIC_EnableIRQ(Reserved194_IRQn);
    NVIC_EnableIRQ(Reserved195_IRQn);
}

void DISABLE_ESC_INT(void)
{
    NVIC_DisableIRQ(Reserved192_IRQn);
    NVIC_DisableIRQ(Reserved194_IRQn);
    NVIC_DisableIRQ(Reserved195_IRQn);
}

void HW_SetLed(UINT8 RunLed, UINT8 ErrorLed)
{
}
