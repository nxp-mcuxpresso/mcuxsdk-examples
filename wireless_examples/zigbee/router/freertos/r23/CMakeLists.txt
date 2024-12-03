cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(zigbee_router_freertos_r23 LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        examples/_boards/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH
        examples/_boards/${board}/wireless_examples/zigbee
        examples/_boards/${board}/wireless_examples/zigbee/router/freertos
        examples/_boards/${board}/wireless_examples/zigbee/router/freertos/r23
)

include(../variable.cmake)
include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/zigbee/router/freertos/reconfig.cmake OPTIONAL)
include(../router.cmake)
