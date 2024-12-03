mcux_set_variable(board evkcmimxrt1060)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1062)
endif()

include(${SdkRootDirPath}/devices/RT/RT1060/${device}/variable.cmake)
