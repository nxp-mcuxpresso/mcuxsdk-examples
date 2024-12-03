cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mbedtls_selftest_thread LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/mbedtls_examples/mbedtls_selftest_thread)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/mbedtls/example/mbedtls_selftest_thread/selftest_thread.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/mbedtls/example/mbedtls_selftest_thread
)

include(${SdkRootDirPath}/examples/_boards/${board}/mbedtls_examples/mbedtls_selftest_thread/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)