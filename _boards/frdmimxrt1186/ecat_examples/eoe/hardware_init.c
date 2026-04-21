/*
 * Copyright 2025-2026 NXP
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
#include "fsl_ele_base_api.h"
#include "fsl_gpt.h"

#include "ecat_def.h"
#include "ecatslv.h"
#include "ecat_hw.h"
#include "ecatappl.h"

#include "app.h"

UINT32 EcatTimerCnt;
volatile uint32_t g_systickCounter;

#ifdef EEPROM_I2C_EMULATOR
/*! FLEXSPI NOR flash driver Structure */
static flexspi_nor_config_t norConfig;
static uint8_t eeprom_updated = 0;
static uint16_t eeprom_updated_time = 0;
enum EEPROM_access_status
{
    memory_reading = 0,
    memory_writing,
    memory_addr_hi,
    memory_addr_lo,
};

static enum EEPROM_access_status EEPROM_status = memory_addr_hi;
lpi2c_slave_handle_t g_lpi2cHandle;
volatile bool g_i2cEeprom_updated = false;

static uint32_t pflashEscDataOffset = 0;
static uint32_t pflashEscDataLength = 0;

uint8_t esc_eeprom_cache_default[] = {
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

uint8_t esc_eeprom_cache[ESC_EMULATOR_EEPROM_SIZE];

#endif

#ifdef EEPROM_I2C_EMULATOR
/* These settings are related to board design for external QSPI flash memory, please adjust it accordingly based on your design */
status_t FLEXSPI_NorFlash_GetConfig(uint32_t instance,
                                           flexspi_nor_config_t *config)
{
    config->memConfig.tag = 0x42464346UL;
    config->memConfig.version = 0x56010400UL;
    config->memConfig.readSampleClkSrc = kFLEXSPIReadSampleClk_LoopbackFromDqsPad;
    config->memConfig.csHoldTime = 3;
    config->memConfig.csSetupTime = 3;
    config->memConfig.controllerMiscOption = 0x10;
    config->memConfig.deviceType = kFLEXSPIDeviceType_SerialNOR;
    config->memConfig.sflashPadType = kSerialFlash_4Pads;
    config->memConfig.serialClkFreq = kFLEXSPISerialClk_100MHz;
    config->memConfig.sflashA1Size = 16u * 1024u * 1024u,
    config->memConfig.configModeType[0] = kDeviceConfigCmdType_Generic,
 
    config->memConfig.lookupTable[0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0xEB, RADDR_SDR, FLEXSPI_4PAD, 0x18);
    config->memConfig.lookupTable[1] = FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_4PAD, FLASH_DUMMY_CYCLES, READ_SDR, FLEXSPI_4PAD, 0x04);
    config->memConfig.lookupTable[4 * 1 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x05, READ_SDR, FLEXSPI_1PAD, 0x04);
    config->memConfig.lookupTable[4 * 3 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x06, STOP, FLEXSPI_1PAD, 0x0);    
    config->memConfig.lookupTable[4 * 5 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x20, RADDR_SDR, FLEXSPI_1PAD, 0x18);    
    config->memConfig.lookupTable[4 * 8 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0xD8, RADDR_SDR, FLEXSPI_1PAD, 0x18);
    config->memConfig.lookupTable[4 * 9 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x02, RADDR_SDR, FLEXSPI_1PAD, 0x18);
    config->memConfig.lookupTable[4 * 9 + 1] = FLEXSPI_LUT_SEQ(WRITE_SDR, FLEXSPI_1PAD, 0x04, STOP, FLEXSPI_1PAD, 0x0);    
    config->memConfig.lookupTable[4 * 11 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x60, STOP, FLEXSPI_1PAD, 0x0);       

    config->pageSize = 256u;
    config->sectorSize = 4u * 1024u;
    config->blockSize = 64u * 1024u;
    config->ipcmdSerialClkFreq = 0x1;
    config->isUniformBlockSize = false;

    return kStatus_Success;
}

status_t flash_eeprom_emulator_init()
{
    uint32_t serialNorTotalSize;
    uint32_t serialNorSectorSize;

    int sectorNum = 0;
    status_t status;

    ROM_API_Init();
    /* Clean up FLEXSPI NOR flash driver Structure */
    memset(&norConfig, 0U, sizeof(flexspi_nor_config_t));

#if (__CORTEX_M == 7U)
    /* Disable I cache */
    SCB_DisableICache();
#endif

    /* Setup FLEXSPI NOR Configuration Block */
    status = FLEXSPI_NorFlash_GetConfig(FlexSpiInstance, &norConfig);
    if (kStatus_Success != status)
    {
        return status;
    }

    /* Initializes the FLEXSPI module for the other FLEXSPI APIs */
    status = ROM_FLEXSPI_NorFlash_Init(FlexSpiInstance, &norConfig);
    /* Perform software reset after initializing flexspi module */
    ROM_FLEXSPI_NorFlash_ClearCache(FlexSpiInstance);
    serialNorTotalSize  = norConfig.memConfig.sflashA1Size;
    serialNorSectorSize = norConfig.sectorSize;

#if (__CORTEX_M == 7U)
    /* Enable I cache */
    SCB_EnableICache();
#endif

    sectorNum = (ESC_EMULATOR_EEPROM_SIZE + serialNorSectorSize - 1) / serialNorSectorSize;
    pflashEscDataOffset =  serialNorTotalSize - (sectorNum * serialNorSectorSize);
    pflashEscDataLength = serialNorSectorSize * sectorNum;

    return kStatus_Success;
}

void eeprom_emulator_set_update_status(uint8_t status, uint16_t time)
{
    eeprom_updated = status;
    eeprom_updated_time = time;
}

uint16_t eeprom_emulator_get_update_time()
{
    return eeprom_updated_time;
}

void flash_eeprom_emulator_update(uint8_t *buf, uint32_t len)
{
    MEMCPY(buf, (void const *)(FLEXSPI_AMBA_BASE + pflashEscDataOffset), len);
}

status_t flash_eeprom_emulator_flash(uint8_t *buf, uint32_t len)
{
    status_t status = kStatus_Success;
    uint32_t serialNorPageSize   = norConfig.pageSize;
    int page_index = 0;
    status = ROM_FLEXSPI_NorFlash_Erase(FlexSpiInstance, &norConfig, pflashEscDataOffset, pflashEscDataLength);
    if (kStatus_Success != status) {
        PRINTF("\r\n Erase sector failure!\r\n");
        return status;
    }

    /* Program user buffer into flash*/
    for (page_index = 0; page_index < (len + serialNorPageSize - 1) / serialNorPageSize; page_index++) {
        status = ROM_FLEXSPI_NorFlash_ProgramPage(FlexSpiInstance, &norConfig, pflashEscDataOffset + page_index * serialNorPageSize, (const uint32_t *)(buf + page_index * serialNorPageSize));
        if (kStatus_Success != status) {
            PRINTF("\r\n Pra sector failure!\r\n");
            return status;
        }
    }
    DCACHE_InvalidateByRange(FLEXSPI_AMBA_BASE + pflashEscDataOffset, len);

    return status;
}

int eeprom_emulator_init(void)
{
    uint16_t u16Crc = 0x00FF, i, j;
    int crcInvalid = 0;
    flash_eeprom_emulator_init();
    flash_eeprom_emulator_update(esc_eeprom_cache, ESC_EMULATOR_EEPROM_SIZE);

    for(i = 0; i < 14; i++) {
        u16Crc ^= esc_eeprom_cache[i];
        for(j = 0; j < 8; j++) {
            if(u16Crc & 0x80) {
                u16Crc = (u16Crc << 1) ^ 0x07;
            } else {
                u16Crc <<= 1;
            }
        }
    }

    u16Crc &= 0x00FF;
    if(u16Crc != esc_eeprom_cache[14]) {
        crcInvalid = 1;
    }
    
    return crcInvalid;
}

void LPI2C_Slave_Init()
{
    LPI2C_Type *base = I2C_SLAVE_DEV;
    lpi2c_slave_config_t slaveConfig;
    LPI2C_SlaveGetDefaultConfig(&slaveConfig);
    slaveConfig.address0 = 0x50; //I2C_TARGET_ADDR;
    LPI2C_SlaveInit(base, &slaveConfig, I2C_SLAVE_CLOCK_FREQUENCY);
    /* Disable LPI2C IRQ sources while we configure stuff. */
    LPI2C_SlaveDisableInterrupts(base, (uint32_t)kLPI2C_SlaveIrqFlags);
    LPI2C_MasterEnable(base, false);
    LPI2C_SlaveEnable(base, true);
    /* Clear all flags. */
    LPI2C_SlaveClearStatusFlags(base, (uint32_t)kLPI2C_SlaveClearFlags);

    /* Enable LPI2C internal IRQ sources. NVIC IRQ was enabled in CreateHandle() */
    LPI2C_SlaveEnableInterrupts(base, (uint32_t)kLPI2C_SlaveIrqFlags);
    EnableIRQ(LPI2C3_IRQn);
}

void eeprom_emulator_flash(uint16_t curr_time_ms)
{
    if (eeprom_updated == 1) {
        if ((uint16_t)(curr_time_ms - eeprom_emulator_get_update_time()) > EMULATOR_EEPROM_UPDATE_TIME_MS) { // exceed 100ms
            flash_eeprom_emulator_flash(esc_eeprom_cache, ESC_EMULATOR_EEPROM_SIZE);
            eeprom_emulator_set_update_status(0, curr_time_ms);
        }
    }
}
#endif // EEPROM_I2C_EMULATOR

static void Ecat_KickOff(void)
{
    BLK_CTRL_WAKEUPMIX->ECAT_MISC_CFG |= BLK_CTRL_WAKEUPMIX_ECAT_MISC_CFG_GLB_RST_MASK;
    BLK_CTRL_WAKEUPMIX->ECAT_MISC_CFG &= ~BLK_CTRL_WAKEUPMIX_ECAT_MISC_CFG_GLB_EN_MASK;
    BLK_CTRL_WAKEUPMIX->ECAT_MISC_CFG &= ~BLK_CTRL_WAKEUPMIX_ECAT_MISC_CFG_RMII_SEL0_MASK;
    BLK_CTRL_WAKEUPMIX->ECAT_MISC_CFG &= ~BLK_CTRL_WAKEUPMIX_ECAT_MISC_CFG_RMII_SEL1_MASK;

    BLK_CTRL_WAKEUPMIX->MISC_IO_CTRL &= ~(1 << BLK_CTRL_WAKEUPMIX_MISC_IO_CTRL_ECAT_LINK_ACT0_POL_SHIFT);
    BLK_CTRL_WAKEUPMIX->MISC_IO_CTRL &= ~(1 << BLK_CTRL_WAKEUPMIX_MISC_IO_CTRL_ECAT_LINK_ACT1_POL_SHIFT);

    BLK_CTRL_WAKEUPMIX->ECAT_MISC_CFG |= (1 << BLK_CTRL_WAKEUPMIX_ECAT_MISC_CFG_EEPROM_SIZE_OPTION_SHIFT);

    SRC_GENERAL_REG->SRMASK &= ~(0x1 << SRC_GENERAL_SRMASK_ECAT_RSTO_MASK_SHIFT);

    BLK_CTRL_WAKEUPMIX->ECAT_MISC_CFG |= (BLK_CTRL_WAKEUPMIX_ECAT_MISC_CFG_PHY_OFFSET_VEC(2));
    BLK_CTRL_WAKEUPMIX->ECAT_MISC_CFG &= ~BLK_CTRL_WAKEUPMIX_ECAT_MISC_CFG_GLB_RST_MASK;
    BLK_CTRL_WAKEUPMIX->ECAT_MISC_CFG |= BLK_CTRL_WAKEUPMIX_ECAT_MISC_CFG_GLB_EN_MASK;
}

UINT16 HW_Init(void)
{
    UINT32 intMask;
    UINT16 led_status = 0;
    xbar_control_config_t xbaraConfig;
    uint32_t gptFreq;
    gpt_config_t gptConfig;
    rgpio_pin_config_t pinConfig = {.pinDirection = kRGPIO_DigitalOutput, .outputLogic = 0};

    /* Init board hardware. */
    BOARD_CommonSetting();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    PRINTF("Start the SSC EoE example...\r\n");

#ifdef EEPROM_I2C_EMULATOR
    /* Initialize EEPROM Emulator */
    if (eeprom_emulator_init()) {
        PRINTF("Initialize EEPROM Emulator...\r\n");
        memcpy(esc_eeprom_cache, esc_eeprom_cache_default, sizeof(esc_eeprom_cache_default));
    }
    LPI2C_Slave_Init();
#endif

    /*Enable GPT1*/
    GPT_GetDefaultConfig(&gptConfig);
    GPT_Init(GPT1, &gptConfig);
    gptFreq = CLOCK_GetRootClockFreq(kCLOCK_Root_Gpt1);
    GPT_SetClockDivider(GPT1, 100);
    GPT_SetOutputCompareValue(GPT1, kGPT_OutputCompare_Channel1, gptFreq / 100000);
    GPT_EnableInterrupts(GPT1, kGPT_OutputCompare1InterruptEnable);
    EnableIRQ(GPT1_IRQn);
    GPT_StartTimer(GPT1);

    Ecat_KickOff();

    SDK_DelayAtLeastUs(90000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /*set port0 page register*/
    ECAT_EscMdioWrite(ECAT, 0x00, 31, 0x07);

    /*enable prot0 coustomized LED */
    ECAT_EscMdioRead(ECAT, 0x00, 19, &led_status);
    ECAT_EscMdioWrite(ECAT, 0x00, 19, led_status | (1 << 3));

    /*Set led1 to LINK100 and set led0 to ACK*/
    ECAT_EscMdioRead(ECAT, 0x00, 17, &led_status);
    ECAT_EscMdioWrite(ECAT, 0x00, 17, led_status | (1 << 3) | (1 << 5));

    /*set port1 page register*/
    ECAT_EscMdioWrite(ECAT, 0x01, 31, 0x07);

    /*enable prot1 coustomized LED */
    ECAT_EscMdioRead(ECAT, 0x01, 19, &led_status);
    ECAT_EscMdioWrite(ECAT, 0x01, 19, led_status | (1 << 3));

    /*Set led1 to LINK100 and set led0 to ACK*/
    ECAT_EscMdioRead(ECAT, 0x01, 17, &led_status);
    ECAT_EscMdioWrite(ECAT, 0x01, 17, led_status | (1 << 3) | (1 << 5));

    /*Disable phy eee mode*/
    ECAT_EscMdioWrite(ECAT, 0x00, 31, 4);
    ECAT_EscMdioWrite(ECAT, 0x00, 16, 0x4077);
    ECAT_EscMdioWrite(ECAT, 0x00, 31, 0);
    ECAT_EscMdioWrite(ECAT, 0x00, 13, 0x0007);
    ECAT_EscMdioWrite(ECAT, 0x00, 14, 0x003c);
    ECAT_EscMdioWrite(ECAT, 0x00, 13, 0x4007);
    ECAT_EscMdioWrite(ECAT, 0x00, 14, 0x0);
    ECAT_EscMdioWrite(ECAT, 0x00, 0, 0x1200);
    ECAT_EscMdioWrite(ECAT, 0x01, 31, 4);
    ECAT_EscMdioWrite(ECAT, 0x01, 16, 0x4077);
    ECAT_EscMdioWrite(ECAT, 0x01, 31, 0);
    ECAT_EscMdioWrite(ECAT, 0x01, 13, 0x0007);
    ECAT_EscMdioWrite(ECAT, 0x01, 14, 0x003c);
    ECAT_EscMdioWrite(ECAT, 0x01, 13, 0x4007);
    ECAT_EscMdioWrite(ECAT, 0x01, 14, 0x0);
    ECAT_EscMdioWrite(ECAT, 0x01, 0, 0x1200);

    RGPIO_PinInit(GPIO_LED, GPIO_LED_PIN, &pinConfig);

    /*config Sync0/1 IRQ*/
    XBAR_Init(kXBAR_DSC1);

    XBAR_SetSignalsConnection(kXBAR1_InputEcatSyncOut0, kXBAR1_OutputDma4MuxReq154);
    BLK_CTRL_WAKEUPMIX->XBAR_TRIG_SYNC_CTRL1 = 0x0;
    xbaraConfig.activeEdge                   = kXBAR_EdgeRising;
    xbaraConfig.requestType                  = kXBAR_RequestInterruptEnable;
    XBAR_SetOutputSignalConfig(kXBAR1_OutputDma4MuxReq154, &xbaraConfig);
    BLK_CTRL_WAKEUPMIX->XBAR_TRIG_SYNC_CTRL1 |= 0xff << 16;
    BLK_CTRL_WAKEUPMIX->XBAR_TRIG_SYNC_CTRL1 |= 0xff << 8;
    BLK_CTRL_WAKEUPMIX->XBAR_TRIG_SYNC_CTRL2 |= 3;
    BLK_CTRL_WAKEUPMIX->XBAR_TRIG_SYNC_CTRL2 |= 3 << 4;

    XBAR_SetSignalsConnection(kXBAR1_InputEcatSyncOut1, kXBAR1_OutputDma4MuxReq155);
    xbaraConfig.activeEdge  = kXBAR_EdgeRising;
    xbaraConfig.requestType = kXBAR_RequestInterruptEnable;
    XBAR_SetOutputSignalConfig(kXBAR1_OutputDma4MuxReq155, &xbaraConfig);

    do
    {
        intMask = 0x93;
        HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
        intMask = 0;
        HW_EscReadDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
    } while (intMask != 0x93);

    intMask = 0x00;

    HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

    /*Enable PDI IRQ*/
    EnableIRQ(ECAT_INT_IRQn);
    NVIC_EnableIRQ(XBAR1_CH0_CH1_IRQn);

    return 0;
}

void ECAT_INT_IRQHandler(void)
{
    PDI_Isr();

    SDK_ISR_EXIT_BARRIER;
}

/*config Sync0/1 IRQ*/
void XBAR1_CH0_CH1_IRQHandler(void)
{
    bool status;
    XBAR_GetOutputStatusFlag(kXBAR1_OutputDma4MuxReq154, &status);
    if (status)
    {
        XBAR_ClearOutputStatusFlag(kXBAR1_OutputDma4MuxReq154);
        Sync0_Isr();
    }

    XBAR_GetOutputStatusFlag(kXBAR1_OutputDma4MuxReq155, &status);
    if (status)
    {
        XBAR_ClearOutputStatusFlag(kXBAR1_OutputDma4MuxReq155);
        Sync1_Isr();
    }

    SDK_ISR_EXIT_BARRIER;
}

void HW_Release(void)
{
}

void GPT1_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    GPT_ClearStatusFlags(GPT1, kGPT_OutputCompare1Flag);
#if ECAT_TIMER_INT
    ECAT_CheckTimer();
#endif
    EcatTimerCnt++;
    g_systickCounter++;
#ifdef EEPROM_I2C_EMULATOR
    eeprom_emulator_flash(g_systickCounter);
#endif

    /*
     *  RT118x ELE requires ping every 24 hours, which is mandatory,
     *  otherwise soc may reset.
     *
     *  note:
     *    1. This is generic rule for all RT118x demos.
     *    2. Most of RT118x demos don't ping ELE every 24 hours, that
     *       is because those demos focus on the function demonstrate only.
     *       It is still MUST to ping ELE every 24 hours if demo run
     *       duration > 24 hours.
     *    3. Below is an example to ping the ELE every 23(but not 24)
     *       hours, in case of any clock inaccuracy.
     */
    if (g_systickCounter >= (23 * 60 * 60 * 1000UL))
    {
        g_systickCounter = 0;
        ELE_BaseAPI_Ping(MU_RT_S3MUA);
    }

    SDK_ISR_EXIT_BARRIER;
}

#ifdef EEPROM_I2C_EMULATOR
uint8_t EEPROM_addr;
uint16_t EEPROM_memory_addr;
uint32_t ui32PrimaskReg;
void LPI2C3_IRQHandler(void)
{
    LPI2C_Type *base = I2C_SLAVE_DEV;
    uint32_t flags;
    uint8_t recv;
    /* Disable all interrupts */
    ui32PrimaskReg = DisableGlobalIRQ();
    /* Disable LPI2C IRQ sources while we configure stuff. */
    LPI2C_SlaveDisableInterrupts(base, (uint32_t)kLPI2C_SlaveIrqFlags);
    while(1) {
        flags = LPI2C_SlaveGetStatusFlags(base);
        if (0U != (flags & ((uint32_t)kLPI2C_SlaveBitErrFlag | (uint32_t)kLPI2C_SlaveFifoErrFlag))) {
            LPI2C_SlaveClearStatusFlags(base, kLPI2C_SlaveClearFlags);
            break;
        } else {
            if (0U != (flags & (uint32_t)kLPI2C_SlaveAddressValidFlag)) {
                EEPROM_addr = (uint8_t)(base->SASR & 0xffU);
                if (EEPROM_addr & 0x01) {
                    EEPROM_status = memory_reading;
                } else {
                    EEPROM_status = memory_addr_hi;

                }
            }

            if (0U != (flags & (uint32_t)kLPI2C_SlaveRxReadyFlag)) {
                recv = (uint8_t)(base->SRDR & LPI2C_SRDR_DATA_MASK);
                if (EEPROM_status == memory_addr_hi) {
                    EEPROM_memory_addr = (recv << 8);
                    EEPROM_status = memory_addr_lo;
                } else if (EEPROM_status == memory_addr_lo) {
                    EEPROM_memory_addr += recv;
                    EEPROM_status = memory_writing;
                }
                else if (EEPROM_status == memory_writing)
                {
                    esc_eeprom_cache[EEPROM_memory_addr++] = recv;
                    g_i2cEeprom_updated = true;
                    if (EEPROM_memory_addr >= ESC_EMULATOR_EEPROM_SIZE) {
                        EEPROM_memory_addr = 0;
                    }
                } else {
                    break;
                }
            }

            if (0U != (flags & (uint32_t)kLPI2C_SlaveRepeatedStartDetectFlag)) {
                LPI2C_SlaveClearStatusFlags(base, kLPI2C_SlaveRepeatedStartDetectFlag);
            }

            if (0U != (flags & (uint32_t)kLPI2C_SlaveTxReadyFlag)) {
                
                if (EEPROM_status != memory_reading) {
                    continue;

                }
                base->STDR = esc_eeprom_cache[EEPROM_memory_addr++];
                if (EEPROM_memory_addr >= ESC_EMULATOR_EEPROM_SIZE) {
                    EEPROM_memory_addr = 0;
                }
            }

            if (0U != (flags & (uint32_t)kLPI2C_SlaveStopDetectFlag)) {
                if (EEPROM_status == memory_writing) {
                    eeprom_emulator_set_update_status(1, g_systickCounter);
                } else {
                }
                EEPROM_status = memory_addr_hi;
                break;

            }
        }
    }

    LPI2C_SlaveClearStatusFlags(base, kLPI2C_SlaveClearFlags);
    LPI2C_SlaveEnableInterrupts(base, (uint32_t)kLPI2C_SlaveIrqFlags);

    /* Enable all interrupts */
    EnableGlobalIRQ(ui32PrimaskReg);

    SDK_ISR_EXIT_BARRIER;
}
#endif // EEPROM_I2C_EMULATOR

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
    NVIC_EnableIRQ(ECAT_INT_IRQn);
    NVIC_EnableIRQ(XBAR1_CH0_CH1_IRQn);
    NVIC_EnableIRQ(GPT1_IRQn);
}

void DISABLE_ESC_INT(void)
{
    NVIC_DisableIRQ(XBAR1_CH0_CH1_IRQn);
    NVIC_DisableIRQ(ECAT_INT_IRQn);
    NVIC_DisableIRQ(GPT1_IRQn);
}

void HW_SetLed(UINT8 RunLed, UINT8 ErrorLed)
{
}