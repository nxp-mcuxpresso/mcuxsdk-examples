cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(zigbee_router_ble_wuart_bm_r23 LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH
        examples/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/zigbee
        ${board_root}/${board}/wireless_examples/bluetooth
        ${board_root}/${board}/wireless_examples/zigbee/router_ble_wuart/bm
        ${board_root}/${board}/wireless_examples/zigbee/router_ble_wuart/bm/r23
        ${board_root}/${board}/wireless_examples/bluetooth/w_uart/${core_id}/bm
)

include(../../freertos/variable.cmake)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/zigbee/router_ble_wuart/bm/reconfig.cmake OPTIONAL)

include(../router.cmake)
