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
#include "srtm_netc_service.h"

#include "app_srtm.h"
#include "board.h"
#include "rsc_table.h"
#include "fsl_mu.h"

static srtm_dispatcher_t disp;
static srtm_peercore_t core;
static srtm_service_t i2cService;
static srtm_service_t netcService;
static SemaphoreHandle_t monSig;
static volatile app_srtm_state_t srtmState;
static struct rpmsg_lite_instance *rpmsgHandle;
static app_rpmsg_monitor_t rpmsgMonitor;
static void *rpmsgMonitorParam;
static TimerHandle_t linkupTimer;
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
static srtm_status_t APP_SRTM_NETC_WritePCIConfig(srtm_netc_adapter_t adapter,
                                                  uint8_t bus,
                                                  uint8_t devFn,
                                                  uint16_t reg,
                                                  uint32_t val,
                                                  uint8_t size);

/* LPI2C1, LPI2C2 in AONMIX(Always On Domain) */
static struct _i2c_bus platform_i2c_buses[] = {
    {.bus_id         = 0,
     .base_addr      = LPI2C1_BASE,
     .type           = SRTM_I2C_TYPE_LPI2C,
     .switch_idx     = I2C_SWITCH_NONE,
     .switch_channel = SRTM_I2C_SWITCH_CHANNEL_UNSPECIFIED},
    {.bus_id         = 1,
     .base_addr      = LPI2C2_BASE,
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

static struct _srtm_netc_adapter netc_adapter = {.writePCIConfig = APP_SRTM_NETC_WritePCIConfig,
                                                };

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
    //uint32_t needStop = (flags & SRTM_I2C_FLAG_NEED_STOP) ? kLPI2C_TransferDefaultFlag : kLPI2C_TransferNoStopFlag;

    switch (type)
    {
        case SRTM_I2C_TYPE_LPI2C:
            //retVal = BOARD_LPI2C_Send((LPI2C_Type *)base_addr, slaveAddr, 0, 0, buf, len, needStop);
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

#define PCIE_CONFIG_SPACE_BASE         0x4CA00000U
#define PCIE_CONFIG_SPACE_SIZE         0x100000U
#define PCIE_VIRTUAL_CONFIG_SPACE_BASE 0x88224000U

#define PCIE_CONFIG_ENETC2_BASE        0x4CA80000U

#define REG_PCIE_DEV_CTRL              0x48U
#define PCIE_DEV_CTRL_INIT_FLR_MASK    0x8000U

/* Share ENETC2 VF */
static srtm_status_t APP_SRTM_NETC_WritePCIConfig(srtm_netc_adapter_t adapter,
                                                  uint8_t bus,
                                                  uint8_t devFn,
                                                  uint16_t reg,
                                                  uint32_t val,
                                                  uint8_t size)
{
    uint16_t rid = ((uint16_t)bus << 8U) | devFn;
    uint32_t addr = PCIE_CONFIG_SPACE_BASE + ((uint32_t)rid * 4U * 1024U) + reg;
    uint32_t vaddr = PCIE_VIRTUAL_CONFIG_SPACE_BASE + ((uint32_t)rid * 4U * 1024U) + reg;

    /* Avoid writting ENETC2 PF except SR-IOV */
    if ((addr >= PCIE_CONFIG_ENETC2_BASE) && (addr < (PCIE_CONFIG_ENETC2_BASE + 0x150U)))
    {
        return SRTM_Status_Success;
    }

    /* Write */
    if (size == 1U)
    {
        *((volatile uint8_t *)addr) = (uint8_t)val;
        *((volatile uint8_t *)vaddr) = *((volatile uint8_t *)addr);
    }
    else if (size == 2U)
    {
        *((volatile uint16_t *)addr) = (uint16_t)val;
        *((volatile uint16_t *)vaddr) = *((volatile uint16_t *)addr);
    }
    else if (size == 4U)
    {
        *((volatile uint32_t *)addr) = (uint32_t)val;
        *((volatile uint32_t *)vaddr) = *((volatile uint32_t *)addr);
    }
    else
    {
        return SRTM_Status_Error;
    }

    /* Wait PCIE_DEV_CTL INIT_FLR bit self-clear */
    if ((reg == REG_PCIE_DEV_CTRL) && ((uint16_t)val == PCIE_DEV_CTRL_INIT_FLR_MASK))
    {
        while ((*((volatile uint16_t *)addr) & 0x8000U) != 0U)
        {
        }
        *((volatile uint16_t *)vaddr) = *((volatile uint16_t *)addr);
    }

    return SRTM_Status_Success;
}

/* Share ENETC2 VF */
void APP_SRTM_NETC_VirtualizePCIConfig(void)
{
    memset((void *)PCIE_VIRTUAL_CONFIG_SPACE_BASE, 0U, PCIE_CONFIG_SPACE_SIZE);
    memcpy((void *)PCIE_VIRTUAL_CONFIG_SPACE_BASE, (void *)PCIE_CONFIG_SPACE_BASE, PCIE_CONFIG_SPACE_SIZE);

    /* disable func except for ENETC2 */
    (*((volatile uint32_t *)(PCIE_VIRTUAL_CONFIG_SPACE_BASE)))             = 0x0000ffffU; /* ENETC0 */
    (*((volatile uint32_t *)(PCIE_VIRTUAL_CONFIG_SPACE_BASE + 0x40000U)))  = 0x0000ffffU; /* ENETC1 */
    (*((volatile uint32_t *)(PCIE_VIRTUAL_CONFIG_SPACE_BASE + 0x80000U)))  = 0x080B1131U; /* ENETC2 with virtual ID */
    (*((volatile uint32_t *)(PCIE_VIRTUAL_CONFIG_SPACE_BASE + 0xc0000U)))  = 0x0000ffffU; /* TMR */
    (*((volatile uint32_t *)(PCIE_VIRTUAL_CONFIG_SPACE_BASE + 0x8000U)))   = 0x0000ffffU; /* ECAM Event Collector */
}

static void APP_SRTM_InitNETCService(void)
{
    netcService = SRTM_NETCService_Create(&netc_adapter);
    SRTM_Dispatcher_RegisterService(disp, netcService);
}

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

/*
 * MU Interrrupt RPMsg handler
 * IPC between Cortex-M7 and Cortex-A55
 */
#ifdef MU7_B_IRQHandler
#undef MU7_B_IRQHandler
#endif

int32_t MU7_B_IRQHandler(void)
{
    uint32_t status = MU_GetStatusFlags(RPMSG_LITE_MU);
#if !(defined(FSL_FEATURE_MU_NO_CORE_STATUS) && (0 != FSL_FEATURE_MU_NO_CORE_STATUS))
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
        if (core != NULL)
	{
            SRTM_PeerCore_SetState(core, SRTM_PeerCore_State_Deactivated);
	}

        PRINTF("Other side is in power down\r\n");
        MU_ClearStatusFlags(RPMSG_LITE_MU, (uint32_t)kMU_OtherSideEnterPowerDownInterruptFlag);
        APP_SRTM_OtherSideResetHandler();
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

    /* Create and add SRTM I2C channel to peer core*/
    rpmsgConfig.epName = APP_SRTM_I2C_CHANNEL_NAME;
    chan               = SRTM_RPMsgEndpoint_Create(&rpmsgConfig);
    SRTM_PeerCore_AddChannel(core, chan);

    SRTM_Dispatcher_AddPeerCore(disp, core);

    /* Create and add SRTM NETC channel to peer core */
    rpmsgConfig.epName = APP_SRTM_NETC_CHANNEL_NAME;
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

static void APP_SRTM_InitServices(void)
{
    APP_SRTM_InitI2CService();
    APP_SRTM_InitNETCService();
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
