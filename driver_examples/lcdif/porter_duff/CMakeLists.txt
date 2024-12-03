cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lcdif_porter_duff LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/lcdif/porter_duff)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lcdif/porter_duff/lcdif_porter_duff.c
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
             examples/_boards/${board}/driver_examples/lcdif/porter_duff/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/lcdif/porter_duff/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)