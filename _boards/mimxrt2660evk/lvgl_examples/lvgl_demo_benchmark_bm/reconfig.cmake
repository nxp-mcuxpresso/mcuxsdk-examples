# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/lvgl_examples/hardware_init.c
            ${board_root}/${board}/lvgl_examples/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/lvgl_examples
             ${board_root}/${board}
)

mcux_add_macro(
    CC "-DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x4000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x4000\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x4000"
)
