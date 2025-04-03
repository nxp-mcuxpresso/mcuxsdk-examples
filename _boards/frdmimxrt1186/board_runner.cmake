board_runner_args(jlink "--device=${CONFIG_MCUX_TOOLCHAIN_JLINK_CPU_IDENTIFIER}")
board_runner_args(linkserver "--device=${CONFIG_MCUX_HW_DEVICE_ID}:FRDM-IMXRT1186")
board_runner_args(linkserver "--core=${core_id}")

include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
