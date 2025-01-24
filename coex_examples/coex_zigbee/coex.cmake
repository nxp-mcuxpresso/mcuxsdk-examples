mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${COEX_SRC_BASE}/main.c
            ${COEX_SRC_BASE}/common/controller_coex_nxp.c
            ${COEX_SRC_BASE}/common/coex_shell.c
            ${COEX_SRC_BASE}/common/coex_shell.h
            ${COEX_SRC_BASE}/common/coex_zigbee.c
            middleware/edgefast_bluetooth/source/shell/shell_common.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${COEX_SRC_BASE}
             ${COEX_SRC_BASE}/common
)

# Fix build errors temporarily
mcux_add_armgcc_configuration(
    LD "-Wl,-z -Wl,muldefs"
)

include(${SdkRootDirPath}/${COEX_SRC_BASE}/${ZIGBEE_ROLE}/freertos/variable.cmake OPTIONAL)

include(${SdkRootDirPath}/${COEX_BOARD_BASE}/zigbee/${ZIGBEE_ROLE}/freertos/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/${COEX_SRC_BASE}/${ZIGBEE_ROLE}/freertos/${ZIGBEE_ROLE}.cmake)
