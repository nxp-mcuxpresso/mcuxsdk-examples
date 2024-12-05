cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flexio3_i2c_read_accel_value_transfer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/flexio3/i2c/read_accel_value_transfer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexio/i2c/read_accel_value_transfer/flexio_i2c_read_accel_value_transfer.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/flexio3/i2c/read_accel_value_transfer
             ${board_root}/${board}/driver_examples/flexio3/i2c/read_accel_value_transfer/${core_id}
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/flexio3/i2c/read_accel_value_transfer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)