cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(zigbee_coordinator_bm_r23 LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH
        ${board_root}/${board}/wireless_examples/zigbee
        ${board_root}/${board}/wireless_examples/zigbee/coordinator/bm
        ${board_root}/${board}/wireless_examples/zigbee/coordinator/bm/r23
)

include(../../freertos/variable.cmake)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/zigbee/coordinator/bm/reconfig.cmake OPTIONAL)

include(../coordinator.cmake)
