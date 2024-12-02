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
