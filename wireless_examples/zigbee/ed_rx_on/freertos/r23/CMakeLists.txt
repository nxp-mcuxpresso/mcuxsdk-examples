cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(zigbee_ed_rx_on_freertos_r23 LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH
        ${board_root}/${board}/wireless_examples/zigbee
        ${board_root}/${board}/wireless_examples/zigbee/ed_rx_on/freertos
        ${board_root}/${board}/wireless_examples/zigbee/ed_rx_on/freertos/r23
)

include(../variable.cmake)
include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/zigbee/ed_rx_on/freertos/reconfig.cmake OPTIONAL)
include(../ed_rx_on.cmake)
