
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/
    SOURCES ncp_examples/ncp_device/FreeRTOSConfig.h
            FreeRTOSConfigBoard.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/ncp_examples/ncp_device/
    PREINCLUDE TRUE
    SOURCES app_config.h
)

mcux_add_include(
    INCLUDES .
)

mcux_add_armgcc_configuration(
    TARGETS flash_release
    AS "-g"
    CC "-g"
    CX "-g"
)

mcux_add_macro(
    CC "-DBT_PLATFORM\
       -DLWIP_HOOK_FILENAME=\\\"lwip_default_hooks.h\\\"\
       -DCONFIG_WPA_SUPP_CRYPTO_MBEDTLS_PSA\
       -DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_config_client.h\\\"\
       -DEDGEFAST_BT_LITTLEFS_MFLASH\
       -DGATT_CLIENT\
       -DGATT_DB\
       -DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_OS_FREE_RTOS\
       -DFSL_OSA_TASK_ENABLE=1\
       -DMCUX_ENABLE_TRNG_AS_ENTROPY_SEED\
       -DLFS_CONFIG=littlefs_config.h\
       -DCONFIG_HOSTAPD=0\
       -DCONFIG_ARM=1"
)

mcux_remove_macro(
    MBEDTLS_MCUX_ELS_PKC_API
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
    TARGETS flash_debug flash_release
    LINKER ${device_root}/Wireless/RW/RW612/gcc/RW612_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/ncp_examples/ncp_device/linker/armgcc/RW610_flash.ld
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
