
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/vglite/vglite_sdk/example/vglite_window/vglite_window.h
            middleware/vglite/vglite_sdk/example/vglite_window/vglite_window.c
            ${board_root}/${board}/FreeRTOSConfigBoard.h
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
    TARGETS hyperram_debug hyperram_release flexspi_nor_hyperram_debug flexspi_nor_hyperram_release
    LD "--semihosting\
       --redirect __iar_sh_stdout=__iar_sh_stdout_swo"
)

mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc355/0x0_1216 \
        -DBOARD_USE_PCAL6524=1 \
        -DSDK_I2C_BASED_COMPONENT_USED=1"
)
