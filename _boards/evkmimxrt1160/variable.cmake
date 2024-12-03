mcux_set_variable(board evkmimxrt1160)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1166)
endif()

include(${SdkRootDirPath}/devices/RT/RT1160/${device}/variable.cmake)
