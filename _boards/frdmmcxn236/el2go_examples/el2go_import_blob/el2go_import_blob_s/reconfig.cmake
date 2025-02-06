#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

#add cc-defines
mcux_add_macro(
    CC "OCOTP_NV_COUNTERS_RAM_EMULATION=1\
        PSA_WANT_ALG_SHA_1"
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
#armgcc configurations
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
)