cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lpc_i2c_polling_b2b_master LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/i2c/polling_b2b/master)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lpc_i2c/polling_b2b/master/i2c_polling_b2b_master.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/i2c/polling_b2b/master
             examples/_boards/${board}/driver_examples/i2c/polling_b2b/master/${core_id}
)

mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_I2C_DRIVER_TRANSACTIONAL_APIS=0"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/i2c/polling_b2b/master/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)