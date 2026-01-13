/*
 * Copyright 2023-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

#if SRTM_I2C_SERVICE_USED
#include "fsl_lpi2c.h"
#include "srtm_i2c_service.h"
#endif

#if SRTM_NETC_SERVICE_USED
#include "srtm_netc_service.h"
#endif

#include "srtm_dispatcher.h"
#include "srtm_peercore.h"
#include "srtm_message.h"
#include "srtm_rpmsg_endpoint.h"
#if SRTM_AUDIO_SERVICE_USED
#include "srtm_audio_service.h"
#include "srtm_sai_edma_adapter.h"
#include "srtm_pdm_edma_adapter.h"
#endif
#include "scmi_cpu.h"
#include "scmi.h"

#include "app_srtm.h"
#include "board.h"
#include "rsc_table.h"
#include "fsl_mu.h"
#if SRTM_IO_SERVICE_USED
#include "srtm_io_service.h"
#include "fsl_adapter_gpio.h"
#endif

static srtm_dispatcher_t disp;
static srtm_peercore_t core;
#if SRTM_I2C_SERVICE_USED
static srtm_service_t i2cService;
#endif
#if SRTM_NETC_SERVICE_USED
static srtm_service_t netcService;
#endif
static SemaphoreHandle_t monSig;
volatile app_srtm_state_t srtmState;
static struct rpmsg_lite_instance *rpmsgHandle;
static app_rpmsg_monitor_t rpmsgMonitor;
static void *rpmsgMonitorParam;
static TimerHandle_t linkupTimer;

#if SRTM_IO_SERVICE_USED
static srtm_service_t ioService;
#define PIN_FUNC_ID_SIZE (5)
/* + 1: there is no GPIO0, but index start from 0 */
#define APP_PORT_NUM (FSL_FEATURE_SOC_RGPIO_COUNT + 1)
#define APP_PIN_NUM 32
#define APP_GPIO_IDX(ioId) ((uint8_t)(((uint16_t)ioId) >> 8U))
#define APP_PIN_IDX(ioId)  ((uint8_t)ioId)

typedef struct
{
    uint16_t ioId;
    uint8_t portIdx;
    uint8_t pinIdx;
    TimerHandle_t timer; /* GPIO glitch detect timer */
    srtm_io_event_t event;
    srtm_io_direction_t direction;
    bool wakeup;
    uint8_t value;
    uint32_t pinFuncId[PIN_FUNC_ID_SIZE];
    uint32_t inputMask;
    uint32_t outputMask;
    hal_gpio_handle_t p_gpioHandle;
} app_io_t;

GPIO_HANDLE_DEFINE(g_GpioPort2Pin4Handle);
GPIO_HANDLE_DEFINE(g_GpioPort2Pin10Handle);
GPIO_HANDLE_DEFINE(g_GpioPort2Pin11Handle);
GPIO_HANDLE_DEFINE(g_GpioPort2Pin18Handle);

/*
 * NOTE: MCU DRIVERS DON'T SUPPORT SAVE CONTEXT FOR RESUME, BUT LINUX DRIVERS DO.
 * WHEN MCU CORE RUNS INTO VLLS MODE, MOST PERIPHERALS STATE WILL BE LOST. HERE PROVIDES
 * AN EXAMPLE TO SAVE DEVICE STATE BY APPLICATION IN A SUSPEND CONTEXT LOCATING IN TCM
 * WHICH CAN KEEP DATA IN VLLS MODE.
 */
typedef struct
{
    struct
    {
        app_io_t data[APP_PORT_NUM][APP_PIN_NUM];
    } io;
} app_suspend_ctx_t;

static app_suspend_ctx_t suspendContext = {
    .io.data = {
        /* port 0(GPIO0 instance) */
        {
            [0 ... (APP_PIN_NUM - 1)] = {
                .portIdx = 0,
                .pinIdx = 0,
                .timer = NULL,
                .event = SRTM_IoEventNone,
                .wakeup = false,
                .value = 0,
                .direction = SRTM_IoDirectionInput,
                .pinFuncId = {0},
                .inputMask = 0,
                .outputMask = 0,
                .p_gpioHandle = NULL,
             }
        },
        /* port 1(GPIO1 instance) */
        {
            [0 ... (APP_PIN_NUM - 1)] = {
                .portIdx = 0,
                .pinIdx = 0,
                .timer = NULL,
                .event = SRTM_IoEventNone,
                .wakeup = false,
                .value = 0,
                .direction = SRTM_IoDirectionInput,
                .pinFuncId = {0},
                .inputMask = 0,
                .outputMask = 0,
                .p_gpioHandle = NULL,
             }
        },
        /* port 2(GPIO2 instance) */
        {
            [0 ... 3] = {
                .portIdx = 0,
                .pinIdx = 0,
                .timer = NULL,
                .event = SRTM_IoEventNone,
                .wakeup = false,
                .value = 0,
                .direction = SRTM_IoDirectionInput,
                .pinFuncId = {0},
                .inputMask = 0,
                .outputMask = 0,
                .p_gpioHandle = NULL,
             },
             [4] = {
                .portIdx = 2,
                .pinIdx = 4,
                .timer = NULL,
                .event = SRTM_IoEventNone,
                .wakeup = false,
                .value = 0,
                .direction = SRTM_IoDirectionOutput,
                .pinFuncId = {IOMUXC_PAD_GPIO_IO04__GPIO2_IO_4},
                .inputMask = IOMUXC_PAD_PU(1) | IOMUXC_PAD_FSEL1(0x3) | IOMUXC_PAD_DSE(0xF),
                .outputMask = IOMUXC_PAD_PU(1) | IOMUXC_PAD_FSEL1(0x2) | IOMUXC_PAD_DSE(0xF),
                .p_gpioHandle = g_GpioPort2Pin4Handle,
             },
             [5 ... 9] = {
                .portIdx = 0,
                .pinIdx = 0,
                .timer = NULL,
                .event = SRTM_IoEventNone,
                .wakeup = false,
                .value = 0,
                .direction = SRTM_IoDirectionInput,
                .pinFuncId = {0},
                .inputMask = 0,
                .outputMask = 0,
                .p_gpioHandle = NULL,
             },
             [10] = {
                .portIdx = 2,
                .pinIdx = 10,
                .timer = NULL,
                .event = SRTM_IoEventNone,
                .wakeup = false,
                .value = 0,
                .direction = SRTM_IoDirectionInput,
                .pinFuncId = {IOMUXC_PAD_GPIO_IO10__GPIO2_IO_10},
                .inputMask = IOMUXC_PAD_PU(1) | IOMUXC_PAD_FSEL1(0x2) | IOMUXC_PAD_DSE(0xF),
                .outputMask = IOMUXC_PAD_PU(1) | IOMUXC_PAD_FSEL1(0x2) | IOMUXC_PAD_DSE(0xF),
                .p_gpioHandle = g_GpioPort2Pin10Handle,
             },
             [11] = {
                .portIdx = 2,
                .pinIdx = 11,
                .timer = NULL,
                .event = SRTM_IoEventNone,
                .wakeup = false,
                .value = 0,
                .direction = SRTM_IoDirectionInput,
                .pinFuncId = {IOMUXC_PAD_GPIO_IO11__GPIO2_IO_11},
                .inputMask = IOMUXC_PAD_PU(1) | IOMUXC_PAD_FSEL1(0x2) | IOMUXC_PAD_DSE(0xF),
                .outputMask = IOMUXC_PAD_PU(1) | IOMUXC_PAD_FSEL1(0x2) | IOMUXC_PAD_DSE(0xF),
                .p_gpioHandle = g_GpioPort2Pin11Handle,
             },
             [12 ... 17] = {
                .portIdx = 0,
                .pinIdx = 0,
                .timer = NULL,
                .event = SRTM_IoEventNone,
                .wakeup = false,
                .value = 0,
                .pinFuncId = {0},
                .inputMask = 0,
                .outputMask = 0,
                .p_gpioHandle = NULL,
             },
             [18] = {
                .portIdx = 2,
                .pinIdx = 18,
                .timer = NULL,
                .event = SRTM_IoEventNone,
                .wakeup = false,
                .value = 0,
                .direction = SRTM_IoDirectionInput,
                .pinFuncId = {IOMUXC_PAD_GPIO_IO18__GPIO2_IO_18},
                .inputMask = IOMUXC_PAD_PU(1) | IOMUXC_PAD_FSEL1(0x2) | IOMUXC_PAD_DSE(0xF),
                .outputMask = IOMUXC_PAD_PU(1) | IOMUXC_PAD_FSEL1(0x2) | IOMUXC_PAD_DSE(0xF),
                .p_gpioHandle = g_GpioPort2Pin18Handle,
             },
             [19 ... (APP_PIN_NUM - 1)] = {
                .portIdx = 0,
                .pinIdx = 0,
                .timer = NULL,
                .event = SRTM_IoEventNone,
                .wakeup = false,
                .value = 0,
                .direction = SRTM_IoDirectionInput,
                .pinFuncId = {0},
                .inputMask = 0,
                .outputMask = 0,
                .p_gpioHandle = NULL,
             },
        },
        /* port 3(GPIO3 instance) */
        {
            [0 ... (APP_PIN_NUM - 1)] = {
                .portIdx = 0,
                .pinIdx = 0,
                .timer = NULL,
                .event = SRTM_IoEventNone,
                .wakeup = false,
                .value = 0,
                .direction = SRTM_IoDirectionInput,
                .pinFuncId = {0},
                .inputMask = 0,
                .outputMask = 0,
                .p_gpioHandle = NULL,
             }
        },
        /* port 4(GPIO4 instance) */
        {
            [0 ... (APP_PIN_NUM - 1)] = {
                .portIdx = 0,
                .pinIdx = 0,
                .timer = NULL,
                .event = SRTM_IoEventNone,
                .wakeup = false,
                .value = 0,
                .direction = SRTM_IoDirectionInput,
                .pinFuncId = {0},
                .inputMask = 0,
                .outputMask = 0,
                .p_gpioHandle = NULL,
             }
        },
        /* port 5(GPIO5 instance) */
        {
            [0 ... (APP_PIN_NUM - 1)] = {
                .portIdx = 0,
                .pinIdx = 0,
                .timer = NULL,
                .event = SRTM_IoEventNone,
                .wakeup = false,
                .value = 0,
                .direction = SRTM_IoDirectionInput,
                .pinFuncId = {0},
                .inputMask = 0,
                .outputMask = 0,
                .p_gpioHandle = NULL,
             }
        },
    }
};

#endif

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
#if SRTM_I2C_SERVICE_USED
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
#endif

#if SRTM_NETC_SERVICE_USED
static srtm_status_t APP_SRTM_NETC_WritePCIConfig(srtm_netc_adapter_t adapter,
                                                  uint8_t bus,
                                                  uint8_t devFn,
                                                  uint16_t reg,
                                                  uint32_t val,
                                                  uint8_t size);

static struct _srtm_netc_adapter netc_adapter = {.writePCIConfig = APP_SRTM_NETC_WritePCIConfig,
                                                };
#endif

void APP_SRTM_SetRpmsgMonitor(app_rpmsg_monitor_t monitor, void *param)
{
    rpmsgMonitor      = monitor;
    rpmsgMonitorParam = param;
}

#if SRTM_I2C_SERVICE_USED
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

    if (retVal == kStatus_LPI2C_Timeout)
    {
        BOARD_I2C_ReleaseBus((int32_t)LPI2C_GetInstance((LPI2C_Type *)base_addr));
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

    if (retVal == kStatus_LPI2C_Timeout)
    {
        BOARD_I2C_ReleaseBus((int32_t)LPI2C_GetInstance((LPI2C_Type *)base_addr));
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
#endif

#if SRTM_NETC_SERVICE_USED

#define PCIE_CONFIG_SPACE_BASE         0x4CA00000U
#define PCIE_CONFIG_SPACE_SIZE         0x100000U
#define PCIE_VIRTUAL_CONFIG_SPACE_BASE 0x88224000U

#define PCIE_CONFIG_ENETC0_BASE        0x4CA00000U

#define REG_PCIE_DEV_CTRL              0x48U
#define PCIE_DEV_CTRL_INIT_FLR_MASK    0x8000U

/* Share ENETC0 VF */
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
    if ((addr >= PCIE_CONFIG_ENETC0_BASE) && (addr < (PCIE_CONFIG_ENETC0_BASE + 0x190U)))
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

/* Share ENETC0 VF */
void APP_SRTM_NETC_VirtualizePCIConfig(void)
{
    memset((void *)PCIE_VIRTUAL_CONFIG_SPACE_BASE, 0U, PCIE_CONFIG_SPACE_SIZE);
    memcpy((void *)PCIE_VIRTUAL_CONFIG_SPACE_BASE, (void *)PCIE_CONFIG_SPACE_BASE, PCIE_CONFIG_SPACE_SIZE);

    /* disable func except for ENETC0 */
    (*((volatile uint32_t *)(PCIE_VIRTUAL_CONFIG_SPACE_BASE)))             = 0x080B1131U; /* ENETC0 with virtual ID */
    (*((volatile uint32_t *)(PCIE_VIRTUAL_CONFIG_SPACE_BASE + 0x1000U)))   = 0x0000ffffU; /* TMR0 */
    (*((volatile uint32_t *)(PCIE_VIRTUAL_CONFIG_SPACE_BASE + 0x2000U)))   = 0x0000ffffU; /* EMDIO0 */
    (*((volatile uint32_t *)(PCIE_VIRTUAL_CONFIG_SPACE_BASE + 0x8000U)))   = 0x0000ffffU; /* ECAM Event Collector */
    (*((volatile uint32_t *)(PCIE_VIRTUAL_CONFIG_SPACE_BASE + 0x100000U))) = 0x0000ffffU; /* ENETC1 */
    (*((volatile uint32_t *)(PCIE_VIRTUAL_CONFIG_SPACE_BASE + 0x108000U))) = 0x0000ffffU; /* ECAM Event Collector */
}

static void APP_SRTM_InitNETCService(void)
{
    netcService = SRTM_NETCService_Create(&netc_adapter);
    SRTM_Dispatcher_RegisterService(disp, netcService);
}
#endif

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

void APP_SRTM_HandleLmmPowerChange(uint32_t lm, uint32_t flags)
{
    /*
     * MU will mark the acore status as POWER DOWN when Acore is in cpuidle state.
     * It cannot determine whether the other party is at suspend mode.
     * so it is selected to identify through system manager notification.
     */
    if (lm == SYSTEM_PLATFORM_LMID_A55)
    {
        if (flags == SCMI_SYS_STATE_SUSPEND)
        {
#if (defined(SRTM_AUDIO_SERVICE_USED) && (0 == SRTM_AUDIO_SERVICE_USED))
            PRINTF("Other side(AP) entered suspend to memory state through linux command: echo mem > /sys/power/state\r\n");
#endif
        }
        else if(flags == SCMI_SYS_STATE_WARM_RESET)
        {
            PRINTF("Other side(AP) entered poweroff state through linux command: poweroff\r\n");
            if (core != NULL)
            {
                SRTM_PeerCore_SetState(core, SRTM_PeerCore_State_Deactivated);
            }
            APP_SRTM_OtherSideResetHandler();
        }
        else
        {
            /* To do for other power mode. */
        }
    }
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

    (void)chan;
    (void)rpmsgConfig;
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

#if SRTM_I2C_SERVICE_USED
    /* Create and add SRTM I2C channel to peer core*/
    rpmsgConfig.epName = APP_SRTM_I2C_CHANNEL_NAME;
    chan               = SRTM_RPMsgEndpoint_Create(&rpmsgConfig);
    SRTM_PeerCore_AddChannel(core, chan);
#endif

#if SRTM_NETC_SERVICE_USED
    /* Create and add SRTM NETC channel to peer core */
    rpmsgConfig.epName = APP_SRTM_NETC_CHANNEL_NAME;
    chan               = SRTM_RPMsgEndpoint_Create(&rpmsgConfig);
    SRTM_PeerCore_AddChannel(core, chan);
#endif

#if SRTM_IO_SERVICE_USED
    /* Create and add SRTM IO channel to peer core */
    rpmsgConfig.epName = APP_SRTM_IO_CHANNEL_NAME;
    chan               = SRTM_RPMsgEndpoint_Create(&rpmsgConfig);
    SRTM_PeerCore_AddChannel(core, chan);
#endif

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
    return CLOCK_GetRate(kCLOCK_pdm);
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
    saiTxConfig.config.fifo.fifoWatermark = FSL_FEATURE_SAI_FIFO_COUNTn(APP_SRTM_SAI) - 8;
    saiTxConfig.mclk                      = APP_SAI_CLK_FREQ;

#if defined(DEMO_SAI_TX_CONFIG_StopOnSuspend)
    saiTxConfig.stopOnSuspend = DEMO_SAI_TX_CONFIG_StopOnSuspend;
#else
    saiTxConfig.stopOnSuspend = false; /* Keep playing audio on APD suspend. */
#endif
    saiTxConfig.threshold = 1U; /* Every period transmitted triggers periodDone message to A core. */
    saiTxConfig.guardTime =
        4000; /* Unit:ms. This is a lower limit that M core should reserve such time data to wakeup A core. */
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
    pdmConfig.pdmSrcClk                 = CLOCK_GetRate(kCLOCK_pdm);;
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

#if SRTM_IO_SERVICE_USED
static void APP_HandleGPIOHander(uint16_t ioId);
/*
 * @brief Set pad control register
 * @param asInput    use gpio as input, unless use as output
 */
static void APP_IO_SetPinConfig(uint16_t ioId, bool asInput)
{
    uint8_t portIdx = APP_GPIO_IDX(ioId);
    uint8_t pinIdx  = APP_PIN_IDX(ioId);

    IOMUXC_SetPinConfig(suspendContext.io.data[portIdx][pinIdx].pinFuncId[0], suspendContext.io.data[portIdx][pinIdx].pinFuncId[1], suspendContext.io.data[portIdx][pinIdx].pinFuncId[2], suspendContext.io.data[portIdx][pinIdx].pinFuncId[3],
                        suspendContext.io.data[portIdx][pinIdx].pinFuncId[4], asInput ? (suspendContext.io.data[portIdx][pinIdx].inputMask) : (suspendContext.io.data[portIdx][pinIdx].outputMask));
}

static srtm_status_t APP_IO_ConfOutput(uint16_t ioId, srtm_io_value_t ioValue)
{
    uint8_t portIdx = APP_GPIO_IDX(ioId);
    uint8_t pinIdx  = APP_PIN_IDX(ioId);

    if (suspendContext.io.data[portIdx][pinIdx].p_gpioHandle != NULL)
    {
        APP_IO_SetPinConfig(ioId, false);

        HAL_GpioSetOutput(suspendContext.io.data[portIdx][pinIdx].p_gpioHandle, (uint8_t)ioValue);
        suspendContext.io.data[portIdx][pinIdx].direction = SRTM_IoDirectionOutput;
        return SRTM_Status_Success;
    }

    return SRTM_Status_Error;
}

static srtm_status_t APP_IO_SetOutput(srtm_service_t service,
                                      srtm_peercore_t core,
                                      uint16_t ioId,
                                      srtm_io_value_t ioValue)
{
    uint8_t portIdx = APP_GPIO_IDX(ioId);
    uint8_t pinIdx  = APP_PIN_IDX(ioId);

    if (suspendContext.io.data[portIdx][pinIdx].p_gpioHandle != NULL)
    {
        suspendContext.io.data[portIdx][pinIdx].value = (uint8_t)ioValue;

        return APP_IO_ConfOutput(ioId, ioValue);
    }
    return SRTM_Status_Error;
}

static srtm_status_t APP_IO_GetInput(srtm_service_t service,
                                     srtm_peercore_t core,
                                     uint16_t ioId,
                                     srtm_io_value_t *pIoValue)
{
    uint8_t portIdx = APP_GPIO_IDX(ioId);
    uint8_t pinIdx  = APP_PIN_IDX(ioId);
    uint8_t pinState;

    if (suspendContext.io.data[portIdx][pinIdx].p_gpioHandle != NULL)
    {
        HAL_GpioGetInput(suspendContext.io.data[portIdx][pinIdx].p_gpioHandle, &pinState);
        *pIoValue = pinState ? SRTM_IoValueHigh : SRTM_IoValueLow;

        return SRTM_Status_Success;
    }
    return SRTM_Status_Error;
}

static srtm_status_t APP_IO_GetDirection(srtm_service_t service,
                                     srtm_peercore_t core,
                                     uint16_t ioId,
                                     srtm_io_direction_t *pIoDir)
{
    uint8_t portIdx = APP_GPIO_IDX(ioId);
    uint8_t pinIdx  = APP_PIN_IDX(ioId);

    if (suspendContext.io.data[portIdx][pinIdx].p_gpioHandle != NULL)
    {
        *pIoDir = suspendContext.io.data[portIdx][pinIdx].direction;

        return SRTM_Status_Success;
    }
    return SRTM_Status_Error;
}

static srtm_status_t APP_IO_ConfInput(uint16_t ioId, srtm_io_event_t event, bool wakeup)
{
    uint8_t portIdx = APP_GPIO_IDX(ioId);
    uint8_t pinIdx  = APP_PIN_IDX(ioId);
    hal_gpio_pin_config_t config;

    APP_IO_SetPinConfig(ioId, true);
    /* Set gpio direction as input */
    config.direction = kHAL_GpioDirectionIn;
    config.port = portIdx;
    config.pin = pinIdx;
    HAL_GpioInit(suspendContext.io.data[portIdx][pinIdx].p_gpioHandle, &config);
    suspendContext.io.data[portIdx][pinIdx].direction = SRTM_IoDirectionInput;
    suspendContext.io.data[portIdx][pinIdx].ioId = ioId;
    HAL_GpioInstallCallback(suspendContext.io.data[portIdx][pinIdx].p_gpioHandle, (hal_gpio_callback_t)APP_HandleGPIOHander, (void *)&suspendContext.io.data[portIdx][pinIdx].ioId);

    switch (event)
    {
        case SRTM_IoEventRisingEdge:
	    HAL_GpioSetTriggerMode(suspendContext.io.data[portIdx][pinIdx].p_gpioHandle, kHAL_GpioInterruptRisingEdge);
	    suspendContext.io.data[portIdx][pinIdx].event = SRTM_IoEventRisingEdge;
            break;
        case SRTM_IoEventFallingEdge:
	    HAL_GpioSetTriggerMode(suspendContext.io.data[portIdx][pinIdx].p_gpioHandle, kHAL_GpioInterruptFallingEdge);
	    suspendContext.io.data[portIdx][pinIdx].event = SRTM_IoEventFallingEdge;
            break;
        case SRTM_IoEventEitherEdge:
	    HAL_GpioSetTriggerMode(suspendContext.io.data[portIdx][pinIdx].p_gpioHandle, kHAL_GpioInterruptEitherEdge);
	    suspendContext.io.data[portIdx][pinIdx].event = SRTM_IoEventEitherEdge;
            break;
        case SRTM_IoEventLowLevel:
	    HAL_GpioSetTriggerMode(suspendContext.io.data[portIdx][pinIdx].p_gpioHandle, kHAL_GpioInterruptLogicZero);
	    suspendContext.io.data[portIdx][pinIdx].event = SRTM_IoEventLowLevel;
            break;
        case SRTM_IoEventHighLevel:
	    HAL_GpioSetTriggerMode(suspendContext.io.data[portIdx][pinIdx].p_gpioHandle, kHAL_GpioInterruptLogicOne);
	    suspendContext.io.data[portIdx][pinIdx].event = SRTM_IoEventHighLevel;
            break;
        default:
	    HAL_GpioSetTriggerMode(suspendContext.io.data[portIdx][pinIdx].p_gpioHandle, kHAL_GpioInterruptDisable);
	    suspendContext.io.data[portIdx][pinIdx].event = SRTM_IoEventNone;
            break;
    }

    return SRTM_Status_Success;
}

static srtm_status_t APP_IO_ConfIEvent(
    srtm_service_t service, srtm_peercore_t core, uint16_t ioId, srtm_io_event_t event, bool wakeup)
{
    uint8_t portIdx = APP_GPIO_IDX(ioId);
    uint8_t pinIdx  = APP_PIN_IDX(ioId);

    if (suspendContext.io.data[portIdx][pinIdx].p_gpioHandle != NULL)
    {
        suspendContext.io.data[portIdx][pinIdx].event  = event;
        suspendContext.io.data[portIdx][pinIdx].wakeup = wakeup;

        return APP_IO_ConfInput(ioId, event, wakeup);
    }

    return SRTM_Status_Error;
}

static void APP_SRTM_InitIoDevice(void)
{
    uint8_t portIdx;
    uint8_t pinIdx;
    hal_gpio_pin_config_t config;

    /* Init io configuration */
    for (portIdx = 0; portIdx < APP_PORT_NUM; portIdx++)
    {
        for (pinIdx = 0; pinIdx < APP_PIN_NUM; pinIdx++)
        {
            if (suspendContext.io.data[portIdx][pinIdx].p_gpioHandle != NULL)
            {
                config.direction = (suspendContext.io.data[portIdx][pinIdx].direction == SRTM_IoDirectionInput) ? (kHAL_GpioDirectionIn) : (kHAL_GpioDirectionOut);
                config.port = suspendContext.io.data[portIdx][pinIdx].portIdx;
                config.pin = suspendContext.io.data[portIdx][pinIdx].pinIdx;
                config.level = suspendContext.io.data[portIdx][pinIdx].value;

                HAL_GpioInit(suspendContext.io.data[portIdx][pinIdx].p_gpioHandle, &config);
            }
        }
    }
}

static void APP_SRTM_InitIoService(void)
{
    uint16_t ioId = 0;
    uint8_t portIdx, pinIdx;

    /* Init IO structure used in the application. */
    APP_SRTM_InitIoDevice();

    ioService = SRTM_IoService_Create();
    for (portIdx = 0; portIdx < APP_PORT_NUM; portIdx++)
    {
        for (pinIdx = 0; pinIdx < APP_PIN_NUM; pinIdx++)
        {
            if (suspendContext.io.data[portIdx][pinIdx].p_gpioHandle != NULL)
            {
                ioId = (portIdx << 8) | pinIdx;
                SRTM_IoService_RegisterPin(ioService, ioId, APP_IO_SetOutput, APP_IO_GetInput, APP_IO_ConfIEvent, APP_IO_GetDirection, NULL);
            }
        }
    }

    SRTM_Dispatcher_RegisterService(disp, ioService);
}

static void APP_HandleGPIOHander(uint16_t ioId)
{
    SRTM_IoService_NotifyInputEvent(ioService, ioId);
}
#endif

static void APP_SRTM_InitServices(void)
{
#if SRTM_I2C_SERVICE_USED
    APP_SRTM_InitI2CService();
#endif
#if SRTM_NETC_SERVICE_USED
    APP_SRTM_InitNETCService();
#endif
#if SRTM_AUDIO_SERVICE_USED
    APP_SRTM_InitAudioService();
#endif
#if SRTM_IO_SERVICE_USED
    APP_SRTM_InitIoService();
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
