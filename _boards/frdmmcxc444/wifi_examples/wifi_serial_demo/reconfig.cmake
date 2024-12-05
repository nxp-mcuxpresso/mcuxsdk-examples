
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/wifi_examples/common/hardware_init.c
            ${board_root}/${board}/wifi_examples/common/app.h
            ${board_root}/${board}/wifi_examples/common/pin_mux.c
            ${board_root}/${board}/wifi_examples/common/pin_mux.h
            ${board_root}/${board}/FlashKLxx256KROM_with_config_write_enabled.board
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/wifi_examples/common
)



# Add additional configuration
mcux_add_macro(
    CC "-DFRDM_MCXC444\
       -DFREEDOM"
)
