#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/${board_root}/${board}/tfm_examples/reconfig.cmake OPTIONAL)

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

mcux_remove_iar_configuration(
    TARGETS debug
    CC "-O0"
)

mcux_remove_iar_configuration(
    TARGETS debug
    CC "-O1"
)

mcux_remove_iar_configuration(
    TARGETS debug release
    CX "-Oh"
    CC "-Oh --debug --no_cse --no_unroll --no_inline --no_code_motion --no_tbaa --no_clustering --no_scheduling -On"
)

mcux_add_iar_configuration(
    TARGETS debug release
    CC "-Ohz"
)