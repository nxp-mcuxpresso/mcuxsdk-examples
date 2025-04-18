cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(hci_bb_mcxw23 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples/ble_controller/hci_bb)


include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_configuration(
    CC "-DUSE_HCI_TRANSPORT -DBLECONTROLLER_ENABLE_WARNINGS"
)
message(board="${board}")
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/wireless_examples/ble_controller/hci_bb_mcxw23/main.c
    SOURCES examples/wireless_examples/ble_controller/hci_bb_mcxw23/hci_custom_cmds.c
    SOURCES examples/wireless_examples/ble_controller/hci_bb_mcxw23/hci_custom_cmds.h
    SOURCES examples/wireless_examples/ble_controller/hci_bb_mcxw23/datapump.c
    SOURCES examples/wireless_examples/ble_controller/hci_bb_mcxw23/radiotestsuite_cmds.c
    SOURCES examples/wireless_examples/ble_controller/hci_bb_mcxw23/hci_uart.c
    SOURCES examples/wireless_examples/ble_controller/hci_bb_mcxw23/hci_uart.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/wireless_examples/ble_controller/hci_bb
)

# include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/ble_controller/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
