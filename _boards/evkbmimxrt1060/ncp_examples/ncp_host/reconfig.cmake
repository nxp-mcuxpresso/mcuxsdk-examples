
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/
    SOURCES FreeRTOSConfigBoard.h
            sdmmc_config.c
            sdmmc_config.h
            ncp_examples/ncp_host/app_config/app_config.h
)

mcux_add_include(
    INCLUDES app_config
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    PREINCLUDE TRUE
    SOURCES examples/ncp_examples/ncp_host/ncp_host_config.h
)


mcux_add_iar_configuration(
    CX "--no_clustering"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_debug flexspi_nor_release
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)
mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
       -DUSE_RTOS=1\
       -DOSA_USED\
       -DUSB_HOST_CONFIG_LOW_POWER_MODE=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1060/MIMXRT1062/gcc/MIMXRT1062xxxxx_flexspi_nor.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1060/MIMXRT1062/iar/MIMXRT1062xxxxx_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1060/MIMXRT1062/arm/MIMXRT1062xxxxx_flexspi_nor.scf
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/ncp_examples/ncp_host/linker/MIMXRT1062xxxxx_flexspi_nor.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/ncp_examples/ncp_host/linker/MIMXRT1062xxxxx_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/ncp_examples/ncp_host/linker/MIMXRT1062xxxxx_flexspi_nor.scf
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
