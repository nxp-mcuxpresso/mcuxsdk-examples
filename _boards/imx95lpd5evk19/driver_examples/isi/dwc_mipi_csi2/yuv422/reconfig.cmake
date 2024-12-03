# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/driver_examples/isi/dwc_mipi_csi2/yuv422/cm7/dpu_example.h
            examples/_boards/${board}/driver_examples/isi/dwc_mipi_csi2/yuv422/cm7/isi_config.h
)

mcux_add_include(
    INCLUDES examples/_boards/${board}/driver_examples/isi/dwc_mipi_csi2/yuv422/cm7/
)

include(${SdkRootDirPath}/examples/_boards/imx95lpd5evk19/driver_examples/isi/dwc_mipi_csi2/reconfig.cmake)
