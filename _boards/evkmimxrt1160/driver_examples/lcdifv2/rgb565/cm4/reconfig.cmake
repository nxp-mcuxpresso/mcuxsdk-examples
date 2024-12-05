
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/lcdifv2/pin_mux.c
            ${board_root}/${board}/driver_examples/lcdifv2/pin_mux.h
            ${board_root}/${board}/driver_examples/lcdifv2/lcdifv2_support.h
            ${board_root}/${board}/driver_examples/lcdifv2/lcdifv2_support.c
            ${board_root}/${board}/driver_examples/lcdifv2/${core_id}/hardware_init.c
            ${board_root}/${board}/driver_examples/lcdifv2/${core_id}/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/lcdifv2/${core_id}
)
