cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(audmix_sai_codec LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/audmix/sai_codec)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/audmix/sai_codec/main.c
            examples/driver_examples/audmix/sai_codec/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/audmix/sai_codec
             ${board_root}/${board}/driver_examples/audmix/sai_codec/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/audmix/sai_codec/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
