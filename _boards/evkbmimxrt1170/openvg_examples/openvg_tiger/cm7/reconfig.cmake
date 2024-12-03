mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/FreeRTOSConfigBoard.h
            examples/_boards/${board}/openvg_examples/openvg_support/pin_mux.c
            examples/_boards/${board}/openvg_examples/openvg_support/pin_mux.h
            examples/_boards/${board}/openvg_examples/openvg_support/assert_func.c
            examples/_boards/${board}/openvg_examples/openvg_tiger/vglite_support.c
            examples/_boards/${board}/openvg_examples/openvg_tiger/vglite_support.h
            examples/_boards/${board}/display_support.h
            examples/_boards/${board}/display_support.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/openvg_examples/openvg_tiger
             examples/_boards/${board}/openvg_examples
             examples/_boards/${board}/openvg_examples/openvg_support
)

mcux_add_iar_configuration(
    TARGETS sdram_debug sdram_release
    LD "--semihosting\
       --redirect __iar_sh_stdout=__iar_sh_stdout_swo"
)

mcux_add_macro(
    CC "-DSTATIC_LINK=1\
        -DgcdSTATIC_LINK=1\
        -DGCID_REV_CID=gc355/0x0_1216"
)

mcux_add_configuration(
    TARGETS sdram_debug
    CC "-DDBG=1"
)

mcux_add_configuration(
    TARGETS sdram_debug
    TOOLCHAINS armgcc
    CC "-g -O0"
)

mcux_add_configuration(
    TOOLCHAINS iar
    CC "--diag_suppress Pe188,Pa089"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_debug sdram_release
    LINKER devices/RT/RT1170/MIMXRT1176/iar/MIMXRT1176xxxxx_cm7_sdram.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_debug sdram_release
    LINKER devices/RT/RT1170/MIMXRT1176/gcc/MIMXRT1176xxxxx_cm7_sdram.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_debug sdram_release
    LINKER examples/_boards/${board}/openvg_examples/openvg_tiger/openvg_tiger_MIMXRT1176xxxxx_cm7_sdram_only.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_debug sdram_release
    LINKER examples/_boards/${board}/openvg_examples/openvg_tiger/openvg_tiger_MIMXRT1176xxxxx_cm7_sdram_only.ld
)
