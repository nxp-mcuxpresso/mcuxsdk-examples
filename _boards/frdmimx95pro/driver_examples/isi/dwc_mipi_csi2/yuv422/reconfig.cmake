# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/display_support.h
            ${board_root}/${board}/driver_examples/isi/dwc_mipi_csi2/yuv422/cm7/isi_config.h
)

mcux_add_include(
    INCLUDES ${board_root}/${board}/driver_examples/isi/dwc_mipi_csi2/yuv422/cm7/
)

include(${SdkRootDirPath}/${board_root}/frdmimx95pro/driver_examples/isi/dwc_mipi_csi2/reconfig.cmake)
