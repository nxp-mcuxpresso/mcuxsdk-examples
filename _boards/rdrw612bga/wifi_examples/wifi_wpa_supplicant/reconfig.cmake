
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/wifi_examples/common/lpm/lpm.c
            examples/_boards/${board}/wifi_examples/common/lpm/lpm.h
            examples/_boards/${board}/wifi_examples/common/lpm/host_sleep.c
            examples/_boards/${board}/wifi_examples/common/lpm/host_sleep.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/usb_support.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/usb_support.c
            middleware/wifi_nxp/example/common/usb/usb_host_msd/usb_api.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/host_msd_fatfs.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/host_msd_fatfs.c
            middleware/wifi_nxp/example/common/usb/usb_host_msd/ffconf.h
            examples/_boards/${board}/wifi_examples/common/pin_mux.c
            examples/_boards/${board}/wifi_examples/common/pin_mux.h
            examples/_boards/${board}/wifi_examples/common/./app.h
            examples/_boards/${board}/wifi_examples/wifi_wpa_supplicant/hardware_init.c
            middleware/wifi_nxp/example/wifi_wpa_supplicant/lwippools.h
            middleware/wifi_nxp/example/wifi_wpa_supplicant/lwiphooks.h
            middleware/wifi_nxp/example/wifi_wpa_supplicant/FreeRTOSConfig.h
            middleware/wifi_nxp/example/wifi_wpa_supplicant/lwipopts.h
            examples/_boards/${board}/wifi_examples/common/app_config/app_config.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    PREINCLUDE TRUE
    SOURCES middleware/wifi_nxp/example/wifi_wpa_supplicant/wifi_config/wifi_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/wifi_examples/common/lpm/
             middleware/wifi_nxp/example/common/usb/usb_host_msd
             examples/_boards/${board}/wifi_examples/common
             middleware/wifi_nxp/example/wifi_wpa_supplicant/wifi_config
             examples/_boards/${board}/wifi_examples/common/app_config
             examples/_boards/${board}/wifi_examples/wifi_wpa_supplicant
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
    LINKER examples/_boards/${board}/wifi_examples/wifi_wpa_supplicant/linker/RW610_flash.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/wifi_wpa_supplicant/linker/RW610_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/wifi_wpa_supplicant/linker/RW610_flash.scf
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
