mcux_set_variable(board evkbmimxrt1170)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1176)
endif()

include(${SdkRootDirPath}/devices/RT/RT1170/${device}/variable.cmake)
