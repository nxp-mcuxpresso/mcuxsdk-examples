
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/xip/mcxn_flexspi_nor_config.c
            ${board_root}/${board}/xip/mcxn_flexspi_nor_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/xip
             ${board_root}/${board}/demo_apps/hello_world_qspi_xip
)
