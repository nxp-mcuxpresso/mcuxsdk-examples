mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/FreeRTOSConfigBoard.h
            examples/_boards/${board}/openvg_examples/openvg_support/pin_mux.c
            examples/_boards/${board}/openvg_examples/openvg_support/pin_mux.h
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
    TARGETS debug release
    LD "--semihosting\
       --redirect __iar_sh_stdout=__iar_sh_stdout_swo"
)

mcux_add_macro(
    CC "-DSTATIC_LINK=1\
        -DgcdSTATIC_LINK=1\
        -DGCID_REV_CID=gc555/0x423_ECO\
        -DBOOT_ENABLE_XSPI1_PSRAM"
)

mcux_add_configuration(
    TARGETS debug
    CC "-DDBG=1"
)

mcux_add_configuration(
    TARGETS debug
    TOOLCHAINS armgcc
    CC "-g -O0"
)

mcux_add_configuration(
    TOOLCHAINS iar
    CC "--diag_suppress Pe188,Pa089,Pa205"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_ram.ld
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core0_ram.icf
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/openvg_examples/openvg_tiger/MIMXRT798Sxxxx_cm33_core0_psram.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/openvg_examples/openvg_tiger/MIMXRT798Sxxxx_cm33_core0_psram.icf
)

