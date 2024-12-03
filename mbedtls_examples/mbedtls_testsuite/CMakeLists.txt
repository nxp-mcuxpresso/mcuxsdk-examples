cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mbedtls_testsuite LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/mbedtls_examples/mbedtls_testsuite)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES 
    middleware/mbedtls/example/mbedtls_testsuite/platform_alt.c
    middleware/mbedtls/example/mbedtls_testsuite/platform_alt.h
    middleware/mbedtls/example/mbedtls_testsuite/mcux_mbedtls_testsuite_config.h
    middleware/mbedtls/example/mbedtls_testsuite/tests/test_suite_postprocessed.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/mbedtls/example/mbedtls_testsuite
)

include(${SdkRootDirPath}/examples/_boards/${board}/mbedtls_examples/mbedtls_testsuite/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)