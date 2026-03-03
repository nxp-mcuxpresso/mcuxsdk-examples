/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "fsl_gpio.h"
#include "fsl_mu.h"
#include "fsl_sdma.h"
#include "fsl_iomuxc.h"
#include "srtm_dispatcher.h"
#include "srtm_peercore.h"
#include "srtm_message.h"
#include "srtm_audio_service.h"
#include "app_srtm.h"
#include "board.h"
#include "lpm.h"
#include "srtm_sai_sdma_adapter.h"
#include "srtm_rpmsg_endpoint.h"

#if APP_SRTM_PDM_USED
#include "srtm_pdm_sdma_adapter.h"
#endif

#include "fsl_debug_console.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_MS2TICK(ms) ((ms + portTICK_PERIOD_MS - 1) / portTICK_PERIOD_MS)

#ifndef DEMO_SAI_TX_CONFIG_UseLocalBuf
#define DEMO_SAI_TX_CONFIG_UseLocalBuf 1
#endif

#if DEMO_SAI_TX_CONFIG_UseLocalBuf
#define BUFFER_LEN (64 * 1024)
uint8_t g_buffer[BUFFER_LEN];
srtm_sai_sdma_local_buf_t g_local_buf = {
    .buf       = (uint8_t *)&g_buffer,
    .bufSize   = BUFFER_LEN,
    .periods   = SRTM_SAI_SDMA_MAX_LOCAL_BUF_PERIODS,
    .threshold = 1,

};
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
app_rpmsg_monitor_t rpmsgMonitor;
volatile app_srtm_state_t srtmState = APP_SRTM_StateRun;

static srtm_dispatcher_t disp;
static srtm_peercore_t core;
static srtm_service_t audioService;

static uint8_t sdmaUseCnt = 0U;
SemaphoreHandle_t monSig;
struct rpmsg_lite_instance *rpmsgHandle;

void *rpmsgMonitorParam;
TimerHandle_t linkupTimer;
srtm_sai_adapter_t saiAdapter;
#if APP_SRTM_PDM_USED
srtm_sai_adapter_t pdmAdapter;
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
bool APP_SRTM_ServiceIdle(void)
{
    srtm_audio_state_t TxState, RxState;

#if APP_SRTM_PDM_USED
    srtm_audio_state_t PdmState;

    SRTM_SaiSdmaAdapter_GetAudioServiceState(saiAdapter, &TxState, &RxState);
    SRTM_PdmSdmaAdapter_GetAudioServiceState(pdmAdapter, &PdmState);
    if ((TxState == SRTM_AudioStateClosed) && (RxState == SRTM_AudioStateClosed) && (PdmState == SRTM_AudioStateClosed))
#else
    SRTM_SaiSdmaAdapter_GetAudioServiceState(saiAdapter, &TxState, &RxState);
    if ((TxState == SRTM_AudioStateClosed) && (RxState == SRTM_AudioStateClosed))
#endif
    {
        return true;
    }
    else
    {
        return false;
    }
}

static uint32_t APP_SRTM_ConfAudioDevice(srtm_audio_format_type_t format, uint32_t srate)
{
    uint32_t freq = 0U;

#if APP_SRTM_CODEC_AK4497_USED
    if (format <= SRTM_Audio_Stereo32Bits)
    {
        IOMUXC_SetPinMux(IOMUXC_SAI1_RXD7_SAI1_TX_SYNC, 0U);
    }
    else
    {
        IOMUXC_SetPinMux(IOMUXC_SAI1_RXD7_SAI1_TX_DATA4, 0U);
    }
#endif

#if APP_SRTM_CODEC_WM8524_USED
    /* When using the WM8524 to playback, A core needs to ensure the AUDIO PLL1/2 are alive. */
#endif
    if ((srate % (uint32_t)kSAI_SampleRate11025Hz) == 0U)
    {
#if APP_SRTM_CODEC_AK4497_USED
        freq = APP_AUDIO_PLL2_FREQ / (CLOCK_GetRootPreDivider(kCLOCK_RootSai1)) /
               (CLOCK_GetRootPostDivider(kCLOCK_RootSai1));
#endif
#if APP_SRTM_CODEC_WM8524_USED
        /* A core need to set SAI3 source to Audio PLL2 361267200Hz to get 22.5792MHz */
        freq = APP_AUDIO_PLL2_FREQ / (CLOCK_GetRootPreDivider(kCLOCK_RootSai3)) /
               (CLOCK_GetRootPostDivider(kCLOCK_RootSai3));
#endif
    }
    else
    {
#if APP_SRTM_CODEC_AK4497_USED
        freq = APP_AUDIO_PLL1_FREQ / (CLOCK_GetRootPreDivider(kCLOCK_RootSai1)) /
               (CLOCK_GetRootPostDivider(kCLOCK_RootSai1));
#endif
#if APP_SRTM_CODEC_WM8524_USED
        /* A core need to set SAI3 source to Audio PLL1 393216000Hz to get 24.576Mhz */
        freq = APP_AUDIO_PLL1_FREQ / (CLOCK_GetRootPreDivider(kCLOCK_RootSai3)) /
               (CLOCK_GetRootPostDivider(kCLOCK_RootSai3));
#endif
    }
    return freq;
}

#if APP_SRTM_PDM_USED
static uint32_t APP_SRTM_ConfPdmDevice(srtm_audio_format_type_t format, uint32_t srate)
{
    uint32_t freq = 0U;

    if ((srate % (uint32_t)kSAI_SampleRate8KHz) == 0U)
    {
        /* Set PDM source to Audio PLL1 393216000HZ */
        freq = APP_AUDIO_PLL1_FREQ / (CLOCK_GetRootPreDivider(kCLOCK_RootPdm)) /
               (CLOCK_GetRootPostDivider(kCLOCK_RootPdm));
    }
    else
    {
        /* Set PDM source to Audio PLL2 361267200HZ */
        freq = APP_AUDIO_PLL2_FREQ / (CLOCK_GetRootPreDivider(kCLOCK_RootPdm)) /
               (CLOCK_GetRootPostDivider(kCLOCK_RootPdm));
    }

    return freq;
}
#endif

static void APP_SRTM_PollLinkup(srtm_dispatcher_t dispatcher, void *param1, void *param2)
{
    if (srtmState == APP_SRTM_StateRun)
    {
        if (rpmsg_lite_is_link_up(rpmsgHandle))
        {
            srtmState = APP_SRTM_StateLinkedUp;
            xSemaphoreGive(monSig);
        }
        else
        {
            /* Start timer to poll linkup status. */
            xTimerStart(linkupTimer, portMAX_DELAY);
        }
    }
}

static void APP_LinkupTimerCallback(TimerHandle_t xTimer)
{
    srtm_procedure_t proc = SRTM_Procedure_Create(APP_SRTM_PollLinkup, NULL, NULL);

    if (proc)
    {
        SRTM_Dispatcher_PostProc(disp, proc);
    }
}

static void APP_SRTM_NotifyPeerCoreReady(struct rpmsg_lite_instance *rpmsgHandle, bool ready)
{
    if (rpmsgMonitor)
    {
        rpmsgMonitor(rpmsgHandle, ready, rpmsgMonitorParam);
    }
}

static void APP_SRTM_Linkup(void)
{
    srtm_channel_t chan;
    srtm_rpmsg_endpoint_config_t rpmsgConfig;

    APP_SRTM_NotifyPeerCoreReady(rpmsgHandle, true);

    /* Create SRTM peer core */
    core = SRTM_PeerCore_Create(1U); /* Assign CA53 core ID to 1U */

    SRTM_PeerCore_SetState(core, SRTM_PeerCore_State_Activated);

    /* Common RPMsg channel config */
    rpmsgConfig.localAddr = RL_ADDR_ANY;
    rpmsgConfig.peerAddr  = RL_ADDR_ANY;

    rpmsgConfig.rpmsgHandle = rpmsgHandle;
    rpmsgConfig.epName      = APP_SRTM_AUDIO_CHANNEL_NAME;
    chan                    = SRTM_RPMsgEndpoint_Create(&rpmsgConfig);
    SRTM_PeerCore_AddChannel(core, chan);
    assert((audioService != NULL) && (saiAdapter != NULL));
    SRTM_AudioService_BindChannel(audioService, saiAdapter, chan);

#if APP_SRTM_PDM_USED
    rpmsgConfig.epName = APP_SRTM_PDM_CHANNEL_NAME;
    chan               = SRTM_RPMsgEndpoint_Create(&rpmsgConfig);
    SRTM_PeerCore_AddChannel(core, chan);
    assert((audioService != NULL) && (pdmAdapter != NULL));
    SRTM_AudioService_BindChannel(audioService, pdmAdapter, chan);
#endif

    SRTM_Dispatcher_AddPeerCore(disp, core);
}

static void APP_SRTM_InitPeerCore(void)
{
    copyResourceTable();

    rpmsgHandle = rpmsg_lite_remote_init((void *)RPMSG_LITE_SRTM_SHMEM_BASE, RPMSG_LITE_SRTM_LINK_ID, RL_NO_FLAGS);
    assert(rpmsgHandle);
    if (rpmsg_lite_is_link_up(rpmsgHandle))
    {
        APP_SRTM_Linkup();
    }
    else
    {
        /* Start timer to poll linkup status. */
        xTimerStart(linkupTimer, portMAX_DELAY);
    }
}

static void APP_SRTM_InitAudioDevice(bool enable)
{
    if (enable)
    {
        if (sdmaUseCnt == 0U) /* SDMA is not initialized. */
        {
            sdma_config_t dmaConfig = {0};

            /* Create SDMA handle */
            SDMA_GetDefaultConfig(&dmaConfig);
            dmaConfig.ratio = kSDMA_ARMClockFreq; /* SDMA2 & SDMA3 must set the clock ratio to 1:1. */
            SDMA_Init(APP_SRTM_DMA, &dmaConfig);
        }
        sdmaUseCnt++;
    }
    else
    {
        sdmaUseCnt--;
        if (sdmaUseCnt == 0U) /* SDMA is not used anymore, deinit it for power saving. */
        {
            SDMA_Deinit(APP_SRTM_DMA);
        }
    }
}

static void APP_SRTM_InitAudioService(void)
{
    srtm_sai_sdma_config_t saiTxConfig;
#if APP_SRTM_PDM_USED
    static srtm_pdm_sdma_config_t pdmConfig;
#endif

    memset(&saiTxConfig, 0, sizeof(srtm_sai_sdma_config_t));

    /* Create SAI SDMA adapter */
    SAI_GetClassicI2SConfig(&saiTxConfig.config, kSAI_WordWidth16bits, kSAI_Stereo,
                            kSAI_Channel0Mask);  /* SAI channel 0 used by default */
    saiTxConfig.dataLine1                  = 0U;
    saiTxConfig.dataLine2                  = 4U; /* SAI channel 4 is used as the another channel for the DSD stream */
    saiTxConfig.config.fifo.fifoWatermark  = FSL_FEATURE_SAI_FIFO_COUNTn(APP_SRTM_SAI) / 2U;
    saiTxConfig.mclkConfig.mclkSourceClkHz = APP_SAI_CLK_FREQ;
    saiTxConfig.mclkConfig.mclkHz =
        saiTxConfig.mclkConfig.mclkSourceClkHz;     /* Set the output mclk equal to its source clk by default */
    saiTxConfig.mclkConfig.mclkOutputEnable = true; /* Enable the MCLK output */

#if defined(DEMO_SAI_TX_CONFIG_StopOnSuspend)
    saiTxConfig.stopOnSuspend = DEMO_SAI_TX_CONFIG_StopOnSuspend;
#else
    saiTxConfig.stopOnSuspend = false; /* Keep playing audio on A53 suspend. */
#endif
    saiTxConfig.threshold = 1U; /* Under the threshold value would trigger periodDone message to A53 */
    saiTxConfig.guardTime =
        1000; /* Unit:ms. This is a lower limit that M core should reserve such time data to wakeup A core. */
    saiTxConfig.dmaChannel                = APP_SAI_TX_DMA_CHANNEL;
    saiTxConfig.ChannelPriority           = APP_SAI_TX_DMA_CHANNEL_PRIORITY;
    saiTxConfig.eventSource               = APP_SAI_TX_DMA_SOURCE;
    saiTxConfig.extendConfig.audioDevInit = APP_SRTM_InitAudioDevice;
    saiTxConfig.extendConfig.audioDevConf = APP_SRTM_ConfAudioDevice;

    saiAdapter = SRTM_SaiSdmaAdapter_Create(APP_SRTM_SAI, APP_SRTM_DMA, &saiTxConfig, NULL);
    assert(saiAdapter);

#if APP_SRTM_PDM_USED
    memset(&pdmConfig, 0, sizeof(srtm_pdm_sdma_config_t));

    /* Creat PDM SDMA adapter */
    pdmConfig.stopOnSuspend             = false; // Keep recording on A core suspend.
    pdmConfig.dmaChannel                = APP_PDM_RX_DMA_CHANNEL;
    pdmConfig.channelPriority           = APP_PDM_RX_DMA_CHANNEL_PRIORITY;
    pdmConfig.eventSource               = APP_PDM_RX_DMA_SOURCE;
    pdmConfig.extendConfig.audioDevInit = APP_SRTM_InitAudioDevice;
    pdmConfig.extendConfig.audioDevConf = APP_SRTM_ConfPdmDevice;
    pdmConfig.pdmSrcClk                 = APP_SAI_CLK_FREQ; /* Default pdm clock source same as SAI */
    pdmConfig.config.qualityMode        = APP_PDM_QUALITY_MODE;
    pdmConfig.config.enableDoze         = false;
    pdmConfig.config.fifoWatermark      = FSL_FEATURE_PDM_FIFO_DEPTH / 2U;
    pdmConfig.config.cicOverSampleRate  = APP_PDM_CICOVERSAMPLE_RATE;
    pdmConfig.channelConfig.gain        = APP_PDM_CHANNEL_GAIN;
    pdmConfig.channelConfig.cutOffFreq  = APP_PDM_CHANNEL_CUTOFF_FREQ;
    pdmAdapter                          = SRTM_PdmSdmaAdapter_Create(APP_SRTM_PDM, APP_SRTM_DMA, &pdmConfig);
    assert(pdmAdapter);
#endif
    /*  Set SAI DMA IRQ Priority. */
    NVIC_SetPriority(APP_SRTM_DMA_IRQn, APP_SAI_TX_DMA_IRQ_PRIO);
    NVIC_SetPriority(APP_SRTM_SAI_IRQn, APP_SAI_IRQ_PRIO);

    /* Create and register audio service */
#if DEMO_SAI_TX_CONFIG_UseLocalBuf
    SRTM_SaiSdmaAdapter_SetTxLocalBuf(saiAdapter, &g_local_buf);
#endif
    audioService = SRTM_AudioService_Create(saiAdapter, NULL);
#if APP_SRTM_PDM_USED
    SRTM_AudioService_AddAudioInterface(audioService, pdmAdapter);
#endif
    SRTM_Dispatcher_RegisterService(disp, audioService);
}

static void APP_SRTM_InitServices(void)
{
    APP_SRTM_InitAudioService();
}

static void SRTM_MonitorTask(void *pvParameters)
{
    /* Initialize services and add to dispatcher */
    APP_SRTM_InitServices();

    /* Start SRTM dispatcher */
    SRTM_Dispatcher_Start(disp);

    xSemaphoreGive(monSig);
    while (true)
    {
        xSemaphoreTake(monSig, portMAX_DELAY);
        if (srtmState == APP_SRTM_StateRun)
        {
            SRTM_Dispatcher_Stop(disp);
            APP_SRTM_InitPeerCore();
            SRTM_Dispatcher_Start(disp);
        }
        else
        {
            SRTM_Dispatcher_Stop(disp);
            /* Need to announce channel as we just linked up. */
            APP_SRTM_Linkup();
            SRTM_Dispatcher_Start(disp);
        }
    }
}

static void SRTM_DispatcherTask(void *pvParameters)
{
    SRTM_Dispatcher_Run(disp);
}

void APP_SRTM_Init(void)
{
    MU_Init(MUB);

    monSig = xSemaphoreCreateBinary();
    assert(monSig);
    linkupTimer =
        xTimerCreate("Linkup", APP_MS2TICK(APP_LINKUP_TIMER_PERIOD_MS), pdFALSE, NULL, APP_LinkupTimerCallback);
    assert(linkupTimer);
    /* Create SRTM dispatcher */
    disp = SRTM_Dispatcher_Create();

    if (xTaskCreate(SRTM_MonitorTask, "SRTM monitor", 256U, NULL, APP_SRTM_MONITOR_TASK_PRIO, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
    if (xTaskCreate(SRTM_DispatcherTask, "SRTM dispatcher", 512U, NULL, APP_SRTM_DISPATCHER_TASK_PRIO, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
}

void APP_SRTM_Suspend(void)
{
    /* For user use. */
}

void APP_SRTM_Resume(void)
{
    /* For user use. */
}
