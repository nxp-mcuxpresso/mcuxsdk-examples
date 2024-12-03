cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(cmsis_ii2c_int_b2b_transfer_master LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/cmsis_driver_examples/i2c/int_b2b_transfer/master)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/cmsis_driver_examples/i2c/int_b2b_transfer/master/cmsis_i2c_int_b2b_transfer_master.c
            examples/_boards/${board}/cmsis_driver_examples/i2c/int_b2b_transfer/master/RTE_Device.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/cmsis_driver_examples/i2c/int_b2b_transfer/master
             examples/_boards/${board}/cmsis_driver_examples/i2c/int_b2b_transfer/master/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/cmsis_driver_examples/i2c/int_b2b_transfer/master/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)