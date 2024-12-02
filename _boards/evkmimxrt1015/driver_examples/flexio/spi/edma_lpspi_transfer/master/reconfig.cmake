
mcux_add_mdk_configuration(
    TARGETS flexspi_nor_debug
    CC "-O2"
)

mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_debug
    CC "-O1"
)
