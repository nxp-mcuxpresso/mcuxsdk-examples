cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ncp_loc_reader LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH middleware/wireless/ble_controller/src/KW4x
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/bluetooth/CMakeLists.txt)

mcux_add_configuration(
# define to force initialization of sqram_rldat_region after reset
# only required for GCC as IAR already initialize correctly this region
    TOOLCHAINS armgcc
    AS "        -DINIT_BLE_RL_DAT"
)

mcux_add_iar_configuration(
    TARGETS  release
    CC "     -Oh "
)
mcux_add_iar_configuration(
    TARGETS  debug
    CC "     --debug "
)
mcux_add_iar_configuration(
    TARGETS  debug
    LD "     --semihosting "
)

mcux_add_linker_symbol(
    SYMBOLS "__use_shmem__=1"
)

mcux_add_armgcc_configuration(
    TARGETS  release
    CC "     -Os "
)

mcux_add_iar_configuration(
    CC "-DTX_INCLUDE_USER_DEFINE_FILE -DENABLE_RAM_VECTOR_TABLE"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    SOURCES
            bluetooth/application/common/app_conn.c
            bluetooth/application/common/app_conn.h
            bluetooth/application/common/ble_init.c
            bluetooth/application/common/ble_init.h
            bluetooth/application/common/app_advertiser.c
            bluetooth/application/common/app_advertiser.h
            bluetooth/application/common/app_scanner.c
            bluetooth/application/common/app_scanner.h
            bluetooth/application/common/app_connection.c
            bluetooth/examples/ncp_loc_reader/app_config.c
            bluetooth/examples/ncp_loc_reader/gatt_db.h
            bluetooth/examples/ncp_loc_reader/gatt_uuid128.h
            bluetooth/examples/ncp_loc_reader/loc_reader.h
            bluetooth/examples/ncp_loc_reader/loc_reader.c
            bluetooth/examples/ncp_loc_reader/shell_loc_reader.c
            bluetooth/examples/ncp_loc_reader/shell_loc_reader.h
            bluetooth/examples/ncp_loc_reader/ncp_app.h
            bluetooth/examples/ncp_loc_reader/ncp_app.c
            bluetooth/application/common/ble_port_fsci/core1/main.c
            bluetooth/examples/ncp_loc_reader/readme.md
            ../../examples/_boards/${board}/wireless_examples/bluetooth/ncp_loc_reader/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/examples/ncp_loc_reader
        bluetooth/application/common/
        bluetooth/application/common/ble_port_fsci
        bluetooth/application/common/ble_port_fsci/core1
)

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/bluetooth/boards/${board}/bluetooth/ncp_loc_reader/app_preinclude.h
)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/bluetooth/ncp_loc_reader/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(
    TOOLCHAINS armgcc iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/ncp_loc_reader.bin
)