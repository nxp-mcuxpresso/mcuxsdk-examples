
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/
    SOURCES ncp_examples/ncp_device/FreeRTOSConfig.h
            FreeRTOSConfigBoard.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/ncp_examples/ncp_device/
    PREINCLUDE TRUE
    SOURCES app_config.h
)

mcux_add_include(
    INCLUDES .
)

mcux_add_mdk_configuration(
    TARGETS flash_release
    CC "-DCONFIG_HAVE_TIME_DISABLE=1"
)

mcux_add_iar_configuration(
    CX "--no_clustering"
    CC "--dlib_config full"
    LD "--semihosting"
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

mcux_add_macro(
    CC "-DBT_PLATFORM\
       -DMBEDTLS_CONFIG_FILE=\\\"els_pkc_mbedtls_config.h\\\"\
       -DMBEDTLS_USER_CONFIG_FILE=\\\"wpa_supp_els_pkc_mbedtls_config.h\\\"\
       -DLWIP_HOOK_FILENAME=\\\"lwip_default_hooks.h\\\"\
       -DEDGEFAST_BT_LITTLEFS_MFLASH\
       -DGATT_CLIENT\
       -DGATT_DB\
       -DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_OS_FREE_RTOS\
       -DFSL_OSA_TASK_ENABLE=1\
       -DLFS_CONFIG=littlefs_config.h\
       -DCONFIG_HOSTAPD=0"
)

mcux_remove_mdk_configuration(
    TARGETS flash_debug
    CC "-O1"
    CX "-O1"
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
    LINKER examples/_boards/${board}/ncp_examples/ncp_device/linker/RW610_flash.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/ncp_examples/ncp_device/linker/RW610_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/ncp_examples/ncp_device/linker/RW610_flash.scf
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
set_source_files_properties(
    ${SdkRootDirPath}/middleware/mbedtls/library/bignum.c
    PROPERTIES
    COMPILE_FLAGS "-O2"
)
endif()
