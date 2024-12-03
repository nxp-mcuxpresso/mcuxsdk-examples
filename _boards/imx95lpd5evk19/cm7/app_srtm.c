/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

#include "fsl_lpi2c.h"

#include "srtm_dispatcher.h"
#include "srtm_peercore.h"
#include "srtm_message.h"
#include "srtm_rpmsg_endpoint.h"
#include "srtm_i2c_service.h"
#include "srtm_audio_service.h"
#include "srtm_sai_edma_adapter.h"
#include "srtm_pdm_edma_adapter.h"
#include "scmi_cpu.h"
#include "scmi.h"

#include "app_srtm.h"
#include "board.h"
#include "rsc_table.h"
#include "fsl_mu.h"

static srtm_dispatcher_t disp;
static srtm_peercore_t core;
static srtm_service_t i2cService;
static SemaphoreHandle_t monSig;
volatile app_srtm_state_t srtmState;
static struct rpmsg_lite_instance *rpmsgHandle;
static app_rpmsg_monitor_t rpmsgMonitor;
static void *rpmsgMonitorParam;
static TimerHandle_t linkupTimer;

#if SRTM_AUDIO_SERVICE_USED
static uint8_t edmaUseCnt = 0U;
static srtm_service_t audioService;
srtm_sai_adapter_t pdmAdapter;
static srtm_sai_adapter_t saiAdapter;
#if SRTM_SAI_EDMA_LOCAL_BUF_ENABLE
#define BUFFER_LEN (64 * 1024)
#if (defined(__ICCARM__))
static uint8_t g_buffer[BUFFER_LEN] @"AudioBuf";
#else
static uint8_t g_buffer[BUFFER_LEN] __attribute__((section("AudioBuf,\"w\",%nobits @")));
#endif
static srtm_sai_edma_local_buf_t g_local_buf = {
    .buf       = (uint8_t *)&g_buffer,
    .bufSize   = BUFFER_LEN,
    .periods   = 4,
    .threshold = 2,
};
#endif
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static srtm_status_t APP_SRTM_I2C_Read(srtm_i2c_adapter_t adapter,
                                       uint32_t base_addr,
                                       srtm_i2c_type_t type,
                                       uint16_t slaveAddr,
                                       uint8_t *buf,
                                       uint16_t len,
                                       uint16_t flags);

static srtm_status_t APP_SRTM_I2C_Write(srtm_i2c_adapter_t adapter,
                                        uint32_t base_addr,
                                        srtm_i2c_type_t type,
                                        uint16_t slaveAddr,
                                        uint8_t *buf,
                                        uint16_t len,
                                        uint16_t flags);

static srtm_status_t APP_SRTM_I2C_SwitchChannel(srtm_i2c_adapter_t adapter,
                                                uint32_t base_addr,
                                                srtm_i2c_type_t type,
                                                uint16_t slaveAddr,
                                                srtm_i2c_switch_channel channel);

/* LPI2C1, LPI2C2 in AONMIX(Always On Domain) */
static struct _i2c_bus platform_i2c_buses[] = {
    {.bus_id         = 3,
     .base_addr      = LPI2C4_BASE,
     .type           = SRTM_I2C_TYPE_LPI2C,
     .switch_idx     = I2C_SWITCH_NONE,
     .switch_channel = SRTM_I2C_SWITCH_CHANNEL_UNSPECIFIED},
};

static struct _srtm_i2c_adapter i2c_adapter = {.read          = APP_SRTM_I2C_Read,
                                               .write         = APP_SRTM_I2C_Write,
                                               .switchchannel = APP_SRTM_I2C_SwitchChannel,
                                               .bus_structure = {
                                                   .buses      = platform_i2c_buses,
                                                   .bus_num    = sizeof(platform_i2c_buses) / sizeof(struct _i2c_bus),
                                                   .switch_num = 0,
                                               }};

void APP_SRTM_SetRpmsgMonitor(app_rpmsg_monitor_t monitor, void *param)
{
    rpmsgMonitor      = monitor;
    rpmsgMonitorParam = param;
}

static srtm_status_t APP_SRTM_I2C_Write(srtm_i2c_adapter_t adapter,
                                        uint32_t base_addr,
                                        srtm_i2c_type_t type,
                                        uint16_t slaveAddr,
                                        uint8_t *buf,
                                        uint16_t len,
                                        uint16_t flags)
{
    status_t retVal   = kStatus_Fail;
    uint32_t needStop = (flags & SRTM_I2C_FLAG_NEED_STOP) ? kLPI2C_TransferDefaultFlag : kLPI2C_TransferNoStopFlag;

    switch (type)
    {
        case SRTM_I2C_TYPE_LPI2C:
            retVal = BOARD_LPI2C_Send((LPI2C_Type *)base_addr, slaveAddr, 0, 0, buf, len, needStop);
            break;
        default:
            break;
    }

    return (retVal == kStatus_Success) ? SRTM_Status_Success : SRTM_Status_TransferFailed;
}

static srtm_status_t APP_SRTM_I2C_Read(srtm_i2c_adapter_t adapter,
                                       uint32_t base_addr,
                                       srtm_i2c_type_t type,
                                       uint16_t slaveAddr,
                                       uint8_t *buf,
                                       uint16_t len,
                                       uint16_t flags)
{
    status_t retVal   = kStatus_Fail;
    uint32_t needStop = (flags & SRTM_I2C_FLAG_NEED_STOP) ? kLPI2C_TransferDefaultFlag : kLPI2C_TransferNoStopFlag;

    switch (type)
    {
        case SRTM_I2C_TYPE_LPI2C:
            retVal = BOARD_LPI2C_Receive((LPI2C_Type *)base_addr, slaveAddr, 0, 0, buf, len, needStop);
            break;
        default:
            break;
    }
    return (retVal == kStatus_Success) ? SRTM_Status_Success : SRTM_Status_TransferFailed;
}

static srtm_status_t APP_SRTM_I2C_SwitchChannel(srtm_i2c_adapter_t adapter,
                                                uint32_t base_addr,
                                                srtm_i2c_type_t type,
                                                uint16_t slaveAddr,
                                                srtm_i2c_switch_channel channel)
{
    uint8_t txBuff[1];

    assert(channel < SRTM_I2C_SWITCH_CHANNEL_UNSPECIFIED);
    txBuff[0] = 1 << (uint8_t)channel;
    return adapter->write(adapter, base_addr, type, slaveAddr, txBuff, sizeof(txBuff),
                          SRTM_I2C_FLAG_NEED_STOP); // APP_SRTM_I2C_Write
}

static void APP_SRTM_InitI2CDevice(void)
{
    lpi2c_master_config_t masterConfig;

    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = LPI2C1_BAUDRATE;
    //LPI2C_MasterInit(LPI2C1, &masterConfig, I2C_SOURCE_CLOCK_FREQ_LPI2C1);
    masterConfig.baudRate_Hz = LPI2C2_BAUDRATE;
    //LPI2C_MasterInit(LPI2C2, &masterConfig, I2C_SOURCE_CLOCK_FREQ_LPI2C2);
}

static void APP_SRTM_InitI2CService(void)
{
    APP_SRTM_InitI2CDevice();
    i2cService = SRTM_I2CService_Create(&i2c_adapter);
    SRTM_Dispatcher_RegisterService(disp, i2cService);
}

#if !(defined(FSL_FEATURE_MU_NO_CORE_STATUS) && (0 != FSL_FEATURE_MU_NO_CORE_STATUS))
static void APP_SRTM_OtherSideResetHandler(void)
{
    portBASE_TYPE taskToWake = pdFALSE;

    /* disable interrupt */
    //MU_DisableInterrupts(RPMSG_LITE_MU, kMU_ResetAssertInterruptEnable);

    srtmState = APP_SRTM_StateReboot;

    /* Wake up monitor to reinitialize the SRTM communication with other side(CA55) */
    if (pdPASS == xSemaphoreGiveFromISR(monSig, &taskToWake))
    {
        portYIELD_FROM_ISR(taskToWake);
    }
}
#endif

/*
 * MU Interrrupt RPMsg handler
 * IPC between Cortex-M7 and Cortex-A55
 */
#ifdef MU7_B_IRQHandler
#undef MU7_B_IRQHandler
#endif

int32_t MU7_B_IRQHandler(void)
{
#if !(defined(FSL_FEATURE_MU_NO_CORE_STATUS) && (0 != FSL_FEATURE_MU_NO_CORE_STATUS))
    uint32_t status = MU_GetStatusFlags(RPMSG_LITE_MU);
    if (status & kMU_OtherSideEnterRunInterruptFlag)
    {
        PRINTF("Other side is in Run\r\n");
    }
    if (status & kMU_OtherSideEnterWaitInterruptFlag)
    {
        PRINTF(" Other side is in Wait\r\n");
    }
    if (status & kMU_OtherSideEnterHaltInterruptFlag)
    {
        PRINTF("Other side is in Halt\r\n");
    }
    if (status & kMU_OtherSideEnterPowerDownInterruptFlag)
    {
        MU_ClearStatusFlags(RPMSG_LITE_MU, (uint32_t)kMU_OtherSideEnterPowerDownInterruptFlag);

        uint32_t runMode, sleepMode, velow, vehigh;
        int32_t status = SCMI_ERR_SUCCESS;
        status = SCMI_CpuInfoGet(SCMI_A2P, AP_DOMAIN_LD, &runMode, &sleepMode, &velow, &vehigh);
        if (status != SCMI_ERR_SUCCESS)
        {
            PRINTF("Get AP info fail\r\n");
        }

        /*
         * Use AP sleep mode state to judge A55 suspend or poweroff state.
         * AP suspend  --  CPU_SLEEP_MODE_SUSPEND.
         * AP poweroff --  CPU_SLEEP_MODE_RUN.
         */
        if (sleepMode == CPU_SLEEP_MODE_SUSPEND)
        {
            PRINTF("Other side(AP) entered suspend to memory state through linux command: echo mem > /sys/power/state\r\n");
        }
        else if (sleepMode == CPU_SLEEP_MODE_RUN)
        {
            PRINTF("Other side(AP) entered poweroff state through linux command: poweroff\r\n");
            if (core != NULL)
            {
                SRTM_PeerCore_SetState(core, SRTM_PeerCore_State_Deactivated);
            }
            APP_SRTM_OtherSideResetHandler();
        }
    }
#endif
    return RPMsg_MU7_B_IRQHandler();
}

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
    /* deinit and init app task(str_echo/pingpong rpmsg) in APP_SRTM_StateReboot only */
    if (rpmsgMonitor && (srtmState == APP_SRTM_StateReboot))
    {
        rpmsgMonitor(rpmsgHandle, ready, rpmsgMonitorParam);
    }
}

static void APP_SRTM_Linkup(void)
{
    srtm_channel_t chan;
    srtm_rpmsg_endpoint_config_t rpmsgConfig;

    /* Create SRTM peer core */
    core = SRTM_PeerCore_Create(PEER_CORE_ID);
    /* Set peer core state to activated */
    SRTM_PeerCore_SetState(core, SRTM_PeerCore_State_Activated);

    /* Common RPMsg channel config */
    rpmsgConfig.localAddr   = RL_ADDR_ANY;
    rpmsgConfig.peerAddr    = RL_ADDR_ANY;
    rpmsgConfig.rpmsgHandle = rpmsgHandle;
#if SRTM_AUDIO_SERVICE_USED
    rpmsgConfig.epName = APP_SRTM_AUDIO_CHANNEL_NAME;
    chan               = SRTM_RPMsgEndpoint_Create(&rpmsgConfig);
    SRTM_PeerCore_AddChannel(core, chan);
    assert((audioService != NULL) && (saiAdapter != NULL));
    SRTM_AudioService_BindChannel(audioService, saiAdapter, chan);

    rpmsgConfig.epName = APP_SRTM_PDM_CHANNEL_NAME;
    chan               = SRTM_RPMsgEndpoint_Create(&rpmsgConfig);
    SRTM_PeerCore_AddChannel(core, chan);
    assert((audioService != NULL) && (pdmAdapter != NULL));
    SRTM_AudioService_BindChannel(audioService, pdmAdapter, chan);
#endif
    /* Create and add SRTM I2C channel to peer core*/
    rpmsgConfig.epName = APP_SRTM_I2C_CHANNEL_NAME;
    chan               = SRTM_RPMsgEndpoint_Create(&rpmsgConfig);
    SRTM_PeerCore_AddChannel(core, chan);

    SRTM_Dispatcher_AddPeerCore(disp, core);
}

static void APP_SRTM_InitPeerCore(void)
{
    copyResourceTable();

    rpmsgHandle = rpmsg_lite_remote_init((void *)RPMSG_LITE_SRTM_SHMEM_BASE, RPMSG_LITE_SRTM_LINK_ID, RL_NO_FLAGS);
    assert(rpmsgHandle);

    APP_SRTM_NotifyPeerCoreReady(rpmsgHandle, true);

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

static void APP_SRTM_ResetServices(void)
{
    /* When A Core resets, we need to avoid async event to send to A Core, to do in here */
}

static void APP_SRTM_DeinitPeerCore(void)
{
    /* Stop linkupTimer if it's started. */
    xTimerStop(linkupTimer, portMAX_DELAY);

    /* Notify application for the peer core disconnection. */
    APP_SRTM_NotifyPeerCoreReady(rpmsgHandle, false);

    if (core)
    {
        /* Need to let services know peer core is now down. */
        APP_SRTM_ResetServices();

        SRTM_Dispatcher_RemovePeerCore(disp, core);
        SRTM_PeerCore_Destroy(core);
        core = NULL;
    }

    if (rpmsgHandle)
    {
        rpmsg_lite_deinit(rpmsgHandle);
        rpmsgHandle = NULL;
    }
}

#if SRTM_AUDIO_SERVICE_USED
static void APP_SRTM_InitPdmDevice(bool enable)
{
    edma_config_t dmaConfig;
    if (enable)
    {
        if (edmaUseCnt == 0U)
        {
            EDMA_GetDefaultConfig(&dmaConfig);
            EDMA_Init(APP_SRTM_PDM_DMA, &dmaConfig);
        }
        edmaUseCnt++;
    }
    else
    {
        edmaUseCnt--;
        if (edmaUseCnt == 0U)
        {
            EDMA_Deinit(APP_SRTM_PDM_DMA);
        }
    }
}

static void APP_SRTM_InitAudioDevice(void)
{
    edma_config_t dmaConfig;

    /* Initialize DMA4 for SAI */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(APP_SAI_DMA, &dmaConfig);

    /* Initialize DMAMUX for SAI */
    EDMA_SetChannelMux(APP_SAI_DMA, APP_SAI_TX_DMA_CHANNEL, APP_SAI_TX_DMA_MUX);
    EDMA_SetChannelMux(APP_SAI_DMA, APP_SAI_RX_DMA_CHANNEL, APP_SAI_RX_DMA_MUX);

    APP_SRTM_InitPdmDevice(true);
}

static uint32_t APP_SRTM_ConfAudioDevice(srtm_audio_format_type_t format, uint32_t srate)
{
    uint32_t freq = 0U;

    if ((srate % (uint32_t)kSAI_SampleRate44100Hz) == 0U)
    {
        freq = (APP_AUDIO_PLL2_FREQ / 32);
    }
    else if((srate % (uint32_t)kSAI_SampleRate8KHz) == 0U)
    {
        freq = (APP_AUDIO_PLL1_FREQ / 32);
    }

    return freq;
}

static uint32_t APP_SRTM_ConfPdmDevice(srtm_audio_format_type_t format, uint32_t srate)
{
    return CLOCK_GetIpFreq(kCLOCK_Root_Pdm);
}

static void APP_SRTM_InitAudioService(void)
{
    srtm_sai_edma_config_t saiTxConfig;
    srtm_sai_edma_config_t saiRxConfig;
    srtm_pdm_edma_config_t pdmConfig;

    APP_SRTM_InitAudioDevice();

    memset(&saiTxConfig, 0, sizeof(saiTxConfig));
    memset(&saiRxConfig, 0, sizeof(saiRxConfig));
    memset(&pdmConfig, 0, sizeof(srtm_pdm_edma_config_t));

    /*  Set IRQ Priorities. */
    NVIC_SetPriority(APP_DMA_IRQN(APP_SAI_DMA, APP_SAI_TX_DMA_CHANNEL), APP_SAI_TX_DMA_IRQ_PRIO);
    NVIC_SetPriority(APP_DMA_IRQN(APP_SAI_DMA, APP_SAI_RX_DMA_CHANNEL), APP_SAI_RX_DMA_IRQ_PRIO);
    NVIC_SetPriority(APP_SRTM_SAI_IRQn, APP_SAI_IRQ_PRIO);
    NVIC_SetPriority(APP_DMA_IRQN(APP_SRTM_PDM_DMA, APP_PDM_RX_DMA_CHANNEL), APP_PDM_DMA_IRQ_PRIO);
    NVIC_SetPriority(APP_DMA_IRQN(APP_MEM2MEM_DMA, APP_MEM2MEM_W_DMA_CHANNEL), APP_M2M_DMA_IRQ_PRIO);
    NVIC_SetPriority(APP_DMA_IRQN(APP_MEM2MEM_DMA, APP_MEM2MEM_R_DMA_CHANNEL), APP_M2M_DMA_IRQ_PRIO);

    /* Create SAI EDMA adapter */
    SAI_GetClassicI2SConfig(&saiTxConfig.config, kSAI_WordWidth16bits, kSAI_Stereo, kSAI_Channel0Mask);
    saiTxConfig.config.syncMode           = kSAI_ModeAsync; /* Tx in Async mode */
    saiTxConfig.config.fifo.fifoWatermark = FSL_FEATURE_SAI_FIFO_COUNTn(APP_SRTM_SAI) - 1;
    saiTxConfig.mclk                      = APP_SAI_CLK_FREQ;

#if defined(DEMO_SAI_TX_CONFIG_StopOnSuspend)
    saiTxConfig.stopOnSuspend = DEMO_SAI_TX_CONFIG_StopOnSuspend;
#else
    saiTxConfig.stopOnSuspend = false; /* Keep playing audio on APD suspend. */
#endif
    saiTxConfig.threshold = 1U; /* Every period transmitted triggers periodDone message to A core. */
    saiTxConfig.guardTime =
        1500; /* Unit:ms. This is a lower limit that M core should reserve such time data to wakeup A core. */
    saiTxConfig.dmaChannel = APP_SAI_TX_DMA_CHANNEL;
    saiTxConfig.extendConfig.audioDevConf = APP_SRTM_ConfAudioDevice;

    SAI_GetClassicI2SConfig(&saiRxConfig.config, kSAI_WordWidth16bits, kSAI_Stereo, kSAI_Channel0Mask);
    saiRxConfig.config.syncMode           = kSAI_ModeSync; /* Rx in Sync mode */
    saiRxConfig.config.fifo.fifoWatermark = 1;
    saiRxConfig.mclk                      = APP_SAI_CLK_FREQ;
#if defined(DEMO_SAI_TX_CONFIG_StopOnSuspend)
    saiTxConfig.stopOnSuspend = DEMO_SAI_TX_CONFIG_StopOnSuspend;
#else
    saiRxConfig.stopOnSuspend = false; /* Keep recording data on APD suspend. */
#endif
    saiRxConfig.threshold  = UINT32_MAX; /* Every period received triggers periodDone message to A core. */
    saiRxConfig.dmaChannel = APP_SAI_RX_DMA_CHANNEL;

    saiAdapter = SRTM_SaiEdmaAdapter_Create(SAI3, APP_SAI_DMA, &saiTxConfig, &saiRxConfig);
    assert(saiAdapter);

#if SRTM_SAI_EDMA_LOCAL_BUF_ENABLE
    SRTM_SaiEdmaAdapter_SetTxLocalBuf(saiAdapter, &g_local_buf);
#endif

    /* Creat PDM SDMA adapter */
    pdmConfig.stopOnSuspend = false; // Keep recording on A core suspend.
    pdmConfig.dmaChannel    = APP_PDM_RX_DMA_CHANNEL;
    pdmConfig.extendConfig.audioDevInit = APP_SRTM_InitPdmDevice;
    pdmConfig.extendConfig.audioDevConf = APP_SRTM_ConfPdmDevice;
    pdmConfig.pdmSrcClk                 = HAL_ClockGetIpFreq(hal_clock_pdm);;
    pdmConfig.config.qualityMode        = APP_PDM_QUALITY_MODE;
    pdmConfig.config.enableDoze         = false;
    pdmConfig.config.fifoWatermark      = FSL_FEATURE_PDM_FIFO_DEPTH / 2U;
    pdmConfig.config.cicOverSampleRate  = APP_PDM_CICOVERSAMPLE_RATE;
    pdmConfig.channelConfig.gain        = APP_PDM_CHANNEL_GAIN;
    pdmConfig.channelConfig.cutOffFreq  = APP_PDM_CHANNEL_CUTOFF_FREQ;
    pdmAdapter                          = SRTM_PdmEdmaAdapter_Create(APP_SRTM_PDM, APP_SRTM_PDM_DMA, &pdmConfig);
    assert(pdmAdapter);

    audioService = SRTM_AudioService_Create(saiAdapter, NULL);
    SRTM_AudioService_AddAudioInterface(audioService, pdmAdapter);
    SRTM_Dispatcher_RegisterService(disp, audioService);
}
#endif

static void APP_SRTM_InitServices(void)
{
    APP_SRTM_InitI2CService();
#if SRTM_AUDIO_SERVICE_USED
    APP_SRTM_InitAudioService();
#endif
}

static void SRTM_DispatcherTask(void *pvParameters)
{
    SRTM_Dispatcher_Run(disp);
}

static void SRTM_MonitorTask(void *pvParameters)
{
    app_srtm_state_t state = APP_SRTM_StateShutdown;

    /* Initialize services and add to dispatcher */
    APP_SRTM_InitServices();

    /* Start SRTM dispatcher */
    SRTM_Dispatcher_Start(disp);

    /* Monitor peer core state change */
    while (true)
    {
        xSemaphoreTake(monSig, portMAX_DELAY);

        if (state == srtmState)
        {
            continue;
        }

        switch (srtmState)
        {
            case APP_SRTM_StateRun:
                assert(state == APP_SRTM_StateShutdown);
                PRINTF("Start SRTM communication\r\n");
                SRTM_Dispatcher_Stop(disp);

                APP_SRTM_InitPeerCore();
                SRTM_Dispatcher_Start(disp);
                state = APP_SRTM_StateRun;
                break;

            case APP_SRTM_StateLinkedUp:
                if (state == APP_SRTM_StateRun)
                {
                    PRINTF("Handle Peer Core Linkup\r\n");
                    SRTM_Dispatcher_Stop(disp);
                    APP_SRTM_Linkup();
                    SRTM_Dispatcher_Start(disp);
                }
                break;

            case APP_SRTM_StateReboot:
                assert(state == APP_SRTM_StateRun);

                PRINTF("Handle Peer Core Reboot\r\n");

                SRTM_Dispatcher_Stop(disp);
                /* Remove peer core from dispatcher */
                APP_SRTM_DeinitPeerCore();

                /* enable clock of MU before accessing registers of MU */
                MU_Init(RPMSG_LITE_MU);

                /* Initialize peer core and add to dispatcher */
                APP_SRTM_InitPeerCore();

                /* Restore srtmState to Run. */
                srtmState = APP_SRTM_StateRun;

                SRTM_Dispatcher_Start(disp);
                break;

            default:
                assert(false);
                break;
        }
    }
}

void APP_SRTM_Init(void)
{
    monSig = xSemaphoreCreateBinary();
    assert(monSig);

    linkupTimer =
        xTimerCreate("Linkup", pdMS_TO_TICKS(APP_LINKUP_TIMER_PERIOD_MS), pdFALSE, NULL, APP_LinkupTimerCallback);
    assert(linkupTimer);


    /* Enable mu interrupts for remote(slave) side core */
    MU_Init(RPMSG_LITE_MU);
#if !(defined(FSL_FEATURE_MU_NO_CORE_STATUS) && (0 != FSL_FEATURE_MU_NO_CORE_STATUS))
    MU_EnableInterrupts(RPMSG_LITE_MU, kMU_OtherSideEnterPowerDownInterruptEnable);
    MU_EnableInterrupts(RPMSG_LITE_MU, kMU_OtherSideEnterRunInterruptEnable);
    MU_EnableInterrupts(RPMSG_LITE_MU, kMU_OtherSideEnterHaltInterruptEnable);
    MU_EnableInterrupts(RPMSG_LITE_MU, kMU_OtherSideEnterWaitInterruptEnable);
    MU_EnableInterrupts(RPMSG_LITE_MU, kMU_OtherSideEnterStopInterruptEnable);
#endif

    /* Create SRTM dispatcher */
    disp = SRTM_Dispatcher_Create();

    xTaskCreate(SRTM_MonitorTask, "SRTM monitor", 256U, NULL, APP_SRTM_MONITOR_TASK_PRIO, NULL);
    xTaskCreate(SRTM_DispatcherTask, "SRTM dispatcher", 512U, NULL, APP_SRTM_DISPATCHER_TASK_PRIO, NULL);
}

void APP_SRTM_StartCommunication(void)
{
    srtmState = APP_SRTM_StateRun;
    xSemaphoreGive(monSig);
}
