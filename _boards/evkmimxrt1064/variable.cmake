mcux_set_variable(board evkmimxrt1064)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1064)
endif()

include(${SdkRootDirPath}/devices/RT/RT1064/${device}/variable.cmake)
