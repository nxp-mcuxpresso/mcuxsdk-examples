cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(zigbee_coordinator_freertos_r23 LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        examples/_boards/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH
        examples/_boards/${board}/wireless_examples/zigbee
        examples/_boards/${board}/wireless_examples/zigbee/coordinator/freertos
        examples/_boards/${board}/wireless_examples/zigbee/coordinator/freertos/r23
)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/zigbee/coordinator/freertos/reconfig.cmake OPTIONAL)
include(../coordinator.cmake)
