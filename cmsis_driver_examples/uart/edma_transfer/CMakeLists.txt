cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(cmsis_uart_edma_transfer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/cmsis_driver_examples/uart/edma_transfer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/cmsis_driver_examples/usart/edma_transfer/cmsis_usart_edma_transfer.c
            examples/_boards/${board}/cmsis_driver_examples/uart/edma_transfer/RTE_Device.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/cmsis_driver_examples/uart/edma_transfer
             examples/_boards/${board}/cmsis_driver_examples/uart/edma_transfer/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/cmsis_driver_examples/uart/edma_transfer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)