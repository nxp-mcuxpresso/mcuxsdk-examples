cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(maestro_playback LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/audio_examples/maestro_playback)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_playback
             ${board_root}/${board}
             ${board_root}/${board}/audio_examples/maestro_playback
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_playback/*.c
            examples/audio_examples/maestro_playback/*.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_playback/ssrc
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_playback/ssrc/*.c
            examples/audio_examples/maestro_playback/ssrc/*.h
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
        -DPRINTF_FLOAT_ENABLE=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/audio_examples/maestro_playback/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

