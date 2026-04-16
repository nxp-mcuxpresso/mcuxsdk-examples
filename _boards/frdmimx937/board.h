/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "fsl_clock.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "clock_config.h"
#if defined(BOARD_USE_PCAL6408) && BOARD_USE_PCAL6408
#include "fsl_pcal6408.h"
#endif
#if defined(BOARD_USE_PCA6416A) && BOARD_USE_PCA6416A
#include "fsl_pca6416a.h"
#endif
#include "fsl_iomuxc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "IMX952LPD5-EVK"

/*
 * M7's domain id, please find the domain id that assigned to M7
 * from system manager configuration.
 * From imx-sm/configs/mx952alt.cfg,
 * LM1                 name="Boot", rpc=scmi, boot=2, did=3, default
 *
 * "did=3" means that the domain id is 3.
 */
#ifndef SOC_M7_DOMAIN_ID
#define SOC_M7_DOMAIN_ID 3
#endif

/* for System Manager */

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
 * On iMX952 Emulator
 * +---------------------------------+
 * |   CM33    |  CM7        | CA55  |
 * -----------------------------------
 * |   1      |   2          | 3     |
 * +---------------------------------+
 *
 * On iMX952 LPDDR5 19x19 EVK
 * +---------------------------------+
 * |   CM33    |  CM7        | CA55  |
 * -----------------------------------
 * |   2      |   3          | 1     |
 * +---------------------------------+
 */

/* boad type: imx937 emulator or imx937 lpddr5 19x19 evk */
#define BOARD_DEBUG_UART_INSTANCE 3

#define BOARD_DEBUG_UART_INSTANCE_FOR_CM7_WITH_SM 3U
#define BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITH_SM kCLOCK_lpuart3
#define BOARD_DEBUG_UART_CLK_FREQ_FOR_CM7_WITH_SM   CLOCK_GetRate(BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITH_SM)

#define BOARD_DEBUG_UART_INSTANCE_FOR_CM7_WITHOUT_SM 3U
#define BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITHOUT_SM kCLOCK_lpuart3
#define BOARD_DEBUG_UART_CLK_FREQ_FOR_CM7_WITHOUT_SM   CLOCK_GetRate(BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITHOUT_SM)

#define BOARD_DEBUG_UART_CLOCK_ROOT kCLOCK_lpuart3

#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetRate(BOARD_DEBUG_UART_CLOCK_ROOT)
#define BOARD_DEBUG_UART_BAUDRATE   115200U
#define BOARD_DEBUG_UART_TYPE       kSerialPort_Uart

#define VDEV0_VRING_BASE (0x88000000U)
#define VDEV1_VRING_BASE (0x88010000U)

#define LED_INIT()
#define LED_TOGGLE()

#define BOARD_CODEC_I2C_BASEADDR LPI2C4
#define BOARD_CODEC_I2C_INSTANCE 4U


/* PCAL6408 */
#define BOARD_PCAL6408_I2C1            LPI2C1
#define BOARD_PCAL6408_I2C1_ADDR       (0x20U)
#define BOARD_PCAL6408_I2C1_CLOCK_ROOT kCLOCK_lpi2c1
#define BOARD_PCAL6408_I2C1_CLOCK_FREQ CLOCK_GetRate(BOARD_PCAL6408_I2C1_CLOCK_ROOT)

#define BOARD_PCAL6408_I2C3            LPI2C3
#define BOARD_PCAL6408_I2C3_ADDR       (0x20U)
#define BOARD_PCAL6408_I2C3_CLOCK_ROOT kCLOCK_lpi2c3
#define BOARD_PCAL6408_I2C3_CLOCK_FREQ CLOCK_GetRate(BOARD_PCAL6408_I2C3_CLOCK_ROOT)


/* PCA6416A (U14) */
#define BOARD_PCA6416A_I2C            LPI2C2
#define BOARD_PCA6416A_I2C_ADDR       (0x21U)
#define BOARD_PCA6416A_I2C_CLOCK_ROOT kCLOCK_lpi2c2
#define BOARD_PCA6416A_I2C_CLOCK_FREQ CLOCK_GetRate(BOARD_PCA6416A_I2C_CLOCK_ROOT)

/* PCA6416 pin index mapping: P0_[0..7] => 0..7, P1_[0..7] => 8..15 */

#define BOARD_PCA6416A_ENET1_N_RST        (0U)
#define BOARD_PCA6416A_ENET2_N_RST        (1U)
#define BOARD_PCA6416A_CH_SPI_SEL         (2U)
#define BOARD_PCA6416A_CH_UART_SEL        (3U)
#define BOARD_PCA6416A_CH_CAN_SEL         (4U)
#define BOARD_PCA6416A_CH_PWM_SEL         (5U)
#define BOARD_PCA6416A_CAN_STBY           (6U)
#define BOARD_PCA6416A_MKEY_PWRDIS        (7U)

#define BOARD_PCA6416A_EKEY_SDIO_N_RST    (8U + 0U)
#define BOARD_PCA6416A_EKEY_WL_NDIS1      (8U + 1U)
#define BOARD_PCA6416A_EKEY_WL_NDIS2      (8U + 2U)
#define BOARD_PCA6416A_EKEY_PWR_EN        (8U + 3U)
#define BOARD_PCA6416A_EXT_5V0_PWR_EN     (8U + 4U)
#define BOARD_PCA6416A_EXT_3V3_PWR_EN     (8U + 5U)
#define BOARD_PCA6416A_MKEY_PWR_EN        (8U + 6U)
#define BOARD_PCA6416A_USB2_PWR_EN        (8U + 7U)


/* For i2c release bus */
#define I2C_MAX_INSTANCE_NUM 5
#define I2C1_RELEASE_SDA_GPIO  GPIO1
#define I2C1_RELEASE_SDA_PIN   1U
#define I2C1_RELEASE_SCL_GPIO  GPIO1
#define I2C1_RELEASE_SCL_PIN   0U
#define I2C1_RELEASE_SCL_PAD_USE_AS_GPIO_ARGS IOMUXC_PAD_I2C1_SCL__GPIO1_IO_0
#define I2C1_RELEASE_SDA_PAD_USE_AS_GPIO_ARGS IOMUXC_PAD_I2C1_SDA__GPIO1_IO_1
#define I2C1_RELEASE_SCL_PAD_USE_AS_SCL_ARGS IOMUXC_PAD_I2C1_SCL__LPI2C1_SCL
#define I2C1_RELEASE_SDA_PAD_USE_AS_SDA_ARGS IOMUXC_PAD_I2C1_SDA__LPI2C1_SDA

#define I2C2_RELEASE_SDA_GPIO  GPIO1
#define I2C2_RELEASE_SDA_PIN   3U
#define I2C2_RELEASE_SCL_GPIO  GPIO1
#define I2C2_RELEASE_SCL_PIN   2U
#define I2C2_RELEASE_SCL_PAD_USE_AS_GPIO_ARGS IOMUXC_PAD_I2C2_SCL__GPIO1_IO_2
#define I2C2_RELEASE_SDA_PAD_USE_AS_GPIO_ARGS IOMUXC_PAD_I2C2_SDA__GPIO1_IO_3
#define I2C2_RELEASE_SCL_PAD_USE_AS_SCL_ARGS IOMUXC_PAD_I2C2_SCL__LPI2C2_SCL
#define I2C2_RELEASE_SDA_PAD_USE_AS_SDA_ARGS IOMUXC_PAD_I2C2_SDA__LPI2C2_SDA

#define I2C3_RELEASE_SDA_GPIO  GPIO2
#define I2C3_RELEASE_SDA_PIN   0U
#define I2C3_RELEASE_SCL_GPIO  GPIO2
#define I2C3_RELEASE_SCL_PIN   1U
#define I2C3_RELEASE_SCL_PAD_USE_AS_GPIO_ARGS IOMUXC_PAD_GPIO_IO01__GPIO2_IO_1
#define I2C3_RELEASE_SDA_PAD_USE_AS_GPIO_ARGS IOMUXC_PAD_GPIO_IO00__GPIO2_IO_0
#define I2C3_RELEASE_SCL_PAD_USE_AS_SCL_ARGS IOMUXC_PAD_GPIO_IO01__LPI2C3_SCL
#define I2C3_RELEASE_SDA_PAD_USE_AS_SDA_ARGS IOMUXC_PAD_GPIO_IO00__LPI2C3_SDA

#define I2C4_RELEASE_SDA_GPIO  GPIO2
#define I2C4_RELEASE_SDA_PIN   30U
#define I2C4_RELEASE_SCL_GPIO  GPIO2
#define I2C4_RELEASE_SCL_PIN   31U
#define I2C4_RELEASE_SCL_PAD_USE_AS_GPIO_ARGS IOMUXC_PAD_GPIO_IO31__GPIO2_IO_31
#define I2C4_RELEASE_SDA_PAD_USE_AS_GPIO_ARGS IOMUXC_PAD_GPIO_IO30__GPIO2_IO_30
#define I2C4_RELEASE_SCL_PAD_USE_AS_SCL_ARGS IOMUXC_PAD_GPIO_IO31__LPI2C4_SCL
#define I2C4_RELEASE_SDA_PAD_USE_AS_SDA_ARGS IOMUXC_PAD_GPIO_IO30__LPI2C4_SDA
#ifdef I2C_RETRY_TIMES
#define I2C_RELEASE_BUS_COUNT I2C_RETRY_TIMES
#else
#define I2C_RELEASE_BUS_COUNT 40000U
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
void BOARD_InitDebugConsole(void);
static inline void BOARD_SetAudmixMode(uint32_t val)
{
    if (SCMI_ERR_SUCCESS != SCMI_MiscControlSet(SCMI_A2P, 8U, 1U, &val))
    {
        PRINTF("\r\n Fail to set AUDMIX to bypass mode.");
        assert(false);
    }
}

static inline void BOARD_ModifyMQSSettings(uint32_t clearMask, uint32_t setValue)
{
    uint32_t regValue;
    uint32_t ctrlId = 1U;
    uint32_t numVal = 1U;
    
    if (SCMI_ERR_SUCCESS != SCMI_MiscControlGet(SCMI_A2P, ctrlId, &numVal, &regValue))
    {
        PRINTF("\r\n Fail to get MQS_SETTINGS.");
        assert(false);
    }
    
    regValue &= ~clearMask;
    regValue |= setValue;
    
    if (SCMI_ERR_SUCCESS != SCMI_MiscControlSet(SCMI_A2P, ctrlId, numVal, &regValue))
    {
        PRINTF("\r\n Fail to set MQS_SETTINGS.");
        assert(false);
    }
}
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
status_t BOARD_PCA6416A_I2C_Send(void *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 const uint8_t *txBuff,
                                 uint8_t txBuffSize,
                                 uint32_t flags);
status_t BOARD_PCA6416A_I2C_Receive(void *base,
                                    uint8_t deviceAddress,
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

#define BOARD_ENABLE_ELE 1
#define SOC_ELE_MU_INST_BASE (ELE_MU5_MUA)
void BOARD_ShowEleFwInfo(void);

void BOARD_ConfigMPU(void);

/* MQS init helper used by examples/driver_examples/sai/mqs_sai */
void BOARD_InitMQS(uint32_t clkDiv);
status_t BOARD_I2C_ReleaseBus(int32_t i2cInstIdx);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* _BOARD_H_ */
