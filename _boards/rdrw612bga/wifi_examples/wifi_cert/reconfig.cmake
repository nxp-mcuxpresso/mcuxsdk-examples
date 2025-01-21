
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wifi_nxp/example/wifi_cert/lwip_config/lwipopts.h
            middleware/wifi_nxp/example/wifi_cert/lwip_config/lwippools.h
            middleware/wifi_nxp/example/wifi_cert/freertos_config/FreeRTOSConfig.h
            ${board_root}/${board}/wifi_examples/common/pin_mux.c
            ${board_root}/${board}/wifi_examples/common/pin_mux.h
            ${board_root}/${board}/wifi_examples/common/./hardware_init.c
            ${board_root}/${board}/wifi_examples/common/./app.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    PREINCLUDE TRUE
    SOURCES middleware/wifi_nxp/example/wifi_cert/wifi_config/wifi_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/wifi_examples/common
             middleware/wifi_nxp/example/wifi_cert/wifi_config
             middleware/wifi_nxp/example/wifi_cert/lwip_config
             middleware/wifi_nxp/example/wifi_cert/freertos_config
)

mcux_add_mdk_configuration(
    TARGETS debug
    CC "-O0"
    CX "-O0"
)
mcux_add_mdk_configuration(
    TARGETS flash_debug
    CC "-O0"
    CX "-O0"
    LD "--no_remove"
)
mcux_add_mdk_configuration(
    TARGETS flash_release
    LD "--no_remove"
)
mcux_add_iar_configuration(
    CX "--no_clustering"
    TARGETS flash_release
    CC "--no_inline"
)

mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DCONFIG_MONOLITHIC_WIFI=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)

mcux_remove_mdk_configuration(
    TARGETS debug
    CC "-O1"
    CX "-O1"
)
mcux_remove_mdk_configuration(
    TARGETS flash_debug
    CC "-O1"
    CX "-O1"
    LD "--remove"
)
mcux_remove_mdk_configuration(
    TARGETS flash_release
    LD "--remove"
)

mcux_remove_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS flash_release
    AS "-DNDEBUG"
    CC "-DNDEBUG"
    CX "-DNDEBUG"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/RW/RW612/gcc/RW612_ram.ld
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/Wireless/RW/RW612/gcc/RW612_flash.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/RW/RW612/iar/RW612_ram.icf
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/Wireless/RW/RW612/iar/RW612_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/RW/RW612/arm/RW612_ram.scf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/Wireless/RW/RW612/arm/RW612_flash.scf
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/wifi_examples/common/linker/RW610_flash.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/wifi_examples/common/linker/RW610_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/wifi_examples/common/linker/RW610_flash.scf
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x400\"\
        --predefine=\"-D__heap_size__=0x400\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x400\
        --config_def=__heap_size__=0x400"
)
