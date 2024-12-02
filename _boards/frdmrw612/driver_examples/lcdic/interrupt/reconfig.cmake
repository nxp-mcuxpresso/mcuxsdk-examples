
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/driver_examples/lcdic/pin_mux.c
            examples/_boards/${board}/driver_examples/lcdic/pin_mux.h
)

mcux_add_mdk_configuration(
    TARGETS debug flash_debug
    CC "-O0"
)

mcux_add_macro(
    CC "-DMCUX_DBI_LEGACY=0"
)

mcux_remove_mdk_configuration(
    TARGETS debug flash_debug
    CC "-O1"
)
