cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(fmstr_example_uart LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/freemaster_examples/fmstr_uart)

include(${SdkRootDirPath}/CMakeLists.txt)

# Include optional board specific configuration cmake file
include(${SdkRootDirPath}/examples/_boards/${board}/freemaster_examples/fmstr_uart/reconfig.cmake OPTIONAL)

# Load common source files
include(../common/CMakeLists.txt)
include(../common/example/common/CMakeLists.txt)