
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/vglite/vglite_sdk/example/tiger_freertos/tiger_paths.h
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
    TARGETS sdram_debug sdram_release flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LD "--semihosting\
       --redirect __iar_sh_stdout=__iar_sh_stdout_swo"
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc355/0x0_1216 \
        -DCUSTOM_VGLITE_MEMORY_CONFIG=1"
)
