
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/vglite/vglite_sdk/example/clock_freertos/clock_analog.h
            middleware/vglite/vglite_sdk/example/clock_freertos/hour_needle.h
            middleware/vglite/vglite_sdk/example/clock_freertos/minute_needle.h
            middleware/vglite/vglite_sdk/example/clock_freertos/layer.h
            middleware/vglite/vglite_sdk/example/clock_freertos/layer.c
            middleware/vglite/vglite_sdk/example/vglite_window/vglite_window.h
            middleware/vglite/vglite_sdk/example/vglite_window/vglite_window.c
            examples/_boards/${board}/vglite_examples/vglite_support/pin_mux.c
            examples/_boards/${board}/vglite_examples/vglite_support/pin_mux.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/vglite/vglite_sdk/example/vglite_window
)

mcux_add_iar_configuration(
    CC "--diag_suppress Pa082,Pa050,Pa092"
    CX "--diag_suppress Pa082,Pa050,Pa092"
)

mcux_add_iar_configuration(
    TARGETS debug release flash_debug flash_release
    LD "--semihosting\
       --redirect __iar_sh_stdout=__iar_sh_stdout_swo"
)

mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc255/0x40A \
        -DBOARD_ENABLE_PSRAM_CACHE=0 \
        -DSSD1963_DATA_WITDH=8 \
        -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
        -DSDK_DEBUGCONSOLE_UART \
        -DCUSTOM_VGLITE_MEMORY_CONFIG=1"
)
