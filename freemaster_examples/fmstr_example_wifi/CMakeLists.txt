cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(fmstr_example_wifi LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/freemaster_examples/fmstr_wifi)

include(${SdkRootDirPath}/CMakeLists.txt)

# Include optional board specific configuration cmake file
include(${SdkRootDirPath}/examples/_boards/${board}/freemaster_examples/fmstr_wifi/reconfig.cmake OPTIONAL)

# Load common source files
include(../common/CMakeLists.txt)
include(../common/example/common/CMakeLists.txt)

# Add board config file
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        ${project_board_port_path}/wifi_board.h
)

# Add FreeRTOS config file
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/_boards/${board}/FreeRTOSConfigBoard.h
)

# Preinclude app config
mcux_add_source(
    SOURCES ../common/network/config_wifi/app_config.h
    PREINCLUDE TRUE
)

# Add wifi example source files
mcux_add_source(
    SOURCES
        ../common/network/network_wifi.h
        ../common/network/network_wifi.c
        ../common/network/config_wifi/wifi_config.h
        ../common/network/config_wifi/lwipopts.h
        ../common/network/config_wifi/lwippools.h
        ../common/network/config_wifi/FreeRTOSConfig.h
)

# Add network example include paths
mcux_add_include(
    INCLUDES
        ../common/network
        ../common/network/config_wifi
)

# Define example flags
mcux_add_macro(
    CC "USE_RTOS=1"
)