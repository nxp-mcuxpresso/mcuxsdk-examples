/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#if defined(BOARD_USE_PCAL6524) && BOARD_USE_PCAL6524
#include "fsl_pcal6524.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "FRDM-IMXRT1152"
#ifndef DEBUG_CONSOLE_UART_INDEX
#define DEBUG_CONSOLE_UART_INDEX 1
#endif
#if defined(CONFIG_OT_CLI_IW612) && (CONFIG_OT_CLI_IW612 == 1)
#ifndef BOARD_APP_UART_INSTANCE
#define BOARD_APP_UART_INSTANCE 2U
#endif

#ifndef BOARD_APP_UART_BAUDRATE
#define BOARD_APP_UART_BAUDRATE 115200
#endif /* BOARD_APP_UART_BAUDRATE */
#endif
/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE     kSerialPort_Uart
#define BOARD_DEBUG_UART_CLK_FREQ 24000000

#if DEBUG_CONSOLE_UART_INDEX == 1
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART1
#define BOARD_DEBUG_UART_INSTANCE 1U
#define BOARD_UART_IRQ            LPUART1_IRQn
#define BOARD_UART_IRQ_HANDLER    LPUART1_IRQHandler
#if defined(CONFIG_OT_CLI_IW612) && (CONFIG_OT_CLI_IW612 == 1)
#define BOARD_APP_UART_BASEADDR    (uint32_t) LPUART2
#define BOARD_APP_UART_CLK_FREQ    CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart2)
#define BOARD_APP_UART_IRQ_HANDLER LPUART2_IRQHandler
#define BOARD_APP_UART_IRQ         LPUART2_IRQn
#endif
#elif DEBUG_CONSOLE_UART_INDEX == 8
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART8
#define BOARD_DEBUG_UART_INSTANCE 8U
#define BOARD_UART_IRQ            LPUART8_IRQn
#define BOARD_UART_IRQ_HANDLER    LPUART8_IRQHandler
#else
#error "Unsupported UART"
#endif

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE (115200U)
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/* Definitions for eRPC MU transport layer */
#if defined(FSL_FEATURE_MU_SIDE_A)
#define MU_BASE        MUA
#define MU_IRQ         MUA_IRQn
#define MU_IRQ_HANDLER MUA_IRQHandler
#endif
#if defined(FSL_FEATURE_MU_SIDE_B)
#define MU_BASE        MUB
#define MU_IRQ         MUB_IRQn
#define MU_IRQ_HANDLER MUB_IRQHandler
#endif
#define MU_IRQ_PRIORITY (2)

/*! @brief The USER_LED used for board */
#define LOGIC_LED_ON  (1U)
#define LOGIC_LED_OFF (0U)
#ifndef BOARD_USER_LED_GPIO
#define BOARD_USER_LED_GPIO GPIO8
#endif
#ifndef BOARD_USER_LED_GPIO_PIN
#define BOARD_USER_LED_GPIO_PIN (7U)
#endif

#define USER_LED_INIT(output)                                            \
    GPIO_PinWrite(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, output); \
    BOARD_USER_LED_GPIO->GDIR |= (1U << BOARD_USER_LED_GPIO_PIN)                       /*!< Enable target USER_LED */
#define USER_LED_OFF() \
    GPIO_PortClear(BOARD_USER_LED_GPIO, 1U << BOARD_USER_LED_GPIO_PIN)                 /*!< Turn off target USER_LED */
#define USER_LED_ON() GPIO_PortSet(BOARD_USER_LED_GPIO, 1U << BOARD_USER_LED_GPIO_PIN) /*!<Turn on target USER_LED*/
#define USER_LED_TOGGLE()                                       \
    GPIO_PinWrite(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, \
                  0x1 ^ GPIO_PinRead(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN)) /*!< Toggle target USER_LED */

/*! @brief Define the port interrupt number for the board switches */
#ifndef BOARD_USER_BUTTON_GPIO
#define BOARD_USER_BUTTON_GPIO GPIO13
#endif
#ifndef BOARD_USER_BUTTON_GPIO_PIN
#define BOARD_USER_BUTTON_GPIO_PIN (0U)
#endif
#define BOARD_USER_BUTTON_IRQ         GPIO13_Combined_0_31_IRQn
#define BOARD_USER_BUTTON_IRQ_HANDLER GPIO13_Combined_0_31_IRQHandler
#define BOARD_USER_BUTTON_NAME        "SW6"

/*! @brief The board flash size */
#define BOARD_FLASH_SIZE (0x1000000U)

/*! @brief PHY0 (JL1111) address; this 100M RMII PHY is connected to ENET_QOS. */
#define BOARD_ENET0_PHY_ADDRESS (0x02U)

/*! @brief PHY1 (YT8531) address; this 1G RGMII PHY is connected to ENET_1G. */
#define BOARD_ENET1_PHY_ADDRESS (0x05U)

/* USB PHY condfiguration */
#define BOARD_USB_PHY_D_CAL     (0x07U)
#define BOARD_USB_PHY_TXCAL45DP (0x06U)
#define BOARD_USB_PHY_TXCAL45DM (0x06U)

#define BOARD_ARDUINO_INT_IRQ   (GPIO1_INT3_IRQn)
#define BOARD_ARDUINO_I2C_IRQ   (LPI2C1_IRQn)
#define BOARD_ARDUINO_I2C_INDEX (1)

#define BOARD_HAS_SDCARD (1U)

#define BOARD_CODEC_I2C_BASEADDR             LPI2C4
#define BOARD_CODEC_I2C_INSTANCE             4U
#define BOARD_CODEC_I2C_CLOCK_SOURCE_SELECT  (0U)
#define BOARD_CODEC_I2C_CLOCK_SOURCE_DIVIDER (6U)
#define BOARD_CODEC_I2C_CLOCK_FREQ           (24000000U)

/* Touch panel. */
#define BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR      LPI2C1
#define BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_ROOT    kCLOCK_Root_Lpi2c1
#define BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_SOURCE  (1U)  /* OSC24M. */
#define BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_DIVIDER (12U) /* Divider = 12, LPI2C clock frequency 2M. */
#define BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_FREQ    CLOCK_GetRootClockFreq(BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_ROOT)

/* PCAL6524 I/O Expander */
#define BOARD_PCAL6524_I2C            LPI2C4
#define BOARD_PCAL6524_I2C_ADDR       (0x22U)
#define BOARD_PCAL6524_I2C_CLOCK_ROOT kCLOCK_Root_Lpi2c4
#define BOARD_PCAL6524_I2C_CLOCK_FREQ CLOCK_GetRootClockFreq(BOARD_PCAL6524_I2C_CLOCK_ROOT)
#define BOARD_PCAL6524_INT_GPIO        GPIO3
#define BOARD_PCAL6524_INT_PIN         26
#define BOARD_PCAL6524_INT_IRQ         GPIO3_Combined_16_31_IRQn
#define BOARD_PCAL6524_INT_IRQ_HANDLER GPIO3_Combined_16_31_IRQHandler
/* PCAL6524 Output func pins */
#define BOARD_PCAL6524_CAN1_STBY      (0U + 0U)
#define BOARD_PCAL6524_CAN2_STBY      (0U + 1U)
#define BOARD_PCAL6524_Backlight_CTL  (0U + 2U)
#define BOARD_PCAL6524_LCD_RST_B      (0U + 3U)
#define BOARD_PCAL6524_WL_RST         (0U + 4U)
#define BOARD_PCAL6524_BT_RST         (0U + 5U)
#define BOARD_PCAL6524_PDn            (0U + 6U)
#define BOARD_PCAL6524_ENET_QOS_RST_B (0U + 7U)
#define BOARD_PCAL6524_ETH0_RST_B     (8U + 0U)
#define BOARD_PCAL6524_BT_DEV_WAKE    (8U + 1U)
#define BOARD_PCAL6524_WL_DEV_WAKE    (8U + 2U)
#define BOARD_PCAL6524_LCM_PWR_EN     (8U + 3U)
#define BOARD_PCAL6524_CTP_RST_B      (8U + 4U)
/* PCAL6524 Input func pins */
#define BOARD_PCAL6524_HP_DET_B       (8U + 5U)
#define BOARD_PCAL6524_CCC_INTB       (8U + 6U)
#define BOARD_PCAL6524_AUD_INT        (8U + 7U)
#define BOARD_PCAL6524_WIFI_WAKE_B    (16U + 0U)
#define BOARD_PCAL6524_BT_WAKE_B      (16U + 1U)
#define BOARD_PCAL6524_RST_IND        (16U + 2U)
#define BOARD_PCAL6524_CTP_INT        (16U + 3U)
/* PCAL6524 unused pins — routed only to test points (P2_4=TP9, P2_5=TP12,
 * P2_6=TP14, P2_7=TP16). Safe for examples that need free GPIO without
 * disturbing any on-board peripheral. */
#define BOARD_PCAL6524_P2_4           (16U + 4U)
#define BOARD_PCAL6524_P2_5           (16U + 5U)
#define BOARD_PCAL6524_P2_6           (16U + 6U)
#define BOARD_PCAL6524_P2_7           (16U + 7U)

/* SD card detection method when using wifi module. */
#define BOARD_WIFI_SD_DETECT_TYPE kSDMMCHOST_DetectCardByHostDATA3

/* @Brief Board Bluetooth HCI UART configuration */
#define BOARD_BT_UART_BASEADDR    LPUART7
#define BOARD_BT_UART_INSTANCE    7
#define BOARD_BT_UART_BAUDRATE    3000000
#define BOARD_BT_UART_CLK_FREQ    CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart7);
#define BOARD_BT_UART_IRQ         LPUART7_IRQn
#define BOARD_BT_UART_IRQ_HANDLER LPUART7_IRQHandler

/*! @brief The Ethernet port used by network examples, default use 1G port. */
/* Below comment is for test script to easily define which port to be used, please don't delete. */
/* @TEST_ANCHOR */
#ifndef BOARD_NETWORK_USE_100M_ENET_PORT
#define BOARD_NETWORK_USE_100M_ENET_PORT (0U)
#endif
#if defined(CONFIG_OT_CLI_IW612) && (CONFIG_OT_CLI_IW612 == 1)
/* Timer Manager definition. */
#define BOARD_TM_INSTANCE   1
#define BOARD_TM_CLOCK_ROOT kCLOCK_Root_Gpt1
#endif

/* Board MPU region index definition. Higher index takes priority on memory overlap. */
#if __CORTEX_M == 7
#define BOARD_MPU_REGION_FULL_BG      (0)  /* Whole 4G space background without access permission.
                                              Needed for Arm errata 1013783-B */
#define BOARD_MPU_REGION_FLEXSPI2_BG  (2)  /* FLEXSPI2 background (overlaid by HYPERRAM region) */
#define BOARD_MPU_REGION_CODE_SRAM_BG (3)  /* Code/SRAM background space under 1GB */
#define BOARD_MPU_REGION_ITCM         (4)  /* ITCM */
#define BOARD_MPU_REGION_DTCM         (5)  /* DTCM */
#define BOARD_MPU_REGION_OCRAM_A      (6)  /* OCRAM first region */
#define BOARD_MPU_REGION_OCRAM_B      (7)  /* OCRAM second region */
#define BOARD_MPU_REGION_FLEXSPI1     (8)  /* FLEXSPI1 */
#define BOARD_MPU_REGION_HYPERRAM     (9)  /* HyperRAM on FLEXSPI2 */
#define BOARD_MPU_REGION_NONCACHE     (10) /* Noncacheable space in application */
#define BOARD_MPU_REGION_AIPS         (11) /* AIPS 1-4 */
#define BOARD_MPU_REGION_SIM_M_DISP   (12) /* SIM_M/DISP configuration space */
#define BOARD_MPU_REGION_SIM_M7       (13) /* SIM_M7 configuration space */
#define BOARD_MPU_REGION_GPU2D_CDOG   (14) /* GPU2D/CDOG */
#define BOARD_MPU_REGION_AIPS_M7      (15) /* AIPS M7 */
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
uint32_t BOARD_DebugConsoleSrcFreq(void);

void BOARD_InitDebugConsole(void);

#if __CORTEX_M == 7
void BOARD_ResetMPU(void);
#endif
void BOARD_ConfigMPU(void);
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz);
status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subaddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize);
status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subaddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize);
status_t BOARD_LPI2C_SendSCCB(LPI2C_Type *base,
                              uint8_t deviceAddress,
                              uint32_t subAddress,
                              uint8_t subaddressSize,
                              uint8_t *txBuff,
                              uint8_t txBuffSize);
status_t BOARD_LPI2C_ReceiveSCCB(LPI2C_Type *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subaddressSize,
                                 uint8_t *rxBuff,
                                 uint8_t rxBuffSize);

void BOARD_Codec_I2C_Init(void);
status_t BOARD_Codec_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Codec_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

void BOARD_MIPIPanelTouch_I2C_Init(void);
status_t BOARD_MIPIPanelTouch_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_MIPIPanelTouch_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

/* Generic I2C device wrapper functions for components */
status_t BOARD_I2C_DeviceSend(void *base,
                               uint8_t deviceAddress,
                               uint32_t subAddress,
                               uint8_t subAddressSize,
                               const uint8_t *txBuff,
                               uint8_t txBuffSize,
                               uint32_t flags);
status_t BOARD_I2C_DeviceReceive(void *base,
                                  uint8_t deviceAddress,
                                  uint32_t subAddress,
                                  uint8_t subAddressSize,
                                  uint8_t *rxBuff,
                                  uint8_t rxBuffSize,
                                  uint32_t flags);

#if defined(BOARD_USE_PCAL6524) && BOARD_USE_PCAL6524
void BOARD_PCAL6524_I2C_Init(void);
status_t BOARD_PCAL6524_I2C_Send(uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 const uint8_t *txBuff,
                                 uint8_t txBuffSize,
                                 uint32_t flags);
status_t BOARD_PCAL6524_I2C_Receive(uint8_t deviceAddress,
                                    uint32_t subAddress,
                                    uint8_t subAddressSize,
                                    uint8_t *rxBuff,
                                    uint8_t rxBuffSize,
                                    uint32_t flags);

void BOARD_InitPCAL6524(pcal6524_handle_t *handle);

void BOARD_PullMIPIPanelTouchResetPin(bool pullUp);
void BOARD_ConfigMIPIPanelTouchIntPin(uint8_t mode);
void BOARD_PCAL6524_ProcessInterrupt(void);

extern volatile bool g_pcal6524IntFlag;
extern volatile bool g_touchIntFlag;

#endif /* BOARD_USE_PCAL6524 */

#endif /* SDK_I2C_BASED_COMPONENT_USED */

void BOARD_SD_Pin_Config(uint32_t speed, uint32_t strength);
void BOARD_MMC_Pin_Config(uint32_t speed, uint32_t strength);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
