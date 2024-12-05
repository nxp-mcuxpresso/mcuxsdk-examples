
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/component_examples/flash_component/octal_flash/octal_flash.c
            ${board_root}/evkmimx8ulp/component_examples/flash_component/flexspi_octal/hardware_init.c
            ${board_root}/evkmimx8ulp/component_examples/flash_component/flexspi_octal/app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/component_examples/flash_component/flexspi_octal
             ${board_root}/evkmimx8ulp
)
