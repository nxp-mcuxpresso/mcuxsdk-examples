mcux_set_variable(board evkmimxrt1010)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1011)
endif()

include(${SdkRootDirPath}/devices/RT/RT1010/${device}/variable.cmake)
