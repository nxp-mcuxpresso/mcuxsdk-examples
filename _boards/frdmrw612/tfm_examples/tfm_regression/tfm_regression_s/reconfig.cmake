#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/examples/_boards/${board}/tfm_examples/reconfig.cmake OPTIONAL)

#add cc-defines
mcux_add_macro(
     CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DOCOTP_NV_COUNTERS_RAM_EMULATION=1\
       -DTFM_FIH_PROFILE_ON\
       -DTFM_FIH_PROFILE_MEDIUM\
       -DFIH_CFI_ALT\
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
#mcux_remove_armgcc_configuration(
#    TARGETS debug
#    CC "-O0"
#)
mcux_add_armgcc_configuration(
    CC "-Wno-format"
)

#iar configurations
mcux_remove_iar_configuration(
    TARGETS debug
    CX "--diag_suppress=Pa082,Pa050"
    CC "--diag_suppress=Pa082,Pa050 -On"
)

mcux_add_iar_configuration(
    TARGETS debug
    CC "-Oh"
)
