
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/vglite/vglite_sdk/example/cube_freertos/draw_clock.h
            middleware/vglite/vglite_sdk/example/cube_freertos/draw_clock.c
            middleware/vglite/vglite_sdk/example/cube_freertos/draw_cube.h
            middleware/vglite/vglite_sdk/example/cube_freertos/draw_cube.c
            middleware/vglite/vglite_sdk/example/cube_freertos/path_nxplogo.h
            middleware/vglite/vglite_sdk/example/cube_freertos/path_tiger.h
            middleware/vglite/vglite_sdk/example/cube_freertos/clock_paths/bg_paths.h
            middleware/vglite/vglite_sdk/example/cube_freertos/clock_paths/h_paths.h
            middleware/vglite/vglite_sdk/example/cube_freertos/clock_paths/m_paths.h
            middleware/vglite/vglite_sdk/example/cube_freertos/clock_paths/s_paths.h
            middleware/vglite/vglite_sdk/example/vglite_window/vglite_window.h
            middleware/vglite/vglite_sdk/example/vglite_window/vglite_window.c
            ${board_root}/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/vglite/vglite_sdk/example/cube_freertos/clock_paths
             middleware/vglite/vglite_sdk/example/vglite_window
)

mcux_add_iar_configuration(
    CC "--diag_suppress Pa082,Pa050,Pa092"
    CX "--diag_suppress Pa082,Pa050,Pa092"
)

mcux_add_iar_configuration(
    TARGETS hyperram_debug hyperram_release
    LD "--semihosting\
       --redirect __iar_sh_stdout=__iar_sh_stdout_swo"
)

mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc355/0x0_1216 \
        -DCUSTOM_VGLITE_MEMORY_CONFIG=1 \
        -DBOARD_USE_PCAL6524=1 \
        -DSDK_I2C_BASED_COMPONENT_USED=1"
)

# Remove default linker scripts. The app requires a custom set of linker scripts
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/gcc/MIMXRT1152xxxxx_sdram.ld
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/iar/MIMXRT1152xxxxx_sdram.icf
)

# Add custom linker scripts
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/vglite/vglite_sdk/linker/MIMXRT1152xxxxx_sdram_cube.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/vglite/vglite_sdk/linker/MIMXRT1152xxxxx_sdram_cube.icf
)
