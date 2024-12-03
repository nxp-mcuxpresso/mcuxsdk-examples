
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(soem_servo_motor_bm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/soem_examples/soem_servo_motor/bm)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/soem_examples/soem_servo_motor/bm/soem_servo_motor.c
    SOURCES examples/soem_examples/soem_servo_motor/bm/axis_nc.c
    SOURCES examples/soem_examples/soem_servo_motor/bm/axis_nc.h
    SOURCES examples/soem_examples/soem_servo_motor/bm/cia402.c
    SOURCES examples/soem_examples/soem_servo_motor/bm/cia402.h
    SOURCES examples/soem_examples/soem_servo_motor/bm/servo.c
    SOURCES examples/soem_examples/soem_servo_motor/bm/servo.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/soem_examples/soem_servo_motor/bm
)

include(${SdkRootDirPath}/examples/_boards/${board}/soem_examples/soem_servo_motor/bm/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)