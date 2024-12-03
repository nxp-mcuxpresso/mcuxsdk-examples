mcux_set_variable(board evkbimxrt1050)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1052)
endif()

include(${SdkRootDirPath}/devices/RT/RT1050/${device}/variable.cmake)
