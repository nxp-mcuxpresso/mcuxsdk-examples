/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_cache.h"
#include "fsl_lpi2c.h"
#if defined(BOARD_USE_PCA6416) && BOARD_USE_PCA6416
#include "fsl_pca6416a.h"
#endif
#if defined(BOARD_USE_PCA6408) && BOARD_USE_PCA6408
#include "fsl_pcal6408.h"
#endif
#if defined(BOARD_USE_PCA954X) && BOARD_USE_PCA954X
#include "fsl_pca954x.h"
#endif
#include "fsl_clock.h"
#include "fsl_iomuxc.h"
#if BOARD_ENABLE_ELE
#include "fsl_ele_base_api.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
static void BOARD_InitPCA954X(uint32_t i2cDevId, void *base, uint32_t clkFreq, uint8_t i2cAddr);
static status_t BOARD_PCA954X_SelectChan(uint32_t i2cDevId, uint32_t chan);
static status_t BOARD_PCA954X_DeselectMux(uint32_t i2cDevId, uint32_t chan);

static void BOARD_InitPCA6408(uint32_t i2cDevId, void *base, uint32_t clkFreq, uint8_t i2cAddr);
static status_t BOARD_PCA6408_SetPinToHigh(uint32_t i2cDevId, uint8_t pinIdx);
static status_t BOARD_PCA6408_SetPinToLow(uint32_t i2cDevId, uint8_t pinIdx);
static status_t BOARD_PCA6408_SetPinAsInput(uint32_t i2cDevId, uint8_t pinIdx);
static status_t BOARD_PCA6408_SetPinAsOutput(uint32_t i2cDevId, uint8_t pinIdx);

static void BOARD_InitPCA6416(uint32_t i2cDevId, void *base, uint32_t clkFreq, uint8_t i2cAddr);
static status_t BOARD_PCA6416_SetPinToHigh(uint32_t i2cDevId, uint8_t pinIdx);
static status_t BOARD_PCA6416_SetPinToLow(uint32_t i2cDevId, uint8_t pinIdx);
static status_t BOARD_PCA6416_SetPinAsInput(uint32_t i2cDevId, uint8_t pinIdx);
static status_t BOARD_PCA6416_SetPinAsOutput(uint32_t i2cDevId, uint8_t pinIdx);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static LPI2C_Type * lpi2cBases[] = LPI2C_BASE_PTRS;
static clock_ip_name_t lpi2cClkId[] = LPI2C_CLOCKS;

static pca954x_handle_t g_pca954xHandle[BOARD_PCA9544_NUM + BOARD_PCA9548_NUM];
static pcal6408_handle_t g_pca6408Handle[BOARD_PCA6408_NUM];
static pca6416a_handle_t g_pca6416Handle[BOARD_PCA6416_NUM];

i2c_device_table_t g_i2cDevTbl[] = {
    [BOARD_PCA9544_I2C6_ID & I2C_DEVICE_ARRAY_IDX_MASK] = {
        .handle = &g_pca954xHandle[BOARD_PCA9544_I2C6_HANDLE_IDX],
        .parent_id = BOARD_UNKNOWN_ID,
    },
   [BOARD_PCA9548_I2C3_ID & I2C_DEVICE_ARRAY_IDX_MASK] = {
        .handle = &g_pca954xHandle[BOARD_PCA9548_I2C3_HANDLE_IDX],
        .parent_id = BOARD_UNKNOWN_ID,
    },
   [BOARD_PCA6416_I2C6_S1_ID & I2C_DEVICE_ARRAY_IDX_MASK] = {
        .handle = &g_pca6416Handle[BOARD_PCA6416_I2C6_S1_HANDLE_IDX],
        .parent_id = BOARD_PCA9544_I2C6_ID,
    },
   [BOARD_PCA6408_I2C6_S2_ID & I2C_DEVICE_ARRAY_IDX_MASK] = {
        .handle = &g_pca6408Handle[BOARD_PCA6408_I2C6_S2_HANDLE_IDX],
        .parent_id = BOARD_PCA9544_I2C6_ID,
    },
   [BOARD_PCA6416_I2C6_S3_ID & I2C_DEVICE_ARRAY_IDX_MASK] = {
        .handle = &g_pca6416Handle[BOARD_PCA6416_I2C6_S3_HANDLE_IDX],
        .parent_id = BOARD_PCA9544_I2C6_ID,
    },
   [BOARD_PCA6416_I2C3_S5_20_ID & I2C_DEVICE_ARRAY_IDX_MASK] = {
        .handle = &g_pca6416Handle[BOARD_PCA6416_I2C3_S5_20_HANDLE_IDX],
        .parent_id = BOARD_PCA9548_I2C3_ID,
    },
   [BOARD_PCA6416_I2C3_S5_21_ID & I2C_DEVICE_ARRAY_IDX_MASK] = {
        .handle = &g_pca6416Handle[BOARD_PCA6416_I2C3_S5_21_HANDLE_IDX],
        .parent_id = BOARD_PCA9548_I2C3_ID,
    },
    [BOARD_PCA6416_I2C3_S6_ID & I2C_DEVICE_ARRAY_IDX_MASK] = {
	 .handle = &g_pca6416Handle[BOARD_PCA6416_I2C3_S6_HANDLE_IDX],
	 .parent_id = BOARD_PCA9548_I2C3_ID,
     },
    [BOARD_PCA6408_I2C3_S7_ID & I2C_DEVICE_ARRAY_IDX_MASK] = {
	 .handle = &g_pca6408Handle[BOARD_PCA6408_I2C3_S7_HANDLE_IDX],
	 .parent_id = BOARD_PCA9548_I2C3_ID,
     },

};

static pca954x_t g_pca954xData[] = {
    [PCA9544_TYPE] = {
        .id = PCA9544_ID,
        .init = BOARD_InitPCA954X,
	.select = BOARD_PCA954X_SelectChan,
	.deselect = BOARD_PCA954X_DeselectMux,
    },
    [PCA9548_TYPE] = {
        .id = PCA9548_ID,
        .init = BOARD_InitPCA954X,
	.select = BOARD_PCA954X_SelectChan,
	.deselect = BOARD_PCA954X_DeselectMux,
    },
};

static pca64xx_t g_pca64xxData[] = {
    [PCA6408_TYPE - PCA6408_TYPE] = {
        .init = BOARD_InitPCA6408,
	.set_pin_to_high = BOARD_PCA6408_SetPinToHigh,
	.set_pin_to_low = BOARD_PCA6408_SetPinToLow,
	.set_pin_as_input = BOARD_PCA6408_SetPinAsInput,
	.set_pin_as_output = BOARD_PCA6408_SetPinAsOutput,
	.select = BOARD_MUX_Select,
	.deselect = BOARD_MUX_Deselect,
    },
    [PCA6416_TYPE - PCA6408_TYPE] = {
        .init = BOARD_InitPCA6416,
	.select = BOARD_PCA954X_SelectChan,
	.deselect = BOARD_PCA954X_DeselectMux,
	.set_pin_to_high = BOARD_PCA6416_SetPinToHigh,
	.set_pin_to_low = BOARD_PCA6416_SetPinToLow,
	.set_pin_as_input = BOARD_PCA6416_SetPinAsInput,
	.set_pin_as_output = BOARD_PCA6416_SetPinAsOutput,
	.select = BOARD_MUX_Select,
	.deselect = BOARD_MUX_Deselect,
    },
};


/*******************************************************************************
 * Code
 ******************************************************************************/


/* Get UART Clock Id. */
clock_ip_name_t BOARD_GetUartClkId(uint32_t uartInstIdx)
{
    clock_ip_name_t uart_clk_id[] = LPUART_CLOCKS;

    return uart_clk_id[uartInstIdx];
}

/* Get SWO Clock Id. */
clock_ip_name_t BOARD_GetSwoClkId(uint32_t swoPortIdx)
{
    (void)swoPortIdx;
    clock_ip_name_t swo_clk_id[1] = { kCLOCK_Swotrace }; /* There are 32 swo ports and share one clock root for all of swo ports */

    return swo_clk_id[0];
}

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{

    /* clang-format off */
    clk_t clk = {
        .clkId = kCLOCK_IpInvalid,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    /* clang-format on */

    if (BOARD_DEBUG_CONSOLE_TYPE == BOARD_DEBUG_UART_TYPE)
    {

        clk.clkId = BOARD_GetUartClkId(BOARD_DEBUG_CONSOLE_PORT);
    }
    else if (BOARD_DEBUG_CONSOLE_TYPE == BOARD_DEBUG_SWO_TYPE)
    {
        clk.clkId = BOARD_GetSwoClkId(BOARD_DEBUG_CONSOLE_PORT);
    }
    CLOCK_SetRate(&clk);
    CLOCK_EnableClock(clk.clkId);
    DbgConsole_Init(BOARD_DEBUG_CONSOLE_PORT, BOARD_DEBUG_CONSOLE_BAUDRATE, BOARD_DEBUG_CONSOLE_TYPE,
                    CLOCK_GetRate(clk.clkId));
}

void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz)
{
    lpi2c_master_config_t lpi2cConfig = {0};

    /*
     * lpi2cConfig.debugEnable = false;
     * lpi2cConfig.ignoreAck = false;
     * lpi2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * lpi2cConfig.baudRate_Hz = 100000U;
     * lpi2cConfig.busIdleTimeout_ns = 0;
     * lpi2cConfig.pinLowTimeout_ns = 0;
     * lpi2cConfig.sdaGlitchFilterWidth_ns = 0;
     * lpi2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&lpi2cConfig);
    LPI2C_MasterInit(base, &lpi2cConfig, clkSrc_Hz);
}

status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subAddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize,
                          uint32_t flags)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = flags;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subAddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize,
                             uint32_t flags)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = flags;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = rxBuff;
    xfer.dataSize       = rxBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

void BOARD_I2C_DeviceInit(LPI2C_Type *base, uint32_t clkFreq)
{
    BOARD_LPI2C_Init(base, clkFreq);
}

status_t BOARD_I2C_DeviceSend( void *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 const uint8_t *txBuff,
                                 uint8_t txBuffSize,
                                 uint32_t flags)
{
    return BOARD_LPI2C_Send(base, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize, flags);
}

status_t BOARD_I2C_DeviceReceive( void *base,
                                    uint8_t deviceAddress,
                                    uint32_t subAddress,
                                    uint8_t subAddressSize,
                                    uint8_t *rxBuff,
                                    uint8_t rxBuffSize,
                                    uint32_t flags)
{
    return BOARD_LPI2C_Receive(base, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize,
                               flags);
}

#if defined(BOARD_USE_PCA954X) && BOARD_USE_PCA954X
static void BOARD_InitPCA954X(uint32_t i2cDevId, void *base, uint32_t clkFreq, uint8_t i2cAddr)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    void *handle = g_i2cDevTbl[i2cDevArrayIdx].handle;
    pca954x_t *pca954xData = g_i2cDevTbl[i2cDevArrayIdx].data;
    uint8_t id = pca954xData->id;

    BOARD_I2C_DeviceInit(base, clkFreq);

    pca954x_config_t config = {
        .id              = (enum pca954x_id)id,
        .i2cBase         = base,
        .i2cAddr         = i2cAddr,
        .I2C_SendFunc    = BOARD_I2C_DeviceSend,
        .I2C_ReceiveFunc = BOARD_I2C_DeviceReceive,
    };

    PCA954X_Init(handle, &config);
}

static status_t BOARD_PCA954X_SelectChan(uint32_t i2cDevId, uint32_t chan)
{
    uint8_t i2cDevArrayIdx = i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK;
    void *handle = g_i2cDevTbl[i2cDevArrayIdx].handle;

    return PCA954X_SelectChan(handle, chan);
}

static status_t BOARD_PCA954X_DeselectMux(uint32_t i2cDevId, uint32_t chan)
{
    uint8_t i2cDevArrayIdx = i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK;
    void *handle = g_i2cDevTbl[i2cDevArrayIdx].handle;

    return PCA954X_DeselectMux(handle, chan);
}

#endif /* BOARD_USE_PCA954X. */

void BOARD_InitI2cDevice(uint32_t i2cDevId)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    uint8_t i2cInstIdx = (i2cDevId & I2C_INSTANCE_IDX_MASK) >> I2C_INSTANCE_IDX_SHIFT;
    LPI2C_Type *base = lpi2cBases[i2cInstIdx];
    clock_ip_name_t clkId = lpi2cClkId[i2cInstIdx];
    uint32_t clkFreq = CLOCK_GetRate(clkId);
    uint8_t i2cAddr = (i2cDevId & I2C_DEVICE_ADDR_MASK) >> I2C_DEVICE_ADDR_SHIFT;
    uint8_t deviceType = (i2cDevId & I2C_DEVICE_TYPE_MASK) >> I2C_DEVICE_TYPE_SHIFT;

    if (deviceType < PCA954X_TYPE)
    {
        pca954x_t *pca954xData = NULL;

        g_i2cDevTbl[i2cDevArrayIdx].data = &g_pca954xData[deviceType];
	pca954xData = g_i2cDevTbl[i2cDevArrayIdx].data;
	pca954xData->init(i2cDevId, base, clkFreq, i2cAddr);
    }
    else if (deviceType < PCA64XX_TYPE)
    {
        pca64xx_t *pca64xxData = NULL;

        g_i2cDevTbl[i2cDevArrayIdx].data = &g_pca64xxData[deviceType - PCA6408_TYPE];
	pca64xxData = g_i2cDevTbl[i2cDevArrayIdx].data;
	pca64xxData->init(i2cDevId, base, clkFreq, i2cAddr);
    }
    else
    {
        PRINTF("i2cDevId = %d, it's an unknown device, pls choose a correct device id\r\n", i2cDevId);
	assert(false);
    }
}

status_t BOARD_MUX_Select(uint32_t i2cDevId, uint32_t chan)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    uint8_t deviceType = (i2cDevId & I2C_DEVICE_TYPE_MASK) >> I2C_DEVICE_TYPE_SHIFT;
    pca954x_t *pca954xData = NULL;

    (void)deviceType;
    assert(deviceType < PCA954X_TYPE);

    if (g_i2cDevTbl[i2cDevArrayIdx].data == NULL)
    {
        BOARD_InitI2cDevice(i2cDevId);
    }

    pca954xData = g_i2cDevTbl[i2cDevArrayIdx].data;
    assert(pca954xData != NULL);

    return pca954xData->select(i2cDevId, chan);
}

status_t BOARD_MUX_Deselect(uint32_t i2cDevId, uint32_t chan)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    uint8_t deviceType = (i2cDevId & I2C_DEVICE_TYPE_MASK) >> I2C_DEVICE_TYPE_SHIFT;
    pca954x_t *pca954xData = NULL;

    (void)deviceType;
    assert(deviceType < PCA954X_TYPE);

    if (g_i2cDevTbl[i2cDevArrayIdx].data == NULL)
    {
        BOARD_InitI2cDevice(i2cDevId);
    }

    pca954xData = g_i2cDevTbl[i2cDevArrayIdx].data;
    assert(pca954xData != NULL);

    return pca954xData->deselect(i2cDevId, chan);
}

status_t BOARD_EXPANDER_SetPinToHigh(uint32_t i2cDevId, uint8_t pinIdx)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    uint32_t parentId = g_i2cDevTbl[i2cDevArrayIdx].parent_id;
    uint8_t deviceType = (i2cDevId & I2C_DEVICE_TYPE_MASK) >> I2C_DEVICE_TYPE_SHIFT;
    uint8_t chanIdx = (i2cDevId & I2C_CHAN_IDX_MASK) >> I2C_CHAN_IDX_SHIFT;
    pca64xx_t *pca64xxData = NULL;
    status_t status = kStatus_Fail;

    (void)deviceType;
    assert(deviceType >= PCA954X_TYPE);
    assert(deviceType < PCA64XX_TYPE);

    if (g_i2cDevTbl[i2cDevArrayIdx].data == NULL)
    {
        BOARD_InitI2cDevice(i2cDevId);
    }

    pca64xxData = g_i2cDevTbl[i2cDevArrayIdx].data;
    assert(pca64xxData != NULL);

    if (parentId != BOARD_UNKNOWN_ID && pca64xxData->select != NULL)
    {
        status = pca64xxData->select(parentId, chanIdx);
    }

    if (status == kStatus_Success)
    {
        status = pca64xxData->set_pin_as_output(i2cDevId, pinIdx);
        if (status == kStatus_Success)
        {
            status = pca64xxData->set_pin_to_high(i2cDevId, pinIdx);
        }
    }
    if (status == kStatus_Success && parentId != BOARD_UNKNOWN_ID && pca64xxData->select != NULL)
    {
        status = pca64xxData->deselect(parentId, chanIdx);
    }

    return status;
}

status_t BOARD_EXPANDER_SetPinToLow(uint32_t i2cDevId, uint8_t pinIdx)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    uint32_t parentId = g_i2cDevTbl[i2cDevArrayIdx].parent_id;
    uint8_t deviceType = (i2cDevId & I2C_DEVICE_TYPE_MASK) >> I2C_DEVICE_TYPE_SHIFT;
    uint8_t chanIdx = (i2cDevId & I2C_CHAN_IDX_MASK) >> I2C_CHAN_IDX_SHIFT;
    pca64xx_t *pca64xxData = NULL;
    status_t status = kStatus_Fail;

    (void)deviceType;
    assert(deviceType >= PCA954X_TYPE);
    assert(deviceType < PCA64XX_TYPE);

    if (g_i2cDevTbl[i2cDevArrayIdx].data == NULL)
    {
        BOARD_InitI2cDevice(i2cDevId);
    }

    pca64xxData = g_i2cDevTbl[i2cDevArrayIdx].data;
    assert(pca64xxData != NULL);

    if (parentId != BOARD_UNKNOWN_ID && pca64xxData->select != NULL)
    {
        status = pca64xxData->select(parentId, chanIdx);
    }
    if (status == kStatus_Success)
    {
        status = pca64xxData->set_pin_as_output(i2cDevId, pinIdx);
        if (status == kStatus_Success)
        {
            status = pca64xxData->set_pin_to_low(i2cDevId, pinIdx);
        }
    }

    if (status == kStatus_Success && parentId != BOARD_UNKNOWN_ID && pca64xxData->select != NULL)
    {
        status = pca64xxData->deselect(parentId, chanIdx);
    }
    return status;
}

status_t BOARD_EXPANDER_SetPinAsInput(uint32_t i2cDevId, uint8_t pinIdx)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    uint32_t parentId = g_i2cDevTbl[i2cDevArrayIdx].parent_id;
    uint8_t deviceType = (i2cDevId & I2C_DEVICE_TYPE_MASK) >> I2C_DEVICE_TYPE_SHIFT;
    uint8_t chanIdx = (i2cDevId & I2C_CHAN_IDX_MASK) >> I2C_CHAN_IDX_SHIFT;
    status_t status = kStatus_Fail;
    pca64xx_t *pca64xxData = NULL;

    (void)deviceType;
    assert(deviceType >= PCA954X_TYPE);
    assert(deviceType < PCA64XX_TYPE);

    if (g_i2cDevTbl[i2cDevArrayIdx].data == NULL)
    {
        BOARD_InitI2cDevice(i2cDevId);
    }

    pca64xxData = g_i2cDevTbl[i2cDevArrayIdx].data;

    assert(pca64xxData->set_pin_as_input != NULL);

    if (parentId != BOARD_UNKNOWN_ID && pca64xxData->select != NULL)
    {
        status = pca64xxData->select(parentId, chanIdx);
    }
    if (status == kStatus_Success)
    {
        status = pca64xxData->set_pin_as_input(i2cDevId, pinIdx);
    }

    if (status == kStatus_Success && parentId != BOARD_UNKNOWN_ID && pca64xxData->select != NULL)
    {
        status = pca64xxData->deselect(parentId, chanIdx);
    }
    return status;
}

status_t BOARD_EXPANDER_SetPinAsOutput(uint32_t i2cDevId, uint8_t pinIdx)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    uint32_t parentId = g_i2cDevTbl[i2cDevArrayIdx].parent_id;
    uint8_t deviceType = (i2cDevId & I2C_DEVICE_TYPE_MASK) >> I2C_DEVICE_TYPE_SHIFT;
    uint8_t chanIdx = (i2cDevId & I2C_CHAN_IDX_MASK) >> I2C_CHAN_IDX_SHIFT;
    status_t status = kStatus_Fail;
    pca64xx_t *pca64xxData = NULL;

    (void)deviceType;
    assert(deviceType >= PCA954X_TYPE);
    assert(deviceType < PCA64XX_TYPE);

    if (g_i2cDevTbl[i2cDevArrayIdx].data == NULL)
    {
        BOARD_InitI2cDevice(i2cDevId);
    }

    pca64xxData = g_i2cDevTbl[i2cDevArrayIdx].data;

    assert(pca64xxData->set_pin_as_input != NULL);

    if (parentId != BOARD_UNKNOWN_ID && pca64xxData->select != NULL)
    {
        status = pca64xxData->select(parentId, chanIdx);
    }
    if (status == kStatus_Success)
    {
        status = pca64xxData->set_pin_as_output(i2cDevId, pinIdx);
    }

    if (status == kStatus_Success && parentId != BOARD_UNKNOWN_ID && pca64xxData->select != NULL)
    {
        status = pca64xxData->deselect(parentId, chanIdx);
    }
    return status;
}

#if defined(BOARD_USE_PCA6408) && BOARD_USE_PCA6408
static void BOARD_InitPCA6408(uint32_t i2cDevId, void *base, uint32_t clkFreq, uint8_t i2cAddr)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    void *handle = g_i2cDevTbl[i2cDevArrayIdx].handle;

    BOARD_I2C_DeviceInit(base, clkFreq);

    pcal6408_config_t config = {
        .i2cBase         = base,
        .i2cAddr         = i2cAddr,
        .I2C_SendFunc    = BOARD_I2C_DeviceSend,
        .I2C_ReceiveFunc = BOARD_I2C_DeviceReceive,
    };

    PCAL6408_Init(handle, &config);
}

/*
 * Configures signal to 1
 */
static status_t BOARD_PCA6408_SetPinToHigh(uint32_t i2cDevId, uint8_t pinIdx)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    void *handle = g_i2cDevTbl[i2cDevArrayIdx].handle;

   return PCAL6408_SetPins(handle, (1U << pinIdx));
}

/*
 * Configures signal to 0
 */
static status_t BOARD_PCA6408_SetPinToLow(uint32_t i2cDevId, uint8_t pinIdx)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    void *handle = g_i2cDevTbl[i2cDevArrayIdx].handle;

   return PCAL6408_ClearPins(handle, (1U << pinIdx));
}

/*
 * Configures signal as input
 */
static status_t BOARD_PCA6408_SetPinAsInput(uint32_t i2cDevId, uint8_t pinIdx)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    void *handle = g_i2cDevTbl[i2cDevArrayIdx].handle;

   return PCAL6408_SetDirection(handle, (1U << pinIdx), kPCAL6408_Input);
}

/*
 * Configures signal as output
 */
static status_t BOARD_PCA6408_SetPinAsOutput(uint32_t i2cDevId, uint8_t pinIdx)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    void *handle = g_i2cDevTbl[i2cDevArrayIdx].handle;

   return PCAL6408_SetDirection(handle, (1U << pinIdx), kPCAL6408_Output);
}

#endif /* BOARD_USE_PCA6408. */

#if defined(BOARD_USE_PCA6416) && BOARD_USE_PCA6416
static void BOARD_InitPCA6416(uint32_t i2cDevId, void *base, uint32_t clkFreq, uint8_t i2cAddr)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    void *handle = g_i2cDevTbl[i2cDevArrayIdx].handle;

    BOARD_I2C_DeviceInit(base, clkFreq);

    pca6416a_config_t config = {
        .i2cBase         = base,
        .i2cAddr         = i2cAddr,
        .I2C_SendFunc    = BOARD_I2C_DeviceSend,
        .I2C_ReceiveFunc = BOARD_I2C_DeviceReceive,
    };

    PCA6416A_Init(handle, &config);
}

/*
 * Configures signal to 1
 */
static status_t BOARD_PCA6416_SetPinToHigh(uint32_t i2cDevId, uint8_t pinIdx)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    void *handle = g_i2cDevTbl[i2cDevArrayIdx].handle;

   return PCA6416A_SetPins(handle, (1U << pinIdx));
}

/*
 * Configures signal to 0
 */
static status_t BOARD_PCA6416_SetPinToLow(uint32_t i2cDevId, uint8_t pinIdx)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    void *handle = g_i2cDevTbl[i2cDevArrayIdx].handle;

   return PCA6416A_ClearPins(handle, (1U << pinIdx));
}

/*
 * Configures signal as input
 */
static status_t BOARD_PCA6416_SetPinAsInput(uint32_t i2cDevId, uint8_t pinIdx)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    void *handle = g_i2cDevTbl[i2cDevArrayIdx].handle;

   return PCA6416A_SetDirection(handle, (1U << pinIdx), kPCA6416A_Input);
}

/*
 * Configures signal as output
 */
static status_t BOARD_PCA6416_SetPinAsOutput(uint32_t i2cDevId, uint8_t pinIdx)
{
    uint8_t i2cDevArrayIdx = (i2cDevId & I2C_DEVICE_ARRAY_IDX_MASK) >> I2C_DEVICE_ARRAY_IDX_SHIFT;
    void *handle = g_i2cDevTbl[i2cDevArrayIdx].handle;

   return PCA6416A_SetDirection(handle, (1U << pinIdx), kPCA6416A_Output);
}
#endif /* BOARD_USE_PCA6416. */

/*
 * Change macro definitions as follows when choose cache policy as non-cacheable:
 * #define CACHE_MODE_NONCACHEABLE (1U)
 * #define CACHE_MODE_WRITE_THROUGH (0U)
 * #define CACHE_MODE_WRITE_BACK (0U)
 *
 *
 * Change macro definitions as follows when choose cache policy as Write-Through:
 * #define CACHE_MODE_NONCACHEABLE (0U)
 * #define CACHE_MODE_WRITE_THROUGH (1U)
 * #define CACHE_MODE_WRITE_BACK (0U)
 *
 *
 * Change macro definitions as follows when choose cache policy as Write-Back:
 * #define CACHE_MODE_NONCACHEABLE (0U)
 * #define CACHE_MODE_WRITE_THROUGH (0U)
 * #define CACHE_MODE_WRITE_BACK (1U)
 */
#ifndef CACHE_MODE_NONCACHEABLE
#define CACHE_MODE_NONCACHEABLE  (1U)
#endif
#ifndef CACHE_MODE_WRITE_THROUGH
#define CACHE_MODE_WRITE_THROUGH (0U)
#endif
#ifndef CACHE_MODE_WRITE_BACK
#define CACHE_MODE_WRITE_BACK    (0U)
#endif

#if __CORTEX_M == 7
void BOARD_ConfigMPU(void)
{
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RW_m_ncache$$Base[];
    /* RW_m_ncache_unused is a auxiliary region which is used to get the whole size of noncache section */
    extern uint32_t Image$$RW_m_ncache_unused$$Base[];
    extern uint32_t Image$$RW_m_ncache_unused$$ZI$$Limit[];
    uint32_t nonCacheStart = (uint32_t)Image$$RW_m_ncache$$Base;
    uint32_t size          = ((uint32_t)Image$$RW_m_ncache_unused$$Base == nonCacheStart) ?
                        0 :
                        ((uint32_t)Image$$RW_m_ncache_unused$$ZI$$Limit - nonCacheStart);
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __NCACHE_REGION_START[];
    extern uint32_t __NCACHE_REGION_SIZE[];
    uint32_t nonCacheStart = (uint32_t)__NCACHE_REGION_START;
    uint32_t size          = (uint32_t)__NCACHE_REGION_SIZE;
#endif
    uint32_t bitPos = 0U;
    uint32_t regionIdx = 0U;
    uint32_t lastStartAddr = 0U;

#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
    }
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
    }
#endif

    /* Disable MPU */
    ARM_MPU_Disable();

    /* MPU configure:
     * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable,
     * SubRegionDisable, Size)
     * API in mpu_armv7.h.
     * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches
     * disabled.
     * param AccessPermission  Data access permissions, allows you to configure read/write access for User and
     * Privileged mode.
     *      Use MACROS defined in mpu_armv7.h:
     * ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
     * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
     *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribute    Shareability        Cache
     *     0             x           0           0             Strongly Ordered    shareable
     *     0             x           0           1              Device             shareable
     *     0             0           1           0              Normal             not shareable   Outer and inner write
     * through no write allocate
     *     0             0           1           1              Normal             not shareable   Outer and inner write
     * back no write allocate
     *     0             1           1           0              Normal             shareable       Outer and inner write
     * through no write allocate
     *     0             1           1           1              Normal             shareable       Outer and inner write
     * back no write allocate
     *     1             0           0           0              Normal             not shareable   outer and inner
     * noncache
     *     1             1           0           0              Normal             shareable       outer and inner
     * noncache
     *     1             0           1           1              Normal             not shareable   outer and inner write
     * back write/read allocate
     *     1             1           1           1              Normal             shareable       outer and inner write
     * back write/read allocate
     *     2             x           0           0              Device              not shareable
     *  Above are normal use settings, if your want to see more details or want to config different inner/outer cache
     * policy.
     *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
     * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
     * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in
     * mpu_armv7.h.
     */

    /*
     * Add default region to deny access to whole address space to workaround speculative prefetch.
     * Refer to Arm errata 1013783-B for more details.
     *
     */
    /* Region 0 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(1, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_4GB);
    regionIdx++;

    /* Region 1 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);
    regionIdx++;

    /* Region 2 setting: Memory with Normal type, not shareable, outer/inner write back for M7 ITCM(Include alias area) */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512KB);
    regionIdx++;

    /* Region 3 setting: Memory with Normal type, shareable, outer/inner write back for M7 DTCM(Include alias area) */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 1, 1, 1, 0, ARM_MPU_REGION_SIZE_512KB);
    regionIdx++;

#if defined(CACHE_MODE_WRITE_THROUGH) && CACHE_MODE_WRITE_THROUGH
    /* Region 4 setting: Memory with Normal type, not shareable, write through for M33 System TCM(Include alias area) */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 0, 0, ARM_MPU_REGION_SIZE_512KB);
    regionIdx++;
#elif defined(CACHE_MODE_WRITE_THROUGH) && CACHE_MODE_WRITE_BACK
    /* Region 4 setting: Memory with Normal type, not shareable, outer/inner write back for M33 System TCM(Include alias area) */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512KB);
    regionIdx++;
#elif defined(CACHE_MODE_NONCACHEABLE) && CACHE_MODE_NONCACHEABLE
    /* Region 4 setting: Memory with Normal type, not shareable, noncacheable for M33 System TCM(Include alias area) */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512KB);
    regionIdx++;
#endif

    /* Region 5 setting: Memory with Normal type, not shareable, outer/inner write back for XSPI1. */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x28000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128MB);
    regionIdx++;

    /* Region 6 setting: Memory with Device type, not shareable, non-cacheable for DEBUG_DAP */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x40000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_32MB);
    regionIdx++;

    /* Region 7 setting: Memory with Device type, not shareable, non-cacheable for AIPS-2, AIPS-3 */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x42000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_16MB);
    regionIdx++;

    /* Region 8 setting: Memory with Device type, not shareable, non-cacheable for AIPS-4 */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x49000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_8MB);
    regionIdx++;

    /* Region 9 setting: Memory with Normal type, not shareable, write back write/read allocate. */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);
    regionIdx++;

    /* Region 10 setting: Memory with Normal type, shareable, noncacheable */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x82000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 1, 0, 0, 0, ARM_MPU_REGION_SIZE_64MB);
    regionIdx++;

#if defined(CACHE_MODE_WRITE_THROUGH) && CACHE_MODE_WRITE_THROUGH
    /* Region 11 setting: Memory with Normal type, not shareable, write trough */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x86000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 0, 0, ARM_MPU_REGION_SIZE_64MB);
    regionIdx++;
#elif defined(CACHE_MODE_WRITE_BACK) && CACHE_MODE_WRITE_BACK
    /* Region 11 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x86000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_64MB);
    regionIdx++;
#elif defined(CACHE_MODE_NONCACHEABLE) && CACHE_MODE_NONCACHEABLE
    /* Region 11 setting: Memory with Normal type, not shareable, noncacheable */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x86000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_64MB);
    regionIdx++;
#endif

    /*
     * Decompose to powers of two
     * 10 = 0b1010 = 2^1 + 2^3
     *
     * Split size 0xa000 to two regions:
     * 0xa000 = 0b1010 0000 0000 0000 = 2^13 + 2^15
     * region n size: 2^13
     * region n+1 size: 2^15
     * Note: The MPU of armv7 region size should be 2^N, 5<=N<=32, region base should be multiples of size.
     */
    lastStartAddr = nonCacheStart;
    while (size > 0)
    {
       if (size & 1)
       {
           assert(bitPos >= 5);
           regionIdx++;
           /* Region n setting: Memory with Normal type, shareable, non-cacheable */
           MPU->RBAR = ARM_MPU_RBAR(regionIdx, lastStartAddr);
           MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 1, 0, 0, 0, bitPos - 1);
           lastStartAddr += (1 << bitPos);
       }
       size >>= 1;
       ++bitPos;
    }

    /* Region n setting: Memory with Normal type, shareable, non-cacheable for pdm(micfil) buffer(0x81000000-0x810FFFFF) between mcu(FreeRTOS) and mpu(Linux) */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0x81000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 1, 0, 0, 0, ARM_MPU_REGION_SIZE_1MB);
    regionIdx++;

    /* Region n setting: Memory with Normal type, shareable, non-cacheable for audio buffer(0xC0000000-0xCFFFFFFF) between mcu(FreeRTOS) and mpu(Linux) */
    MPU->RBAR = ARM_MPU_RBAR(regionIdx, 0xC0000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 1, 0, 0, 0, ARM_MPU_REGION_SIZE_256MB);
    regionIdx++;

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);

    /* Enable I cache and D cache */
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    SCB_EnableDCache();
#endif
#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    SCB_EnableICache();
#endif
}
#elif __CORTEX_M == 33
void BOARD_ConfigMPU(void)
{
    uint8_t attr;

    /* Disable code cache(ICache) and system cache(DCache) */
    XCACHE_DisableCache(SOC_CORE_ICACHE_CTRL);
    XCACHE_DisableCache(SOC_CORE_DCACHE_CTRL);

    /* NOTE: All TCRAM is non-cacheable regardless of MPU setting. */

    /*
     * default cache policy(default memory access behavior) after enable mpu on cortex-m33(according to RM of
     * cortex-m33): 0x00000000-0x1FFFFFFF Normal memory, Non-shareable, Write-Through, not Write Allocate
     * 0x20000000-0x3FFFFFFF Normal memory, Non-shareable, Write-Back, Write Allocate
     * 0x40000000-0x5FFFFFFF Device, Shareable
     * 0x60000000-0x7FFFFFFF Normal memory, Non-shareable, Write-Back, Write Allocate
     * 0x80000000-0x9FFFFFFF Normal memory, Non-shareable, Write-Through, not Write Allocate
     * 0xA0000000-0xDFFFFFFF Device, Shareable
     * 0xE0000000-0xE003FFFF Device, Shareable
     * 0xE0040000-0xE0043FFF Device, Shareable
     * 0xE0044000-0xE00EFFFF Device, Shareable
     * 0xF0000000-0xFFFFFFFF Device, Shareable
     */
    /* Disable MPU */
    ARM_MPU_Disable();

    /* Attr0: Device-nGnRnE */
    ARM_MPU_SetMemAttr(0U, ARM_MPU_ATTR(ARM_MPU_ATTR_DEVICE, ARM_MPU_ATTR_DEVICE_nGnRnE));

    /* Attr1: Normal memory, Outer non-cacheable, Inner non-cacheable */
    ARM_MPU_SetMemAttr(1U, ARM_MPU_ATTR(ARM_MPU_ATTR_NON_CACHEABLE, ARM_MPU_ATTR_NON_CACHEABLE));

    /* Attr2: Normal memory, Inner write-through transient, read allocate. Inner write-through transient, read allocate
     */
    attr = ARM_MPU_ATTR_MEMORY_(0U, 0U, 1U, 0U);
    ARM_MPU_SetMemAttr(2U, ARM_MPU_ATTR(attr, attr));

    /* Attr3: Normal memory, Outer write-back transient, read/write allocate. Inner write-back transient, read/write
     * allocate */
    attr = ARM_MPU_ATTR_MEMORY_(0U, 1U, 1U, 1U);
    ARM_MPU_SetMemAttr(3U, ARM_MPU_ATTR(attr, attr));

        /* Attr0: device. */
    ARM_MPU_SetMemAttr(4U, ARM_MPU_ATTR(ARM_MPU_ATTR_DEVICE, ARM_MPU_ATTR_DEVICE_nGnRnE));

#if CACHE_MODE_NONCACHEABLE
    /* NOTE: DDR is used as shared memory for A/M core communication, set it to non-cacheable. */
    /* Region 0: [0x80000000, 0xDFFFFFFF](DRAM), outer shareable, read/write, any privileged, executable. Attr 1
     * (non-cacheable). */
    ARM_MPU_SetRegion(0U, ARM_MPU_RBAR(0x80000000, ARM_MPU_SH_OUTER, 0U, 1U, 0U), ARM_MPU_RLAR(0xDFFFFFFF, 1U));
#elif CACHE_MODE_WRITE_THROUGH
    /* Region 0: [0x80000000, 0xDFFFFFFF](DRAM), outer shareable, read/write, any privileged, executable. Attr 2
     * (Normal memory, Inner write-through transient, read allocate. Inner write-through transient, read allocate). */
    ARM_MPU_SetRegion(0U, ARM_MPU_RBAR(0x80000000, ARM_MPU_SH_OUTER, 0U, 1U, 0U), ARM_MPU_RLAR(0xDFFFFFFF, 2U));
#elif CACHE_MODE_WRITE_BACK
    /* Region 0: [0x80000000, 0xDFFFFFFF](DRAM), outer shareable, read/write, any privileged, executable. Attr 3
     * (Normal memory, Outer write-back transient, read/write allocate. Inner write-back transient, read/write
     * allocate). */
    ARM_MPU_SetRegion(0U, ARM_MPU_RBAR(0x80000000, ARM_MPU_SH_OUTER, 0U, 1U, 0U), ARM_MPU_RLAR(0xDFFFFFFF, 3U));
#endif
    ARM_MPU_SetRegion(1U, ARM_MPU_RBAR(0x28000000, ARM_MPU_SH_OUTER, 0U, 1U, 0U), ARM_MPU_RLAR(0x3FFFFFFF, 4U));

    /*
     * Region 2: [0x20800000, 0x208A0FFF](NETC OCRAM), outer shareable, read/write, any privileged, executable. Attr 1
     * This region is used as RPMSG share memory between Cortex-M33 Core1(M33S) and AP(Non Secure Linux).
     * (non-cacheable).
     */
    ARM_MPU_SetRegion(2U, ARM_MPU_RBAR(0x20800000, ARM_MPU_SH_OUTER, 0U, 1U, 0U), ARM_MPU_RLAR(0x208A0FFF, 1U));

    /*
     * Region 3: [0x20039000, 0x2003BFFF](System TCM of Cortex-m33 Core1), outer shareable, read/write, any privileged, executable. Attr 1
     * This range 0x20039000~0x2003A7FF is used as RPMSG share memory between Cortex-M33 Core1(M33S, as remote) and Cortex-m7 Core0(as master).
     * This range 0x2003A800~0x2003BFFF is used as RPMSG share memory between Cortex-M33 Core1(M33S, as remote) and Cortex-m7 Core1(as master).
     * (non-cacheable).
     */
    ARM_MPU_SetRegion(3U, ARM_MPU_RBAR(0x20039000, ARM_MPU_SH_OUTER, 0U, 1U, 0U), ARM_MPU_RLAR(0x2003BFFF, 1U));

    /*
     * Region 4: [0x2033A800, 0x2033BFFF](DTCM of Cortex-m7 Core1), outer shareable, read/write, any privileged, executable. Attr 1
     * This range 0x2033A800~0x2033BFFF is used as RPMSG share memory between Cortex-M33 Core1(M33S, as master) and Cortex-m7 Core1(as remote).
     * (non-cacheable).
     */
    ARM_MPU_SetRegion(4U, ARM_MPU_RBAR(0x2033A800, ARM_MPU_SH_OUTER, 0U, 1U, 0U), ARM_MPU_RLAR(0x2033BFFF, 1U));

    /*
     * Region 5: [0x2043A800, 0x2043BFFF](DTCM of Cortex-m7 Core0), outer shareable, read/write, any privileged, executable. Attr 1
     * This range 0x2043A800~0x2043BFFF is used as RPMSG share memory between Cortex-M33 Core1(M33S, as master) and Cortex-m7 Core0(as remote).
     * (non-cacheable).
     */
    ARM_MPU_SetRegion(5U, ARM_MPU_RBAR(0x2043A800, ARM_MPU_SH_OUTER, 0U, 1U, 0U), ARM_MPU_RLAR(0x2043BFFF, 1U));

    /* Enable MPU(The MPU is enabled during HardFault and NMI handlers; use default memory map when access the memory within region) */
    ARM_MPU_Enable(MPU_CTRL_HFNMIENA_Msk | MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable ICache and DCache */
    XCACHE_EnableCache(SOC_CORE_ICACHE_CTRL);
    XCACHE_EnableCache(SOC_CORE_DCACHE_CTRL);
    /* flush pipeline */
    __DSB();
    __ISB();
}
#endif

void BOARD_InitLpuart1Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_UART1_RXD__LPUART1_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_UART1_TXD__LPUART1_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_UART1_RXD__LPUART1_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_UART1_TXD__LPUART1_TX,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_InitLpuart2Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_UART2_RXD__LPUART2_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_UART2_TXD__LPUART2_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_UART2_RXD__LPUART2_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_UART2_TXD__LPUART2_TX,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_InitLpuart3Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO31__LPUART3_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO30__LPUART3_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO31__LPUART3_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO30__LPUART3_TX,
                        IOMUXC_PAD_DSE(7U));
}

#if BOARD_IMX943_TYPE == BOARD_IMX943_EMULATOR
void BOARD_InitLpuart4Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO40__LPUART4_TX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO41__LPUART4_RX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO41__LPUART4_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO40__LPUART4_TX,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_InitLpuart5Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO01__LPUART5_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO00__LPUART5_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO01__LPUART5_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO00__LPUART5_TX,
                        IOMUXC_PAD_DSE(7U));
}
#else
void BOARD_InitLpuart4Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO14__LPUART4_TX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO15__LPUART4_RX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO15__LPUART4_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO14__LPUART4_TX,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_InitLpuart5Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_DAP_TDI__LPUART5_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_DAP_TDO_TRACESWO__LPUART5_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_DAP_TDI__LPUART5_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_DAP_TDO_TRACESWO__LPUART5_TX,
                        IOMUXC_PAD_DSE(15U));
}
#endif

void BOARD_InitLpuart6Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO21__LPUART6_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO20__LPUART6_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO21__LPUART6_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO20__LPUART6_TX,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_InitLpuart7Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO32__LPUART7_TX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO33__LPUART7_RX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO33__LPUART7_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO32__LPUART7_TX,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_InitI2C6Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO28__LPI2C6_SCL, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO29__LPI2C6_SDA, 1U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO28__LPI2C6_SCL,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO29__LPI2C6_SDA,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
}

void BOARD_InitLpuart8Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
#if BOARD_LPUART8_USE_JTAG_PADS
    IOMUXC_SetPinMux(IOMUXC_PAD_DAP_TCLK_SWCLK__LPUART8_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_DAP_TMS_SWDIO__LPUART8_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_DAP_TCLK_SWCLK__LPUART8_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_DAP_TMS_SWDIO__LPUART8_TX,
                        IOMUXC_PAD_DSE(15U));
#elif BOARD_LPUART8_USE_GPIO_IO12_GPIO_IO13_PADS
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO13__LPUART8_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO12__LPUART8_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO13__LPUART8_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO12__LPUART8_TX,
                        IOMUXC_PAD_DSE(7U));

    BOARD_InitI2C6Pins();
    /* Route LPSPI8 of SoC to arduino interface */
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL1);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL1);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL3);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, SPI8_SEL3);
#else
#error "Pls find PADS for function LPUART8"
#endif
}

void BOARD_InitLpuart9Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO45__LPUART9_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO44__LPUART9_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO45__LPUART9_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO44__LPUART9_TX,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_InitLpuart10Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO49__LPUART10_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO48__LPUART10_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO49__LPUART10_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO48__LPUART10_TX,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_InitLpuart11Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO25__LPUART11_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO24__LPUART11_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO25__LPUART11_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO24__LPUART11_TX,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_InitLpuart12Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO27__LPUART12_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO26__LPUART12_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO27__LPUART12_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO26__LPUART12_TX,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_DeinitLpuart1Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_UART1_RXD__GPIO1_IO4, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_UART1_TXD__GPIO1_IO5, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_UART1_RXD__GPIO1_IO4,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_UART1_TXD__GPIO1_IO5,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_DeinitLpuart2Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_UART2_RXD__GPIO1_IO6, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_UART2_TXD__GPIO1_IO7, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_UART2_RXD__GPIO1_IO6,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_UART2_TXD__GPIO1_IO7,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_DeinitLpuart3Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO31__GPIO2_IO31, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO30__GPIO2_IO30, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO31__GPIO2_IO31,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO30__GPIO2_IO30,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_DeinitLpuart4Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO14__GPIO2_IO14, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO15__GPIO2_IO15, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO15__GPIO2_IO15,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO14__GPIO2_IO14,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_DeinitLpuart5Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_DAP_TDI__GPIO4_IO4, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_DAP_TDO_TRACESWO__GPIO4_IO7, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_DAP_TDI__GPIO4_IO4,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_DAP_TDO_TRACESWO__GPIO4_IO7,
                        IOMUXC_PAD_DSE(15U));
}

void BOARD_DeinitLpuart6Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO21__GPIO2_IO21, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO20__GPIO2_IO20, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO21__GPIO2_IO21,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO20__GPIO2_IO20,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_DeinitLpuart7Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO32__GPIO3_IO0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO33__GPIO3_IO1, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO33__GPIO3_IO1,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO32__GPIO3_IO0,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_DeinitLpuart8Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_DAP_TCLK_SWCLK__GPIO4_IO6, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_DAP_TMS_SWDIO__GPIO4_IO5, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_DAP_TCLK_SWCLK__GPIO4_IO6,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_DAP_TMS_SWDIO__GPIO4_IO5,
                        IOMUXC_PAD_DSE(15U));
}

void BOARD_DeinitLpuart9Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO45__GPIO3_IO13, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO44__GPIO3_IO12, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO45__GPIO3_IO13,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO44__GPIO3_IO12,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_DeinitLpuart10Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO49__GPIO3_IO17, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO48__GPIO3_IO16, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO49__GPIO3_IO17,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO48__GPIO3_IO16,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_DeinitLpuart11Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO17__GPIO2_IO17, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO16__GPIO2_IO16, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO17__GPIO2_IO17,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO16__GPIO2_IO16,
                        IOMUXC_PAD_DSE(7U));
}

void BOARD_DeinitLpuart12Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO27__GPIO2_IO27, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO26__GPIO2_IO26, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO27__GPIO2_IO27,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO26__GPIO2_IO26,
                        IOMUXC_PAD_DSE(7U));
}

uint32_t BOARD_GetUartFreq(uint32_t uartInstIdx)
{
    clock_ip_name_t clkId = BOARD_GetUartClkId(uartInstIdx);

    return CLOCK_GetRate(clkId);
}

uint32_t BOARD_Lpuart1GetFreq(void)
{
    return BOARD_GetUartFreq(1);
}

uint32_t BOARD_Lpuart2GetFreq(void)
{
    return BOARD_GetUartFreq(2);
}

uint32_t BOARD_Lpuart3GetFreq(void)
{
    return BOARD_GetUartFreq(3);
}

uint32_t BOARD_Lpuart4GetFreq(void)
{
    return BOARD_GetUartFreq(4);
}

uint32_t BOARD_Lpuart5GetFreq(void)
{
    return BOARD_GetUartFreq(5);
}

uint32_t BOARD_Lpuart6GetFreq(void)
{
    return BOARD_GetUartFreq(6);
}

uint32_t BOARD_Lpuart7GetFreq(void)
{
    return BOARD_GetUartFreq(7);
}

uint32_t BOARD_Lpuart8GetFreq(void)
{
    return BOARD_GetUartFreq(8);
}

uint32_t BOARD_Lpuart9GetFreq(void)
{
    return BOARD_GetUartFreq(9);
}

uint32_t BOARD_Lpuart10GetFreq(void)
{
    return BOARD_GetUartFreq(10);
}

uint32_t BOARD_Lpuart11GetFreq(void)
{
    return BOARD_GetUartFreq(11);
}

uint32_t BOARD_Lpuart12GetFreq(void)
{
    return BOARD_GetUartFreq(12);
}

void BOARD_InitLpuartPins(uint32_t instIdx)
{
    switch (instIdx)
    {
        case 1:
            BOARD_InitLpuart1Pins();
            break;
        case 2:
            BOARD_InitLpuart2Pins();
            break;
        case 3:
            BOARD_InitLpuart3Pins();
            break;
        case 4:
            BOARD_InitLpuart4Pins();
            break;
        case 5:
            BOARD_InitLpuart5Pins();
            break;
        case 6:
            BOARD_InitLpuart6Pins();
            break;
        case 7:
            BOARD_InitLpuart7Pins();
            break;
        case 8:
            BOARD_InitLpuart8Pins();
            break;
        case 9:
            BOARD_InitLpuart9Pins();
            break;
        case 10:
            BOARD_InitLpuart10Pins();
            break;
        case 11:
            BOARD_InitLpuart11Pins();
            break;
        case 12:
            BOARD_InitLpuart12Pins();
            break;
        default:
            assert(false);
            break;
    }
}

void BOARD_InitDebugConsolePins(void)
{
    //hal_pinctrl_t hal_pinctrl = {0};

    //hal_pinctrl.set_gpr = true;
    //hal_pinctrl.gpr_instance = 0;
    /*
     * +-------------------------------+
     * | domain id |  master           |
     * +-------------------------------+
     * | 0         |  Edgelock Enclave |
     * +-------------------------------+
     * | 1         |  MTR              |
     * +-------------------------------+
     * | 2         |  Cortex-M33 CORE0 |
     * +-------------------------------+
     * | 3         |  Cortex-A55       |
     * +-------------------------------+
     * | 4         |  Cortex-M7 CORE0  |
     * +-------------------------------+
     * | 5         |  uSDHC1           |
     * +-------------------------------+
     * | 6         |  uSDHC2           |
     * +-------------------------------+
     * | 7         |  DMA              |
     * +-------------------------------+
     * | 8         |  CoreSight        |
     * +-------------------------------+
     * | 9         |  DAP(AHB_AP_SYS)  |
     * +-------------------------------+
     * | 10        |  ISP              |
     * +-------------------------------+
     * | 11        |  USB1             |
     * +-------------------------------+
     * | 12        |  V2X-FH           |
     * +-------------------------------+
     * | 13        |  Cortex-M33 CORE1 |
     * +-------------------------------+
     * | 14        |  Cortex-M7 CORE1  |
     * +-------------------------------+
     *
     * default IOMUXC_GPR's value is 0x1320
     *
     * core0_did - iomuxc_gpr0[3:0]  - 0x0 - Edgelock Enclave
     * core1_did - iomuxc_gpr0[7:4]  - 0x2 - cortex-m33 core0
     * core2_did - iomuxc_gpr0[11:8] - 0x3 - cortex-a55
     * core3_did - iomuxc_gpr0[15:12]- 0x1 - MTR
     */
    //hal_pinctrl.gpr_val = BOARD_IOMUXC_GPR_VAL;
    //HAL_PinctrlSetMiscCfg(&hal_pinctrl);

#if BOARD_DEBUG_UART_TYPE == kSerialPort_Uart
    BOARD_InitLpuartPins(BOARD_DEBUG_UART_INSTANCE);
#else
#warn "Not initialize any pins for debug console with non uart port"
#endif
}

#if BOARD_ENABLE_ELE
void BOARD_ShowEleFwInfo(void)
{
    status_t sts;
    uint32_t ele_fw_ver;
    /* Get ELE FW Version */
    do
    {
        sts = ELE_BaseAPI_GetFwVersion(SOC_ELE_MU_INST_BASE, &ele_fw_ver);
    } while (sts != kStatus_Success);
    PRINTF("Ele Firmware Version: 0x%x\r\n", ele_fw_ver);
}
#endif


/*******************************************************************************
 * Motor Control MUX Selection
 ******************************************************************************/

void BOARD_SelectM1PWM(void)
{
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH2_SEL);
}

void BOARD_SelectM1Encoder(void)
{
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, ETH2_SEL);
    SDK_DelayAtLeastUs(2000U, SystemCoreClock);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH2_SEL);
    SDK_DelayAtLeastUs(10000U, SystemCoreClock);
}

void BOARD_SelectM1SINC(void)
{
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, SD1_SEL);
}

void BOARD_SelectM1Faults(void)
{
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, CAN5_SEL);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, CAN5_SEL);
    SDK_DelayAtLeastUs(100U, SystemCoreClock);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, SAI3_SEL);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, SAI3_SEL);
    SDK_DelayAtLeastUs(100U, SystemCoreClock);
}

void BOARD_SelectM1ENDAT(void)
{
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH2_SEL);
    SDK_DelayAtLeastUs(100U, SystemCoreClock);
}

void BOARD_SelectM2PWM(void)
{
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
    SDK_DelayAtLeastUs(2000U, SystemCoreClock);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
    SDK_DelayAtLeastUs(10000U, SystemCoreClock);
}

void BOARD_SelectM2Encoder(void)
{
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
    SDK_DelayAtLeastUs(2000U, SystemCoreClock);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
    SDK_DelayAtLeastUs(10000U, SystemCoreClock);
}

void BOARD_SelectM2SINC(void)
{
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, SD2_SEL);
    SDK_DelayAtLeastUs(2000U, SystemCoreClock);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, SD2_SEL);
    SDK_DelayAtLeastUs(10000U, SystemCoreClock);
}

void BOARD_SelectM2ENDAT(void)
{
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
    SDK_DelayAtLeastUs(100U, SystemCoreClock);
}

void BOARD_SelectFTUART(void)
{
    BOARD_InitI2C6Pins();
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, UART_M_FT_SEL);
}
