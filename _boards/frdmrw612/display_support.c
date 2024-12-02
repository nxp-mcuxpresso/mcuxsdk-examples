/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "display_support.h"
#include "fsl_gpio.h"
#include "fsl_i2c.h"
#include "fsl_debug_console.h"

#if (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
#include "fsl_st7796s.h"
#include "fsl_gt911.h"
#include "fsl_dbi_lcdic_dma.h"
#include "fsl_inputmux.h"
#else
#include "fsl_ili9341.h"
#include "fsl_ft6x06.h"
#include "fsl_dbi_spi_dma.h"
#endif

#include "fsl_video_common.h"
#include "fsl_dc_fb_dbi.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LCDIC LCDIC
#define DEMO_DMA   DMA0

#define DEMO_LCDC_TX_DMA_CH 0

#define DEMO_TOUCH_I2C            I2C2
#define DEMO_TOUCH_I2C_CLOCK_FREQ CLOCK_GetFlexCommClkFreq(2)

#define DEMO_SPI                   SPI1
#define DEMO_SPI_CLOCK_FREQ CLOCK_GetFlexCommClkFreq(1)
#define DEMO_SPI_IRQn              FLEXCOMM1_IRQn
#define DEMO_SPI_TX_DMA_CH 3
#define DEMO_SPI_RX_DMA_CH 2

#define DEMO_DBI_DC_PORT 1
#define DEMO_DBI_DC_PIN (52 - (DEMO_DBI_DC_PORT * 32U))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* LCD functions. */

/* Touch panel functions. */
static status_t DEMO_TouchI2C_Init(void);
static status_t DEMO_TouchI2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
static status_t DEMO_TouchI2C_Receive(uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

#if (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
static status_t DEMO_InitLcdc(void);
#else
static status_t DEMO_InitSpi(void);
#endif
static status_t DEMO_CreateDC(void);
static status_t DEMO_CreateDbiIface(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static dc_fb_dbi_handle_t s_dcDbiHandle;
const dc_fb_t g_dc =
{
    .ops = &g_dcFbOpsDbi,
    .prvData = &s_dcDbiHandle,
    .config = NULL,
};

#if (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
static st7796s_handle_t lcdHandle;
static dma_handle_t s_lcdDmaTxHandle;
static dbi_lcdic_dma_prv_data_t s_dbiLcdicDmaPrvData;
static gt911_handle_t touchHandle;
#else /* DEMO_PANEL_ILI9341 */
static ili9341_handle_t lcdHandle;
static dma_handle_t s_lcdDmaTxHandle, s_lcdDmaRxHandle;
static dbi_spi_dma_prv_data_t s_dbiSpiDmaPrvData;
static ft6x06_handle_t touchHandle;
#endif

#if (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
AT_NONCACHEABLE_SECTION_ALIGN(static dma_descriptor_t s_dmaDesc[2], 16);
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t BOARD_PrepareDisplayController(void)
{
#if (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
    DEMO_InitLcdc();
    DEMO_CreateDbiIface();
    DEMO_CreateDC();
#else
    DEMO_InitSpi();
    DEMO_CreateDbiIface();
    DEMO_CreateDC();
#endif
    return kStatus_Success;
}

status_t BOARD_PrepareTouchPanel(void)
{
    status_t status;

    status = DEMO_TouchI2C_Init();

    return status;
}

status_t BOARD_InitTouchPanel(void)
{
    status_t status;

#if (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
    gt911_config_t touchConfig = {.I2C_SendFunc     = DEMO_TouchI2C_Send,
                                  .I2C_ReceiveFunc  = DEMO_TouchI2C_Receive,
                                  .timeDelayMsFunc  = VIDEO_DelayMs,
                                  .intPinFunc       = NULL,
                                  .pullResetPinFunc = NULL,
                                  .touchPointNum    = 1,
                                  .i2cAddrMode      = kGT911_I2cAddrAny,
                                  .intTrigMode      = kGT911_IntFallingEdge};

    status = GT911_Init(&touchHandle, &touchConfig);
#else
    ft6x06_config_t touchConfig = {.I2C_SendFunc     = DEMO_TouchI2C_Send,
                                  .I2C_ReceiveFunc  = DEMO_TouchI2C_Receive
    };

    status = FT6X06_Init(&touchHandle, &touchConfig);
#endif

    if (status != kStatus_Success)
    {
        PRINTF("Touch panel init failed\n");
    }

    return status;
}

status_t BOARD_GetTouchPanelPoint(int *x, int *y)
{
    status_t status;

    static int touch_x = 0;
    static int touch_y = 0;

#if (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)

    status = GT911_GetSingleTouch(&touchHandle, &touch_x, &touch_y);
    *x = touch_y;
    *y = touchHandle.resolutionX - touch_x;

#else

    touch_event_t touch_event;

    status = FT6X06_GetSingleTouch(&touchHandle, &touch_event, &touch_x, &touch_y);
    if ((status == kStatus_Success) && (touch_event == kTouch_Down) || (touch_event == kTouch_Contact))
    {
        status = kStatus_Success;
    }
    else
    {
        status = kStatus_Fail;
    }

    *x = DEMO_PANEL_WIDTH  - touch_y;
    *y = touch_x;

#endif

    return status;
}

static status_t DEMO_TouchI2C_Init(void)
{
    i2c_master_config_t i2cConfig = {0};

    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(DEMO_TOUCH_I2C, &i2cConfig, DEMO_TOUCH_I2C_CLOCK_FREQ);
    return kStatus_Success;
}

static status_t DEMO_TouchI2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    i2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = subAddress;
    masterXfer.subaddressSize = subAddressSize;
    masterXfer.data           = (uint8_t *)txBuff;
    masterXfer.dataSize       = txBuffSize;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(DEMO_TOUCH_I2C, &masterXfer);
}

static status_t DEMO_TouchI2C_Receive(uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    i2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.subaddress     = subAddress;
    masterXfer.subaddressSize = subAddressSize;
    masterXfer.data           = rxBuff;
    masterXfer.dataSize       = rxBuffSize;
    masterXfer.direction      = kI2C_Read;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(DEMO_TOUCH_I2C, &masterXfer);
}

#if (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
status_t DEMO_InitLcdc(void)
{
    lcdic_config_t config;
    status_t status;

    LCDIC_GetDefaultConfig(&config);
    config.mode        = kLCDIC_4WireSPI;
    config.endian      = kLCDIC_BigEndian;
    config.txThreshold = kLCDIC_TxThreshold3Word;
    config.timerRatio0 = 8;
    config.timerRatio1 = 9;
    config.spiCtrlFlags =
        kLCDIC_SPI_MsbFirst | kLCDIC_SPI_ClkActiveLow | kLCDIC_SPI_ClkPhaseSecondEdge | kLCDIC_SPI_DcCmdLow;
    config.cmdShortTimeout_Timer0 = 0U; /* disable */
    config.cmdLongTimeout_Timer1  = 0U; /* disable */

    status = LCDIC_Init(DEMO_LCDIC, &config);
    if (kStatus_Success != status)
    {
        PRINTF("LCDIC init failed\r\n");
        for (;;)
            ;
    }

    /* The NVIC IRQ is pending during init, clear it. */
    NVIC_ClearPendingIRQ(LCDIC_IRQn);
    NVIC_SetPriority(LCDIC_IRQn, 3);
    NVIC_SetPriority(DMA0_IRQn, 3);

    INPUTMUX_AttachSignal(INPUTMUX, DEMO_LCDC_TX_DMA_CH, kINPUTMUX_LcdTxRegToDmaSingleToDma0);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Dmac0InputTriggerLcdTxRegToDmaSingleEna, true);

    return status;
}

status_t DEMO_CreateDbiIface(void)
{
    status_t status;

    const dbi_lcdic_dma_config_t dbiConfig = {
        .lcdic = DEMO_LCDIC,
        .txDmaHandle = &s_lcdDmaTxHandle,
        .dmaDesc = &s_dmaDesc,
        .dataFormat = kLCDIC_DataFormatHalfWord,
        .teSyncMode = kLCDIC_TeNoSync,
    };

    /* Configure the DMA channel,priority and handle. */
    DMA_CreateHandle(&s_lcdDmaTxHandle, DEMO_DMA, DEMO_LCDC_TX_DMA_CH);

    /* Create DBI interface. */
    status = DBI_LCDIC_DMA_CreateHandle(&s_dcDbiHandle.dbiIface, &dbiConfig, &s_dbiLcdicDmaPrvData);
    if (kStatus_Success != status)
    {
        PRINTF("DBI interface creation failed\r\n");
        for (;;)
            ;
    }

    return kStatus_Success;
}

status_t DEMO_CreateDC(void)
{
    status_t status;

    memset(&s_dcDbiHandle, 0, sizeof(dc_fb_dbi_handle_t));

    /* DC DBI handle. */
    s_dcDbiHandle.width = DEMO_PANEL_WIDTH;
    s_dcDbiHandle.height = DEMO_PANEL_HEIGHT;
    s_dcDbiHandle.pixelFormat = kVIDEO_PixelFormatRGB565;
    s_dcDbiHandle.useTEPin = false;

    /* DC DBI config. */
    static const st7796s_config_t st7796sConfig = {.driverPreset    = kST7796S_DriverPresetLCDPARS035,
                                            .pixelFormat     = kST7796S_PixelFormatRGB565,
                                            .orientationMode = kST7796S_Orientation270,
                                            .teConfig        = kST7796S_TEDisabled,
                                            .invertDisplay   = true,
                                            .flipDisplay     = true,
                                            .bgrFilter       = true};

    status = ST7796S_Init(&lcdHandle, &st7796sConfig, &s_dcDbiHandle.dbiIface);
    if (kStatus_Success != status)
    {
        PRINTF("Panel init failed\r\n");
        for (;;)
            ;
    }

    return status;
}

#else
static void DEMO_InitDbiDcPin(void)
{
    const gpio_pin_config_t pinConfig  = {.pinDirection = kGPIO_DigitalOutput, .outputLogic = 0};
    GPIO_PinInit(GPIO, DEMO_DBI_DC_PORT, DEMO_DBI_DC_PIN, &pinConfig);
}

static void DEMO_DbiPullDcPin(bool high)
{
    if (high)
    {
        GPIO_PinWrite(GPIO, DEMO_DBI_DC_PORT, DEMO_DBI_DC_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(GPIO, DEMO_DBI_DC_PORT, DEMO_DBI_DC_PIN, 0);
    }
}

static status_t DEMO_InitSpi(void)
{
    spi_master_config_t spiConfig;
    status_t status;

    /* Initialize the SPI and DMA. */
    SPI_MasterGetDefaultConfig(&spiConfig);
    spiConfig.polarity                  = kSPI_ClockPolarityActiveLow;
    spiConfig.phase                     = kSPI_ClockPhaseSecondEdge;
    spiConfig.baudRate_Bps              = 16 * 1000 * 1000;
    spiConfig.sselNum                   = kSPI_Ssel0;

    status = SPI_MasterInit(DEMO_SPI, &spiConfig, DEMO_SPI_CLOCK_FREQ);
    if (kStatus_Success != status)
    {
        PRINTF("SPI init failed\r\n");
        for (;;)
            ;
    }

    /* The NVIC IRQ is pending during init, clear it. */
    NVIC_ClearPendingIRQ(DEMO_SPI_IRQn);

    NVIC_SetPriority(DEMO_SPI_IRQn, 3);
    NVIC_SetPriority(DMA0_IRQn, 3);

    return status;
}

status_t DEMO_CreateDbiIface(void)
{
    status_t status;
    const dbi_spi_dma_config_t dbiConfig =
    {
        .spi = DEMO_SPI,
        .txDmaHandle = &s_lcdDmaTxHandle,
        .rxDmaHandle = &s_lcdDmaRxHandle,
        .dcPinFunc = DEMO_DbiPullDcPin,
        .dataWidth = kSPI_Data16Bits,
    };

    /* Initialize the DBI DC pin. */
    DEMO_InitDbiDcPin();

    /* Configure the DMA channel,priority and handle. */
    DMA_CreateHandle(&s_lcdDmaTxHandle, DEMO_DMA, DEMO_SPI_TX_DMA_CH);
    DMA_CreateHandle(&s_lcdDmaRxHandle, DEMO_DMA, DEMO_SPI_RX_DMA_CH);

    /* Create DBI interface. */
    status = DBI_SPI_DMA_CreateHandle(&s_dcDbiHandle.dbiIface,
            &dbiConfig,
                                          &s_dbiSpiDmaPrvData    /* private data */
                                          );
    if (kStatus_Success != status)
    {
        PRINTF("DBI interface creation failed\r\n");
        for (;;)
            ;
    }

    return kStatus_Success;
}

status_t DEMO_CreateDC(void)
{
    status_t status;

    memset(&s_dcDbiHandle, 0, sizeof(dc_fb_dbi_handle_t));

    /* DC DBI handle. */
    s_dcDbiHandle.width = DEMO_PANEL_WIDTH;
    s_dcDbiHandle.height = DEMO_PANEL_HEIGHT;
    s_dcDbiHandle.pixelFormat = kVIDEO_PixelFormatRGB565;
    s_dcDbiHandle.useTEPin = false;

    /* DC DBI config. */
    status = ILI9341_InitDBI(&lcdHandle, NULL, &s_dcDbiHandle.dbiIface);
    if (kStatus_Success != status)
    {
        PRINTF("Panel init failed\r\n");
        for (;;)
            ;
    }

    return status;
}
#endif
