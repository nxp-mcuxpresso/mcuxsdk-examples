cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(co_manager_bm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/canopennode_examples/manager/bm)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/canopennode_examples/manager/bm/co_manager_bm.c
            examples/canopennode_examples/manager/bm/port/CO_app_NXP.c
            examples/canopennode_examples/manager/bm/port/CO_app_NXP.h
            examples/canopennode_examples/manager/bm/port/CO_driver_NXP.c
            examples/canopennode_examples/manager/bm/port/CO_driver_target.h
            examples/canopennode_examples/manager/bm/port/OD.c
            examples/canopennode_examples/manager/bm/port/OD.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/canopennode_examples/manager/bm/port
             ${board_root}/${board}/canopennode_examples/manager/bm
             ${board_root}/${board}/canopennode_examples/manager/bm/${core_id}
)

mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x4000"
)

mcux_add_iar_configuration(
    CC "--diag_suppress=Pe188,Pe186"
    CX "--diag_suppress=Pe188,Pe186"
)

include(${SdkRootDirPath}/${board_root}/${board}/canopennode_examples/manager/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
