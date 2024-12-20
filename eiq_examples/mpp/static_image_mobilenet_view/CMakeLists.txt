cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(static_image_mobilenet_view LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/eiq_examples/mpp/examples/static_image_mobilenet_view)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/mpp/static_image_mobilenet_view/static_image_mobilenet_view.c
            examples/_boards/${board}/eiq_examples/mpp/examples/static_image_mobilenet_view/mpp_config.h
            middleware/eiq/mpp/images/stopwatch128_128_rgb.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/eiq_examples/mpp/examples/static_image_mobilenet_view/
    middleware/eiq/mpp/images/
)

# board files
include(${SdkRootDirPath}/examples/_boards/${board}/eiq_examples/mpp/board.cmake)
# model and post-processing
include(${SdkRootDirPath}/middleware/eiq/mpp/models/mobilenet_v1_0.25_128_quant_int8/model.cmake)

# top level
include(${SdkRootDirPath}/CMakeLists.txt)
# board & app specific cmake
include(${SdkRootDirPath}/examples/_boards/${board}/eiq_examples/mpp/examples/static_image_mobilenet_view/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

