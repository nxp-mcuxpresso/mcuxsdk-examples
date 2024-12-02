cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(usart_polling_example LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/usart/polling)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lpc_miniusart/polling/usart_polling.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/usart/polling
             examples/_boards/${board}/driver_examples/usart/polling/${core_id}
)

mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_USART_DRIVER_TRANSACTIONAL_APIS=0"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/usart/polling/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)