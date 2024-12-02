
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/ftm_quad_decoder_peripheral/peripherals.c
            examples/_boards/${board}/demo_apps/ftm_quad_decoder_peripheral/peripherals.h
)

mcux_add_macro(
    CC "-DFRDM_K22F\
       -DFREEDOM"
)
