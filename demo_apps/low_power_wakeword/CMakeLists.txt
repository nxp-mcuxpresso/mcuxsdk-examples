cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(low_power_wakeword LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/low_power_wakeword)

mcux_add_source(
    SOURCES low_power_wakeword.c
            low_power_wakeword.h
            voice_engine_common.c
            voice_engine_common.h
            voice_engine_voiceseeker_voicespot.c
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/low_power_wakeword/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)