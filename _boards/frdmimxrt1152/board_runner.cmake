board_runner_args(jlink "--device=${CONFIG_MCUX_TOOLCHAIN_JLINK_CPU_IDENTIFIER}" "--tool-opt=-jlinkscriptfile ${CMAKE_CURRENT_LIST_DIR}/frdmimxrt1152_connect_cm7.jlinkscript")
board_runner_args(linkserver "--device=${CONFIG_MCUX_HW_DEVICE_ID}:FRDM-IMXRT1152")

include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
