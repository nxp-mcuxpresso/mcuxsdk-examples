
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mmccard_polling LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/sdmmc_examples/mmccard_polling)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/sdmmc/example/mmccard_polling/mmccard_polling.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/sdmmc/example/mmccard_polling
)

include(${SdkRootDirPath}/${board_root}/${board}/sdmmc_examples/mmccard_polling/${core_id}/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/${board_root}/${board}/sdmmc_examples/mmccard_polling/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
