cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(zigbee_router_freertos LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        examples/_boards/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH
        examples/_boards/${board}/wireless_examples/zigbee
        examples/_boards/${board}/wireless_examples/zigbee/router/freertos
)

include(variable.cmake)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/zigbee/router/freertos/reconfig.cmake OPTIONAL)

include(router.cmake)
