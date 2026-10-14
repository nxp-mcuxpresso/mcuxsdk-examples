mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/vglite_examples/cube_raster_freertos/app.h
)

mcux_add_armgcc_configuration(
    CC "-Wno-unused-function -fno-if-conversion -fno-if-conversion2 -Wno-error=array-bounds"
)

mcux_add_iar_configuration(
    CC "--diag_suppress Pe223,Pe1696,Pe111,Pe236"
)

# Framebuffers live at fixed PSRAM addresses by default (see display_support.h).
# Unset those so that the framebuffers in the .ncache linker region instead; the
# custom RT2663 ram linker scripts size .ncache (in PSRAM) large enough to hold
# a double-buffered display in either RGB565 or ARGB8888 format.
mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
        -DUSE_PSRAM=1\
        -DSKIP_SYSCLK_INIT=1\
        -D__STARTUP_INITIALIZE_NONCACHEDATA\
        -D__STARTUP_INITIALIZE_QADATA\
        -D__STARTUP_INITIALIZE_RAMFUNCTION"
    CX "-DSDK_DEBUGCONSOLE_UART\
        -DUSE_PSRAM=1\
        -DSKIP_SYSCLK_INIT=1\
        -D__STARTUP_INITIALIZE_NONCACHEDATA\
        -D__STARTUP_INITIALIZE_QADATA\
        -D__STARTUP_INITIALIZE_RAMFUNCTION"
)

# Provide explicit heap/stack sizes to the linkers, matching the proven recipe.
mcux_add_iar_configuration(
    LD "--config_def=__heap_size__=0x8000\
        --config_def=__stack_size__=0x2000"
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__heap_size__=0x2000\
        -Xlinker --defsym=__stack_size__=0x2000"
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_ram.ld
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_debug psram_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_psram.ld
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_ram.icf
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_debug psram_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_psram.icf
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/vglite/vglite_sdk/linker/MIMXRT2663xxxxx_ram.ld
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_debug psram_release
    LINKER middleware/vglite/vglite_sdk/linker/MIMXRT2663xxxxx_psram.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/vglite/vglite_sdk/linker/MIMXRT2663xxxxx_ram.icf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_debug psram_release
    LINKER middleware/vglite/vglite_sdk/linker/MIMXRT2663xxxxx_psram.icf
)

