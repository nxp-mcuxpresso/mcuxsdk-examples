#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/mbedtls/example/mbedtls_selftest_thread/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_iar_configuration(
    CX "--diag_suppress Pe188"
)
mcux_add_armgcc_configuration(
    CC "-Wno-int-in-bool-context"
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
       -DMBEDTLS_CONFIG_FILE=\\\"els_pkc_mbedtls_thread_config.h\\\"\
       -DFREESCALE_KSDK_BM"
)

mcux_remove_mdk_configuration(
    TARGETS debug flash_debug
    CC "-O1"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x8000\
        -Xlinker --defsym=__heap_size__=0x12000"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x8000\
        --config_def=__heap_size__=0x12000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x8000\"\
        --predefine=\"-D__heap_size__=0x12000\""
)
