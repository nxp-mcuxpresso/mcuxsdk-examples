cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(maestro_record LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/audio_examples/maestro_record)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_record/*.c
            examples/audio_examples/maestro_record/*.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_record
             examples/_boards/${board}/audio_examples/maestro_record
             examples/_boards/${board}
)

# Add additional configuration
mcux_add_source(
    TOOLCHAINS armgcc mcux
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_record/vit/vit_proc.c
            examples/audio_examples/maestro_record/vit/vit_proc.h
)

mcux_add_include(
    TOOLCHAINS armgcc mcux
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_record/vit
)

mcux_add_source(
    TOOLCHAINS armgcc mcux
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_record/voice_seeker/voice_seeker.c
            examples/audio_examples/maestro_record/voice_seeker/voice_seeker.h
)

mcux_add_include(
    TOOLCHAINS armgcc mcux
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_record/voice_seeker
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_record/opus_enc/opusmem2mem_file.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_record/opus_enc
)

include(${SdkRootDirPath}/examples/_boards/${board}/audio_examples/maestro_record/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)