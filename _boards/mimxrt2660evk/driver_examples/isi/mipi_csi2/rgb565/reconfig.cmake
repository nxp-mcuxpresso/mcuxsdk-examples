# Copyright 2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_configuration(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
)

mcux_add_source(
    SOURCES isi_config.h
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/isi/mipi_csi2/reconfig.cmake)
