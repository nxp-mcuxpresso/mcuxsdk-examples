mcux_set_variable(board evkbmimxrt1170)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1176)
endif()

include(${SdkRootDirPath}/devices/RT/RT1170/${device}/variable.cmake)
