
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
            examples/_boards/${board}/vglite_examples/vglite_support/pin_mux.c
            examples/_boards/${board}/vglite_examples/vglite_support/pin_mux.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
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
    TARGETS sdram_debug sdram_release
    LD "--semihosting\
       --redirect __iar_sh_stdout=__iar_sh_stdout_swo"
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc355/0x0_1216 \
        -DCUSTOM_VGLITE_MEMORY_CONFIG=1"
)

# Remove default linker scripts. The app requires a custom set of linker scripts
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT1170/MIMXRT1176/gcc/MIMXRT1176xxxxx_cm7_sdram.ld
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT1170/MIMXRT1176/iar/MIMXRT1176xxxxx_cm7_sdram.icf
)

# Add custom linker scripts
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/vglite/vglite_sdk/linker/MIMXRT1176xxxxx_cm7_sdram_cube.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/vglite/vglite_sdk/linker/MIMXRT1176xxxxx_cm7_sdram_cube.icf
)