#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
include(${SdkRootDirPath}/${board_root}/${board}/el2go_examples/reconfig.cmake OPTIONAL)

mcux_add_macro(
     CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DOTP_NV_COUNTERS_RAM_EMULATION=1\
       -DITS_RAM_FS=1\
       -DPS_RAM_FS=1\
       -DPSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT\
       -DTFM_FIH_PROFILE_ON\
       -DTFM_FIH_PROFILE_MEDIUM\
       -DFIH_CFI_ALT\
       -DHARDENING_MACROS_ENABLED\
       -DPS_MAX_ASSET_SIZE=416\
       -DPS_STACK_SIZE=0xC00\
       "
)

# TF-M linker file preprocessing
mcux_set_list(
    TFM_LINKER_DEFINES_IMPORT "-D__ARM_ARCH_8M_MAIN__ -DCONFIG_TFM_USE_TRUSTZONE -DCONFIG_TFM_PARTITION_META -DENABLE_HEAP -DNS_HEAP_SIZE=0x00004000 -DNS_STACK_SIZE=0x00004000"
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
    TOOLCHAINS armgcc iar mdk
    TARGETS debug
    CC "-DDEBUG"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
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

mcux_remove_iar_configuration(
    TARGETS debug release
    CX "-Oh"
    CC "-Oh --debug --no_cse --no_unroll --no_inline --no_code_motion --no_tbaa --no_clustering --no_scheduling"
)

mcux_add_iar_configuration(
    TARGETS debug release
    CC "-Ohz"
)

mcux_add_iar_configuration(
    CC "--diag_suppress Pe188,Pe177,Pe186"
    CX "--diag_suppress Pe188,Pe177,Pe186"
)

