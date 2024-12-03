cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(fmstr_example_can LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/freemaster_examples/fmstr_can)

include(${SdkRootDirPath}/CMakeLists.txt)

# Include optional board specific configuration cmake file
include(${SdkRootDirPath}/examples/_boards/${board}/freemaster_examples/fmstr_can/reconfig.cmake OPTIONAL)

# Load common source files
include(../common/CMakeLists.txt)
include(../common/example/common/CMakeLists.txt)