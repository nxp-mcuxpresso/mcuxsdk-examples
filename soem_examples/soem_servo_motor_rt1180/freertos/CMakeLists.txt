
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(soem_servo_motor_rt1180_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/soem_examples/soem_servo_motor_rt1180/freertos)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/soem_examples/soem_servo_motor_rt1180/freertos/soem_servo_motor.c
    SOURCES examples/soem_examples/soem_servo_motor_rt1180/freertos/axis_nc.c
    SOURCES examples/soem_examples/soem_servo_motor_rt1180/freertos/axis_nc.h
    SOURCES examples/soem_examples/soem_servo_motor_rt1180/freertos/cia402.c
    SOURCES examples/soem_examples/soem_servo_motor_rt1180/freertos/cia402.h
    SOURCES examples/soem_examples/soem_servo_motor_rt1180/freertos/servo.c
    SOURCES examples/soem_examples/soem_servo_motor_rt1180/freertos/servo.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/soem_examples/soem_servo_motor_rt1180/freertos
)

include(${SdkRootDirPath}/${board_root}/${board}/soem_examples/soem_servo_motor_rt1180/freertos/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/${board_root}/${board}/soem_examples/soem_servo_motor_rt1180/freertos/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)