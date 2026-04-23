/*
 * Copyright 2023, 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
#include "display_support.h"
#include "pmic_support.h"
#include "fsl_device_registers.h"
#include "app.h"
#include "mpp_config.h"

#ifdef USE_USB_CAMERA
#include "usb_host_config.h"
#include "usb_host.h"
#include "usb_phy.h"
#endif /* USE_USB_CAMERA */

#ifdef MCMGR_USED
#include "mcmgr.h"
#include "fsl_cache.h"
#endif /* MCMGR_USED */

/*${header:end}*/

/*${function:start}*/

#ifdef USE_USB_CAMERA
extern usb_host_handle g_HostHandle;
void USB0_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_HostHandle);
}

void USB1_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_HostHandle);
}

void USB_HostClockInit(void)
{
    uint32_t usbClockFreq = 24000000;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    /* Power on COM VDDN domain for USB */
    POWER_DisablePD(kPDRUNCFG_DSR_VDDN_COM);

    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    {
        /* Power on usb ram array as need, powered USB0RAM array*/
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

void USB_HostTaskFn(void *param)
{
    USB_HostEhciTaskFunction(param);
}
#endif /* USE_USB_CAMERA */

#ifdef MCMGR_USED
#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void)
{
    uint32_t image_size;
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    image_size = (uint32_t)&Image$$CORE1_REGION$$Length;
#elif defined(__ICCARM__)
    image_size = (uint32_t)__section_end("__core1_image") - (uint32_t)__section_begin("__core1_image");
#elif defined(__GNUC__)
    image_size = (uint32_t)core1_image_size;
#endif
    return image_size;
}
#endif /* CORE1_IMAGE_COPY_TO_RAM */

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
void invalidate_cache_for_core1_image_memory(uint32_t address, uint32_t size_byte)
{
    XCACHE_CleanInvalidateCacheByRange(address, size_byte);
}
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY */
#endif /* MCMGR_USED */

#if (DEMO_PANEL_RM67162 == DEMO_PANEL)
void BOARD_MIPI_TE_GPIO_IRQ_Handler(void)
{
    uint32_t intStat;

    intStat = GPIO_PinGetInterruptFlag(BOARD_MIPI_TE_GPIO, BOARD_MIPI_TE_PIN);

    GPIO_GpioClearInterruptFlags(BOARD_MIPI_TE_GPIO, 1U << BOARD_MIPI_TE_PIN);

    if (intStat != 0U)
    {
        BOARD_DisplayTEPinHandler();
    }
}
#endif

void BOARD_Init(void)
{
    BOARD_ConfigMPU();
    BOARD_InitAHBSC();
    BOARD_InitBootPins();
    BOARD_InitPsRamPins_Xspi2();

    // Disable LDO
    POWER_SetVddnSupplySrc(kVddSrc_PMIC);
    POWER_SetVdd1SupplySrc(kVddSrc_PMIC);
    POWER_SetVdd2SupplySrc(kVddSrc_PMIC);
    POWER_ApplyPD();

    BOARD_InitPmicPins();
    BOARD_InitPmic();
    BOARD_SetPmicVdd2Voltage(1100000U); /* 1.1v for 325MHz clock. */

    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();
    BOARD_Init16bitsPsRam(XSPI2);

#if (defined(HAL_ENABLE_INFERENCE_TFLITE) && (HAL_ENABLE_INFERENCE_TFLITE == 1) && defined(APP_USE_NEUTRON64_MODEL))
    POWER_DisablePD(kPDRUNCFG_APD_NPU);
    POWER_DisablePD(kPDRUNCFG_PPD_NPU);
    POWER_ApplyPD();
#endif

#if (DEMO_PANEL_TFT_PROTO_5 == DEMO_PANEL)
#if (SSD1963_DRIVEN_BY == SSD1963_DRIVEN_BY_FLEXIO)
    BOARD_InitFlexIOPanelPins();
#else /* SSD1963_DRIVEN_BY_LCDIF */
    BOARD_InitLcdDBIPanelPins();
#endif

    CLOCK_EnableClock(kCLOCK_Gpio2);
    RESET_PeripheralReset(kGPIO2_RST_SHIFT_RSTn);
#else
#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
    BOARD_InitI2cPins();
#endif
    BOARD_InitMipiPanelPinsEvk();

    CLOCK_EnableClock(kCLOCK_Gpio1);
    RESET_PeripheralReset(kGPIO1_RST_SHIFT_RSTn);
#endif

    CLOCK_EnableClock(kCLOCK_Gpio2);
    RESET_PeripheralReset(kGPIO2_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Gpio3);
    RESET_PeripheralReset(kGPIO3_RST_SHIFT_RSTn);

#if (defined(HAL_ENABLE_JPEG_HW) && (HAL_ENABLE_JPEG_HW == 1))
    /* Disable JPEG decoder power down. */
    POWER_DisablePD(kPDRUNCFG_PPD_JPEGDEC);
    POWER_DisablePD(kPDRUNCFG_APD_JPEGDEC);
    POWER_ApplyPD();
#endif

    POWER_DisablePD(kPDRUNCFG_SHUT_SENSEP_MAINCLK);
    POWER_ApplyPD();
}

#ifdef MCMGR_USED
/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* Initialize MCMGR - low level multicore management library. Call this
       function as close to the reset entry as possible to allow CoreUp event
       triggering. The SystemInitHook() weak function overloading is used in this
       application. */
    (void)MCMGR_EarlyInit();
}
/*${function:end}*/
#endif /* MCMGR_USED */
