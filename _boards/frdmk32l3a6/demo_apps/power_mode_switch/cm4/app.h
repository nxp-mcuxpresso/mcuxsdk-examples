/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define APP_SMC SMC0

#define APP_MU                        MUA
#define APP_CORE1_BOOT_MODE           kMU_CoreBootFromDflashBase
#define APP_MU_IRQHandler             MUA_IRQHandler
#define APP_MU_XFER_CMD_CHANNEL_INDEX 0U
#define APP_MU_SHAKE_HAND_VALUE       0xAAAAAAAA

#define APP_LLWU LLWU0
#define APP_LLWU_LPTMR_IDX                                                               \
    0U /* According to Table 4-11. Wakeup Sources for LLWU0 inputs, LLWU_M4DR - LPTMR0 \ \
          Trigger*/
#define APP_LLWU_IRQHandler LLWU0_IRQHandler

#define APP_LPTMR            LPTMR0
#define APP_LPTMR_IRQn       LPTMR0_IRQn
#define APP_LPTMR_IRQHandler LPTMR0_IRQHandler

#define FLASH_SPI_MASTER          LPSPI1
#define FLASH_SPI_MASTER_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_Lpspi1)

#define DPCommand 0xB9

/* Power mode definition used in application. */
typedef enum _app_power_mode
{
    kAPP_PowerModeMin = 'A' - 1,
    kAPP_PowerModeRun,   /* Normal RUN mode */
    kAPP_PowerModeWait,  /* WAIT mode. */
    kAPP_PowerModeStop,  /* STOP mode. */
    kAPP_PowerModeVlpr,  /* VLPR mode. */
    kAPP_PowerModeVlpw,  /* VLPW mode. */
    kAPP_PowerModeVlps,  /* VLPS mode. */
    kAPP_PowerModeLls,   /* LLS mode. */
    kAPP_PowerModeVlls2, /* VLLS2 mode. */
    kAPP_PowerModeVlls0, /* VLLS0 mode. */
    kAPP_PowerModeHsrun, /* HSRUN mode. */
    kAPP_PowerModeMax
} app_power_mode_t;

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
