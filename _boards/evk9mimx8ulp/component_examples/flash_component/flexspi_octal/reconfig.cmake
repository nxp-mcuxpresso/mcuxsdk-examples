
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/component_examples/flash_component/octal_flash/octal_flash.c
            examples/_boards/evkmimx8ulp/component_examples/flash_component/flexspi_octal/hardware_init.c
            examples/_boards/evkmimx8ulp/component_examples/flash_component/flexspi_octal/app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/component_examples/flash_component/flexspi_octal
             examples/_boards/evkmimx8ulp
)
