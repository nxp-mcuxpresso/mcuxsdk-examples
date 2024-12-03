cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(usb_pd_freertos_om13790dock LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/usb_examples/usb_pd_om13790dock/freertos)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/usb/example/usb_pd_om13790dock/freertos/pd_app.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/usb/example/usb_pd_om13790dock/freertos
)

include(${SdkRootDirPath}/examples/_boards/${board}/usb_examples/usb_pd_om13790dock/freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)