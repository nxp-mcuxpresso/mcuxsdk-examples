cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(hci_bb_bm LANGUAGES C CXX ASM 
    PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wireless_examples
)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES
           ../../common/bm/main.c
           ../hci_bb.c
           ../hci_bb_debug.h
)

mcux_add_configuration(
    CC "        -DSERIAL_MANAGER_NON_BLOCKING_MODE=1\
                -DHAL_RPMSG_SELECT_ROLE=0\
                -DPRINTF_FLOAT_ENABLE=0\
                -DSCANF_FLOAT_ENABLE=0\
                -DPRINTF_ADVANCED_ENABLE=0\
                -DSCANF_ADVANCED_ENABLE=0\
                -DSDK_COMPONENT_INTEGRATION=1\
                -DFSL_OSA_TASK_ENABLE=1\
                -DHAL_FLASH_ROMAPI_DRIVER=1\
                -DgUseHciTransportDownward_d=1\
                -DSERIAL_MANAGER_TASK_HANDLE_TX=1\
                -DOSA_USED\
                -DMCUXPRESSO_SDK\
                -DSDK_OS_BAREMETAL"
)

mcux_add_include(
    INCLUDES .
             ../
)

# We would need a bluetooth component that is simply exposing some HCI defines
mcux_add_include(
    INCLUDES ../../../../../../middleware/wireless/bluetooth/host/interface
)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/ble_controller/hci_bb/bm/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/ble_controller/hci_bb/bm/${multicore_foldername}/reconfig.cmake OPTIONAL)
