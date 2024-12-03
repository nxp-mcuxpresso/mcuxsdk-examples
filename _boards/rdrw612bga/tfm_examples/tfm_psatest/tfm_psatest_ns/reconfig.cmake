#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/examples/_boards/${board}/tfm_examples/reconfig.cmake OPTIONAL)

mcux_add_macro(
     CC "-DPLATFORM_HAS_ATTEST_PK\
       "
)

mcux_add_armgcc_configuration(
    CC "-Wno-implicit-function-declaration"
)

mcux_add_mdk_configuration(
    CC "-Wno-implicit-function-declaration"
)

#mcux_add_iar_configuration(
#    CC "--diag_suppress Pe223"
#)