
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/wifi_examples/common/lpm/lpm.c
            ${board_root}/${board}/wifi_examples/common/lpm/lpm.h
            ${board_root}/${board}/wifi_examples/common/lpm/host_sleep.c
            ${board_root}/${board}/wifi_examples/common/lpm/host_sleep.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/usb_support.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/usb_support.c
            middleware/wifi_nxp/example/common/usb/usb_host_msd/usb_api.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/host_msd_fatfs.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/host_msd_fatfs.c
            middleware/wifi_nxp/example/common/usb/usb_host_msd/ffconf.h
            ${board_root}/${board}/wifi_examples/common/pin_mux.c
            ${board_root}/${board}/wifi_examples/common/pin_mux.h
            ${board_root}/${board}/wifi_examples/common/./app.h
            ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/hardware_init.c
            ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/lwip_config/lwippools.h
            ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/lwip_config/lwiphooks.h
            ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/freertos_config/FreeRTOSConfig.h
            ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/lwip_config/lwipopts.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    PREINCLUDE TRUE
    SOURCES ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/wifi_config/wifi_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/wifi_examples/common/lpm/
             middleware/wifi_nxp/example/common/usb/usb_host_msd
             ${board_root}/${board}/wifi_examples/common
             ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/wifi_config
             ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/lwip_config
             ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/freertos_config
             ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant
)

mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DSDK_OS_FREE_RTOS\
       -DFSL_OSA_TASK_ENABLE=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DMCUX_ENABLE_TRNG_AS_ENTROPY_SEED\
       -DMBEDTLS_MCUX_ELS_PKC_API\
       -DMBEDTLS_MCUX_USE_PKC\
       -DCONFIG_MONOLITHIC_WIFI=1\
       -DMBEDTLS_USER_CONFIG_FILE=\\\"wpa_supp_els_pkc_mbedtls_config.h\\\"\
       -DMBEDTLS_CONFIG_FILE=\\\"els_pkc_mbedtls_config.h\\\"\
       -DLWIP_HOOK_FILENAME=\\\"lwip_default_hooks.h\\\""
)


mcux_add_mdk_configuration(
    CC "-mfloat-abi=hard"
)

mcux_add_mdk_configuration(
    TARGETS flash_release flash_debug
    CC "-DCONFIG_HAVE_TIME_DISABLE=1"
)

mcux_add_iar_configuration(
    CX "--no_clustering"
    LD "--semihosting"
    CC "--dlib_config full"
)

mcux_add_iar_configuration(
    TARGETS flash_release
    CC "--no_inline"
)

mcux_add_armgcc_configuration(
    TARGETS flash_release
    AS "-g"
    CC "-g"
    CX "-g"
)

mcux_remove_mdk_configuration(
    TARGETS flash_debug
    CC "-O0"
    CX "-O0"
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
    LINKER ${device_root}/Wireless/RW/RW612/gcc/RW612_flash.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/Wireless/RW/RW612/iar/RW612_flash.icf
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
    LINKER ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/linker/RW610_flash.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/linker/RW610_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/linker/RW610_flash.scf
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
if(${CONFIG_TOOLCHAIN} STREQUAL "armgcc")
if(EXISTS "${SdkRootDirPath}/middleware/mbedtls/library/bignum.c")
set_source_files_properties(
    ${SdkRootDirPath}/middleware/mbedtls/library/bignum.c
    PROPERTIES
    COMPILE_FLAGS "-O2"
)
endif()
endif()
