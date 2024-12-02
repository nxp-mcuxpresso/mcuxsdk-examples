
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/usb/example/boards/evkmimxrt1015_om13790host/usb_pd/bm/hardware_init.c
            middleware/usb/example/boards/evkmimxrt1015_om13790host/usb_pd/bm/pin_mux.c
            middleware/usb/example/boards/evkmimxrt1015_om13790host/usb_pd/bm/pin_mux.h
            middleware/usb/example/boards/evkmimxrt1015_om13790host/usb_pd/bm/VIF/consumer_provider.xml
            middleware/usb/example/boards/evkmimxrt1015_om13790host/usb_pd/bm/VIF/drp.xml
            middleware/usb/example/boards/evkmimxrt1015_om13790host/usb_pd/bm/VIF/drp_try_snk.xml
            middleware/usb/example/boards/evkmimxrt1015_om13790host/usb_pd/bm/VIF/drp_try_src.xml
            middleware/usb/example/boards/evkmimxrt1015_om13790host/usb_pd/bm/VIF/provider_consumer.xml
            middleware/usb/example/boards/evkmimxrt1015_om13790host/usb_pd/bm/pd_board_config.h
            middleware/usb/example/usb_pd_om13790host/bm/pd_app_demo.c
            middleware/usb/example/usb_pd_om13790host/bm/pd_app.h
            middleware/usb/example/usb_pd_om13790host/bm/pd_command_app.c
            middleware/usb/example/usb_pd_om13790host/bm/pd_command_interface.c
            middleware/usb/example/usb_pd_om13790host/bm/pd_command_interface.h
            middleware/usb/example/usb_pd_om13790host/bm/pd_power_app.c
            middleware/usb/example/usb_pd_om13790host/bm/pd_power_interface.c
            middleware/usb/example/usb_pd_om13790host/bm/pd_power_interface.h
            middleware/usb/example/usb_pd_om13790host/bm/pd_power_nx20p3483.c
            middleware/usb/example/usb_pd_om13790host/bm/pd_power_nx20p3483.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/usb/example/boards/evkmimxrt1015_om13790host/usb_pd/bm
)

mcux_add_macro(
    CC "-DI2C_RETRY_TIMES=40000"
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
