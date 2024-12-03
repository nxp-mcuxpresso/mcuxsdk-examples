
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/wifi_examples/common/usb_device_board_config.h
            examples/_boards/${board}/wifi_examples/common/hardware_init.c
            examples/_boards/${board}/wifi_examples/common/app.h
            examples/_boards/${board}/wifi_examples/common/pin_mux.c
            examples/_boards/${board}/wifi_examples/common/pin_mux.h
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/wifi_examples/common
)



# Add additional configuration
mcux_add_macro(
    CC "-DFRDM_K32L2A4S\
       -DFREEDOM"
)
