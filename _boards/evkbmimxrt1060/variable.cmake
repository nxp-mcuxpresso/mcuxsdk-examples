mcux_set_variable(board evkbmimxrt1060)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1062)
endif()

include(${SdkRootDirPath}/devices/RT/RT1060/${device}/variable.cmake)
