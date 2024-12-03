
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/wifi_examples/wifi_serial_common/hardware_init.c
            examples/_boards/${board}/wifi_examples/wifi_serial_common/app.h
            examples/_boards/${board}/wifi_examples/wifi_serial_common/pin_mux.c
            examples/_boards/${board}/wifi_examples/wifi_serial_common/pin_mux.h
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/wifi_examples/wifi_serial_common
)
