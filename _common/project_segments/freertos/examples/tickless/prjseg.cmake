#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

if (CONFIG_MCUX_PRJSEG_ENABLE_FREERTOS_TICKLESS)
    # Intentionally kept empty
endif()

# Add unified tickless segment
if(CONFIG_MCUX_PRJSEG_FREERTOS_TICKLESS_HAL)
    mcux_add_source(
        SOURCES tickless_impl.c
                tickless_hal.h
                tickless_api.h
    )

    mcux_add_include(
        INCLUDES ./
    )

    # Add timer HAL implementation based on configuration
    if(CONFIG_MCUX_PRJSEG_TICKLESS_TIMER_GPT)
        mcux_add_source(
            SOURCES tickless_timer_gpt_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_TIMER_GPT"
            CX "CONFIG_TICKLESS_TIMER_GPT"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_TIMER_LPTMR)
        mcux_add_source(
            SOURCES tickless_timer_lptmr_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_TIMER_LPTMR"
            CX "CONFIG_TICKLESS_TIMER_LPTMR"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_TIMER_OSTIMER)
        mcux_add_source(
            SOURCES tickless_timer_ostimer_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_TIMER_OSTIMER"
            CX "CONFIG_TICKLESS_TIMER_OSTIMER"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_TIMER_RTC)
        mcux_add_source(
            SOURCES tickless_timer_rtc_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_TIMER_RTC"
            CX "CONFIG_TICKLESS_TIMER_RTC"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_TIMER_RTC_JDP)
        mcux_add_source(
            SOURCES tickless_timer_rtc_jdp_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_TIMER_RTC_JDP"
            CX "CONFIG_TICKLESS_TIMER_RTC_JDP"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_TIMER_RTC_LPC)
        mcux_add_source(
            SOURCES tickless_timer_rtc_lpc_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_TIMER_RTC_LPC"
            CX "CONFIG_TICKLESS_TIMER_RTC_LPC"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_TIMER_NONE)
        # Intentionally kept empty
    endif()

    # Add GPIO HAL implementation based on configuration
    if(CONFIG_MCUX_PRJSEG_TICKLESS_GPIO_STANDARD)
        mcux_add_source(
            SOURCES tickless_gpio_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_GPIO_STANDARD"
            CX "CONFIG_TICKLESS_GPIO_STANDARD"
        )
    endif()

    if (CONFIG_MCUX_PRJSEG_TICKLESS_GPIO_IGPIO)
        # Yes, this is same as CONFIG_MCUX_PRJSEG_TICKLESS_GPIO_STANDARD
        mcux_add_source(
            SOURCES tickless_gpio_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_GPIO_STANDARD"
            CX "CONFIG_TICKLESS_GPIO_STANDARD"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_GPIO_RGPIO)
        mcux_add_source(
            SOURCES tickless_gpio_rgpio_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_GPIO_RGPIO"
            CX "CONFIG_TICKLESS_GPIO_RGPIO"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_GPIO_SIUL2)
        mcux_add_source(
            SOURCES tickless_gpio_siul2_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_GPIO_SIUL2"
            CX "CONFIG_TICKLESS_GPIO_SIUL2"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_GPIO_LPC)
        mcux_add_source(
            SOURCES tickless_gpio_lpc_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_GPIO_LPC"
            CX "CONFIG_TICKLESS_GPIO_LPC"
        )
    endif()
endif()
