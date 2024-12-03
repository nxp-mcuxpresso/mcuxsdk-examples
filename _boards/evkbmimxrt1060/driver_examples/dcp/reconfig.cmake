
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_release
    CC "-O0"
    CX "-O0"
)

mcux_remove_armgcc_configuration(
    TARGETS flexspi_nor_release
    CC "-Os"
    CX "-Os"
)
