#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/${board_root}/${board}/tfm_examples/reconfig.cmake OPTIONAL)
include (${SdkRootDirPath}/drivers/lpuart/CMakeLists.txt OPTIONAL)
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
    TARGETS debug release
    CC "-O1"
)
mcux_add_mdk_configuration(
    TARGETS debug release
    CC "-O0"
)

#armgcc configurations
mcux_remove_macro(
    TOOLCHAINS armgcc
    TARGETS debug release
    CC "-DDEBUG"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS debug release
    CC "-DNDEBUG"
)
mcux_remove_armgcc_configuration(
    TARGETS debug release
    CC "-O0"
    CX "-O0"
)
mcux_add_armgcc_configuration(
    TARGETS debug release
    CC "-Os"
    CX "-Os"
)

#iar configurations
mcux_remove_iar_configuration(
    TARGETS debug
    CX "--diag_suppress=Pa082,Pa050"
    CC "--diag_suppress=Pa082,Pa050 -On"
)

#armgcc configurations
mcux_remove_iar_configuration(
    TOOLCHAINS iar
    TARGETS debug release
    CC "-DDEBUG"
)

mcux_add_iar_configuration(
    TARGETS debug release
    CC "-Ohz"
)
