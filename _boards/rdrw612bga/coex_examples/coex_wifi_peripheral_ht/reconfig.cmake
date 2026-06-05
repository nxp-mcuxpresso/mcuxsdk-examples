mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/coex_examples/coex_wifi_peripheral_ht/pin_mux.c
            examples/_boards/${board}/coex_examples/coex_wifi_peripheral_ht/pin_mux.h
            examples/_boards/${board}/coex_examples/coex_wifi_peripheral_ht/hardware_init.c
            examples/_boards/${board}/coex_examples/coex_wifi_peripheral_ht/FreeRTOSConfig.h
            examples/_boards/${board}/coex_examples/coex_wifi_peripheral_ht/app_config.h
            examples/coex_examples/coex_wifi_peripheral_ht/app_config.cmake
            middleware/wireless/coex/src/configs/rw61x/wifi/wifi_config.h
            middleware/wireless/coex/src/configs/rw61x/lwip/lwipopts.h
            middleware/wireless/coex/src/configs/rw61x/lwip/lwiphooks.h
            middleware/wireless/coex/src/configs/rw61x/mbedtls/mbedtls_config_client.h
            middleware/wireless/coex/src/coex_pm/coex_lpm.c
            middleware/wireless/coex/src/coex_pm/coex_lpm.h
            middleware/wireless/coex/src/coex_pm/host_sleep.c
            middleware/wireless/coex/src/coex_pm/host_sleep.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wireless/coex/build/${board}/common
             middleware/wireless/coex/src/configs/rw61x/wifi
             middleware/wireless/coex/src/configs/rw61x/lwip
             middleware/wireless/coex/src/configs/rw61x/mbedtls
             examples/_boards/${board}/coex_examples/coex_wifi_peripheral_ht
             examples/coex_examples/coex_wifi_peripheral_ht
             middleware/wireless/coex/src/coex_pm
)

mcux_add_armgcc_configuration(
    TARGETS flash_release
    AS "-g"
    CC "-g"
    CX "-g"
)

mcux_add_macro(
  CC "-DBT_PLATFORM\
      -DCONFIG_ARM=1\
      -DEDGEFAST_BT_LITTLEFS_MFLASH\
      -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0\
      -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096\
      -DGATT_CLIENT\
      -DGATT_DB\
      -DHAL_AUDIO_DMA_INIT_ENABLE=0\
      -DLFS_NO_ERROR=1\
      -DLFS_NO_INTRINSICS=1\
      -DgMemManagerLight=0\
      -DDEBUG_CONSOLE_ENABLE_ECHO_FUNCTION\
      -DMCUX_ENABLE_TRNG_AS_ENTROPY_SEED\
      -DCONFIG_WPA_SUPP_CRYPTO_MBEDTLS_PSA\
      -DLWIP_HOOK_FILENAME=\\\"lwip_default_hooks.h\\\"\
      -DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_config_client.h\\\"\
      -DMBEDTLS_WPA_SUPPLICANT_CONFIG_FILE=\\\"wpa_supp_els_pkc_mbedtls_config.h\\\"\
      -DCONFIG_HOSTAPD=0"
)

mcux_remove_macro(
    TOOLCHAINS armgcc
    TARGETS flash_release
    AS "-DNDEBUG"
    CC "-DNDEBUG"
    CX "-DNDEBUG"
)

mcux_remove_macro(
    MBEDTLS_MCUX_ELS_PKC_API
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
    LINKER middleware/wireless/coex/build/${board}/linker/RW61x_flash.ld
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)
if(${CONFIG_TOOLCHAIN} STREQUAL "armgcc")
if(EXISTS "${SdkRootDirPath}/middleware/mbedtls3x/library/bignum.c")
set_source_files_properties(
    ${SdkRootDirPath}/middleware/mbedtls3x/library/bignum.c
    PROPERTIES
    COMPILE_FLAGS "-O2"
)
endif()
endif()
