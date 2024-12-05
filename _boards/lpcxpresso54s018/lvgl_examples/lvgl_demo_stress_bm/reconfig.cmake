
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/lvgl_examples/pin_mux.c
            ${board_root}/${board}/lvgl_examples/pin_mux.h
            ${board_root}/${board}/lvgl_examples/hardware_init.c
            ${board_root}/${board}/lvgl_examples/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/lvgl_examples
)

mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
)

mcux_add_iar_configuration(
    LD "--config_def XIP_IMAGE=1"
)

mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    AS "-DXIP_IMAGE"
)

mcux_add_macro(
    AS "-DIMG_BAUDRATE=96000000"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x1000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x1000\""
)
