/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SDMMC_CONFIG_H_
#define _SDMMC_CONFIG_H_

#include "fsl_common.h"

#ifdef SD_ENABLED
#include "fsl_sd.h"
#endif
#ifdef MMC_ENABLED
#include "fsl_mmc.h"
#endif
#ifdef SDIO_ENABLED
#include "fsl_sdio.h"
#endif
#include "clock_config.h"
#include "fsl_sdmmc_host.h"
#include "fsl_sdmmc_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* @brief host basic configuration
 *
 * The uSD slot on MIMXRT2660-EVK is wired to USDHC0 (CMD/CLK/DAT0-3 on
 * PIO7_6..PIO7_11; VSELECT on PIO6_11). Board-level nets are labelled
 * SD1_*, but the controller is USDHC0 in chip nomenclature.
 */
#define BOARD_SDMMC_SD_HOST_BASEADDR   COMM__USDHC_0
#define BOARD_SDMMC_SD_HOST_IRQ        COMM_USDHC0_IRQn
#define BOARD_SDMMC_MMC_HOST_BASEADDR  COMM__USDHC_0
#define BOARD_SDMMC_MMC_HOST_IRQ       COMM_USDHC0_IRQn
/* WiFi M.2 SDIO is on USDHC1 (not the uSD slot's USDHC0). */
#define BOARD_SDMMC_SDIO_HOST_BASEADDR COMM__USDHC_1
#define BOARD_SDMMC_SDIO_HOST_IRQ      COMM_USDHC1_IRQn

/* @brief card detect configuration
 *
 * Card detect on MIMXRT2660-EVK is routed through the PCA9555 I2C IO
 * expander pin P0_4 (active-low: 0 = card present). The PCA9555 INT line
 * (EXP_nIRQ2) is wired to PIO3_29 and fires on any input change; the read
 * itself is on-demand via PCA9555_ReadPins inside sdmmc_config.c. The
 * cd type is GPIO-style — the PCA9555 read is treated as a logical GPIO.
 */
#ifndef BOARD_SDMMC_SD_CD_TYPE
#define BOARD_SDMMC_SD_CD_TYPE kSD_DetectCardByGpioCD
#endif
#define BOARD_SDMMC_SD_CARD_DETECT_DEBOUNCE_DELAY_MS (100U)
#define BOARD_SDMMC_SD_CD_INSERT_LEVEL               (0U)
#define BOARD_SDMMC_SD_CD_PCA9555_PIN                (4U)  /* P0_4 */

/*! @brief SD power control
 *
 * Power gating (SD_PWREN) on MIMXRT2660-EVK drives an NX3P1100 load switch
 * through the PCAL6524 IO expander pin P2_5 (active-high: HIGH = card
 * powered). Handled by BOARD_SDCardPowerControl in sdmmc_config.c.
 */

/*! @brief SD IO voltage
 *
 * VSELECT on MIMXRT2660-EVK is driven as a plain GPIO (PIO6_11 muxed to
 * HSP_GPIO4_11 in BOARD_InitUSDHC0Pins). Software toggles it via
 * BOARD_SDCardIoVoltageControl: LOW = 3.3V, HIGH = 1.8V.
 */
#define BOARD_SDMMC_SD_IO_VOLTAGE_CONTROL_TYPE kSD_IOVoltageCtrlByGpio
#define BOARD_SDMMC_SD_VSELECT_GPIO            HSP__GPIO_4
#define BOARD_SDMMC_SD_VSELECT_GPIO_PIN        (11U)

#define BOARD_SDMMC_SD_HOST_SUPPORT_SDR104_FREQ (200000000U / 2U)
#define BOARD_SDMMC_MMC_HOST_SUPPORT_HS200_FREQ (200000000U)
/*! @brief mmc configuration */
#define BOARD_SDMMC_MMC_VCC_SUPPLY  kMMC_VoltageWindows270to360
#define BOARD_SDMMC_MMC_VCCQ_SUPPLY kMMC_VoltageWindows270to360
/*! @brief align with cache line size */
#define BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE       (32U)
#define BOARD_SDMMC_MMC_SUPPORT_8_BIT_DATA_WIDTH 1U
#define BOARD_SDMMC_MMC_TUNING_TYPE              0
/*!@ brief host interrupt priority*/
#define BOARD_SDMMC_SD_HOST_IRQ_PRIORITY   (5U)
#define BOARD_SDMMC_MMC_HOST_IRQ_PRIORITY  (5U)
#define BOARD_SDMMC_SDIO_HOST_IRQ_PRIORITY (5U)
/*!@brief dma descriptor buffer size
 * As DTCM is on AHB bus, there is 1KB boundary access limitation.
 * To avoid DMA access across 1KB boundary, use 64 bytes alignment
 * which is same with USDHC DMA burst length used.
 */
#define BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE (64U)
/*! @brief cache maintain function enabled for RW buffer */
#define BOARD_SDMMC_HOST_CACHE_CONTROL kSDMMCHOST_CacheControlRWBuffer

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief BOARD SD configurations.
 */
#ifdef SD_ENABLED
void BOARD_SD_Config(void *card, sd_cd_t cd, uint32_t hostIRQPriority, void *userData);
#endif

/*!
 * @brief BOARD SDIO configurations.
 */
#ifdef SDIO_ENABLED
void BOARD_SDIO_Config(void *card, sd_cd_t cd, uint32_t hostIRQPriority, sdio_int_t cardInt);
#endif

/*!
 * @brief BOARD MMC configurations.
 */
#ifdef MMC_ENABLED
void BOARD_MMC_Config(void *card, uint32_t hostIRQPriority);
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _SDMMC_CONFIG_H_ */
