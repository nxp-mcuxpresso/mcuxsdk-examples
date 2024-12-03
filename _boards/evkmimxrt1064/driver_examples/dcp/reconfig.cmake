
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_release
    CC "-O3"
    CX "-O3"
)

mcux_remove_armgcc_configuration(
    TARGETS flexspi_nor_release
    CC "-Os"
    CX "-Os"
)
