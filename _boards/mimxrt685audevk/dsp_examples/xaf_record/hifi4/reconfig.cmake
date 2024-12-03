#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_xtensa_configuration(
    CC "-DSDK_DEBUGCONSOLE=1\
        -DXOS_CLOCK_FREQ=594000000\
        -DXF_CORE_ID=0\
        -DXF_CFG_CORES_NUM=1\
        -DXA_PCM_GAIN=1\
        -DXA_VIT_PRE_PROC=1\
        -DXA_VOICE_SEEKER=1\
        -DXA_RENDERER=1\
        -DXA_CAPTURER=1\
        -DXA_DISABLE_EVENT=1\
        -DVIT_DEVICE_ID=VIT_IMXRT600\
        -DRENDERER_I2S\
        -DCAPTURER_DMIC\
        -DDMIC_BOARD_CONNECTED=1"
)
