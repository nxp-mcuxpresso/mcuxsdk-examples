
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wifi_nxp/example/wifi_test_mode/lwip_config/lwipopts.h
            middleware/wifi_nxp/example/wifi_test_mode/lwip_config/lwippools.h
            examples/_boards/${board}/wifi_examples/common/lpm/lpm.c
            examples/_boards/${board}/wifi_examples/common/lpm/lpm.h
            examples/_boards/${board}/wifi_examples/common/lpm/host_sleep.c
            examples/_boards/${board}/wifi_examples/common/lpm/host_sleep.h
            middleware/wifi_nxp/example/wifi_test_mode/freertos_config/FreeRTOSConfig.h
            examples/_boards/${board}/wifi_examples/common/pin_mux.c
            examples/_boards/${board}/wifi_examples/common/pin_mux.h
            examples/_boards/${board}/wifi_examples/common/./hardware_init.c
            examples/_boards/${board}/wifi_examples/common/./app.h
            examples/_boards/${board}/wifi_examples/common/app_config/app_config.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    PREINCLUDE TRUE
    SOURCES middleware/wifi_nxp/example/wifi_test_mode/wifi_config/wifi_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/wifi_examples/common/lpm/
             examples/_boards/${board}/wifi_examples/common
             middleware/wifi_nxp/example/wifi_test_mode/wifi_config
             examples/_boards/${board}/wifi_examples/common/app_config
             middleware/wifi_nxp/example/wifi_test_mode/lwip_config
             middleware/wifi_nxp/example/wifi_test_mode/freertos_config
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

mcux_remove_macro(
    TOOLCHAINS armgcc iar
    TARGETS flash_release
    AS "-DNDEBUG"
    CC "-DNDEBUG"
    CX "-DNDEBUG"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/RW/RW612/gcc/RW612_ram.ld
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/Wireless/RW/RW612/gcc/RW612_flash.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/RW/RW612/iar/RW612_ram.icf
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/Wireless/RW/RW612/iar/RW612_flash.icf
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/RW610_flash.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/RW610_flash.icf
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x400\
        --config_def=__heap_size__=0x400"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x400\"\
        --predefine=\"-D__heap_size__=0x400\""
)
