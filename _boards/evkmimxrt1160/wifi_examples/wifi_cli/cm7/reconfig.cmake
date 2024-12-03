
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/wifi_examples/wifi_cli/wifi_config.h
            examples/_boards/${board}/wifi_examples/wifi_cli/lwipopts.h
            examples/_boards/${board}/wifi_examples/common/pin_mux.c
            examples/_boards/${board}/wifi_examples/common/pin_mux.h
            examples/_boards/${board}/wifi_examples/wifi_cli/lwippools.h
            examples/_boards/${board}/wifi_examples/wifi_cli/lwiphooks.h
            examples/_boards/${board}/wifi_examples/wifi_cli/FreeRTOSConfig.h

            examples/_boards/${board}/wifi_examples/common/${core_id}/hardware_init.c
            examples/_boards/${board}/wifi_examples/common/${core_id}/app.h
            examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
            examples/_boards/${board}/wifi_bt_config.c
            examples/_boards/${board}/wifi_bt_config.h
            examples/_boards/${board}/wifi_examples/common/app_config/app_config.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/wifi_examples/wifi_cli
             examples/_boards/${board}/wifi_examples/common
             examples/_boards/${board}/wifi_examples/common/${core_id}
             examples/_boards/${board}/wifi_examples/common/app_config
)

mcux_project_remove_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wifi_nxp/example/wifi_cli
)

mcux_add_iar_configuration(
    CX "--no_clustering"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_release flexspi_nor_debug
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)
mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1160/MIMXRT1166/gcc/MIMXRT1166xxxxx_cm7_flexspi_nor.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1160/MIMXRT1166/iar/MIMXRT1166xxxxx_cm7_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1160/MIMXRT1166/arm/MIMXRT1166xxxxx_cm7_flexspi_nor.scf
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT1166xxxxx_cm7_flexspi_nor.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT1166xxxxx_cm7_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT1166xxxxx_cm7_flexspi_nor.scf
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x400\
        --config_def=__heap_size__=0x400"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x400\"\
        --predefine=\"-D__heap_size__=0x400\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)
