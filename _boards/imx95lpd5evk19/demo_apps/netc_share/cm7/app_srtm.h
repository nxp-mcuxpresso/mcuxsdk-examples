/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_SRTM_H_
#define _APP_SRTM_H_

#include "rpmsg_lite.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* IRQ handler priority definition, bigger number stands for lower priority */

/* Task priority definition, bigger number stands for higher priority */
#define APP_SRTM_MONITOR_TASK_PRIO    (4U)
#define APP_SRTM_DISPATCHER_TASK_PRIO (3U)
#define APP_LPI2C_IRQ_PRIO            (5U)
#define APP_SAI_TX_DMA_IRQ_PRIO       (5U)
#define APP_SAI_RX_DMA_IRQ_PRIO       (5U)
#define APP_SAI_IRQ_PRIO              (5U)

/* Define the timeout ms to polling the A Core link up status */
#define APP_LINKUP_TIMER_PERIOD_MS (10U)

#define RPMSG_LITE_SRTM_SHMEM_BASE (VDEV0_VRING_BASE)
#define RPMSG_LITE_SRTM_LINK_ID    (RL_PLATFORM_IMX95_M7_A55_SRTM_LINK_ID)
#define RPMSG_LITE_MU (RPMSG_LITE_M7_A55_MU)

#define APP_SRTM_I2C_CHANNEL_NAME   "rpmsg-i2c-channel"
#define APP_SRTM_NETC_CHANNEL_NAME  "rpmsg-pci-channel"

#define PEER_CORE_ID (1U)


/* I2C service */
#define LPI2C1_BAUDRATE              (400000)
//#define I2C_SOURCE_CLOCK_FREQ_LPI2C1 CLOCK_GetIpFreq(kCLOCK_Root_Lpi2c1)

#define LPI2C2_BAUDRATE              (400000)
//#define I2C_SOURCE_CLOCK_FREQ_LPI2C2 CLOCK_GetIpFreq(kCLOCK_Root_Lpi2c2)

#define I2C_SWITCH_NONE 1

typedef void (*app_rpmsg_monitor_t)(struct rpmsg_lite_instance *rpmsgHandle, bool ready, void *param);
typedef void (*app_irq_handler_t)(IRQn_Type irq, void *param);

extern int32_t RPMsg_MU7_B_IRQHandler(void);
/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum
{
    APP_SRTM_StateRun = 0x0U,
    APP_SRTM_StateLinkedUp,
    APP_SRTM_StateReboot,
    APP_SRTM_StateShutdown,
} app_srtm_state_t;

/* Initialize SRTM contexts */
void APP_SRTM_Init(void);

/* Create RPMsg channel and start SRTM communication */
void APP_SRTM_StartCommunication(void);

/* Set RPMsg channel init/deinit monitor */
void APP_SRTM_SetRpmsgMonitor(app_rpmsg_monitor_t monitor, void *param);

void APP_SRTM_NETC_VirtualizePCIConfig(void);
#if defined(__cplusplus)
}
#endif

#endif /* _APP_SRTM_H_ */
