/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "fsl_trdc.h"
#include "fsl_modcon.h"
#if defined(BOARD_USE_PCAL6524) && BOARD_USE_PCAL6524
#include "fsl_pcal6524.h"
#endif
#if defined(BOARD_USE_PCA9555) && BOARD_USE_PCA9555
#include "fsl_pca9555.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "MIMXRT2660-EVK"

/*! @brief Route every TRDC master to the privileged (domain 0) view.
 *  Selects the exhaustive branch of BOARD_ConfigTRDC() which walks every
 *  MAIN / CMPT / WAKE / AUDIO / COMM / MEDIA master and pins its MDA to
 *  DID=0. Set to 0 to fall back to the minimal init that only covers the
 *  masters actually driven by the CPU. */
#ifndef BOARD_TRDC_ALL_MASTER_TO_PREVELEGE_DOMAIN
#define BOARD_TRDC_ALL_MASTER_TO_PREVELEGE_DOMAIN 1
#endif

/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpuart0_fclk)
#define BOARD_DEBUG_UART_TYPE     kSerialPort_Uart
#ifndef BOARD_DEBUG_UART_INSTANCE
#define BOARD_DEBUG_UART_INSTANCE 0U
#endif
#ifndef BOARD_UART_IRQ
#define BOARD_UART_IRQ LPUART1_IRQn
#endif
#ifndef BOARD_UART_IRQ_HANDLER
#define BOARD_UART_IRQ_HANDLER LPUART1_IRQHandler
#endif
#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE (115200U)
#endif

/*! @brief Define the port interrupt number for the board switches */
#ifndef BOARD_USER_BUTTON_GPIO
#define BOARD_USER_BUTTON_GPIO VBAT__GPIO
#endif
#ifndef BOARD_USER_BUTTON_GPIO_PIN
#define BOARD_USER_BUTTON_GPIO_PIN (4U)
#endif
#define BOARD_USER_BUTTON_IRQ         VBAT_GPIO_CH0_IRQn
#define BOARD_USER_BUTTON_IRQ_HANDLER VBAT_GPIO_CH0_IRQHandler
#define BOARD_USER_BUTTON_NAME        "SW5"

#ifndef BOARD_USER_BUTTON_6_GPIO
#define BOARD_USER_BUTTON_6_GPIO WAKE__GPIO
#endif
#ifndef BOARD_USER_BUTTON_6_GPIO_PIN
#define BOARD_USER_BUTTON_6_GPIO_PIN (0U)
#endif
#define BOARD_USER_BUTTON_6_IRQ         WAKE_GPIO_CH0_IRQn
#define BOARD_USER_BUTTON_6_IRQ_HANDLER WAKE_GPIO_CH0_IRQHandler
#define BOARD_USER_BUTTON_6_NAME        "SW6"


/*! @brief The board flash size */
#define BOARD_FLASH_SIZE (0x800000U)

/*! @brief The ENET PHY address. */
#define BOARD_ENET0_PHY_ADDRESS (0x02U) /* Phy address of enet port 0. */

/*! @brief The ENET PHY used for board. */
#ifndef BOARD_ENET_PHY_RESET_GPIO
#define BOARD_ENET_PHY_RESET_GPIO GPIO1
#endif
#ifndef BOARD_ENET_PHY_RESET_GPIO_PIN
#define BOARD_ENET_PHY_RESET_GPIO_PIN (9U)
#endif

#define BOARD_ENET_PHY_RESET                                                          \
    GPIO_WritePinOutput(BOARD_ENET_PHY_RESET_GPIO, BOARD_ENET_PHY_RESET_GPIO_PIN, 0); \
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));                          \
    GPIO_WritePinOutput(BOARD_ENET_PHY_RESET_GPIO, BOARD_ENET_PHY_RESET_GPIO_PIN, 1); \
    SDK_DelayAtLeastUs(100, CLOCK_GetFreq(kCLOCK_CpuClk))

/* USB PHY condfiguration */
#define BOARD_USB_PHY_D_CAL     (0x0CU)
#define BOARD_USB_PHY_TXCAL45DP (0x06U)
#define BOARD_USB_PHY_TXCAL45DM (0x06U)

#define BOARD_ARDUINO_INT_IRQ   (GPIO1_INT3_IRQn)
#define BOARD_ARDUINO_I2C_IRQ   (LPI2C1_IRQn)
#define BOARD_ARDUINO_I2C_INDEX (1)

/* @Brief Board accelerator sensor configuration */
#define BOARD_ACCEL_I2C_BASEADDR LPI2C1
/* Select USB1 PLL (480 MHz) as LPI2C's clock source */
#define BOARD_ACCEL_I2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for LPI2C clock source */
#define BOARD_ACCEL_I2C_CLOCK_SOURCE_DIVIDER (5U)
#define BOARD_ACCEL_I2C_CLOCK_FREQ           (CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8 / (BOARD_ACCEL_I2C_CLOCK_SOURCE_DIVIDER + 1U))

#define BOARD_CODEC_I2C_BASEADDR             LPI2C1
#define BOARD_CODEC_I2C_INSTANCE             1U
#define BOARD_CODEC_I2C_CLOCK_SOURCE_SELECT  (0U)
#define BOARD_CODEC_I2C_CLOCK_SOURCE_DIVIDER (5U)
#define BOARD_CODEC_I2C_CLOCK_FREQ           (10000000U)

#define BOARD_CAMERA_I2C_BASEADDR             HSP__LPI2C_0
// #define BOARD_CAMERA_I2C_CLOCK_SOURCE_DIVIDER (5U)
// #define BOARD_CAMERA_I2C_CLOCK_SOURCE_SELECT  (0U) /* Select USB1 PLL (480 MHz) as LPI2C's clock source */
#define BOARD_CAMERA_I2C_CLOCK_FREQ           (24000000U)

/* @brief Display panel control signals */
#define BOARD_MIPI_PANEL_BL_GPIO            HSP__GPIO_1
#define BOARD_MIPI_PANEL_BL_PIN             28U

#define BOARD_DBI_PANEL_LCD_RST_GPIO       HSP__GPIO_1
#define BOARD_DBI_PANEL_LCD_RST_PIN        23U
#define BOARD_DBI_PANEL_TE_GPIO            HSP__GPIO_1
#define BOARD_DBI_PANEL_TE_PIN             20U
#define BOARD_DBI_PANEL_BLK_GPIO           HSP__GPIO_1
#define BOARD_DBI_PANEL_BLK_PIN            21U
#define BOARD_DBI_PANEL_INT_GPIO           HSP__GPIO_1
#define BOARD_DBI_PANEL_INT_PIN            22U

#if (DEMO_PANEL == DEMO_PANEL_RK055MHD091A0)
#define BOARD_TOUCH_I2C_BASEADDR   HSP__LPI2C_0
#define BOARD_TOUCH_I2C_CLOCK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpi2c0_fclk)
#define BOARD_TOUCH_I2C_CLOCK kCLOCK_MAIN_hsp_lpi2c0
#elif (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
#define BOARD_TOUCH_I2C_BASEADDR   HSP__LPI2C_1
#define BOARD_TOUCH_I2C_CLOCK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpi2c1_fclk)
#define BOARD_TOUCH_I2C_CLOCK kCLOCK_MAIN_hsp_lpi2c1
#elif (DEMO_PANEL == DEMO_PANEL_LCM_RGB_5INCH)
#define BOARD_TOUCH_I2C_BASEADDR   HSP__LPI2C_1
#define BOARD_TOUCH_I2C_CLOCK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpi2c1_fclk)
#define BOARD_TOUCH_I2C_CLOCK kCLOCK_MAIN_hsp_lpi2c1
#endif

#define BOARD_BT_UART_INSTANCE    3
#define BOARD_BT_UART_BAUDRATE    3000000
#define BOARD_BT_UART_CLK_FREQ    BOARD_DEBUG_UART_CLK_FREQ
#define BOARD_BT_UART_IRQ         LPUART3_IRQn
#define BOARD_BT_UART_IRQ_HANDLER LPUART3_IRQHandler

/*! @brief board has sdcard */
#define BOARD_HAS_SDCARD (1U)

/*! @brief The USER_LED used for board */
#define LOGIC_LED_ON  (0U)
#define LOGIC_LED_OFF (1U)
#ifndef BOARD_USER_LED_GPIO
#define BOARD_USER_LED_GPIO HSP__GPIO_0
#endif
#ifndef BOARD_USER_LED_GPIO_PIN
#define BOARD_USER_LED_GPIO_PIN 26U
#endif


#define USER_LED_INIT(output)                                            \
    GPIO_PinWrite(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, output); \
    BOARD_USER_LED_GPIO->PDDR |= (1U << BOARD_USER_LED_GPIO_PIN)                       /*!< Enable target USER_LED */
#define USER_LED_ON() GPIO_PortSet(BOARD_USER_LED_GPIO, 1U << BOARD_USER_LED_GPIO_PIN) /*!<Turn on target USER_LED*/
#define USER_LED_OFF() \
    GPIO_PortClear(BOARD_USER_LED_GPIO, 1U << BOARD_USER_LED_GPIO_PIN)                 /*!< Turn off target USER_LED */
#define USER_LED_TOGGLE()                                       \
    GPIO_PinWrite(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, \
                  0x1 ^ GPIO_PinRead(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN)) /*!< Toggle target USER_LED */

/* Board RGB LED color mapping */
#ifndef BOARD_LED_GREEN_GPIO
#define BOARD_LED_GREEN_GPIO HSP__GPIO_0
#endif
#ifndef BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LED_GREEN_GPIO_PIN 18U
#endif
#ifndef BOARD_LED_BLUE_GPIO
#define BOARD_LED_BLUE_GPIO HSP__GPIO_1
#endif
#ifndef BOARD_LED_BLUE_GPIO_PIN
#define BOARD_LED_BLUE_GPIO_PIN 5U
#endif

#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO HSP__GPIO_0
#endif
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN 6U
#endif

/*!
 * @brief The Ethernet PHY used by network examples.
 * Set to 0 to use external PHY over RMII (default).
 * Set to 1 to use internal digital 10BASE-T1S PHY.
 */
/* Below comment is for test script to easily define which PHY to be used, please don't delete. */
/* @TEST_ANCHOR */
#ifndef BOARD_NETWORK_USE_TENBASET_PHY
#define BOARD_NETWORK_USE_TENBASET_PHY (0U)
#endif

/* PCAL6524 I/O Expander */
#define BOARD_PCAL6524_I2C             HSP__LPI2C_0
#define BOARD_PCAL6524_I2C_ADDR        (0x22U)
#define BOARD_PCAL6524_I2C_CLOCK_ROOT  kCLOCK_Root_MAIN_lpi2c0_fclk
#define BOARD_PCAL6524_I2C_CLOCK_FREQ  CLOCK_GetRootClockFreq(BOARD_PCAL6524_I2C_CLOCK_ROOT)
#define BOARD_PCAL6524_INT_GPIO        HSP__GPIO_0
#define BOARD_PCAL6524_INT_PIN         27
#define BOARD_PCAL6524_INT_IRQ         HSP_GPIO0_CH0_IRQn
#define BOARD_PCAL6524_INT_IRQ_HANDLER HSP_GPIO0_CH0_IRQHandler
/* PCAL6524 (U23) Output func pins */
#define BOARD_PCAL6524_CSI_RST_B      (8U + 1U)
#define BOARD_PCAL6524_CSI_PWR_CTL    (8U + 2U)
#define BOARD_PCAL6524_BT_RST         (8U + 3U)
#define BOARD_PCAL6524_WL_RST         (8U + 4U)
#define BOARD_PCAL6524_WIFI_RST_B     (8U + 5U)
#define BOARD_PCAL6524_BT_DEV_WAKE    (8U + 6U)
#define BOARD_PCAL6524_WL_DEV_WAKE    (8U + 7U)
#define BOARD_PCAL6524_LCM_PWR_EN1    (16U + 0U) /* RGB panel power enable */
#define BOARD_PCAL6524_ETH0_RST_B     (16U + 1U)
#define BOARD_PCAL6524_ETH1_RST_B     (16U + 2U)
#define BOARD_PCAL6524_LCD_RST_B      (16U + 3U) /* MIPI-DSI panel LCD reset */
#define BOARD_PCAL6524_CTP_RST_B      (16U + 4U) /* MIPI-DSI panel touch reset */
#define BOARD_PCAL6524_SD_PWREN       (16U + 5U)
#define BOARD_PCAL6524_USB0_PWR       (16U + 6U)
#define BOARD_PCAL6524_LCD_RST        (16U + 7U) /* RGB panel touch reset */
/* PCAL6524 (U23) Input func pins */
#define BOARD_PCAL6524_WIFI_WAKE_B_3V3 (0U + 0U)
#define BOARD_PCAL6524_ETH0_INT_B     (0U + 1U)
#define BOARD_PCAL6524_ETH1_INT_B     (0U + 2U)
#define BOARD_PCAL6524_LCD_TOUCH_INT  (0U + 3U) /* RGB panel touch interrupt */
#define BOARD_PCAL6524_USB0_OC        (0U + 4U)
#define BOARD_PCAL6524_USB0_ID        (0U + 5U)
#define BOARD_PCAL6524_CTP_INT        (0U + 6U) /* MIPI-DSI panel touch interrupt */
#define BOARD_PCAL6524_LCD_LPTE       (0U + 7U)
#define BOARD_PCAL6524_BT_WAKE_B_3V3  (8U + 0U)

/* PCA9555A I/O Expander (U?? — second IO expander, distinct from PCAL6524).
 * Shares the LPI2C0 bus with PCAL6524 but lives at a different I2C address.
 * Its open-drain INT line (EXP_nIRQ2) is wired to PIO3_29 (HSP_GPIO1 pin 29). */
#define BOARD_PCA9555_I2C             HSP__LPI2C_0
#define BOARD_PCA9555_I2C_ADDR        (0x21U)
#define BOARD_PCA9555_I2C_CLOCK_ROOT  kCLOCK_Root_MAIN_lpi2c0_fclk
#define BOARD_PCA9555_I2C_CLOCK_FREQ  CLOCK_GetRootClockFreq(BOARD_PCA9555_I2C_CLOCK_ROOT)
#define BOARD_PCA9555_INT_GPIO        HSP__GPIO_1
#define BOARD_PCA9555_INT_PIN         29
#define BOARD_PCA9555_INT_IRQ         HSP_GPIO1_CH0_IRQn
#define BOARD_PCA9555_INT_IRQ_HANDLER HSP_GPIO1_CH0_IRQHandler

#define BOARD_PCA9555_CSI_PWDN     (8U + 0U) /* Parallel CSI camera (J95) power-down */
#define BOARD_PCA9555_BL_EN_RGB    (8U + 1U) /* RGB panel backlight enable */
#define BOARD_PCA9555_LCM_PWR_EN2  (8U + 2U) /* MIPI-DSI panel power enable */

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

/* Initialize a caller-owned PCAL6524 handle. Existing ENET examples keep
 * their own static handle and call this directly. */
void BOARD_InitPCAL6524(pcal6524_handle_t *handle);

/* Lazy-initialized board-level singleton + ISR-ready helpers. The first call
 * to BOARD_GetPCAL6524Handle() invokes BOARD_InitPCAL6524 on the singleton
 * (lockFunc = generic IRQ-masking lock). Subsequent calls return the same
 * pointer without re-initializing. Use these when the PCAL6524 INT line and
 * board.c's MCU GPIO ISR are involved.
 *
 * The MCU GPIO ISR for the PCAL6524 INT line is BOARD_PCAL6524_INT_IRQ_HANDLER
 * (HSP_GPIO0_CH0_IRQHandler), defined as a weak symbol in board.c so apps
 * that want a different dispatch pattern can override it. The default fires
 * PCAL6524_InterruptHandler on the singleton handle. */
pcal6524_handle_t *BOARD_GetPCAL6524Handle(void);
void BOARD_EnablePCAL6524Interrupt(void);

extern volatile bool g_pcal6524IntFlag;

#endif /* BOARD_USE_PCAL6524 */

#if defined(BOARD_USE_PCA9555) && BOARD_USE_PCA9555
void BOARD_PCA9555_I2C_Init(void);

/* Initialize a caller-owned PCA9555 handle. */
void BOARD_InitPCA9555(pca9555_handle_t *handle);

/* Lazy-initialized board-level singleton + ISR-ready helpers. Mirror of the
 * PCAL6524 pattern above. The MCU GPIO ISR is BOARD_PCA9555_INT_IRQ_HANDLER
 * (HSP_GPIO1_CH0_IRQHandler), weak, defaults to
 * PCA9555_InterruptHandler(singleton). */
pca9555_handle_t *BOARD_GetPCA9555Handle(void);
void BOARD_EnablePCA9555Interrupt(void);

#endif /* BOARD_USE_PCA9555 */

/* ELE API related macros */
#define RELEASE_RDC                                                        (0x17C40206u)
#define RELEASE_RDC_SIZE                                                   (0x2u)
#define RELEASE_RDC_RESPONSE_HDR                                           (0xE1C40206u)
#define ALL_RDC                                                            (0xAAu)
#define SHIFT_8                                                            (8u)
#define MSG_RESPONSE_MAX                                                   (16u)
#define RESPONSE_SUCCESS                                                   (0xd6u)
#define RESPONSE_ALREADY_GRANTED                                           (0xd329u)
#define MSG_TAG_RESP                                                       (0xE1u)


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
uint32_t BOARD_DebugConsoleSrcFreq(void);

void BOARD_InitDebugConsole(void);

/* Disable cache and MPU and clear all MPU regions; needed before whole MPU reconfiguration. */
void BOARD_ResetMPU(void);
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
void BOARD_Accel_I2C_Init(void);
status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff);
status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
void BOARD_Codec_I2C_Init(void);
status_t BOARD_Codec_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Codec_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
void BOARD_Camera_I2C_Init(void);
status_t BOARD_Camera_I2C_SendSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Camera_I2C_ReceiveSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#if defined(BOARD_TOUCH_I2C_BASEADDR)
void BOARD_PanelTouch_I2C_Init(void);
status_t BOARD_PanelTouch_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_PanelTouch_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#endif /* BOARD_TOUCH_I2C_BASEADDR */
#endif /* SDK_I2C_BASED_COMPONENT_USED */

void BOARD_SD_Pin_Config(uint32_t speed, uint32_t strength);
void BOARD_MMC_Pin_Config(uint32_t speed, uint32_t strength);
void BOARD_RequestTRDC(void);
void BOARD_CommonSetting(void);

/*! @brief Bring up an APS256XXN DDR Octal 16-bit PSRAM on the given XSPI.
 *
 * Configures XSPI1 pin mux (via BOARD_InitXSPI1PsRamPins), releases XSPI from
 * reset, sets up the DDR-mode controller, programs the APS256XXN LUT, and
 * switches the device into X16 mode. Ported from fpga_rt700/board.c.
 *
 * @param base MAIN__XSPI_0 or MAIN__XSPI_1. Pin mux is only applied for
 *             MAIN__XSPI_1 (that's the on-board PSRAM per MCUX-88783
 *             Arduino.xlsx APS256XXN pin table).
 */
void BOARD_Init16bitsPsRam(XSPI_Type *base);

/*! @brief Last-captured APS256XXN identity from BOARD_Init16bitsPsRam.
 *
 * BOARD_Init16bitsPsRam runs before BOARD_InitDebugConsole in most SDK
 * example bring-up sequences, so it cannot PRINTF directly. It reads
 * MR1 (Vendor ID) and MR2 (Density) and stashes them here for the app
 * to log later via BOARD_LogPsRamID(), or for a debugger to inspect.
 *
 * Zero until the first successful BOARD_Init16bitsPsRam(XSPI1).
 * Expected values per APS256XXN spec §7.7 Tables 9/12:
 *   MR1[4:0] = 0x0D (APM Vendor ID), MR2[2:0] = 0x7 (256 Mb Density);
 *   full bytes read 0x8D / 0xDF on this board.
 */
/*! @brief PSRAM init progress marker: 0 = global reset never completed,
 * 1 = global reset OK, 2 = warm-up read started, 3 = MR write started,
 * 4 = identity read done. */
extern volatile uint32_t g_boardPsRamInitStage;

extern volatile uint8_t g_boardPsRamMR1;
extern volatile uint8_t g_boardPsRamMR2;
extern volatile bool    g_boardPsRamIdOk;

/*! @brief Flush the last-captured PSRAM identity to the debug console.
 *
 * Call this from the application after BOARD_InitDebugConsole() to
 * print the MR1/MR2 bytes captured by BOARD_Init16bitsPsRam and whether
 * they matched the expected APS256XXN signature. Safe to call before
 * BOARD_Init16bitsPsRam has run (prints zeros with a mismatch tag).
 */
void BOARD_LogPsRamID(void);

void BOARD_EarlyConfigTCM(void);
void BOARD_EarlyConfigLLC(void);

void BOARD_ConfigMPU(void);
void BOARD_ConfigTRDC(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
