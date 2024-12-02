#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/examples/_boards/${board}/tfm_examples/reconfig.cmake OPTIONAL)

mcux_add_source(
#    BASE_PATH ${SdkRootDirPath}
#    PREINCLUDE TRUE
    SOURCES 
	cm33_core0/hardware_init.c
	cm33_core0/pin_mux.c
	cm33_core0/pin_mux.h
	cm33_core0/app.h
)

mcux_add_include(
#    BASE_PATH ${SdkRootDirPath}
    INCLUDES .
)

#add cc-defines
mcux_add_macro(
     CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DOCOTP_NV_COUNTERS_RAM_EMULATION=1\
       "
)

#mdk configurations:
mcux_remove_mdk_configuration(
    TARGETS debug
    CC "-O1"
    CX "-O1"
)
mcux_add_mdk_configuration(
    TARGETS debug
    CC "-Oz"
    CX "-Oz"
)

#armgcc configurations
mcux_remove_macro(
    TOOLCHAINS armgcc
    TARGETS debug
    CC "-DDEBUG"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS debug
    CC "-DNDEBUG"
)


#iar configurations
mcux_remove_iar_configuration(
    TARGETS debug
    CX "--diag_suppress=Pa082,Pa050"
    CC "--diag_suppress=Pa082,Pa050 -On"
)

mcux_add_iar_configuration(
    TARGETS debug
    CC "-Oh"
)
