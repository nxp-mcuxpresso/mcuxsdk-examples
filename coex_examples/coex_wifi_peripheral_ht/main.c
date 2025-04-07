/** @file main.c
 *
 *  @brief main file
 *
 *  Copyright 2008-2025 NXP
 *
 *  NXP CONFIDENTIAL
 *  The source code contained or described herein and all documents related to
 *  the source code ("Materials") are owned by NXP, its
 *  suppliers and/or its licensors. Title to the Materials remains with NXP,
 *  its suppliers and/or its licensors. The Materials contain
 *  trade secrets and proprietary and confidential information of NXP, its
 *  suppliers and/or its licensors. The Materials are protected by worldwide copyright
 *  and trade secret laws and treaty provisions. No part of the Materials may be
 *  used, copied, reproduced, modified, published, uploaded, posted,
 *  transmitted, distributed, or disclosed in any way without NXP's prior
 *  express written permission.
 *
 *  No license under any patent, copyright, trade secret or other intellectual
 *  property right is granted to or conferred upon you by disclosure or delivery
 *  of the Materials, either expressly, by implication, inducement, estoppel or
 *  otherwise. Any license under such intellectual property rights must be
 *  express and approved by NXP in writing.
 *
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////

// SDK Included Files
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"

#if !defined(RW612_SERIES)
#include "wifi.h"
#include "fsl_sai.h"
#include "fsl_sdmmc_host.h"
#endif /* RW612_SERIES */

#include "fsl_common.h"
#include "fsl_device_registers.h"

#include "app.h"

#include "BT_common.h"
#include "BT_version.h"
#include "BT_hci_api.h"

#include "fsl_gpio.h"
#if !defined(RW612_SERIES)
#include "fsl_iomuxc.h"
#include "fsl_gpc.h"
#include "fsl_lpuart_edma.h"
#include "fsl_dmamux.h"
#endif /* RW612_SERIES */

//#include "appl_utils.h"

#include "fsl_adapter_uart.h"
#include "usb_host_config.h"
#include "usb_host.h"

//#include "netif/ethernet.h"
//#include "ethernetif.h"
//#include "fsl_enet.h"
#include "app_config.h"

#ifdef RW612_SERIES
#if defined(MBEDTLS_NXP_SSSAPI)
#include "sssapi_mbedtls.h"
#elif defined(MBEDTLS_MCUX_CSS_API)
#include "platform_hw_ip.h"
#include "css_mbedtls.h"
#elif defined(MBEDTLS_MCUX_CSS_PKC_API)
#include "platform_hw_ip.h"
#include "css_pkc_mbedtls.h"
#elif defined(MBEDTLS_MCUX_ELS_PKC_API)
#include "platform_hw_ip.h"
#include "els_pkc_mbedtls.h"
#elif defined(MBEDTLS_MCUX_ELS_API)
#include "platform_hw_ip.h"
#include "els_mbedtls.h"
#elif defined(MBEDTLS_MCUX_ELE_S400_API)
#include "ele_mbedtls.h"
#else
#ifdef CONFIG_KSDK_MBEDTLS
#include "ksdk_mbedtls.h"
#endif
#endif

#if defined(APP_LOWPOWER_ENABLED) && (APP_LOWPOWER_ENABLED > 0)
#include "PWR_Interface.h"
#endif /* APP_LOWPOWER_ENABLED */
#endif /* RW612_SERIES */

#ifndef CONFIG_WIFI_BLE_COEX_APP
#define CONFIG_WIFI_BLE_COEX_APP 1 // needs to define CONFIG_WIFI_BLE_COEX_APP with value, 0 for disable Wi-Fi, 1 for enable Wi-Fi
#endif

#ifndef CONFIG_DISABLE_BLE
#define CONFIG_DISABLE_BLE 0 // needs to define CONFIG_DISABLE_BLE with value, 0 for enable BLE, 1 for disable BLE
#endif

#if (defined(HPS) || defined(IPSPR))
#include "fsl_phyksz8081.h"
#endif /* HPS | IPSPR */

#include "coex.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

const int TASK_MAIN_PRIO       = (configMAX_PRIORITIES-5);
const int TASK_MAIN_STACK_SIZE = (2 * 1024);
TaskHandle_t task_main_handle;

#ifdef A2DP_SINK
TaskHandle_t audio_task_handler;
OSA_SEMAPHORE_HANDLE_DEFINE(xSemaphoreAudio);
#endif /* A2DP_SINK */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#ifdef A2DP_SINK
extern void (* a2dp_snk_cb)(UCHAR *data, UINT16 datalen);
#endif /* A2DP_SINK */
extern void BOARD_InitHardware(void);
extern void coex_controller_init();
extern void otSysRunIdleTask(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

#ifdef RW612_SERIES
#if defined(configUSE_TICKLESS_IDLE) && (configUSE_TICKLESS_IDLE == 1)
/* Tickless idle is allowed by default but can be disabled runtime with APP_SetTicklessIdle */
static int ticklessIdleAllowed = 1;
#endif
#endif

/* Ethernet configuration. */
#if (defined(HPS) || defined(IPSPR))
phy_ksz8081_resource_t g_phy_resource;
#define EXAMPLE_ENET ENET
#endif /* HPS | IPSPR */

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (defined(HPS) || defined(IPSPR))
void BOARD_InitModuleClock(void)
{
    /* Set 50MHz output clock required by PHY. */
    CLOCK_EnableClock(kCLOCK_TddrMciEnetClk);
}

static void MDIO_Init(void)
{
    uint32_t i = ENET_GetInstance(EXAMPLE_ENET);

    (void)CLOCK_EnableClock(s_enetClock[i]);
    (void)CLOCK_EnableClock(s_enetExtraClock[i]);
    ENET_SetSMI(EXAMPLE_ENET, CLOCK_GetMainClkFreq(), false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET, phyAddr, regAddr, pData);
}
#endif /* HPS | IPSPR */

#ifdef A2DP_SINK
void AudioTask(void *handle)
{
    OSA_SemaphoreCreate(xSemaphoreAudio, 0);
    while (1U)
    {
        OSA_SemaphoreWait(xSemaphoreAudio, osaWaitForever_c);
        if(NULL != a2dp_snk_cb)
        {
            a2dp_snk_cb(NULL, 0);
        }

    }
}
#endif /* A2DP_SINK */

static void printSeparator(void)
{
    PRINTF("========================================\r\n");
}

void task_main(void *param)
{
    printSeparator();
#if defined(RW612_SERIES)
    PRINTF("     Initialize RW612 Module\r\n");
#elif defined(WIFI_IW610_BOARD_MURATA_2LL_M2)
    PRINTF("     Initialize Nighthwak-2LL (IW610) M2 Module\r\n");
#endif
    printSeparator();

    coex_cli_init();
    coex_controller_init();

#if(CONFIG_DISABLE_BLE == 0)
    extern int USB_HostMsdFatfsInit(void);

#if (defined(CONFIG_BT_SNOOP) && (CONFIG_BT_SNOOP > 0))
    (void)USB_HostMsdFatfsInit();
#endif
#endif

#if ((CONFIG_WIFI_BLE_COEX_APP == 1) || (CONFIG_OT_CLI == 1))
    coex_menuPrint();
    while (1)
    {
        int ch = pollChar();
        if (ch != -1)
        {
            coex_menuAction(ch);
        }
    }
#elif ((CONFIG_WIFI_BLE_COEX_APP == 0) && (CONFIG_DISABLE_BLE == 0))
    appl_main();

    while(1)
    {
        OSA_TimeDelay(1000);
    }
#endif
}

#if defined (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK) \
        && (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK == 1U)
void stackOverflowHookHandler(void* task_name)
{
    printf("stack-overflow exception from task: %s\r\n",(char*)task_name);
}
#endif /* #if defined (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK) && (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK == 1U) */


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

int main(void)
{
    BaseType_t result = 0;
    (void)result;

    extern void BOARD_InitHardware(void);    /*fix build warning: function declared implicitly.*/
    BOARD_InitHardware();

#if (defined(HPS) || defined(IPSPR))
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 1U};

    BOARD_InitModuleClock();
    ENET_ResetHardware();

    GPIO_PortInit(GPIO, 0U);
    GPIO_PortInit(GPIO, 1U);
    GPIO_PinInit(GPIO, 0U, 21U, &gpio_config); /* ENET_RST */
    gpio_config.pinDirection = kGPIO_DigitalInput;
    gpio_config.outputLogic  = 0U;
    GPIO_PinInit(GPIO, 1U, 23U, &gpio_config); /* ENET_INT */

    GPIO_PinWrite(GPIO, 0U, 21U, 0U);
    SDK_DelayAtLeastUs(1000000, CLOCK_GetCoreSysClkFreq());
    GPIO_PinWrite(GPIO, 0U, 21U, 1U);

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
#endif /* HPS | IPSPR */

#ifdef RW612_SERIES
    CRYPTO_InitHardware();

    extern void APP_InitServices(void);
    APP_InitServices();

#ifdef OOB_WAKEUP
    Configure_H2C_gpio();
    C2H_sleep_gpio_cfg();
#endif
#endif /* RW612_SERIES */

    printSeparator();
    PRINTF("        Coex APP\r\n");
    printSeparator();

    result =
        xTaskCreate(task_main, "main", TASK_MAIN_STACK_SIZE, NULL, TASK_MAIN_PRIO, &task_main_handle);
    assert(pdPASS == result);

#ifdef A2DP_SINK
    result =
        xTaskCreate(AudioTask, "audio", TASK_MAIN_STACK_SIZE, NULL, BT_TASK_PRIORITY, &audio_task_handler);
    assert(pdPASS == result);
#endif

#if defined (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK) \
        && (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK == 1U)
    EM_register_sof_handler(stackOverflowHookHandler);
#endif /* #if defined (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK) && (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK == 1U) */

    vTaskStartScheduler();
    for (;;)
    {
        ;
    }
}


#ifdef RW612_SERIES
void vApplicationIdleHook(void)
{
#if(CONFIG_OT_CLI)
    otSysRunIdleTask();
#endif
}

#if defined(configUSE_TICKLESS_IDLE) && (configUSE_TICKLESS_IDLE == 1)
void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
#if defined(APP_LOWPOWER_ENABLED) && (APP_LOWPOWER_ENABLED > 0)
    bool abortIdle = false;
    uint64_t expectedIdleTimeUs, actualIdleTimeUs;

    if(ticklessIdleAllowed > 0)
    {
        uint32_t irqMask = DisableGlobalIRQ();

        /* Disable and prepare systicks for low power */
        abortIdle = PWR_SysticksPreProcess((uint32_t)xExpectedIdleTime, &expectedIdleTimeUs);

        if (abortIdle == false)
        {
            /* Enter low power with a maximal timeout */
            actualIdleTimeUs = PWR_EnterLowPower(expectedIdleTimeUs);

            /* Re enable systicks and compensate systick timebase */
            PWR_SysticksPostProcess(expectedIdleTimeUs, actualIdleTimeUs);
        }

        /* Exit from critical section */
        EnableGlobalIRQ(irqMask);
    }
    else
    {
        /* Tickless idle is not allowed, wait for next tick interrupt */
        __WFI();
    }
#endif /* APP_LOWPOWER_ENABLED */
}

void APP_SetTicklessIdle(bool enable)
{
    if(enable == true)
    {
        ticklessIdleAllowed++;
    }
    else
    {
        ticklessIdleAllowed--;
    }
}

#endif /* configUSE_TICKLESS_IDLE */
#endif /* RW612_SERIES*/

#ifndef APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK
/**
 * @brief Loop forever if stack overflow is detected.
 *
 * If configCHECK_FOR_STACK_OVERFLOW is set to 1,
 * this hook provides a location for applications to
 * define a response to a stack overflow.
 *
 * Use this hook to help identify that a stack overflow
 * has occurred.
 *
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName)
{
    PRINTF("ERROR: stack overflow on task %s.\r\n", pcTaskName);

    portDISABLE_INTERRUPTS();

    /* Unused Parameters */
    (void)xTask;
    (void)pcTaskName;

    /* Loop forever */
    for (;;)
    {
    }
}
#endif

#ifndef __GNUC__
void __assert_func(const char *file, int line, const char *func, const char *failedExpr)
{
    PRINTF("ASSERT ERROR \" %s \": file \"%s\" Line \"%d\" function name \"%s\" \n", failedExpr, file, line, func);
    for (;;)
    {
        __BKPT(0);
    }
}
#endif
