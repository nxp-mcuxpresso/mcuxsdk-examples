mcux_set_variable(board evkmimxrt1015)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1015)
endif()

include(${SdkRootDirPath}/devices/RT/RT1015/${device}/variable.cmake)
