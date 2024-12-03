
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/usb/example/boards/evkmimxrt1160_om13790host/usb_pd_alt_mode_dp_host/bm/hardware_init.c
            middleware/usb/example/boards/evkmimxrt1160_om13790host/usb_pd_alt_mode_dp_host/bm/pin_mux.c
            middleware/usb/example/boards/evkmimxrt1160_om13790host/usb_pd_alt_mode_dp_host/bm/pin_mux.h
            middleware/usb/example/boards/evkmimxrt1160_om13790host/usb_pd_alt_mode_dp_host/bm/VIF/displayport_host.xml
            middleware/usb/example/boards/evkmimxrt1160_om13790host/usb_pd_alt_mode_dp_host/bm/pd_board_config.h
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_app.h
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_app_demo.c
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_command_app.c
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_command_interface.c
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_command_interface.h
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_power_app.c
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_power_interface.c
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_power_interface.h
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_app_misc.h
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_dp_board_chip.c
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_dp_board_chip.h
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_crossbar_cbtl.h
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_crossbar_cbtl.c
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_ptn36502.h
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_ptn36502.c
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_dp_hpd_driver.h
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_dp_hpd_driver.c
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_power_nx20p3483.h
            middleware/usb/example/usb_pd_alt_mode_dp_host_om13790host/bm/pd_power_nx20p3483.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/usb/example/boards/evkmimxrt1160_om13790host/usb_pd_alt_mode_dp_host/bm
             examples/_boards/evkmimxrt1160_om13790host/usb_examples/usb_pd_alt_mode_dp_host/bm/${core_id}/..
)

mcux_add_macro(
    CC "-DDEBUG_CONSOLE_ASSERT_DISABLE=1\
       -DI2C_RETRY_TIMES=40000"
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
