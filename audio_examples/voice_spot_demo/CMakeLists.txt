cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(voice_spot_demo LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/audio_examples/voice_spot_demo)

mcux_add_source(
    SOURCES main.c
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/audio_examples/voice_spot_demo/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)