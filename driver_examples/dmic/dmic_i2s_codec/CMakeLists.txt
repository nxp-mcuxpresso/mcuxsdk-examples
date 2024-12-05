cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dmic_i2s_codec LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/dmic/dmic_i2s_codec)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/dmic/dmic_i2s_codec/dmic_i2s_codec.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/dmic/dmic_i2s_codec
             ${board_root}/${board}/driver_examples/dmic/dmic_i2s_codec/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/dmic/dmic_i2s_codec/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)