cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(co_device_bm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/canopennode_examples/device/bm)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/canopennode_examples/device/bm/co_device_bm.c
            examples/canopennode_examples/device/bm/port/CO_app_NXP.c
            examples/canopennode_examples/device/bm/port/CO_app_NXP.h
            examples/canopennode_examples/device/bm/port/CO_driver_NXP.c
            examples/canopennode_examples/device/bm/port/CO_driver_target.h
            examples/canopennode_examples/device/bm/port/OD.c
            examples/canopennode_examples/device/bm/port/OD.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/canopennode_examples/device/bm/port
             ${board_root}/${board}/canopennode_examples/device/bm
             ${board_root}/${board}/canopennode_examples/device/bm/${core_id}
)

mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x4000"
)

include(${SdkRootDirPath}/${board_root}/${board}/canopennode_examples/device/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
