cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(zigbee_coordinator_bm_r23 LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        examples/_boards/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH
        examples/_boards/${board}/wireless_examples/zigbee
        examples/_boards/${board}/wireless_examples/zigbee/coordinator/bm
        examples/_boards/${board}/wireless_examples/zigbee/coordinator/bm/r23
)
include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/zigbee/coordinator/bm/reconfig.cmake OPTIONAL)

include(../coordinator.cmake)
