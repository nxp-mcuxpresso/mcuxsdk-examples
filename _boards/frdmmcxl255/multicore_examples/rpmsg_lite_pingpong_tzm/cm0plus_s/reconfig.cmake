#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# Override heap size
mcux_remove_linker_symbol(
    SYMBOLS "__heap_size__=0x2000"
)
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x600"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/${core_id}_s/hardware_init.c
            ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/${core_id}_s/app.h
)
mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/${core_id}_s)

mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/${core_id}_s)

