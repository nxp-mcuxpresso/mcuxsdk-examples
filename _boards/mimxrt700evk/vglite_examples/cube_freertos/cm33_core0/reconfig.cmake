
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
    TARGETS debug release
    LD "--semihosting\
       --redirect __iar_sh_stdout=__iar_sh_stdout_swo"
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc555/0x423_ECO \
        -DMCUX_DBI_LEGACY=0 \
        -DDBI_USE_MIPI_PANEL=1 \
        -DSDK_I2C_BASED_COMPONENT_USED=1 \
        -DSSD1963_DATA_WITDH=8 \
        -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
        -DCUSTOM_VGLITE_MEMORY_CONFIG=1"
)

# Remove default linker scripts. The app requires a custom set of linker scripts
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_ram.ld
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core0_ram.icf
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/arm/MIMXRT798Sxxxx_cm33_core0_ram.scf
)

# Add custom linker scripts
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/vglite/vglite_sdk/linker/MIMXRT798Sxxxx_cm33_core0_ram_cube.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/vglite/vglite_sdk/linker/MIMXRT798Sxxxx_cm33_core0_ram_cube.icf
)

mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/vglite/vglite_sdk/linker/MIMXRT798Sxxxx_cm33_core0_ram_cube.scf
)