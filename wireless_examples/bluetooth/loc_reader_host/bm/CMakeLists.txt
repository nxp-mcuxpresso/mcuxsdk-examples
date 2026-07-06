cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(loc_reader_host_bm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH ${board_root}/${board}/wireless_examples/bluetooth/
        examples/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/bluetooth/loc_reader_host/
#       ${board_root}/${board}/wireless_examples/bluetooth/loc_reader_host/${core_id}/bm
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/bluetooth/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    SOURCES bluetooth/application/common/ble_port_fsci/core0/main.c
            bluetooth/application/common/ble_port_fsci/core0/app_init.c
            bluetooth/application/common/app_conn.h
            bluetooth/application/common/ble_init.h
            bluetooth/examples/loc_reader_host/loc_reader_host.c
            bluetooth/examples/loc_reader_host/loc_reader_host.h
            bluetooth/examples/loc_reader_host/host_app.c
            bluetooth/examples/loc_reader_host/host_app.h
            bluetooth/examples/loc_reader_host/readme.md
            ../../${board_root}/${board}/wireless_examples/bluetooth/loc_reader_host/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/application/common
        bluetooth/examples/loc_reader_host
        bluetooth/application/common/ble_port_fsci
        bluetooth/application/common/ble_port_fsci/core0
)

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/${bluetooth_root}/boards/${board}/bluetooth/loc_reader_host/app_preinclude.h
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/loc_reader_host/${multicore_foldername}/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/loc_reader_host_${board}.srec
)
