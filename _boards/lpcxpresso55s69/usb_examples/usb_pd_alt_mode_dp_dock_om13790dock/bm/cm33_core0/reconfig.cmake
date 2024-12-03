
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/usb/example/boards/lpcxpresso55s69_om13790dock/usb_pd_alt_mode_dp_dock/bm/hardware_init.c
            middleware/usb/example/boards/lpcxpresso55s69_om13790dock/usb_pd_alt_mode_dp_dock/bm/pin_mux.c
            middleware/usb/example/boards/lpcxpresso55s69_om13790dock/usb_pd_alt_mode_dp_dock/bm/pin_mux.h
            middleware/usb/example/boards/lpcxpresso55s69_om13790dock/usb_pd_alt_mode_dp_dock/bm/VIF/displayport_dock.xml
            middleware/usb/example/boards/lpcxpresso55s69_om13790dock/usb_pd_alt_mode_dp_dock/bm/pd_board_config.h
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_app.h
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_app_demo.c
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_command_app.c
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_command_interface.c
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_command_interface.h
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_power_app.c
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_power_interface.c
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_power_interface.h
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_app_misc.h
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_dp_board_chip.c
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_dp_board_chip.h
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_crossbar_cbtl.h
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_crossbar_cbtl.c
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_ptn36502.h
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_ptn36502.c
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_dp_hpd_detect.h
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_dp_hpd_detect.c
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_power_nx20p3483.h
            middleware/usb/example/usb_pd_alt_mode_dp_dock_om13790dock/bm/pd_power_nx20p3483.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/usb/example/boards/lpcxpresso55s69_om13790dock/usb_pd_alt_mode_dp_dock/bm
             examples/_boards/lpcxpresso55s69_om13790dock/usb_examples/usb_pd_alt_mode_dp_dock/bm/${core_id}/..
)

mcux_add_macro(
    CC "-DI2C_RETRY_TIMES=40000"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000\
        --config_def=__heap_size__=0x1000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\"\
        --predefine=\"-D__heap_size__=0x1000\""
)
