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
#define APP_PDM_DMA_IRQ_PRIO          (5U)
#define APP_M2M_DMA_IRQ_PRIO          (5U)

/* Define the timeout ms to polling the A Core link up status */
#define APP_LINKUP_TIMER_PERIOD_MS (10U)

#define RPMSG_LITE_SRTM_SHMEM_BASE (VDEV0_VRING_BASE)
#define RPMSG_LITE_SRTM_LINK_ID    (RL_PLATFORM_IMX95_M7_A55_SRTM_LINK_ID)
#define RPMSG_LITE_MU (RPMSG_LITE_M7_A55_MU)

#define APP_SRTM_AUDIO_CHANNEL_NAME "rpmsg-audio-channel"
#define APP_SRTM_PDM_CHANNEL_NAME   "rpmsg-micfil-channel"
#define APP_SRTM_I2C_CHANNEL_NAME   "rpmsg-i2c-channel"

#define PEER_CORE_ID (1U)

#define APP_SRTM_SAI           (SAI3)
#define APP_SRTM_SAI_IRQn      SAI3_IRQn



/* I2C service */
#define LPI2C1_BAUDRATE              (400000)
//#define I2C_SOURCE_CLOCK_FREQ_LPI2C1 CLOCK_GetIpFreq(kCLOCK_Root_Lpi2c1)

#define LPI2C2_BAUDRATE              (400000)
//#define I2C_SOURCE_CLOCK_FREQ_LPI2C2 CLOCK_GetIpFreq(kCLOCK_Root_Lpi2c2)

#define I2C_SWITCH_NONE 1

/* Audio service */
#define APP_SAI_DMA            (EDMA5_2)
#define APP_SAI_TX_DMA_CHANNEL (2U)
#define APP_SAI_TX_DMA_MUX     (Dma5RequestMuxSai3Tx)
#define APP_SAI_RX_DMA_CHANNEL (3U)
#define APP_SAI_RX_DMA_MUX     (Dma5RequestMuxSai3Rx)
/* The frequency of the audio pll 1/2 are the fixed value. */
#define APP_AUDIO_PLL1_FREQ (393216000U)
#define APP_AUDIO_PLL2_FREQ (361267200U)
/* The MCLK of the SAI is 12288000Hz by default which can be changed when playback the music. */
#define APP_SAI_CLK_FREQ (12288000U)

/* PDM service */
#define APP_SRTM_PDM           (PDM)
#define APP_SRTM_PDM_DMA       (DMA3)
#define APP_PDM_RX_DMA_CHANNEL (Dma3RequestMuxPDMRequest)
#define APP_PDM_QUALITY_MODE        (kPDM_QualityModeHigh)
#define APP_PDM_CICOVERSAMPLE_RATE  (0U)
#define APP_PDM_CHANNEL_GAIN        (kPDM_DfOutputGain4)
#define APP_PDM_CHANNEL_CUTOFF_FREQ (kPDM_DcRemoverCutOff152Hz)

/* Copies to/from external buffer  */
#define APP_MEM2MEM_DMA           (EDMA5_2)
#define APP_MEM2MEM_W_DMA_CHANNEL (0)
#define APP_MEM2MEM_R_DMA_CHANNEL (1)
#define APP_DMA_IRQN(base, channel) (IRQn_Type)(((DMA_Type *)base == DMA3) ? ((uint32_t)DMA3_0_IRQn + channel) : ((uint32_t)DMA5_2_0_1_IRQn + (channel >> 1)))

/* A55 CPU ID number */
#define AP_DOMAIN_LD              (2U)

/* CPU sleep mode */
#define CPU_SLEEP_MODE_RUN        (0U)
#define CPU_SLEEP_MODE_WAIT       (1U)
#define CPU_SLEEP_MODE_STOP       (2U)
#define CPU_SLEEP_MODE_SUSPEND    (3U)

#ifndef SRTM_AUDIO_SERVICE_USED
#define SRTM_AUDIO_SERVICE_USED 0
#endif

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

#if defined(__cplusplus)
}
#endif

#endif /* _APP_SRTM_H_ */
