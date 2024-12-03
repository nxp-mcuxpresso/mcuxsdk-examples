cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lwip_dhcp_usb_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/lwip_examples/lwip_dhcp_usb/freertos)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/lwip_examples/lwip_dhcp_usb/freertos/lwip_dhcp_usb_freertos.c
            examples/lwip_examples/lwip_dhcp_usb/freertos/lwipopts.h
            examples/lwip_examples/lwip_dhcp_usb/freertos/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/lwip_examples/lwip_dhcp_usb/freertos
)

mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DUSB_STACK_FREERTOS\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_DEBUGCONSOLE=1"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=2048\
             __heap_size__=25600"
)

include(${SdkRootDirPath}/examples/_boards/${board}/lwip_examples/lwip_dhcp_usb/freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)