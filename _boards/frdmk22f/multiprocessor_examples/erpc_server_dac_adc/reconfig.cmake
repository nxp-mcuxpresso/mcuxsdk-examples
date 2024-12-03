#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# Here example requires to use the RTE_Device.h header file in this folder.
# Remove the automaticaly added RTE_Device.h by device and add new one bellow remove statements.
mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}/devices/${soc_portfolio}/${soc_series}/${device}/template
    SOURCES RTE_Device.h
    CONFIG true
)

mcux_project_remove_include(
    BASE_PATH ${SdkRootDirPath}/devices/${soc_portfolio}/${soc_series}/${device}/template
    TARGET_FILES RTE_Device.h
    INCLUDES .
)

mcux_add_source(
    SOURCES RTE_Device.h
            fxos.c
)

mcux_add_include(
    INCLUDES .
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
)
