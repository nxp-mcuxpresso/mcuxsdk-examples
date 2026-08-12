#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
include(${SdkRootDirPath}/${board_root}/${board}/el2go_examples/reconfig.cmake OPTIONAL)

#armgcc configurations
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
mcux_add_armgcc_configuration(
    CC "-Wno-unused-function"
)

mcux_add_armgcc_configuration(
    CC "-Wunused-variable"
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
mcux_add_mdk_configuration(
    CC "-Wno-typedef-redefinition"
)

# iar configurations:
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

mcux_add_iar_configuration(
    CC "--diag_suppress Pe188,Pe177,Pe186"
    CX "--diag_suppress Pe188,Pe177,Pe186"
)

# TF-M linker file preprocessing
mcux_add_macro(
     CC "-DNS_HEAP_SIZE=0x00004000\
         -DNS_STACK_SIZE=0x00008000"
)

mcux_set_list(
    TFM_LINKER_DEFINES_IMPORT "-DNS_HEAP_SIZE=0x00004000 -DNS_STACK_SIZE=0x00004000"
)
