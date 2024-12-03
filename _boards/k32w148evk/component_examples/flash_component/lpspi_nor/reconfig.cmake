
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/kw45b41zevk/component_examples/flash_component/lpspi_nor/hardware_init.c
            examples/_boards/kw45b41zevk/component_examples/flash_component/lpspi_nor/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/kw45b41zevk/component_examples/flash_component/lpspi_nor
)