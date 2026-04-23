
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/xip/flexspi_nor_config.c
            ${board_root}/${board}/xip/flexspi_nor_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/xip
)

mcux_add_iar_configuration(
    LD "--config_def __qspi_xip__=1"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__qspi_xip__=1\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__qspi_xip__=1"
)

