
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/power_mode_switch_rt10xx/bm/power_mode_switch.h
            examples/demo_apps/power_mode_switch_rt10xx/bm/lpm.c
            examples/demo_apps/power_mode_switch_rt10xx/bm/lpm.h
            examples/_boards/${board}/demo_apps/power_mode_switch/specific.c
            examples/_boards/${board}/demo_apps/power_mode_switch/specific.h
            examples/_boards/${board}/demo_apps/power_mode_switch/pin_mux.c
            examples/_boards/${board}/demo_apps/power_mode_switch/pin_mux.h
            examples/_boards/${board}/demo_apps/power_mode_switch/peripherals.c
            examples/_boards/${board}/demo_apps/power_mode_switch/peripherals.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/power_mode_switch_rt10xx/bm
             examples/_boards/${board}/demo_apps/power_mode_switch
)

mcux_add_macro(
    CC "-DHAL_UART_ADAPTER_FIFO=0"
)
mcux_add_macro(
    TOOLCHAINS iar mdk
    CC "-DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_debug flexspi_nor_release
    CC "-DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)
