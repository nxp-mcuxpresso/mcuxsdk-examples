
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(jpegdec_single_stream_single_frame_descriptor LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/jpegdec/single_stream_single_frame/descriptor)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/jpegdec/single_stream_single_frame/descriptor/single_stream_single_frame_descriptor.c
            ${board_root}/${board}/driver_examples/jpegdec/pin_mux.c
            ${board_root}/${board}/driver_examples/jpegdec/pin_mux.h
            ${board_root}/${board}/driver_examples/jpegdec/${multicore_foldername}/hardware_init.c
            ${board_root}/${board}/driver_examples/jpegdec/${multicore_foldername}/app.h
            examples/driver_examples/jpegdec/single_stream_single_frame/jpeg.c
            examples/driver_examples/jpegdec/single_stream_single_frame/jpeg.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}
             ${board_root}/${board}/driver_examples/jpegdec
             ${board_root}/${board}/driver_examples/jpegdec/${multicore_foldername}
             examples/driver_examples/jpegdec/single_stream_single_frame/
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/jpegdec/single_stream_single_frame/descriptor/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)