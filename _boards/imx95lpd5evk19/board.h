/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "hal_clock.h"
#include "clock_config.h"
#if defined(BOARD_USE_ADP5585) && BOARD_USE_ADP5585
#include "fsl_adp5585.h"
#endif
#if defined(BOARD_USE_PCAL6524) && BOARD_USE_PCAL6524
#include "fsl_pcal6524.h"
#endif
#if defined(BOARD_USE_PCAL6408) && BOARD_USE_PCAL6408
#include "fsl_pcal6408.h"
#endif
#if defined(BOARD_USE_PCA6416A) && BOARD_USE_PCA6416A
#include "fsl_pca6416a.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "IMX95LPD5-EVK"
/* for System Manager */

/* sync with 'devices/MIMX95/drivers/fsl_clock.h' of imx-sm-sandbox */
#define CLOCK_NUM_SRC 41UL

/*! Number of device pins */
#define DEV_SM_NUM_PIN    108U
/*! Number of device daisy */
#define DEV_SM_NUM_DAISY  68U
#define SCMI_IOMUXC_BASE AON__IOMUXC0__IOMUXC_BASE
#define SCMI_IOMUXC_CFGREG_OFF (SCMI_IOMUXC_BASE + (DEV_SM_NUM_PIN * 4U))
#define SCMI_IOMUXC_DAISYREG_OFF (SCMI_IOMUXC_BASE + (DEV_SM_NUM_PIN * 8U))

#define SCMI_LMID_M7 1
#define SCMI_LMID_A55 2
#define SCMI_MU_SEL     8

#define SCMI_A2P    0
#define SCMI_NOTIFY 1
#define SCMI_P2A    2
/* Doorbell interrupt request number */
#define SCMI_DBIR_A2P    0    /* 0: A2P channel */
#define SCMI_DBIR_NOTIFY 1    /* 1: P2A notification channel */
#define SCMI_DBIR_P2A    3    /* 2: FuSa reverse channel 3: P2A channel */
#define SCMI_SMA_ADDR    0    /* share memory address. 0: Use of internal MU SRAM */

#if (SCMI_DBIR_NOTIFY == 1)
#define MU_NOTIFY_GENINT_EN(base)  \
    MU_EnableInterrupts(base, kMU_GenInt1InterruptEnable)
#define MU_NOTIFY_GENINT_FLAG kMU_GenInt1Flag
#endif

#if (SCMI_DBIR_P2A == 3)
#define MU_P2A_GENINT_EN(base)  \
    MU_EnableInterrupts(base, kMU_GenInt3InterruptEnable)
#define MU_P2A_GENINT_FLAG kMU_GenInt1Flag
#endif

/*
 * The UART instance to be used for debugging.
 * On iMX95 Emulator
 * +---------------------------------+
 * |   CM33    |  CM7        | CA55  |
 * -----------------------------------
 * |   1      |   2          | 3     |
 * +---------------------------------+
 *
 * On iMX95 LPDDR5 19x19 EVK
 * +---------------------------------+
 * |   CM33    |  CM7        | CA55  |
 * -----------------------------------
 * |   2      |   3          | 1     |
 * +---------------------------------+
 */

/* boad type: imx95 emulator or imx95 lpddr5 19x19 evk */
#define BOARD_IMX95_EMULATOR 1
#define BOARD_IMX95_LPDDR5_19X19_EVK 2
#define BOARD_IMX95_TYPE BOARD_IMX95_LPDDR5_19X19_EVK
#if BOARD_IMX95_TYPE == BOARD_IMX95_EMULATOR
#if defined(CPU_MIMX9596AVZXN_cm33)
#define BOARD_DEBUG_UART_INSTANCE 1
#elif defined(CPU_MIMX9596AVZXN_cm7)
#define BOARD_DEBUG_UART_INSTANCE 2
#endif
#elif BOARD_IMX95_TYPE == BOARD_IMX95_LPDDR5_19X19_EVK
#if defined(CPU_MIMX9596AVZXN_cm33)
#define BOARD_DEBUG_UART_INSTANCE 2
#elif defined(CPU_MIMX9596AVZXN_cm7)
#define BOARD_DEBUG_UART_INSTANCE 3
#endif
#endif

#if BOARD_IMX95_TYPE == BOARD_IMX95_EMULATOR
#define BOARD_DEBUG_UART_INSTANCE_FOR_CM7_WITH_SM 2U
#define BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITH_SM hal_clock_lpuart2
#define BOARD_DEBUG_UART_CLK_FREQ_FOR_CM7_WITH_SM   HAL_ClockGetIpFreq(BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITH_SM)

#define BOARD_DEBUG_UART_INSTANCE_FOR_CM7_WITHOUT_SM 2U
#define BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITHOUT_SM hal_clock_lpuart2
#define BOARD_DEBUG_UART_CLK_FREQ_FOR_CM7_WITHOUT_SM   HAL_ClockGetIpFreq(BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITHOUT_SM)

#define BOARD_DEBUG_UART_INSTANCE_FOR_CM33_WITHOUT_SM 1U
#define BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM33_WITHOUT_SM hal_clock_lpuart1
#define BOARD_DEBUG_UART_CLK_FREQ_FOR_CM33_WITHOUT_SM   HAL_ClockGetIpFreq(BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM33_WITHOUT_SM)
#elif BOARD_IMX95_TYPE == BOARD_IMX95_LPDDR5_19X19_EVK
#define BOARD_DEBUG_UART_INSTANCE_FOR_CM7_WITH_SM 3U
#define BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITH_SM hal_clock_lpuart3
#define BOARD_DEBUG_UART_CLK_FREQ_FOR_CM7_WITH_SM   HAL_ClockGetIpFreq(BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITH_SM)

#define BOARD_DEBUG_UART_INSTANCE_FOR_CM7_WITHOUT_SM 3U
#define BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITHOUT_SM hal_clock_lpuart3
#define BOARD_DEBUG_UART_CLK_FREQ_FOR_CM7_WITHOUT_SM   HAL_ClockGetIpFreq(BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITHOUT_SM)

#define BOARD_DEBUG_UART_INSTANCE_FOR_CM33_WITHOUT_SM 2U
#define BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM33_WITHOUT_SM hal_clock_lpuart2
#define BOARD_DEBUG_UART_CLK_FREQ_FOR_CM33_WITHOUT_SM   HAL_ClockGetIpFreq(BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM33_WITHOUT_SM)
#endif

#if BOARD_DEBUG_UART_INSTANCE == 1
#define BOARD_DEBUG_UART_CLOCK_ROOT hal_clock_lpuart1
#elif BOARD_DEBUG_UART_INSTANCE == 2
#define BOARD_DEBUG_UART_CLOCK_ROOT hal_clock_lpuart2
#elif BOARD_DEBUG_UART_INSTANCE == 3
#define BOARD_DEBUG_UART_CLOCK_ROOT hal_clock_lpuart3
#endif

#define BOARD_DEBUG_UART_CLK_FREQ HAL_ClockGetIpFreq(BOARD_DEBUG_UART_CLOCK_ROOT)
#define BOARD_DEBUG_UART_BAUDRATE   115200U
#define BOARD_DEBUG_UART_TYPE       kSerialPort_Uart

#define VDEV0_VRING_BASE (0x88000000U)
#define VDEV1_VRING_BASE (0x88010000U)

#define LED_INIT()
#define LED_TOGGLE()

#define BOARD_CODEC_I2C_BASEADDR LPI2C4
#define BOARD_CODEC_I2C_INSTANCE 4U

/* ADP5585 */
#define BOARD_ADP5585_I2C            LPI2C2
#define BOARD_ADP5585_I2C_ADDR       (0x34U)
#define BOARD_ADP5585_I2C_CLOCK_ROOT hal_clock_lpi2c2
#define BOARD_ADP5585_I2C_CLOCK_FREQ HAL_ClockGetIpFreq(BOARD_ADP5585_I2C_CLOCK_ROOT)
/* hal api and hal arguments */
#define BOARD_ADP5585_I2C_CLOCK_ROOT_HAL hal_clock_lpi2c2
#define BOARD_ADP5585_I2C_CLOCK_FREQ_HAL HAL_ClockGetIpFreq(BOARD_ADP5585_I2C_CLOCK_ROOT_HAL)

#define BOARD_ADP5585_PDM_MQS_SEL (2U)
#define BOARD_ADP5585_EXP_SEL     (4U)
#define BOARD_ADP5585_CAN_STBY    (8U)

/* PCAL6524 */
#define BOARD_PCAL6524_I2C            LPI2C7
#define BOARD_PCAL6524_I2C_ADDR       (0x22U)
#define BOARD_PCAL6524_I2C_CLOCK_ROOT hal_clock_lpi2c7
#define BOARD_PCAL6524_I2C_CLOCK_FREQ HAL_ClockGetIpFreq(BOARD_PCAL6524_I2C_CLOCK_ROOT)

#define BOARD_PCAL6524_SI5332_RST     (0x8U + 0x5U)
#define BOARD_PCAL6524_USB2_PWR_EN    (0x3U)

/* PCAL6408 */
#define BOARD_PCAL6408_I2C1            LPI2C1
#define BOARD_PCAL6408_I2C1_ADDR       (0x20U)
#define BOARD_PCAL6408_I2C1_CLOCK_ROOT hal_clock_lpi2c1
#define BOARD_PCAL6408_I2C1_CLOCK_FREQ HAL_ClockGetIpFreq(BOARD_PCAL6408_I2C1_CLOCK_ROOT)

#define BOARD_PCAL6408_I2C3            LPI2C3
#define BOARD_PCAL6408_I2C3_ADDR       (0x20U)
#define BOARD_PCAL6408_I2C3_CLOCK_ROOT hal_clock_lpi2c3
#define BOARD_PCAL6408_I2C3_CLOCK_FREQ HAL_ClockGetIpFreq(BOARD_PCAL6408_I2C3_CLOCK_ROOT)

/* output for BOARD_PCAL6408_I2C3 */
#define BOARD_PCAL6408_CSI1_SYNC         (0U)
#define BOARD_PCAL6408_CSI1_PWDN         (1U)
#define BOARD_PCAL6408_CSI1_RST_B        (2U)
#define BOARD_PCAL6408_ETH_CLK_EN        (3U)

#define BOARD_PCAL6408_I2C4            LPI2C4
#define BOARD_PCAL6408_I2C4_ADDR       (0x21U)
#define BOARD_PCAL6408_I2C4_CLOCK_ROOT hal_clock_lpi2c4
#define BOARD_PCAL6408_I2C4_CLOCK_FREQ HAL_ClockGetIpFreq(BOARD_PCAL6408_I2C4_CLOCK_ROOT)

/* output for BOARD_PCAL6408_I2C4 */
#define BOARD_PCAL6408_SLOT_SAI3_SEL     (7U)

#define BOARD_PCAL6408_I2C5            LPI2C5
#define BOARD_PCAL6408_I2C5_ADDR       (0x21U)
#define BOARD_PCAL6408_I2C5_CLOCK_ROOT hal_clock_lpi2c5
#define BOARD_PCAL6408_I2C5_CLOCK_FREQ HAL_ClockGetIpFreq(BOARD_PCAL6408_I2C5_CLOCK_ROOT)

/* output for BOARD_PCAL6408_I2C5 */
#define BOARD_PCAL6408_AQR_PWR_EN        (0U)
#define BOARD_PCAL6408_AQR113C_RST_B_3V3 (1U)
#define BOARD_PCAL6408_ENET1_RST_B       (2U)
#define BOARD_PCAL6408_ETH_CLK_EN        (3U)

/* PCA6416A (U27) */
#define BOARD_PCA6416A_I2C            LPI2C6
#define BOARD_PCA6416A_I2C_ADDR       (0x21U)
#define BOARD_PCA6416A_I2C_CLOCK_ROOT hal_clock_lpi2c6
#define BOARD_PCA6416A_I2C_CLOCK_FREQ HAL_ClockGetIpFreq(BOARD_PCA6416A_I2C_CLOCK_ROOT)

#define BOARD_PCA6416A_CAN1_STBY_B     5U
#define BOARD_PCA6416A_CAN1_EN         6U
#define BOARD_PCA6416A_CAN1_WAKE       7U

#define BOARD_PCA6416A_PDM_CAN_SEL     (8U + 2U)

/* hal api and hal arguments */
#define BOARD_PCAL6524_I2C_CLOCK_ROOT_HAL hal_clock_lpi2c2
#define BOARD_PCAL6524_I2C_CLOCK_FREQ_HAL HAL_ClockGetIpFreq(BOARD_PCAL6524_I2C_CLOCK_ROOT_HAL)
#define BOARD_PCAL6524_I2C_CLOCK_GATE kCLOCK_Lpi2c2

#define BOARD_PCAL6524_EXT1_PWREN  (8U + 5U)
#define BOARD_PCAL6524_ENET1_NRST  (8U + 7U)
#define BOARD_PCAL6524_ENET2_NRST  (16U + 0U)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
void BOARD_InitDebugConsoleForCM7WithSM(void);
void BOARD_InitDebugConsoleForCM7WithoutSM(void);
void BOARD_InitDebugConsole(void);
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz);
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
status_t BOARD_Display_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Display_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#if defined(BOARD_USE_ADP5585) && BOARD_USE_ADP5585
void BOARD_ADP5585_I2C_Init(void);
status_t BOARD_ADP5585_I2C_Send(uint8_t deviceAddress,
                                uint32_t subAddress,
                                uint8_t subAddressSize,
                                const uint8_t *txBuff,
                                uint8_t txBuffSize,
                                uint32_t flags);
status_t BOARD_ADP5585_I2C_Receive(uint8_t deviceAddress,
                                   uint32_t subAddress,
                                   uint8_t subAddressSize,
                                   uint8_t *rxBuff,
                                   uint8_t rxBuffSize,
                                   uint32_t flags);

void BOARD_InitADP5585(adp5585_handle_t *handle);
#endif /* BOARD_USE_ADP5585 */

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
#endif /* BOARD_USE_PCAL6524 */

#if defined(BOARD_USE_PCAL6408) && BOARD_USE_PCAL6408
void BOARD_PCAL6408_I2C_Init(void *base, uint32_t clkSrc_Hz);
status_t BOARD_PCAL6408_I2C_Send(void *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 const uint8_t *txBuff,
                                 uint8_t txBuffSize,
                                 uint32_t flags);
status_t BOARD_PCAL6408_I2C_Receive(void *base,
                                    uint8_t deviceAddress,
                                    uint32_t subAddress,
                                    uint8_t subAddressSize,
                                    uint8_t *rxBuff,
                                    uint8_t rxBuffSize,
                                    uint32_t flags);
void BOARD_InitPCAL6408_I2C1(pcal6408_handle_t *handle);
void BOARD_InitPCAL6408_I2C3(pcal6408_handle_t *handle);
void BOARD_InitPCAL6408_I2C4(pcal6408_handle_t *handle);
void BOARD_InitPCAL6408_I2C5(pcal6408_handle_t *handle);
#endif /* BOARD_USE_PCAL6408 */

#if defined(BOARD_USE_PCA6416A) && BOARD_USE_PCA6416A
void BOARD_PCA6416A_I2C_Init(void);
status_t BOARD_PCA6416A_I2C_Send(uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 const uint8_t *txBuff,
                                 uint8_t txBuffSize,
                                 uint32_t flags);
status_t BOARD_PCA6416A_I2C_Receive(uint8_t deviceAddress,
                                    uint32_t subAddress,
                                    uint8_t subAddressSize,
                                    uint8_t *rxBuff,
                                    uint8_t rxBuffSize,
                                    uint32_t flags);
void BOARD_InitPCA6416A(pca6416a_handle_t *handle);
#endif /* BOARD_USE_PCA6416A */

#endif

/* Mcore suspend setting */
#define OSCPLL_LPM_START 3U
#define OSCPLL_LPM_END   12U
/* SysTick Control and Status Register(SYST_CSR) of cortex-M33 */
#define SYSTICK_CTRL_REG (*((volatile uint32_t *)0xe000e010))
/*
 * SYST_CSR[0] ENABLE(Enable counter):
 * 0: Counter Disabled
 * 1: Counter Enabled
 * SYST_CSR[1] TICKINT(Enables SysTick exception request):
 * 0: Counting down to zero does not assert the SysTick exception request.
 * 1: Counting down to zero asserts the SysTick exception request.
 * More info from
 * https://developer.arm.com/documentation/100235/0004/the-cortex-m33-peripherals/system-timer--systick/systick-control-and-status-register
 */
#define SYSTICK_CLOSE 0U
void BOARD_McoreSUSPEND(void);

void BOARD_ConfigMPU(void);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* _BOARD_H_ */
