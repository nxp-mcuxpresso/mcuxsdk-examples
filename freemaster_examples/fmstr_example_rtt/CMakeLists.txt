cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(fmstr_example_rtt LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/freemaster_examples/fmstr_rtt)

include(${SdkRootDirPath}/CMakeLists.txt)

# Include optional board specific configuration cmake file
include(${SdkRootDirPath}/examples/_boards/${board}/freemaster_examples/fmstr_rtt/reconfig.cmake OPTIONAL)

# Load common source files
include(../common/CMakeLists.txt)
include(../common/example/common/CMakeLists.txt)

# Add rtt config include path
mcux_add_include(
    INCLUDES
        ../common/rtt
)

# Add rtt config source file
mcux_add_source(
    SOURCES
        ../common/rtt/SEGGER_RTT_Conf.h
)