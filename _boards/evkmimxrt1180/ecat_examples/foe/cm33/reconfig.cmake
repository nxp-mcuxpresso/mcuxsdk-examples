mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ecat_examples/foe/foe.c
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ecat_examples/foe
    INCLUDES examples/_boards/${board}/ecat_examples/foe/${core_id}
)




# Add additional configuration
mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
        -DSERIAL_PORT_TYPE_UART=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)

mcux_add_armgcc_configuration(
    TARGETS debug release flexspi_nor_debug flexspi_nor_release
    LD "-Xlinker --defsym=__stack_size__=0x2000 -Xlinker --defsym=__heap_size__=0x10000"
    CC "-O0"
)

mcux_add_iar_configuration(
    TARGETS debug release flexspi_nor_debug flexspi_nor_release
    LD "--config_def=__stack_size__=0x2000 --config_def=__heap_size__=0x10000"
)

mcux_add_mdk_configuration(
    TARGETS debug release flexspi_nor_debug flexspi_nor_release
    LD "--predefine=\"-D__stack_size__=0x2000\" --predefine=\"-D__heap_size__=0x10000\""
    CC "-O0"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1180/MIMXRT1189/iar/MIMXRT1189xxxxx_cm33_flexspi_nor.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1180/MIMXRT1189/gcc/MIMXRT1189xxxxx_cm33_flexspi_nor.ld
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1180/MIMXRT1189/arm/MIMXRT1189xxxxx_cm33_flexspi_nor.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/ecat_examples/foe/linker/MIMXRT1189xxxxx_cm33_flexspi_nor_mcuboot.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/ecat_examples/foe/linker/MIMXRT1189xxxxx_cm33_flexspi_nor_mcuboot.ld
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/ecat_examples/foe/linker/MIMXRT1189xxxxx_cm33_flexspi_nor_mcuboot.scf
)