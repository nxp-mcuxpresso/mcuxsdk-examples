
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/usb_examples/usb_pd_om13790host/bm/hardware_init.c
            ${board_root}/${board}/usb_examples/usb_pd_om13790host/bm/pin_mux.c
            ${board_root}/${board}/usb_examples/usb_pd_om13790host/bm/pin_mux.h
            ${board_root}/${board}/usb_examples/usb_pd_om13790host/bm/pd_board_config.h
            examples/usb_examples/usb_pd_om13790host/bm/pd_app_demo.c
            examples/usb_examples/usb_pd_om13790host/bm/pd_app.h
            examples/usb_examples/usb_pd_om13790host/bm/pd_command_app.c
            examples/usb_examples/usb_pd_om13790host/bm/pd_command_interface.c
            examples/usb_examples/usb_pd_om13790host/bm/pd_command_interface.h
            examples/usb_examples/usb_pd_om13790host/bm/pd_power_app.c
            examples/usb_examples/usb_pd_om13790host/bm/pd_power_interface.c
            examples/usb_examples/usb_pd_om13790host/bm/pd_power_interface.h
            examples/usb_examples/usb_pd_om13790host/bm/pd_power_nx20p3483.c
            examples/usb_examples/usb_pd_om13790host/bm/pd_power_nx20p3483.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/usb_examples/usb_pd_om13790host/bm
)

mcux_add_macro(
    CC "-DI2C_RETRY_TIMES=40000"
)
