cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(usart_wakeup_deepsleep LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/usart/wakeup_deepsleep)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexcomm/usart/wakeup_deepsleep/usart_wakeup_deepsleep.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/usart/wakeup_deepsleep
             examples/_boards/${board}/driver_examples/usart/wakeup_deepsleep/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/usart/wakeup_deepsleep/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)