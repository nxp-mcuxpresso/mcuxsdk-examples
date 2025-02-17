#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_xtensa_configuration(
    CC "-DSDK_DEBUGCONSOLE=1\
        -DXOS_CLOCK_FREQ=230000000\
        -DXF_CORE_ID=0\
        -DXF_CFG_CORES_NUM=1"
    LD "-Wl,--sort-section=alignment"
)

mcux_remove_configuration(
    TARGETS debug
    LD "-mlsp=${XTENSA_LSP_DEBUG}"
)
mcux_remove_configuration(
    TARGETS release
    LD "-mlsp=${XTENSA_LSP_RELEASE}"
)

mcux_add_xtensa_configuration(
    TARGETS debug
    LD "-mlsp=${SdkRootDirPath}/examples/_boards/mimxrt700evk/edgefast_bluetooth_examples/hifi_lc3/linker/hifi4/gdbio"
)
mcux_add_xtensa_configuration(
    TARGETS release
    LD "-mlsp=${SdkRootDirPath}/examples/_boards/mimxrt700evk/edgefast_bluetooth_examples/hifi_lc3/linker/hifi4/min-rt"
)
