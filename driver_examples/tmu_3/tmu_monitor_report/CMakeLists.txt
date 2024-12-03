cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tmu_3_monitor_report LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/tmu/tmu_monitor_report)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/tmu_3/tmu_monitor_report/tmu_monitor_report.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/tmu/tmu_monitor_report
             examples/_boards/${board}/driver_examples/tmu/tmu_monitor_report/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/tmu/tmu_monitor_report/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)