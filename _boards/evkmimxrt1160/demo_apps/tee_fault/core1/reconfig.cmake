
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/tee_fault/core1/resource_config.h
            ${board_root}/${board}/demo_apps/tee_fault/core1/resource_config.c
)

mcux_add_macro(
    CC "-DFLEXSPI_IN_USE"
)
