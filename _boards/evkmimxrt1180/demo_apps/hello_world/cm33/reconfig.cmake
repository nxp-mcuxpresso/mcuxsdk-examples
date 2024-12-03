mcux_remove_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
            flexspi_nor_hyperram_debug  flexspi_nor_hyperram_release
    CC "-DXIP_BOOT_HEADER_ENABLE=0"
)

mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
            flexspi_nor_hyperram_debug  flexspi_nor_hyperram_release
    LD "--predefine=\"-DXIP_BOOT_HEADER_ENABLE=0\""
)

mcux_add_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
            flexspi_nor_hyperram_debug  flexspi_nor_hyperram_release
    CC "-DXIP_BOOT_HEADER_ENABLE=1"
)

mcux_add_mdk_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
            flexspi_nor_hyperram_debug  flexspi_nor_hyperram_release
    LD "--predefine=\"-DXIP_BOOT_HEADER_ENABLE=1\""
)
