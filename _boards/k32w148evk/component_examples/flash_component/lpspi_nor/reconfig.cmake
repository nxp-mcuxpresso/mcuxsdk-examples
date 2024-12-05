
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/kw45b41zevk/component_examples/flash_component/lpspi_nor/hardware_init.c
            ${board_root}/kw45b41zevk/component_examples/flash_component/lpspi_nor/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/kw45b41zevk/component_examples/flash_component/lpspi_nor
)