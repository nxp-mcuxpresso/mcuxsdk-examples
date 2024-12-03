
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/wifi_examples/common/pin_mux.c
            examples/_boards/${board}/wifi_examples/common/pin_mux.h
            middleware/wifi_nxp/example/wifi_cli_prov/wifi_config/wifi_config.h
            middleware/wifi_nxp/example/wifi_cli_prov/lwipopts.h
            middleware/wifi_nxp/example/wifi_cli_prov/lwippools.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/usb_support.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/usb_support.c
            examples/_boards/${board}/wifi_examples/wifi_cli_prov/usb_host_config.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/usb_api.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/host_msd_fatfs.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/host_msd_fatfs.c
            middleware/wifi_nxp/example/common/usb/usb_host_msd/ffconf.h
            middleware/wifi_nxp/example/wifi_cli_prov/FreeRTOSConfig.h
            middleware/wifi_nxp/example/wifi_cli_prov/wifi_mbedtls_config.h
            examples/_boards/${board}/wifi_examples/common/./app.h
            examples/_boards/${board}/wifi_examples/common/app_config/app_config.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/wifi_examples/common
             middleware/wifi_nxp/example/wifi_cli_prov/wifi_config
             middleware/wifi_nxp/example/common/usb/usb_host_msd
             examples/_boards/${board}/wifi_examples/common/app_config
)



mcux_add_iar_configuration(
    CX "--no_clustering"
)
mcux_add_armgcc_configuration(
    TARGETS flash_release
    AS "-g"
    CC "-g"
    CX "-g"
)

mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DMBEDTLS_CONFIG_FILE=\\\"els_pkc_mbedtls_config.h\\\"\
       -DMBEDTLS_USER_CONFIG_FILE=\\\"wifi_mbedtls_config.h\\\""
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
    TARGETS flash_debug flash_release
    LINKER devices/Wireless/RW/RW612/gcc/RW612_flash.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/Wireless/RW/RW612/iar/RW612_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/Wireless/RW/RW612/arm/RW612_flash.scf
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
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/RW610_flash.scf
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x400\
        --config_def=__heap_size__=0x400"
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x400\"\
        --predefine=\"-D__heap_size__=0x400\""
)
