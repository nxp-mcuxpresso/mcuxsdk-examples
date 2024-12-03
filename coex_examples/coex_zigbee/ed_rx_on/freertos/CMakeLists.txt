cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

mcux_set_variable(ZIGBEE_ROLE     ed_rx_on)

mcux_set_variable(COEX_IMAGE_NAME coex_zigbee_${ZIGBEE_ROLE})
mcux_set_variable(COEX_BOARD_BASE examples/_boards/${board}/coex_examples/coex_zigbee)
mcux_set_variable(COEX_SRC_BASE   examples/coex_examples/coex_zigbee)

include(${SdkRootDirPath}/examples/coex_examples/coex_zigbee/app_config.cmake)

if(${CONFIG_WIFI})
if(${CONFIG_WPA_SUPPLICANT})
project(${COEX_IMAGE_NAME} LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        ${COEX_BOARD_BASE}
    CUSTOM_PRJ_CONF_PATH
        ${COEX_SRC_BASE}/wifi
        ${COEX_SRC_BASE}/wifi/wpa_supplicant
        ${COEX_BOARD_BASE}/zigbee
        ${COEX_BOARD_BASE}/wifi
        ${COEX_BOARD_BASE}/wifi/wpa_supplicant
        ${COEX_BOARD_BASE}/edgefast
        ${COEX_BOARD_BASE}/freertos
        ${COEX_SRC_BASE}/edgefast
)
else()
project(${COEX_IMAGE_NAME} LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        ${COEX_BOARD_BASE}
    CUSTOM_PRJ_CONF_PATH
        ${COEX_SRC_BASE}/wifi
        ${COEX_BOARD_BASE}/zigbee
        ${COEX_BOARD_BASE}/wifi
        ${COEX_BOARD_BASE}/edgefast
        ${COEX_BOARD_BASE}/freertos
        ${COEX_SRC_BASE}/edgefast
)
endif()
else()
project(${COEX_IMAGE_NAME} LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        ${COEX_BOARD_BASE}
    CUSTOM_PRJ_CONF_PATH
        ${COEX_BOARD_BASE}/zigbee
        ${COEX_BOARD_BASE}/edgefast
        ${COEX_BOARD_BASE}/freertos
        ${COEX_SRC_BASE}/edgefast
)
endif()

include(${SdkRootDirPath}/${COEX_SRC_BASE}/coex.cmake)
