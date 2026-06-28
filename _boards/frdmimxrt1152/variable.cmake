mcux_set_variable(board frdmimxrt1152)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1152)
endif()

include(${SdkRootDirPath}/devices/RT/RT1150/${device}/variable.cmake)
