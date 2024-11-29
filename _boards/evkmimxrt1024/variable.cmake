mcux_set_variable(board evkmimxrt1024)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1024)
endif()

include(${SdkRootDirPath}/devices/RT/RT1020/${device}/variable.cmake)
