
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/project_template/peripherals.c
            examples/_boards/${board}/project_template/peripherals.h
            examples/_boards/${board}/project_template/pin_mux.c
            examples/_boards/${board}/project_template/pin_mux.h
            examples/_boards/${board}/FlashKLxx256KROM_with_config_write_enabled.board
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/project_template
)

mcux_add_macro(
    CC "-DFRDM_K32L2B\
       -DFREEDOM"
)
