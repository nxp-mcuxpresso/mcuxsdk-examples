
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lib_jpeg_hw_accel LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/display_examples/lib_jpeg_hw_accel)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/display_examples/lib_jpeg_hw_accel/lib_jpeg_hw_accel.c
            examples/display_examples/lib_jpeg_hw_accel/jpeg.h
            examples/display_examples/lib_jpeg_hw_accel/jpeg.c
            examples/display_examples/lib_jpeg_hw_accel/jconfig.h
            examples/_boards/${board}/sdmmc_config.h
            examples/_boards/${board}/sdmmc_config.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/display_examples/lib_jpeg_hw_accel/
             examples/_boards/${board}/display_examples/lib_jpeg_hw_accel
             examples/_boards/${board}/display_examples/lib_jpeg_hw_accel/${multicore_foldername}
)

mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
        -DLIB_JPEG_USE_HW_ACCEL=1"
)

mcux_add_linker_symbol(
      SYMBOLS "__stack_size__=0x1000 __heap_size__=0x20000"
)

include(${SdkRootDirPath}/examples/_boards/${board}/display_examples/lib_jpeg_hw_accel/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)