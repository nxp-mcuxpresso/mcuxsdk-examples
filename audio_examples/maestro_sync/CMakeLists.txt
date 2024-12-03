
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(maestro_sync LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/audio_examples/maestro_sync)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_sync/*.c
            examples/audio_examples/maestro_sync/*.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_sync
             examples/_boards/${board}/audio_examples/maestro_sync
             examples/_boards/${board}
)

# Add additional configuration
mcux_add_macro(
    CC "-DFILE_FS_TABLE_SIZE=3"
)

mcux_add_source(
    TOOLCHAINS armgcc mcux
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_sync/vit/vit_proc.c
            examples/audio_examples/maestro_sync/vit/vit_proc.h
)

mcux_add_include(
    TOOLCHAINS armgcc mcux
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_sync/vit
)

mcux_add_source(
    TOOLCHAINS armgcc mcux
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_sync/voice_seeker/voice_seeker.c
            examples/audio_examples/maestro_sync/voice_seeker/voice_seeker.h
)

mcux_add_include(
    TOOLCHAINS armgcc mcux
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_sync/voice_seeker
)

include(${SdkRootDirPath}/examples/_boards/${board}/audio_examples/maestro_sync/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)