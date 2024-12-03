cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tmu_2_monitor_threshold LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/tmu/monitor_threshold)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/tmu_2/monitor_threshold/tmu_monitor_threshold.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/tmu/monitor_threshold
             examples/_boards/${board}/driver_examples/tmu/monitor_threshold/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/tmu/monitor_threshold/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)