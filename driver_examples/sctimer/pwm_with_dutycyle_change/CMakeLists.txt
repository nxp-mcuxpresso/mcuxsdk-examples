cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sctimer_pwm_with_dutycyle_change LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/sctimer/pwm_with_dutycyle_change)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/sctimer/pwm_with_dutycyle_change/sctimer_update_dutycycle.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/sctimer/pwm_with_dutycyle_change
             ${board_root}/${board}/driver_examples/sctimer/pwm_with_dutycyle_change/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/sctimer/pwm_with_dutycyle_change/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)