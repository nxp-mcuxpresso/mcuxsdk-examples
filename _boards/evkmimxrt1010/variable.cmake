mcux_set_variable(board evkmimxrt1010)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1011)
endif()

include(${SdkRootDirPath}/devices/RT/RT1010/${device}/variable.cmake)
