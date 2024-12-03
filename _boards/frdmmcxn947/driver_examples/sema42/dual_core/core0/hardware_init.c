/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/

/*${function:start}*/
void APP_InitDomain(void)
{
}

void APP_DeinitDomain(void)
{
}

uint8_t APP_GetCore0DomainID(void)
{
    return 1U;
}
void APP_InitInterCoreNotifications(void)
{
    /* Init Mailbox */
    MAILBOX_Init(MAILBOX);
}
uint32_t APP_GetInterCoreNotificationsData(void)
{
    return MAILBOX_GetValue(MAILBOX, PRIMARY_CORE_MAILBOX_CPU_ID);
}
void APP_SetInterCoreNotificationsData(uint32_t data)
{
    MAILBOX_SetValue(MAILBOX, SECONDARY_CORE_MAILBOX_CPU_ID, data);
}

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void)
{
    uint32_t image_size;
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    image_size = (uint32_t)&Image$$CORE1_REGION$$Length;
#elif defined(__ICCARM__)
#pragma section = "__core1_image"
    image_size = (uint32_t)__section_end("__core1_image") - (uint32_t)&core1_image_start;
#elif defined(__GNUC__)
    image_size = (uint32_t)core1_image_size;
#endif
    return image_size;
}
#endif

void APP_BootCore1(void)
{
    /* Boot source for Core 1 from flash */
    SYSCON->CPBOOT = (CORE1_BOOT_ADDRESS & SYSCON_CPBOOT_CPBOOT_MASK);

    int32_t temp = SYSCON->CPUCTRL;
    temp |= 0xc0c48000;
    SYSCON->CPUCTRL = temp | SYSCON_CPUCTRL_CPU1RSTEN_MASK | SYSCON_CPUCTRL_CPU1CLKEN_MASK;
    SYSCON->CPUCTRL = (temp | SYSCON_CPUCTRL_CPU1CLKEN_MASK) & (~SYSCON_CPUCTRL_CPU1RSTEN_MASK);

    /* Wait a little to allow Core 1 boot up */
    SDK_DelayAtLeastUs(1000000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Gpio0);

    // GPIO_EnablePinControlNonSecure(BOARD_LED_RED_GPIO, (1 << BOARD_LED_RED_GPIO_PIN));
    // GPIO_EnablePinControlNonPrivilege(BOARD_LED_RED_GPIO, (1 << BOARD_LED_RED_GPIO_PIN));
}
/*${function:end}*/
