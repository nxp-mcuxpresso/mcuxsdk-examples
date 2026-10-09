cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES
          ./connectivity_test.c
          ./connectivity_test.h
          ./gen_fsk_tests.c
          ./gen_fsk_tests.h
          ./gen_fsk_tests_states.h
          ./menus.c
          ./menus.h
)

# list of strong sync addresses
set(SYNC_ADDR_LIST
    0x8E89BED6 # BLE advertising address, default but not the best
    0x75813B8F
    0x7196A3C5
    0xC9F2D471
    0x913EC5A7
    0x4DB5E792
    0x273F9A5C
    0xA5724BE3
    0x4F63D8B5
    0xA3C5296F
    0xD34E95A7
    0x95C73A6D
    0x7AC5E391
    0xC56B92F3
    0xE35A4DC7
    0x5F2C93B6
    0xB674D9A5
    0x3EA5C7D9
    0xD9725BAC
    0x8BC75D32
    0x6D3AE591
)
# sync address to use from the list: 0 to 20
list(GET SYNC_ADDR_LIST 0 SYNC_ADDR)

mcux_add_configuration(
    CC "        -DHAL_RPMSG_SELECT_ROLE=0\
                -DSDK_COMPONENT_INTEGRATION=1\
                -DFSL_OSA_TASK_ENABLE=1\
                -DENABLE_RAM_VECTOR_TABLE\
                -DOSA_USED\
                -DMCUXPRESSO_SDK\
                -DgGenFskDefaultSyncAddress_c=${SYNC_ADDR}"
)

mcux_add_include(
    INCLUDES .
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/${bluetooth_root}/boards/${board}/app_preinclude_common.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wireless/${bluetooth_root}/boards/${board}
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/genfsk/connectivity_test/reconfig.cmake OPTIONAL)
mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/connectivity_test_bm.bin)