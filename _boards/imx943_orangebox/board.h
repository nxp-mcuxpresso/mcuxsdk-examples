/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_clock.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "IMX943-ORANGEBOX"

//#define BOARD_ENABLE_ELE 0
#define BOARD_ENABLE_ELE 1

/* For cmsis demos */
#define BOARD_Lpuart1GetFreq LPUART1_GetFreq
#define BOARD_Lpuart2GetFreq LPUART2_GetFreq
#define BOARD_Lpuart3GetFreq LPUART3_GetFreq
#define BOARD_Lpuart4GetFreq LPUART4_GetFreq
#define BOARD_Lpuart5GetFreq LPUART5_GetFreq
#define BOARD_Lpuart6GetFreq LPUART6_GetFreq
#define BOARD_Lpuart7GetFreq LPUART7_GetFreq
#define BOARD_Lpuart8GetFreq LPUART8_GetFreq
#define BOARD_Lpuart9GetFreq LPUART9_GetFreq
#define BOARD_Lpuart10GetFreq LPUART10_GetFreq
#define BOARD_Lpuart11GetFreq LPUART11_GetFreq
#define BOARD_Lpuart12GetFreq LPUART12_GetFreq

/*
 * The UART instance to be used for debugging.
 * On iMX943 ORANGEBOX
 * +------------------------------------------------------------------------------------------------------+
 * |   ca55(core0 ~ core3)   | cm33 core0  | cm33 core1            | cm7 core0         | cm7 core1        |
 * --------------------------------------------------------------------------------------------------------
 * |            1            |      2      |  3                    |  11               | 12               |
 * +------------------------------------------------------------------------------------------------------+
 */
#define BOARD_DEBUG_UART_TYPE       kSerialPort_Uart
#define BOARD_DEBUG_SWO_TYPE        kSerialPort_Swo

#define BOARD_DEBUG_UART_BAUDRATE   115200U
#define BOARD_DEBUG_SWO_BAUDRATE   4000000U

#if defined(CPU_MIMX94398AVKM_cm33_core0)

#define VDEV0_VRING_BASE (0x88000000U)
#define VDEV1_VRING_BASE (0x88010000U)

#define BOARD_DEBUG_CONSOLE_TYPE BOARD_DEBUG_UART_TYPE
#define BOARD_DEBUG_UART_INSTANCE 2
#define BOARD_DEBUG_CONSOLE_PORT BOARD_DEBUG_UART_INSTANCE
#define BOARD_DEBUG_CONSOLE_BAUDRATE BOARD_DEBUG_UART_BAUDRATE
#define SOC_CORE_ICACHE_CTRL M33_CACHE_CTRLPC
#define SOC_CORE_DCACHE_CTRL M33_CACHE_CTRLPS
#define SOC_ELE_MU_INST_BASE (ELE_MU0_MUA)
#elif defined(CPU_MIMX94398AVKM_cm33_core1)
/*
 * 0x20800000-0x2089FFFF Shared between AP(ns) and Cortex-M33 Core1(M33S) ← RPMSG area
 *  640 KiB(4 x 32 KB vring + 2 x 512 x 512 bytes buffer) M33S OCRAM(from Cortex-M33 Core1 View)
 * 0x82000000-0x83FFFFFF Shared between AP(ns) and Cortex-M7 Core0 <- RPMSG area - 32M
 * 0x84000000-0x85FFFFFF Shared between AP(ns) and Cortex-M7 Core1 <- RPMSG area - 32M
 */
#define VDEV0_VRING_BASE (0x20800000U)
#define VDEV1_VRING_BASE (0x20810000U)

#define BOARD_DEBUG_CONSOLE_TYPE BOARD_DEBUG_UART_TYPE
//#define BOARD_DEBUG_CONSOLE_TYPE BOARD_DEBUG_SWO_TYPE
#if BOARD_DEBUG_CONSOLE_TYPE == BOARD_DEBUG_UART_TYPE
#define BOARD_DEBUG_UART_INSTANCE 3
#define BOARD_DEBUG_CONSOLE_PORT BOARD_DEBUG_UART_INSTANCE
#define BOARD_DEBUG_CONSOLE_BAUDRATE BOARD_DEBUG_UART_BAUDRATE
#elif BOARD_DEBUG_CONSOLE_TYPE == BOARD_DEBUG_SWO_TYPE
#define BOARD_DEBUG_SWO_PORT 0
#define BOARD_DEBUG_CONSOLE_PORT BOARD_DEBUG_SWO_PORT
#define BOARD_DEBUG_CONSOLE_BAUDRATE BOARD_DEBUG_SWO_BAUDRATE
#endif
#define SOC_CORE_ICACHE_CTRL M33S_CACHE_CTRLPC
#define SOC_CORE_DCACHE_CTRL M33S_CACHE_CTRLPS
#define SOC_ELE_MU_INST_BASE (ELE_MU7_MUA)

#elif defined(CPU_MIMX94398AVKM_cm7_core0)
/*
 * 0x20800000-0x2089FFFF Shared between AP(ns) and Cortex-M33 Core1(M33S) ← RPMSG area
 *  640 KiB(4 x 32 KB vring + 2 x 512 x 512 bytes buffer) M33S OCRAM(from Cortex-M33 Core1 View)
 * 0x82000000-0x83FFFFFF Shared between AP(ns) and Cortex-M7 Core0 <- RPMSG area - 32M
 * 0x84000000-0x85FFFFFF Shared between AP(ns) and Cortex-M7 Core1 <- RPMSG area - 32M
 */
#define VDEV0_VRING_BASE (0x82000000U)
#define VDEV1_VRING_BASE (0x82010000U)

#define BOARD_DEBUG_CONSOLE_TYPE BOARD_DEBUG_UART_TYPE
//#define BOARD_DEBUG_CONSOLE_TYPE BOARD_DEBUG_SWO_TYPE
#if BOARD_DEBUG_CONSOLE_TYPE == BOARD_DEBUG_UART_TYPE
#define BOARD_DEBUG_UART_INSTANCE 11
#define BOARD_DEBUG_CONSOLE_PORT BOARD_DEBUG_UART_INSTANCE
#define BOARD_DEBUG_CONSOLE_BAUDRATE BOARD_DEBUG_UART_BAUDRATE
#elif BOARD_DEBUG_CONSOLE_TYPE == BOARD_DEBUG_SWO_TYPE
#define BOARD_DEBUG_SWO_PORT 1
#define BOARD_DEBUG_CONSOLE_PORT BOARD_DEBUG_SWO_PORT
#define BOARD_DEBUG_CONSOLE_BAUDRATE BOARD_DEBUG_SWO_BAUDRATE
#endif
#define SOC_ELE_MU_INST_BASE (ELE_MU5_MUA)

#elif defined(CPU_MIMX94398AVKM_cm7_core1)
/*
 * 0x20800000-0x2089FFFF Shared between AP(ns) and Cortex-M33 Core1(M33S) ← RPMSG area
 *  640 KiB(4 x 32 KB vring + 2 x 512 x 512 bytes buffer) M33S OCRAM(from Cortex-M33 Core1 View)
 * 0x82000000-0x83FFFFFF Shared between AP(ns) and Cortex-M7 Core0 <- RPMSG area - 32M
 * 0x84000000-0x85FFFFFF Shared between AP(ns) and Cortex-M7 Core1 <- RPMSG area - 32M
 */
#define VDEV0_VRING_BASE (0x84000000U)
#define VDEV1_VRING_BASE (0x84010000U)

#define BOARD_DEBUG_CONSOLE_TYPE BOARD_DEBUG_UART_TYPE
//#define BOARD_DEBUG_CONSOLE_TYPE BOARD_DEBUG_SWO_TYPE
#if BOARD_DEBUG_CONSOLE_TYPE == BOARD_DEBUG_UART_TYPE
#define BOARD_DEBUG_UART_INSTANCE 12
#define BOARD_DEBUG_CONSOLE_PORT BOARD_DEBUG_UART_INSTANCE
#define BOARD_DEBUG_CONSOLE_BAUDRATE BOARD_DEBUG_UART_BAUDRATE
#elif BOARD_DEBUG_CONSOLE_TYPE == BOARD_DEBUG_SWO_TYPE
#define BOARD_DEBUG_SWO_PORT 2
#define BOARD_DEBUG_CONSOLE_PORT BOARD_DEBUG_SWO_PORT
#define BOARD_DEBUG_CONSOLE_BAUDRATE BOARD_DEBUG_SWO_BAUDRATE
#endif
#define SOC_ELE_MU_INST_BASE (ELE_MU6_MUA)

#else
#error "Pls define CPU macro!"
#endif

#define LED_INIT()
#define LED_TOGGLE()

/* PCAL6408 */
#define BOARD_PCAL6408_I2C3            LPI2C3
#define BOARD_PCAL6408_I2C3_ADDR       (0x20U)
#define BOARD_PCAL6408_I2C3_CLOCK_ROOT kCLOCK_Lpi2c3
#define BOARD_PCAL6408_I2C3_CLOCK_FREQ CLOCK_GetRate(BOARD_PCAL6408_I2C3_CLOCK_ROOT)

/* PCAL6524 */
#define BOARD_PCAL6524_I2C3            LPI2C3
#define BOARD_PCAL6524_I2C3_ADDR       (0x21U)
#define BOARD_PCAL6524_I2C3_CLOCK_ROOT kCLOCK_Lpi2c3
#define BOARD_PCAL6524_I2C3_CLOCK_FREQ CLOCK_GetRate(BOARD_PCAL6524_I2C3_CLOCK_ROOT)

/*
 * IOMUXC_GPR VALUE
 * core0_did - iomuxc_gpr0[3:0]  - 0xd(13) - cortex-m33 core1
 * core1_did - iomuxc_gpr0[7:4]  - 0x2     - cortex-m33 core0
 * core2_did - iomuxc_gpr0[11:8] - 0x4     - cortex-m7 core0
 * core3_did - iomuxc_gpr0[15:12]- 0xe(14) - cortex-m7 core1
 */
#define BOARD_IOMUXC_GPR_VAL (0xE42D)

/*
 * @brief The Ethernet PHY addresses.
 * ethernet port    phy addr   endpoint/switch port   phy name              enetc/switch
 * ETH0             0x0        switch port 0          DP83849I(PORT A)      switch(enetc3)
 * ETH1             0x1        switch port 1          DP83849I(PORT B)      switch(enetc3)
 * ETH2             0x5        switch port 2          RTL8211FDI-VD-CG      switch(enetc3)
 * ETH2             0x5        endpoint 2             RTL8211FDI-VD-CG      enetc0
 * ETH3             0x6        endpoint 1             RTL8211FDI-VD-CG      enetc1
 * ETH4             0x7        endpoint 0             RTL8211FDI-VD-CG      enetc2
 *
 * Note: ETH2 and ETH3 cannot be used at the same time due to SoC constraint
 */
#define BOARD_EP0_PHY_ADDR       (0x07U)
#define BOARD_EP1_PHY_ADDR       (0x06U)
#define BOARD_EP2_PHY_ADDR       (0x05U)
#define BOARD_SWT_PORT0_PHY_ADDR (0x00U)
#define BOARD_SWT_PORT1_PHY_ADDR (0x01U)
#define BOARD_SWT_PORT2_PHY_ADDR (0x05U)

#if !defined(BOARD_USE_PCAL6408)
#define BOARD_USE_PCAL6408 1
#endif

#if !defined(BOARD_USE_PCAL6524)
#define BOARD_USE_PCAL6524 1
#endif


#define BOARD_PCAL6408_NUM 1
#define BOARD_PCAL6408_ADDR (0x20)
#define BOARD_PCAL6408_I2C3_HANDLE_IDX 0

#define BOARD_PCAL6524_NUM 1
#define BOARD_PCAL6524_ADDR (0x21)
#define BOARD_PCAL6524_I2C3_HANDLE_IDX 0

#define I2C_INSTANCE_IDX_SHIFT 24U
#define I2C_INSTANCE_IDX_MASK (0xFU << I2C_INSTANCE_IDX_SHIFT)
#define I2C_DEVICE_ADDR_SHIFT 16U
#define I2C_DEVICE_ADDR_MASK (0xFFU << I2C_DEVICE_ADDR_SHIFT)
#define I2C_CHAN_IDX_SHIFT 8U
#define I2C_CHAN_IDX_MASK (0xFFU << I2C_CHAN_IDX_SHIFT)
#define I2C_DEVICE_TYPE_SHIFT 4U
#define I2C_DEVICE_TYPE_MASK (0xFU << I2C_DEVICE_TYPE_SHIFT)
#define I2C_DEVICE_ARRAY_IDX_SHIFT 0x0U
#define I2C_DEVICE_ARRAY_IDX_MASK (0xFU << I2C_DEVICE_ARRAY_IDX_SHIFT)

typedef struct
{
   void *handle;
   void *data;
} i2c_device_table_t;

enum i2c_device_type {
    PCAL6408_TYPE, /* io expander */
    PCAL6524_TYPE, /* io expander */
    UNKNOWN_TYPE,
};

enum i2c_instance_idx {
    LPI2C0_INST_IDX,
    LPI2C1_INST_IDX,
    LPI2C2_INST_IDX,
    LPI2C3_INST_IDX,
    LPI2C4_INST_IDX,
    LPI2C5_INST_IDX,
    LPI2C6_INST_IDX,
    LPI2C7_INST_IDX,
    LPI2C8_INST_IDX,
};

enum expander_pin_idx {
    P0_0,
    P0 = P0_0,
    IMX_V2X_RST_B_3V3 = P0_0,
    P0_1,
    P1 = P0_1,
    MDM_RESET_B_3V3 = P0_1,
    KW47_RST_MASK = P1,
    P0_2,
    P2 = P0_2,
    M2_KE_PWREN = P0_2,
    FXLS8967A_WAKE_UP = P2,
    P0_3,
    P3 = P0_3,
    WIFI_PCIE1_RST_B = P0_3,
    TJA1121A_RST_MASK = P3,
    P0_4,
    P4 = P0_4,
    WIFI_SDIO_RST_B = P0_4,
    IMX_CAN1_WAKE_EN = P4,
    P0_5,
    P5 = P0_5,
    WIFI_BT_S_DIS1_B = P0_5,
    IMX_CAN2_WAKE_EN = P5,
    P0_6,
    P6 = P0_6,
    WIFI_BT_S_DIS2_B = P0_6,
    EN_12V0_EXT_IMX = P6,
    P0_7,
    P7 = P0_7,
    MDM_FULL_CARD_PWR_OFF = P0_7,
    MDM_PEWAKE_B = P7,
    P1_0,
    MDM_W_DISABLE1 = P1_0,
    ADC_SEL = P1_0,
    P1_1,
    CFG_MUX_USB1_SEL = P1_1,
    P1_2,
    WL_WAKE_IN = P1_2,
    P1_3,
    BT_WAKE_IN = P1_3,
    P1_4,
    GPIO_GNSS_RST = P1_4,
    P1_5,
    SD_CARD_ON_OFF = P1_5,
    P1_6,
    PWR_EN_SXF1800 = P1_6,
    P1_7,
    Q_RADIO_PRSNT = P1_7,
    P2_0,
    MQS_MIC_SEL = P2_0,
    P2_1,
    RST_SXF1800_B_3V3 = P2_1,
    P2_2,
    ETH0_RST_B = P2_2,
    P2_3,
    ETH1_RST_B = P2_3,
    P2_4,
    ETH2_RST_B = P2_4,
    P2_5,
    ETH3_RST_B = P2_5,
    P2_6,
    USB2_MUX_SEL = P2_6,
    P2_7,
    MDM_PERST_B = P2_7,
};

/*
 *  i2c instance index                                                     i2c channel index                       i2c device array index
 *           ^                                                                     ^                                         ^
 *           |                                                                     |                                         |
 * ---------------------                                       ---------------------------------------               +-------+-------+
 * |                   |                                       |                                     |               |               |
 * --------------------+---------------------------------------+----------------------------------------------------------------------
 * | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 * --------------------+---------------------------------------+----------------------------------------------------------------------
 *                     |                                       |                                     |               |
 *                     -----------------------------------------                                     --------+--------
 *                                         |                                                                 | 
 *                                         v                                                                 v
 *                                 i2c device address                                              i2c device type
 */
enum i2c_device_id {
    BOARD_PCAL6408_I2C3_ID = (LPI2C3_INST_IDX << I2C_INSTANCE_IDX_SHIFT) | (BOARD_PCAL6408_I2C3_ADDR << I2C_DEVICE_ADDR_SHIFT) | (PCAL6408_TYPE << I2C_DEVICE_TYPE_SHIFT) | 0,
    BOARD_PCAL6524_I2C3_ID = (LPI2C3_INST_IDX << I2C_INSTANCE_IDX_SHIFT) | (BOARD_PCAL6524_I2C3_ADDR << I2C_DEVICE_ADDR_SHIFT) | (PCAL6524_TYPE << I2C_DEVICE_TYPE_SHIFT) | 1,
    BOARD_UNKNOWN_ID      = (UNKNOWN_TYPE << I2C_DEVICE_TYPE_SHIFT) | 2,
};



typedef void (*i2c_device_init_func_t)( uint32_t i2cDevId,
                                            void *base,
                                            uint32_t clkFreq,
                                            uint8_t i2cAddr);
typedef status_t (*i2c_device_set_pin_to_high_func_t)(uint32_t i2cDevId,
                                                uint8_t pinIdx);
typedef status_t (*i2c_device_set_pin_to_low_func_t)( uint32_t i2cDevId,
                                                uint8_t pinIdx);
typedef status_t (*i2c_device_set_pin_as_input_func_t)( uint32_t i2cDevId,
                                                uint8_t pinIdx);
typedef status_t (*i2c_device_set_pin_as_output_func_t)( uint32_t i2cDevId,
                                                uint8_t pinIdx);

typedef struct {
    i2c_device_init_func_t init;
    i2c_device_set_pin_to_high_func_t set_pin_to_high;
    i2c_device_set_pin_to_low_func_t set_pin_to_low;
    i2c_device_set_pin_as_input_func_t set_pin_as_input;
    i2c_device_set_pin_as_output_func_t set_pin_as_output;
} io_expander_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
void BOARD_InitDebugConsole(void);
status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subaddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize,
                          uint32_t flags);
status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subaddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize,
                             uint32_t flags);

void BOARD_InitI2cDevice(uint32_t i2cDevId);

/*
 * How to use the api?
 * e.g.
 * BOARD_EXPANDER_SetPinToHigh(BOARD_PCAL6408_I2C3_ID, P0);
 */
status_t BOARD_EXPANDER_SetPinToHigh(uint32_t i2cDevId, uint8_t pinIdx);
/*
 * How to use the api?
 * e.g.
 * BOARD_EXPANDER_SetPinToLow(BOARD_PCAL6408_I2C3_ID, P0);
 */
status_t BOARD_EXPANDER_SetPinToLow(uint32_t i2cDevId, uint8_t pinIdx);
/*
 * How to use the api?
 * e.g.
 * BOARD_EXPANDER_SetPinAsInput(BOARD_PCAL6408_I2C3_ID, P0);
 */
status_t BOARD_EXPANDER_SetPinAsInput(uint32_t i2cDevId, uint8_t pinIdx);
/*
 * How to use the api?
 * e.g.
 * BOARD_EXPANDER_SetPinAsOutput(BOARD_PCAL6408_I2C3_ID, P0);
 */
status_t BOARD_EXPANDER_SetPinAsOutput(uint32_t i2cDevId, uint8_t pinIdx);

void BOARD_ConfigMPU(void);

void BOARD_InitDebugConsolePins(void);

clock_ip_name_t BOARD_GetUartClkId(uint32_t uartInstIdx);

uint32_t BOARD_GetUartFreq(uint32_t uartInstIdx);

uint32_t BOARD_Lpuart1GetFreq(void);
uint32_t BOARD_Lpuart2GetFreq(void);
uint32_t BOARD_Lpuart3GetFreq(void);
uint32_t BOARD_Lpuart4GetFreq(void);
uint32_t BOARD_Lpuart5GetFreq(void);
uint32_t BOARD_Lpuart6GetFreq(void);
uint32_t BOARD_Lpuart7GetFreq(void);
uint32_t BOARD_Lpuart8GetFreq(void);
uint32_t BOARD_Lpuart9GetFreq(void);
uint32_t BOARD_Lpuart10GetFreq(void);
uint32_t BOARD_Lpuart11GetFreq(void);
uint32_t BOARD_Lpuart12GetFreq(void);

void BOARD_InitI2C6Pins(void);

void BOARD_InitLpuart1Pins(void);
void BOARD_InitLpuart2Pins(void);
void BOARD_InitLpuart3Pins(void);
void BOARD_InitLpuart4Pins(void);
void BOARD_InitLpuart5Pins(void);
void BOARD_InitLpuart6Pins(void);
void BOARD_InitLpuart7Pins(void);
void BOARD_InitLpuart8Pins(void);
void BOARD_InitLpuart9Pins(void);
void BOARD_InitLpuart10Pins(void);
void BOARD_InitLpuart11Pins(void);
void BOARD_InitLpuart12Pins(void);

void BOARD_DeinitLpuart1Pins(void);
void BOARD_DeinitLpuart2Pins(void);
void BOARD_DeinitLpuart3Pins(void);
void BOARD_DeinitLpuart4Pins(void);
void BOARD_DeinitLpuart5Pins(void);
void BOARD_DeinitLpuart6Pins(void);
void BOARD_DeinitLpuart7Pins(void);
void BOARD_DeinitLpuart8Pins(void);
void BOARD_DeinitLpuart9Pins(void);
void BOARD_DeinitLpuart10Pins(void);
void BOARD_DeinitLpuart11Pins(void);
void BOARD_DeinitLpuart12Pins(void);

void BOARD_InitLpuartPins(uint32_t instIdx);

void BOARD_ShowEleFwInfo(void);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* _BOARD_H_ */
