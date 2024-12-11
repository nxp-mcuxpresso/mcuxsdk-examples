mcux_add_macro(
    CC "-DFSL_ETH_ENABLE_CACHE_CONTROL=1"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_debug flexspi_nor_release
    AS "-D__STARTUP_CLEAR_BSS\
        -D__STARTUP_INITIALIZE_RAMFUNCTION\
        -D__STARTUP_INITIALIZE_NONCACHEDATA"
)

mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

mcux_add_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O1"
)
mcux_remove_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O0"
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1170/MIMXRT1176/iar/MIMXRT1176xxxxx_cm4_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1170/MIMXRT1176/arm/MIMXRT1176xxxxx_cm4_flexspi_nor.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1170/MIMXRT1176/gcc/MIMXRT1176xxxxx_cm4_flexspi_nor.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/lwip_examples/linker/lwip_tcm/MIMXRT1176xxxxx_cm4_flexspi_nor_lwiptcm.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/lwip_examples/linker/lwip_tcm/MIMXRT1176xxxxx_cm4_flexspi_nor_lwiptcm.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/lwip_examples/linker/lwip_tcm/MIMXRT1176xxxxx_cm4_flexspi_nor_lwiptcm.ld
)
