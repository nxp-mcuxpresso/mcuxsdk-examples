#
# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include (${SdkRootDirPath}/${board_root}/${board}/tfm_examples/reconfig.cmake OPTIONAL)
include (${SdkRootDirPath}/drivers/lpflexcomm/CMakeLists.txt OPTIONAL)
include (${SdkRootDirPath}/drivers/lpflexcomm/lpuart/CMakeLists.txt OPTIONAL)
include (${SdkRootDirPath}/drivers/gpio/CMakeLists.txt OPTIONAL)

mcux_add_include(
        INCLUDES examples/_boards/${board}/
        BASE_PATH ${SdkRootDirPath}
    )

#add cc-defines
mcux_add_macro(
     CC "-DPLATFORM_DEFAULT_NV_COUNTERS\
       "
)

#mdk configurations:
mcux_remove_mdk_configuration(
    TARGETS debug
    CC "-O1"
    CX "-O1"
)
mcux_add_mdk_configuration(
    TARGETS debug
    CC "-Oz"
    CX "-Oz"
)

#armgcc configurations
mcux_remove_macro(
    TOOLCHAINS armgcc
    TARGETS debug
    CC "-DDEBUG"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS debug
    CC "-DNDEBUG"
)
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

#iar configurations
mcux_remove_iar_configuration(
    TARGETS debug
    CX "--diag_suppress=Pa082,Pa050"
    CC "--diag_suppress=Pa082,Pa050 -On"
)

mcux_add_iar_configuration(
    TARGETS debug release
    CC "-Oh"
)
