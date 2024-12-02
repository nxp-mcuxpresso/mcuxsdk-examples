
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(maestro_usb_speaker LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/audio_examples/maestro_usb_speaker)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_usb_speaker/*.c
            examples/audio_examples/maestro_usb_speaker/*.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_usb_speaker
             examples/_boards/${board}/audio_examples/maestro_usb_speaker
             examples/_boards/${board}
)

include(${SdkRootDirPath}/examples/_boards/${board}/audio_examples/maestro_usb_speaker/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)