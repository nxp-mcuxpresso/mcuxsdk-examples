cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(fmstr_example_any LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/freemaster_examples/fmstr_any)

include(${SdkRootDirPath}/CMakeLists.txt)

# Include optional board specific configuration cmake file
include(${SdkRootDirPath}/examples/_boards/${board}/freemaster_examples/fmstr_any/reconfig.cmake OPTIONAL)

# Load common source files
include(../common/CMakeLists.txt)

# Load example source files using peripherals
include(../common/example/peripherals/CMakeLists.txt)

# Add demo configuration
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
         ${project_board_port_path}/freemaster_democfg.h
)

# Add peripherals source files
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
         ${project_board_port_path}/peripherals.h
         ${project_board_port_path}/peripherals.c
)

# Add clock_config source files
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
         ${project_board_port_path}/clock_config.h
         ${project_board_port_path}/clock_config.c
)