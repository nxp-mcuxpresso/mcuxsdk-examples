mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/openvg_examples/openvg_tiger/vglite_support.c
            ${board_root}/${board}/openvg_examples/openvg_tiger/vglite_support.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/openvg_examples/openvg_tiger
             ${board_root}/${board}/openvg_examples
)

mcux_add_iar_configuration(
    TARGETS hyperram_debug hyperram_release
    LD "--semihosting\
       --redirect __iar_sh_stdout=__iar_sh_stdout_swo"
)

mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)

mcux_add_macro(
    CC "-DSTATIC_LINK=1\
        -DgcdSTATIC_LINK=1\
        -DGCID_REV_CID=gc355/0x0_1216\
        -DBOARD_USE_PCAL6524=1\
        -DSDK_I2C_BASED_COMPONENT_USED=1"
)

mcux_add_configuration(
    TARGETS hyperram_debug
    CC "-DDBG=1"
)

mcux_add_configuration(
    TARGETS hyperram_debug
    TOOLCHAINS armgcc
    CC "-g -O0"
)

mcux_add_configuration(
    TOOLCHAINS iar
    CC "--diag_suppress Pe188,Pa089,Pa082,Pa050,Pa092"
)

# Remove the device default linker scripts. The OpenVG / VGLite path needs a
# much larger malloc() heap, a larger call stack and a large non-cacheable
# region than the device defaults provide, so this example ships its own
# linker scripts whose memory layout resembles the openvg_tiger layout used
# on MIMXRT1176.
#
# Only the hyperram (non-XIP) targets are supported: this example is
# performance sensitive and executing it XIP from FlexSPI NOR makes the
# rendering too slow, so the flexspi_nor_hyperram targets are disabled for
# this board in example.yml.
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS hyperram_debug hyperram_release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/iar/MIMXRT1152xxxxx_hyperram.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS hyperram_debug hyperram_release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/gcc/MIMXRT1152xxxxx_hyperram.ld
)

# Add the openvg_tiger specific linker scripts.
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS hyperram_debug hyperram_release
    LINKER ${board_root}/${board}/openvg_examples/openvg_tiger/openvg_tiger_MIMXRT1152xxxxx_hyperram.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS hyperram_debug hyperram_release
    LINKER ${board_root}/${board}/openvg_examples/openvg_tiger/openvg_tiger_MIMXRT1152xxxxx_hyperram.ld
)
