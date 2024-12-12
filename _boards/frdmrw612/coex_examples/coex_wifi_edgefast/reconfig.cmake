
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/coex/build/${board}/common/app_services_init.c
            middleware/wireless/coex/build/${board}/common/FreeRTOSConfig.h
            middleware/wireless/coex/build/${board}/common/hardware_init.c
            middleware/wireless/coex/build/${board}/common/peripherals.c
            middleware/wireless/coex/build/${board}/common/peripherals.h
            examples/_boards/${board}/coex_examples/coex_wifi_edgefast/pin_mux.c
            examples/_boards/${board}/coex_examples/coex_wifi_edgefast/pin_mux.h
            middleware/wireless/coex/src/configs/rw61x/wifi/wifi_config.h
            middleware/wireless/coex/src/configs/rw61x/lwip/lwipopts.h
            middleware/wireless/coex/src/edgefast/coex_shell.c
            middleware/wireless/coex/src/edgefast/coex_shell.h
            middleware/wireless/coex/src/common/controller_coex_nxp.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    PREINCLUDE TRUE
    SOURCES examples/_boards/${board}/coex_examples/coex_wifi_edgefast/app_config.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/coex/src/configs/rw61x/edgefast/app_bluetooth_config.h
    CONFIG True
    PREINCLUDE TRUE
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wireless/coex/build/${board}/common
             examples/_boards/${board}/coex_examples/coex_wifi_edgefast
             middleware/wireless/coex/src/configs/rw61x/edgefast
             middleware/wireless/coex/src/configs/rw61x/wifi
             middleware/wireless/coex/src/configs/rw61x/lwip
             middleware/wireless/coex/src/edgefast
             middleware/wireless/coex/src/common
)

mcux_add_armgcc_configuration(
    TARGETS flash_release
    AS "-g"
    CC "-g"
    CX "-g"
)

mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DSDK_OS_FREE_RTOS\
       -DFSL_OSA_TASK_ENABLE=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DMCUX_ENABLE_TRNG_AS_ENTROPY_SEED\
       -DMBEDTLS_MCUX_ELS_PKC_API\
       -DMBEDTLS_MCUX_USE_PKC\
       -DMBEDTLS_USER_CONFIG_FILE=\\\"wpa_supp_els_pkc_mbedtls_config.h\\\"\
       -DMBEDTLS_CONFIG_FILE=\\\"els_pkc_mbedtls_config.h\\\"\
       -DLWIP_HOOK_FILENAME=\\\"lwip_default_hooks.h\\\"\
       -DGATT_CLIENT\
       -DGATT_DB\
       -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0\
       -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096\
       -DgMemManagerLight=0\
       -DCFG_BLE\
       -DgPlatformDisableBleLowPower_d=1\
       -DSDK_DEBUGCONSOLE_UART=1\
       -DSHELL_TASK_STACK_SIZE=6144\
       -DLPUART_RING_BUFFER_SIZE=1024U\
       -DHAL_AUDIO_DMA_INIT_ENABLE=0\
       -DLFS_NO_INTRINSICS=1\
       -DLFS_NO_ERROR=1\
       -DCONFIG_ARM=1\
       -DBT_PLATFORM\
       -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1 \
       -DFSL_OSA_MAIN_FUNC_ENABLE=0 \
       -DEDGEFAST_BT_LITTLEFS_MFLASH\
       -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING\
       -DDEBUG_CONSOLE_RX_ENABLE=0\
       -DBOOT_HEADER_ENABLE=1 \
       -DSHELL_ADVANCE=1 \
       -DOSA_USED=1 \
       -DSHELL_USE_COMMON_TASK=0 \
       -DLOG_ENABLE_ASYNC_MODE=1 \
       -DLOG_MAX_ARGUMENT_COUNT=10 \
       -DLOG_ENABLE_OVERWRITE=0 \
       -DMCUXCL_FEATURE_CSSL_MEMORY_C_FALLBACK \
       -DSERIAL_PORT_TYPE_UART=1 \
       -DMFLASH_FILE_BASEADDR=7340032 \
       -DSHELL_TASK_PRIORITY=1 \
       -DCONFIG_HOSTAPD=0"
)


mcux_remove_macro(
    TOOLCHAINS armgcc
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
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/RW/RW612/arm/RW612_ram.scf
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
    LINKER middleware/wireless/coex/build/${board}/linker/RW61x_flash.ld
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)
if(${CONFIG_TOOLCHAIN} STREQUAL "armgcc")
set_source_files_properties(
    ${SdkRootDirPath}/middleware/mbedtls/library/bignum.c
    PROPERTIES
    COMPILE_FLAGS "-O2"
)
endif()
