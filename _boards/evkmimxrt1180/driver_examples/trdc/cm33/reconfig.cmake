mcux_remove_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
    CC "-DXIP_BOOT_HEADER_ENABLE=0"
)

mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
    LD "--predefine=\"-DXIP_BOOT_HEADER_ENABLE=0\""
)

mcux_add_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
    CC "-DXIP_BOOT_HEADER_ENABLE=1"
)

mcux_add_mdk_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
    LD "--predefine=\"-DXIP_BOOT_HEADER_ENABLE=1\""
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/multicore_examples
)

mcux_add_macro(
    CC "-DUSE_HYPERRAM=1"
)

mcux_add_mdk_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "--keep=*(*core1_code)"
)

mcux_add_iar_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)

mcux_add_armgcc_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)