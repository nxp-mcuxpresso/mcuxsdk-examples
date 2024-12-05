cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(usart_transfer_hardware_flow_control LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/usart/transfer_hardware_flow_control)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lpc_miniusart/transfer_hardware_flow_control/usart_transfer_hardware_flow_control.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/usart/transfer_hardware_flow_control
             ${board_root}/${board}/driver_examples/usart/transfer_hardware_flow_control/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/usart/transfer_hardware_flow_control/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)