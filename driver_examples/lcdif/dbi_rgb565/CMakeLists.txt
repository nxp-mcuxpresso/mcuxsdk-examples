cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lcdif_dbi_rgb565 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/lcdif/dbi_rgb565)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lcdif/dbi_rgb565/lcdif_dbi_rgb565.c
            components/video/display/dbi/fsl_dbi.c
            components/video/display/dbi/fsl_dbi.h
            examples/_boards/${board}/driver_examples/lcdif/lcdif_support.h
            examples/_boards/${board}/driver_examples/lcdif/lcdif_support.c
            examples/_boards/${board}/driver_examples/lcdif/pin_mux.c
            examples/_boards/${board}/driver_examples/lcdif/pin_mux.h
            examples/_boards/${board}/driver_examples/lcdif/${multicore_foldername}/hardware_init.c
            examples/_boards/${board}/driver_examples/lcdif/${multicore_foldername}/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/lcdif
             examples/_boards/${board}/driver_examples/lcdif/${multicore_foldername}
             examples/_boards/${board}/driver_examples/lcdif/dbi_rgb565/${core_id}
             components/video/display/dbi
)

mcux_add_macro(
    CC "-DMCUX_DBI_LEGACY=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/lcdif/dbi_rgb565/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)