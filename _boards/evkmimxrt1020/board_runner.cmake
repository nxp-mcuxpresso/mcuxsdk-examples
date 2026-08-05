board_runner_args(jlink "--device=${CONFIG_MCUX_TOOLCHAIN_JLINK_CPU_IDENTIFIER}" "--tool-opt=-jlinkscriptfile ${CMAKE_CURRENT_LIST_DIR}/evkmimxrt1020_sdram_init.jlinkscript")
board_runner_args(linkserver "--device=${CONFIG_MCUX_HW_DEVICE_ID}:EVK-MIMXRT1020")

include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
