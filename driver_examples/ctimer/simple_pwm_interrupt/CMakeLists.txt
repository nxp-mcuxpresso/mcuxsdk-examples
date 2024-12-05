cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ctimer_pwm_interrupt_example LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/ctimer/simple_pwm_interrupt)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/ctimer/simple_pwm_interrupt/simple_pwm_interrupt.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/ctimer/simple_pwm_interrupt
             ${board_root}/${board}/driver_examples/ctimer/simple_pwm_interrupt/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/ctimer/simple_pwm_interrupt/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)