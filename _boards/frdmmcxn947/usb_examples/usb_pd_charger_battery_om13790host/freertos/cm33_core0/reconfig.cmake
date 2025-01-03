
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/usb_examples/usb_pd_charger_battery_om13790host/freertos/hardware_init.c
            ${board_root}/${board}/usb_examples/usb_pd_charger_battery_om13790host/freertos/pin_mux.c
            ${board_root}/${board}/usb_examples/usb_pd_charger_battery_om13790host/freertos/pin_mux.h
            ${board_root}/${board}/usb_examples/usb_pd_charger_battery_om13790host/freertos/pd_board_config.h
            examples/usb_examples/usb_pd_charger_battery_om13790host/freertos/pd_app_demo.c
            examples/usb_examples/usb_pd_charger_battery_om13790host/freertos/pd_app.h
            examples/usb_examples/usb_pd_charger_battery_om13790host/freertos/pd_command_app.c
            examples/usb_examples/usb_pd_charger_battery_om13790host/freertos/pd_command_interface.c
            examples/usb_examples/usb_pd_charger_battery_om13790host/freertos/pd_command_interface.h
            examples/usb_examples/usb_pd_charger_battery_om13790host/freertos/pd_power_app.c
            examples/usb_examples/usb_pd_charger_battery_om13790host/freertos/pd_power_interface.c
            examples/usb_examples/usb_pd_charger_battery_om13790host/freertos/pd_power_interface.h
            examples/usb_examples/usb_pd_charger_battery_om13790host/freertos/pd_power_nx20p3483.c
            examples/usb_examples/usb_pd_charger_battery_om13790host/freertos/pd_power_nx20p3483.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/usb_examples/usb_pd_charger_battery_om13790host/freertos
)

mcux_add_macro(
    CC "-DSDK_OS_FREE_RTOS\
       -DI2C_RETRY_TIMES=40000"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x2000\
        -Xlinker --defsym=__heap_size__=0x1000"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000\
        --config_def=__heap_size__=0x100"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\"\
        --predefine=\"-D__heap_size__=0x100\""
)
