mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/hardware_init.c
            ${board_root}/${board}/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pin_mux.c
            ${board_root}/${board}/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pin_mux.h
            ${board_root}/${board}/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_board_config.h
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_app.h
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_app_demo.c
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_command_app.c
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_command_interface.c
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_command_interface.h
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_power_app.c
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_power_interface.c
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_power_interface.h
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_app_misc.h
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_dp_board_chip.c
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_dp_board_chip.h
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_crossbar_cbtl.h
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_crossbar_cbtl.c
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_ptn36502.h
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_ptn36502.c
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_dp_hpd_driver.h
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_dp_hpd_driver.c
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_power_nx20p3483.h
            examples/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm/pd_power_nx20p3483.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/usb_examples/usb_pd_alt_mode_dp_host_om13790host/bm
)

mcux_add_macro(
    CC "-DI2C_RETRY_TIMES=40000"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x2000\
        -Xlinker --defsym=__heap_size__=0x1000"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000\
        --config_def=__heap_size__=0x1000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\"\
        --predefine=\"-D__heap_size__=0x1000\""
)
