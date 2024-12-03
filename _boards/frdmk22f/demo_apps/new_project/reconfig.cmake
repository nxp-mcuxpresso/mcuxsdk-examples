
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/project_template/peripherals.c
            examples/_boards/${board}/project_template/peripherals.h
            examples/_boards/${board}/project_template/pin_mux.c
            examples/_boards/${board}/project_template/pin_mux.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/project_template
)

mcux_add_macro(
    CC "-DFRDM_K22F\
       -DFREEDOM"
)
