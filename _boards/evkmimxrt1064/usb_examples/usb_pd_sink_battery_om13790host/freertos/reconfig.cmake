
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/usb/example/boards/evkmimxrt1064_om13790host/usb_pd_sink_battery/freertos/hardware_init.c
            middleware/usb/example/boards/evkmimxrt1064_om13790host/usb_pd_sink_battery/freertos/pin_mux.c
            middleware/usb/example/boards/evkmimxrt1064_om13790host/usb_pd_sink_battery/freertos/pin_mux.h
            middleware/usb/example/boards/evkmimxrt1064_om13790host/usb_pd_sink_battery/freertos/pd_board_config.h
            middleware/usb/example/usb_pd_sink_battery_om13790host/freertos/pd_app_demo.c
            middleware/usb/example/usb_pd_sink_battery_om13790host/freertos/pd_app.h
            middleware/usb/example/usb_pd_sink_battery_om13790host/freertos/pd_command_app.c
            middleware/usb/example/usb_pd_sink_battery_om13790host/freertos/pd_command_interface.c
            middleware/usb/example/usb_pd_sink_battery_om13790host/freertos/pd_command_interface.h
            middleware/usb/example/usb_pd_sink_battery_om13790host/freertos/pd_power_app.c
            middleware/usb/example/usb_pd_sink_battery_om13790host/freertos/pd_power_interface.c
            middleware/usb/example/usb_pd_sink_battery_om13790host/freertos/pd_power_interface.h
            middleware/usb/example/usb_pd_sink_battery_om13790host/freertos/pd_power_nx20p3483.c
            middleware/usb/example/usb_pd_sink_battery_om13790host/freertos/pd_power_nx20p3483.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/usb/example/boards/evkmimxrt1064_om13790host/usb_pd_sink_battery/freertos
             middleware/usb/config/freertos
)

mcux_add_macro(
    CC "-DSDK_OS_FREE_RTOS\
       -DDEBUG_CONSOLE_ASSERT_DISABLE=1\
       -DI2C_RETRY_TIMES=40000"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS flexspi_nor_release sdram_release
    CC "-DNDEBUG\
       -D_DEBUG=0"
    CX "-DNDEBUG"
    AS "-DNDEBUG"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x800\
        -Xlinker --defsym=__heap_size__=0x1000"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x800\
        --config_def=__heap_size__=0x1000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x800\"\
        --predefine=\"-D__heap_size__=0x1000\""
)
