cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(openvg_tiger_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/openvg_examples/openvg_tiger)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES tiger_freertos.c
            tiger_path.h
)

mcux_add_include(
    INCLUDES .
)

mcux_add_iar_configuration(
    CX "--diag_suppress=Pe257,Pa039"
    CC "--diag_suppress=Pe257,Pa039,Pe188,Pa089,Pe191,Pe111,Pa093,Pe177,Pe546,Pe236,Pa205,Pe136,Pa084,Pe550,Pe186"
    CX "--diag_suppress=Pe257,Pa039,Pe188,Pa089,Pe191,Pe111,Pa093,Pe177,Pe546,Pe236,Pa205,Pe136,Pa084,Pe550,Pe186"
)

mcux_add_armgcc_configuration(
    CC "-fms-extensions" "-Wno-unused-variable"
    CX "-fms-extensions" "-Wno-unused-variable"
    CC  "-fms-extensions            \
        -Wno-unused-variable        \
        -Wno-unused-function        \
        -Wno-unused-parameter       \
        -Wno-enum-conversion        \
        -Wno-int-conversion         \
        -Wno-nonstandard-extension  \
        -Wno-sign-compare"
    CX  "-fms-extensions            \
        -Wno-unused-variable        \
        -Wno-unused-function        \
        -Wno-unused-parameter       \
        -Wno-enum-conversion        \
        -Wno-int-conversion         \
        -Wno-nonstandard-extension  \
        -Wno-sign-compare"
)

mcux_add_mdk_configuration(
    CC "-fms-extensions" "--diag_suppress=1296"
    CX "-fms-extensions" "--diag_suppress=1296"
    CC "-fms-extensions" "--diag_suppress=1296,257,39,188,89,191,111,93,177,546,236,205,136,84" "-Wno-tautological-constant-out-of-range-compare"
    CX "-fms-extensions" "--diag_suppress=1296,257,39,188,89,191,111,93,177,546,236,205,136,84" "-Wno-tautological-constant-out-of-range-compare"
)

include(${SdkRootDirPath}/${board_root}/${board}/openvg_examples/openvg_tiger/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
