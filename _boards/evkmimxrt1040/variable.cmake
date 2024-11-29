mcux_set_variable(board evkmimxrt1040)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1042)
endif()

include(${SdkRootDirPath}/devices/RT/RT1040/${device}/variable.cmake)
