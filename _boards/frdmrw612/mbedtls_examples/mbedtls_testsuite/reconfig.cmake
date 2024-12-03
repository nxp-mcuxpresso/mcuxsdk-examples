#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_iar_configuration(
    CX "--diag_suppress Pe188"
)
mcux_add_armgcc_configuration(
    CC "-Wno-int-in-bool-context\
       -fno-strict-aliasing"
)
mcux_add_mdk_configuration(
    CC "-mno-unaligned-access"
)
mcux_add_mdk_configuration(
    TARGETS debug flash_debug
    CC "-Oz"
)

mcux_add_macro(
    CC "-DMCUX_ENABLE_TRNG_AS_ENTROPY_SEED\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DFREESCALE_KSDK_BM"
)

mcux_remove_mdk_configuration(
    TARGETS debug flash_debug
    CC "-O1"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x10000\
        -Xlinker --defsym=__heap_size__=0xc000"
)
