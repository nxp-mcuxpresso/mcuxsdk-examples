
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/power_mode_switch_rt10xx/rtos/power_mode_switch.h
            examples/demo_apps/power_mode_switch_rt10xx/rtos/lpm.c
            examples/demo_apps/power_mode_switch_rt10xx/rtos/lpm.h
            ${board_root}/${board}/demo_apps/power_mode_switch/specific.c
            ${board_root}/${board}/demo_apps/power_mode_switch/specific.h
            ${board_root}/${board}/demo_apps/power_mode_switch/pin_mux.c
            ${board_root}/${board}/demo_apps/power_mode_switch/pin_mux.h
            ${board_root}/${board}/demo_apps/power_mode_switch/peripherals.c
            ${board_root}/${board}/demo_apps/power_mode_switch/peripherals.h
            examples/demo_apps/power_mode_switch_rt10xx/rtos/fsl_tickless_gpt.c
            examples/demo_apps/power_mode_switch_rt10xx/rtos/fsl_tickless_gpt.h
            ${board_root}/${board}/demo_apps/power_mode_switch/rtos/FreeRTOSConfig.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/power_mode_switch_rt10xx/rtos
             ${board_root}/${board}/demo_apps/power_mode_switch
)

mcux_add_macro(
    TOOLCHAINS iar mdk
    CC "-DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_debug
    CC "-DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_release
    CC "-DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1\
       -DKEEP_RCOSC_ON"
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)
