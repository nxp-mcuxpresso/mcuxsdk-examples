cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(cmsis_lpi2c_int_b2b_transfer_slave LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/cmsis_driver_examples/lpi2c/int_b2b_transfer/slave)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/cmsis_driver_examples/lpi2c/int_b2b_transfer/slave/cmsis_lpi2c_int_b2b_transfer_slave.c
            ${board_root}/${board}/cmsis_driver_examples/lpi2c/int_b2b_transfer/slave/RTE_Device.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/cmsis_driver_examples/lpi2c/int_b2b_transfer/slave
             ${board_root}/${board}/cmsis_driver_examples/lpi2c/int_b2b_transfer/slave/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/cmsis_driver_examples/lpi2c/int_b2b_transfer/slave/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)