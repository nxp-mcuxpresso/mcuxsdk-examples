mcux_set_variable(board frdmimxrt1186)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1186)
endif()

include(${SdkRootDirPath}/devices/RT/RT1180/${device}/variable.cmake)
