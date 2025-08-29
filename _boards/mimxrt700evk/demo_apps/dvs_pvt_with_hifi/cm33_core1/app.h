/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "pvt_app_config.h"
#include "fsl_utick.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#ifndef DEMO_POWER_CPU1_PRINT_ENABLE
#define DEMO_POWER_CPU1_PRINT_ENABLE 1U
#endif
#if DEMO_POWER_CPU1_PRINT_ENABLE
#define DEMO_LOG PRINTF
#else
#define DEMO_LOG(...)
#endif

#define APP_CORE_NAME "CPU1"
#define APP_MU MU1_MUB

#define APP_USART_RX_ERROR (kLPUART_RxOverrunFlag | kLPUART_RxFifoUnderflowFlag)

/*!< Power down all unnecessary blocks and enable RBB during deep sleep. */
#define APP_DEEPSLEEP_SLEEPCFG    (0U)        /* SLEEPCON->SLEEPCFG */
#define APP_DEEPSLEEP_PDSLEEPCFG0 (0U)        /* PMC->PDSLEEPCFG0 */
#define APP_DEEPSLEEP_RAM_APD     0x3FFC0000U /* PMC->PDSLEEPCFG2, 0x580000 - 0x67FFFF([PT18-PT29]) keep powered */
#define APP_DEEPSLEEP_RAM_PPD     0x0U        /* PMC->PDSLEEPCFG3 */
#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                     \
    (((const uint32_t[]){APP_DEEPSLEEP_SLEEPCFG, APP_DEEPSLEEP_PDSLEEPCFG0, 0U, APP_DEEPSLEEP_RAM_APD, \
                         APP_DEEPSLEEP_RAM_PPD, 0U, 0U}))
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitPowerConfig(void);
void BOARD_RunActiveTest(void);
void BOARD_RestorePeripheralsAfterDSR(void);
void BOARD_EnterSleep(void);
void BOARD_EnterDeepSleep(const uint32_t exclude_from_pd[7]);
void BOARD_NotifyBoot(void);
void POWER_SramDummyRead(uint32_t pt);
void BOARD_BootDSP(void);

/* Init the wait timer used for PVT task to periodic check whether the supply can go lower. */
void pvt_wait_timer_init(void);
void pvt_wait_timer_start(uint32_t ms, utick_callback_t cb);
/*${prototype:end}*/

#endif /* _APP_H_ */
