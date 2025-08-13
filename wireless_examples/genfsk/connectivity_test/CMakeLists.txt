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

mcux_add_configuration(
    CC "        -DHAL_RPMSG_SELECT_ROLE=0\
                -DSDK_COMPONENT_INTEGRATION=1\
                -DFSL_OSA_TASK_ENABLE=1\
                -DENABLE_RAM_VECTOR_TABLE\
                -DOSA_USED\
                -DMCUXPRESSO_SDK"
)

mcux_add_include(
    INCLUDES .
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/bluetooth/boards/${board}/app_preinclude_common.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES bluetooth/boards/${board}
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/genfsk/connectivity_test/reconfig.cmake OPTIONAL)
mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/connectivity_test_bm.bin)