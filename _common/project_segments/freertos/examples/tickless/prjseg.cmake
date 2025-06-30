#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#


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
            SOURCES tickless_gpt_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_TIMER_GPT"
            CX "CONFIG_TICKLESS_TIMER_GPT"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_TIMER_LPTMR)
        mcux_add_source(
            SOURCES tickless_lptmr_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_TIMER_LPTMR"
            CX "CONFIG_TICKLESS_TIMER_LPTMR"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_TIMER_RTC)
        mcux_add_source(
            SOURCES tickless_rtc_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_TIMER_RTC"
            CX "CONFIG_TICKLESS_TIMER_RTC"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_TIMER_RTC_JDP)
        mcux_add_source(
            SOURCES tickless_rtc_jdp_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_TIMER_RTC_JDP"
            CX "CONFIG_TICKLESS_TIMER_RTC_JDP"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_TIMER_OSTIMER)
        mcux_add_source(
            SOURCES tickless_ostimer_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_TIMER_OSTIMER"
            CX "CONFIG_TICKLESS_TIMER_OSTIMER"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_TIMER_LPIT)
        mcux_add_source(
            SOURCES tickless_lpit_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_TIMER_LPIT"
            CX "CONFIG_TICKLESS_TIMER_LPIT"
        )
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

    if(CONFIG_MCUX_PRJSEG_TICKLESS_GPIO_IGPIO)
        mcux_add_source(
            SOURCES tickless_igpio_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_GPIO_IGPIO"
            CX "CONFIG_TICKLESS_GPIO_IGPIO"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_GPIO_RGPIO)
        mcux_add_source(
            SOURCES tickless_rgpio_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_GPIO_RGPIO"
            CX "CONFIG_TICKLESS_GPIO_RGPIO"
        )
    endif()

    if(CONFIG_MCUX_PRJSEG_TICKLESS_GPIO_SIUL2)
        mcux_add_source(
            SOURCES tickless_siul2_hal.c
        )
        mcux_add_macro(
            CC "CONFIG_TICKLESS_GPIO_SIUL2"
            CX "CONFIG_TICKLESS_GPIO_SIUL2"
        )
    endif()
endif()

if(CONFIG_MCUX_PRJSEG_freertos_tickless_lpc_rtc_segment)
    mcux_add_source(
        SOURCES fsl_tickless_rtc.c
                fsl_tickless_rtc.h
    )

    mcux_add_include(
        INCLUDES ./
    )
endif()


if(CONFIG_MCUX_PRJSEG_freertos_tickless_ostimer_segment)
    mcux_add_source(
        SOURCES fsl_tickless_ostimer.c
                fsl_tickless_ostimer.h
    )

    mcux_add_include(
        INCLUDES ./
    )
endif()

if(CONFIG_MCUX_PRJSEG_freertos_tickless_lptmr_portmax32_segment)
    mcux_add_source(
        SOURCES fsl_tickless_lptmr_portmax32.c
                fsl_tickless_lptmr_portmax32.h
    )

    mcux_add_include(
        INCLUDES ./
    )
endif()

if(CONFIG_MCUX_PRJSEG_freertos_tickless_lptmr_segment)
    mcux_add_source(
        SOURCES fsl_tickless_lptmr.c
                fsl_tickless_lptmr.h
    )

    mcux_add_include(
        INCLUDES ./
    )
endif()

if(CONFIG_MCUX_PRJSEG_freertos_tickless_lpit_segment)
    mcux_add_source(
        SOURCES fsl_tickless_lpit.c
                fsl_tickless_lpit.h
    )

    mcux_add_include(
        INCLUDES ./
    )
endif()

if(CONFIG_MCUX_PRJSEG_freertos_tickless_gpt_igpio_segment)
    mcux_add_source(
        SOURCES fsl_tickless_gpt.c
                fsl_tickless_gpt.h
    )

    mcux_add_include(
        INCLUDES ./
    )
endif()

if(CONFIG_MCUX_PRJSEG_freertos_tickless_epit_segment)
    mcux_add_source(
        SOURCES fsl_tickless_epit.c
                fsl_tickless_epit.h
    )

    mcux_add_include(
        INCLUDES ./
    )
endif()

if(CONFIG_MCUX_PRJSEG_freertos_tickless_qn_rtc_segment)
    mcux_add_source(
        SOURCES fsl_tickless_qn_rtc.c
    )
endif()

if(CONFIG_MCUX_PRJSEG_freertos_tickless_rgpio_gpt_segment)
    mcux_add_source(
        SOURCES fsl_tickless_gpt.c
                fsl_tickless_gpt.h
    )

    mcux_add_include(
        INCLUDES ./
    )
endif()
