
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/mbedtls_examples/mbedtls_benchmark
)

mcux_add_mdk_configuration(
    LD "--diag_suppress 6329"
)

mcux_add_macro(
    CC "-DPLLPFD_FREQ_NOT_FROM_OBS\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DFREESCALE_KSDK_BM\
       -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\""
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT1160/MIMXRT1166/iar/MIMXRT1166xxxxx_cm7_ram.icf
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1160/MIMXRT1166/iar/MIMXRT1166xxxxx_cm7_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT1160/MIMXRT1166/arm/MIMXRT1166xxxxx_cm7_ram.scf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1160/MIMXRT1166/arm/MIMXRT1166xxxxx_cm7_flexspi_nor.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT1160/MIMXRT1166/gcc/MIMXRT1166xxxxx_cm7_ram.ld
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1160/MIMXRT1166/gcc/MIMXRT1166xxxxx_cm7_flexspi_nor.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/mbedtls_examples/linker/MIMXRT1166xxxxx_cm7_ram_ocram.icf
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/mbedtls_examples/linker/MIMXRT1166xxxxx_cm7_flexspi_nor_ocram.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/mbedtls_examples/linker/MIMXRT1166xxxxx_cm7_ram_ocram.scf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/mbedtls_examples/linker/MIMXRT1166xxxxx_cm7_flexspi_nor_ocram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/mbedtls_examples/linker/MIMXRT1166xxxxx_cm7_ram_ocram.ld
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/mbedtls_examples/linker/MIMXRT1166xxxxx_cm7_flexspi_nor_ocram.ld
)
