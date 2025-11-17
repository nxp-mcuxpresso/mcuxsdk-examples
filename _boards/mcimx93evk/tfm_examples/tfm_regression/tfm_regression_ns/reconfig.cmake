#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/${board_root}/${board}/tfm_examples/reconfig.cmake OPTIONAL)
include (${SdkRootDirPath}/drivers/lptmr/CMakeLists.txt OPTIONAL)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
    CX "-O0"
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-Os"
    CX "-Os"
)

mcux_remove_iar_configuration(
    TARGETS debug
    CC "-On"
)

mcux_add_iar_configuration(
    TARGETS debug
    CC "-Ohz"
)
