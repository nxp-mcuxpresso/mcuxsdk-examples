/*
 * Copyright 2025 NXP
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
#ifndef DEMO_POWER_CPU0_PRINT_ENABLE
#define DEMO_POWER_CPU0_PRINT_ENABLE 1U
#endif
#if DEMO_POWER_CPU0_PRINT_ENABLE
#define DEMO_LOG PRINTF
#else
#define DEMO_LOG(...)
#endif

#define APP_CORE_NAME            "CPU0"
#define BOOT_FLAG                0x1U /* Flag indicates Core1 Boot Up*/

#define APP_MU MU1_MUA

#define APP_USART_RX_ERROR (kLPUART_RxOverrunFlag | kLPUART_RxFifoUnderflowFlag)

#define APP_LPADC_BASE                ADC0
#define APP_LPADC_IRQn                ADC_IRQn
#define APP_LPADC_IRQ_HANDLER_FUNC    ADC_IRQHandler
#define APP_LPADC_USER_CHANNEL        6U
#define APP_LPADC_USER_CMDID          1U /* The available command number are 1-15 */
#define APP_LPADC_USE_HIGH_RESOLUTION 1U
#define TSENS_CAL_OTP_FUSE_INDEX \
    77U /* TSENS_CAL is an 8-bit signed calibration constant retrieved from non-volatile memory.*/

/***********************Example configuration****************************/
/*!< Power down all unnecessary blocks and enable RBB during deep sleep. */
#define APP_DEEPSLEEP_SLEEPCFG    (0U)     /* SLEEPCON->SLEEPCFG */
#define APP_DEEPSLEEP_PDSLEEPCFG0 (0U)     /* PMC->PDSLEEPCFG0 */
#define APP_DEEPSLEEP_PDSLEEPCFG1 (0U)     /* PMC->PDSLEEPCFG1 */
#define APP_DSR_PDSLEEPCFG0       (0U)     /* PMC->PDSLEEPCFG0 */
#define APP_DEEPSLEEP_RAM_APD     0x3FFFFU /* PMC->PDSLEEPCFG2, all keep powered, the unused PT can be power off. */
#define APP_DEEPSLEEP_RAM_PPD     (0U)     /* PMC->PDSLEEPCFG3 */
#define APP_DEEPSLEEP_PDSLEEPCFG4 \
    (PMC_PDSLEEPCFG4_CPU0_CCACHE_MASK | PMC_PDSLEEPCFG4_CPU0_SCACHE_MASK | PMC_PDSLEEPCFG4_OCOTP_MASK)
#define APP_DEEPSLEEP_PDSLEEPCFG5 (0U)
#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                     \
    (((const uint32_t[]){APP_DEEPSLEEP_SLEEPCFG, APP_DEEPSLEEP_PDSLEEPCFG0, APP_DEEPSLEEP_PDSLEEPCFG1, \
                         APP_DEEPSLEEP_RAM_APD, APP_DEEPSLEEP_RAM_PPD, APP_DEEPSLEEP_PDSLEEPCFG4,      \
                         APP_DEEPSLEEP_PDSLEEPCFG5}))
/************************************************************************/
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void DEMO_InitDebugConsole(void);
void DEMO_DeinitDebugConsole(void);
void DEMO_BootCore1(void);
void BOARD_EnterSleep(void);
void BOARD_EnterDeepSleep(const uint32_t exclude_from_pd[7]);
void BOARD_BootDSP(void);
void BOARD_StopDSP(void);
void BOARD_InitPowerConfig(void);
void BOARD_PowerConfigAfterCPU1Booted(void);
void BOARD_InitTempSensor(void);
float DEMO_GetTempsenorValue(void);

/* Init the wait timer used for PVT task to periodic check whether the supply can go lower. */
void pvt_wait_timer_init(void);
void pvt_wait_timer_start(uint32_t ms, utick_callback_t cb);
/*${prototype:end}*/

#endif /* _APP_H_ */
