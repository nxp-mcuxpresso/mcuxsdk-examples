include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/reconfig.cmake OPTIONAL)

mcux_add_iar_configuration(
    LD "--config_def __qspi_xip__=1"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__qspi_xip__=1\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__qspi_xip__=1"
)

