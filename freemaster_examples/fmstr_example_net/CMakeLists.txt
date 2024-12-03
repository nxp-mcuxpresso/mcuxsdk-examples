cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(fmstr_example_net LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/freemaster_examples/fmstr_net)

include(${SdkRootDirPath}/CMakeLists.txt)

# Include optional board specific configuration cmake file
include(${SdkRootDirPath}/examples/_boards/${board}/freemaster_examples/fmstr_net/reconfig.cmake OPTIONAL)

# Load common source files
include(../common/CMakeLists.txt)
include(../common/example/common/CMakeLists.txt)

# Add network config file
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        ${project_board_port_path}/network_cfg.h
)

# Add FreeRTOS config file
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/_boards/${board}/FreeRTOSConfigBoard.h
)

# Add example NETC source files
mcux_add_source(
    SOURCES
        ../common/network/network_netc.c
    COMPONENTS driver.netc
)

# Add example ENET source files
mcux_add_source(
    SOURCES
        ../common/network/network_enet.c
    COMPONENTS driver.mcx_enet driver.enet
)

# Add network example source files
mcux_add_source(
    SOURCES
        ../common/network/network.h
        ../common/network/network.c
        ../common/network/config_eth/lwipopts.h
        ../common/network/config_eth/FreeRTOSConfig.h
)

# Add network example include paths
mcux_add_include(
    INCLUDES
        ../common/network
        ../common/network/config_eth
)

# Define example flags
mcux_add_macro(
    CC "USE_RTOS=1"
)