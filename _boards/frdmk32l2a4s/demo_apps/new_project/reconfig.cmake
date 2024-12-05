
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/project_template/peripherals.c
            ${board_root}/${board}/project_template/peripherals.h
            ${board_root}/${board}/project_template/pin_mux.c
            ${board_root}/${board}/project_template/pin_mux.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/project_template
)

mcux_add_macro(
    CC "-DFRDM_K32L2A4S\
       -DFREEDOM"
)
