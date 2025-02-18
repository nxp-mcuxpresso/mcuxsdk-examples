/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"

#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_adapter_uart.h"

#include "controller_hci_uart.h"
#include "usb_host_config.h"
#include "usb_phy.h"
#include "usb_host.h"
#include "fsl_xspi.h"
#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)))
#include "fsl_cache.h"
#endif /* CONFIG_BT_SMP */
#include "fsl_adapter_gpio.h"

#include "dsp_config.h"
#include "dsp_support.h"
#include "fsl_sema42.h"

/*${header:end}*/

/*${macro:start}*/
#define APP_SEMA42        SEMA42_4
#define SEMA_PRINTF_NUM   0
#define SEMA_STARTUP_NUM  1
#define SEMA_CORE_ID_CM33 0
#define SEMA_LOCKED_BY_DSP kSEMA42_LockedByProc4

#if defined(__GIC_PRIO_BITS)
#define USB_HOST_INTERRUPT_PRIORITY (25U)
#elif defined(__NVIC_PRIO_BITS) && (__NVIC_PRIO_BITS >= 3)
#define USB_HOST_INTERRUPT_PRIORITY (6U)
#else
#define USB_HOST_INTERRUPT_PRIORITY (3U)
#endif
/*${macro:end}*/

/*${variable:start}*/
GPIO_HANDLE_DEFINE(sync_signal_pin_handle);
static volatile uint32_t SyncSignal_Index = 0;

static struct rpmsg_lite_instance *ipc_rpmsg;
static struct rpmsg_lite_endpoint *ipc_rpmsg_ept;
static rpmsg_queue_handle ipc_rpmsg_queue;
static uint32_t ipc_rpmsg_dsp_ept_addr;
/*${variable:end}*/

/*${function:start}*/

static void sync_signal_pin_callback(void *param)
{
    SyncSignal_Index += 1;
}

static void BOARD_SyncSignal_Init(void)
{
    BOARD_InitSyncSignalPins();
    
    CLOCK_EnableClock(kCLOCK_Gpio0);

    hal_gpio_pin_config_t config;
    config.direction = kHAL_GpioDirectionIn;
    config.port      = 0;
    config.pin       = 7;
    config.level     = 1;
    HAL_GpioInit((hal_gpio_handle_t)sync_signal_pin_handle, &config);

    HAL_GpioInstallCallback((hal_gpio_handle_t)sync_signal_pin_handle, sync_signal_pin_callback, NULL);
}

void BOARD_SyncSignal_Start(uint32_t init_offset)
{
    SyncSignal_Index = 0;

    GPIO_PinClearInterruptFlag(GPIO0, 7U); /* A walk-around for fsl_adapter_gpio will triger once after trigger enabled. */
    HAL_GpioSetTriggerMode((hal_gpio_handle_t)sync_signal_pin_handle, kHAL_GpioInterruptRisingEdge);
}

void BOARD_SyncSignal_Stop(void)
{
    HAL_GpioSetTriggerMode((hal_gpio_handle_t)sync_signal_pin_handle, kHAL_GpioInterruptDisable);
}

uint32_t BOARD_SyncSignal_Count(void)
{
    return SyncSignal_Index;
}

void BOARD_Init_M2(void);
void BOARD_Init_BT_UART(void);

static void BOARD_DSP_IPC_Init(void)
{
    /* Set Hifi4 as Secure privileged master */
    GlikeyWriteEnable(GLIKEY0, 6U);
    AHBSC0->MASTER_SEC_LEVEL = 0x3;
    AHBSC0->MASTER_SEC_ANTI_POL_REG = 0xFFC;

    // CLOCK_EnableClock(kCLOCK_InputMux);
    /* Clear SEMA42 reset */
    RESET_PeripheralReset(kSEMA424_RST_SHIFT_RSTn);

    /* Clear MU4 reset before run DSP core */
    RESET_PeripheralReset(kMU4_RST_SHIFT_RSTn);

    /* SEMA42 init */
    SEMA42_Init(APP_SEMA42);
    /* Reset the sema42 gate */
    SEMA42_ResetAllGates(APP_SEMA42);
	
    /* Copy DSP image to RAM and start DSP core. */
    BOARD_DSP_Init();

    /* Wait for the DSP to lock the semaphore */
    while (SEMA_LOCKED_BY_DSP != SEMA42_GetGateStatus(APP_SEMA42, SEMA_STARTUP_NUM))
    {
    }

    /* DSP core init rpmsg remote. */

    /* Wait for the DSP to unlock the semaphore 1 */
    while (SEMA42_GetGateStatus(APP_SEMA42, SEMA_STARTUP_NUM))
    {
    }

    /* Initialize RPMsg IPC interface between ARM and DSP cores. */
    ipc_rpmsg       = rpmsg_lite_master_init((void *)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_SHMEM_SIZE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
    ipc_rpmsg_queue = rpmsg_queue_create(ipc_rpmsg);
    ipc_rpmsg_ept          = rpmsg_lite_create_ept(ipc_rpmsg, MCU_EPT_ADDR, rpmsg_queue_rx_cb, ipc_rpmsg_queue);
    ipc_rpmsg_dsp_ept_addr = DSP_EPT_ADDR;

    /* Now the DSP core rpmsg will ready to use. */
}

/* IPC send to dsp core, should be called in task context. */
int BOARD_DSP_IPC_Send(uint8_t *data, int size)
{
    int32_t status;
    
    status = rpmsg_lite_send(ipc_rpmsg, ipc_rpmsg_ept, ipc_rpmsg_dsp_ept_addr, (char *)data, size, RL_BLOCK);
    if (status != RL_SUCCESS)
    {
        return -1;
    }

    return 0;
}

/* IPC receive from dsp core, should be called in task context. */
int BOARD_DSP_IPC_Recv(uint8_t *data, int size)
{
    int32_t status;
    
    status = rpmsg_queue_recv(ipc_rpmsg, ipc_rpmsg_queue, NULL, (char *)data, size, NULL, RL_BLOCK);
    if (status != RL_SUCCESS)
    {
        return -1;
    }

    return 0;
}

void BOARD_InitHardware(void)
{
    BOARD_Init_M2();

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitAHBSC();

    BOARD_SyncSignal_Init();

    BOARD_DSP_IPC_Init();

    BOARD_Init_BT_UART();
#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)))
    GlikeyWriteEnable(GLIKEY3, 1U);                                    /* Enable SYSCON0_SEC_CLK_CTRL write */
    SYSCON0->SEC_CLK_CTRL |= SYSCON0_SEC_CLK_CTRL_TRNG_REFCLK_EN_MASK; /* Enable TRNG REF CLOCK */

    CLOCK_AttachClk(kFRO1_DIV2_to_TRNG);                               /* Max 96MHZ with 1.0V nomral supply. */
    CLOCK_SetClkDiv(kCLOCK_DivTrngClk, 1U);
#endif /* CONFIG_BT_SMP */
}

void BOARD_Init_M2(void)
{
    CLOCK_EnableClock(kCLOCK_Gpio3);
    CLOCK_EnableClock(kCLOCK_Gpio7);
    RESET_PeripheralReset(kGPIO3_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO7_RST_SHIFT_RSTn);
}

void BOARD_Init_BT_UART(void)
{
    CLOCK_AttachClk(BOARD_BT_UART_FCCLK_ATTACH);
    CLOCK_SetClkDiv(BOARD_BT_UART_FCCLK_DIV, 1U);

    /* Attach FC1 clock to LP_FLEXCOMM3(HCI uart). */
    CLOCK_AttachClk(BOARD_BT_UART_CLK_ATTACH);
}

#if (defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2) || defined(WIFI_IW416_BOARD_MURATA_1XK_M2)  || \
      defined(WIFI_IW612_BOARD_MURATA_2EL_M2))      
int controller_hci_uart_get_configuration(controller_hci_uart_config_t *config)
{
    if (NULL == config)
    {
        return -1;
    }
    config->clockSrc        = BOARD_BT_UART_CLK_FREQ;
    config->defaultBaudrate = 115200u;
    config->runningBaudrate = BOARD_BT_UART_BAUDRATE;
    config->instance        = BOARD_BT_UART_INSTANCE;
    config->enableRxRTS = 1u;
    config->enableTxCTS = 1u;
    return 0;
}
#endif

void USB_HostClockInit(void)
{
    uint32_t usbClockFreq = 24000000;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    /* Power on COM VDDN domain for USB or eUSB */
    POWER_DisablePD(kPDRUNCFG_DSR_VDDN_COM);    
        
    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    { 
        /* Power on usb ram araay as need, powered USB0RAM array*/
        POWER_DisablePD(kPDRUNCFG_APD_USB0_SRAM);
        POWER_DisablePD(kPDRUNCFG_PPD_USB0_SRAM);
        /* Apply the config */
        POWER_ApplyPD();
        /* disable the read and write gate */
        SYSCON4->USB0_MEM_CTRL |= (SYSCON4_USB0_MEM_CTRL_MEM_WIG_MASK | SYSCON4_USB0_MEM_CTRL_MEM_RIG_MASK |
                                     SYSCON4_USB0_MEM_CTRL_MEM_STDBY_MASK);
        /* Enable the USBPHY0 CLOCK */
        SYSCON4->USBPHY0_CLK_ACTIVE |= SYSCON4_USBPHY0_CLK_ACTIVE_IPG_CLK_ACTIVE_MASK;
        CLOCK_AttachClk(k32KHZ_WAKE_to_USB);
        CLOCK_AttachClk(kOSC_CLK_to_USB_24MHZ);
        CLOCK_EnableClock(kCLOCK_Usb0);
        CLOCK_EnableClock(kCLOCK_UsbphyRef);
        RESET_PeripheralReset(kUSB0_RST_SHIFT_RSTn);
        RESET_PeripheralReset(kUSBPHY0_RST_SHIFT_RSTn);
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);
        USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL_SYS_CLK_HZ, &phyConfig);
    }
}

void USB_HostIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbHOSTEhciIrq[] = USBHS_IRQS;
    irqNumber                = usbHOSTEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
/* USB_HOST_CONFIG_EHCI */

/* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNumber);
}

/*${function:end}*/
