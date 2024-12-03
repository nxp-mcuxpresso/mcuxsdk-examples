cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(i3c_master_read_sensor_lsm6dso LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/i3c/master_read_sensor_lsm6dso)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/i3c/master_read_sensor_lsm6dso/i3c_master_read_sensor_lsm6dso.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/i3c/master_read_sensor_lsm6dso
             examples/_boards/${board}/driver_examples/i3c/master_read_sensor_lsm6dso/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/i3c/master_read_sensor_lsm6dso/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)