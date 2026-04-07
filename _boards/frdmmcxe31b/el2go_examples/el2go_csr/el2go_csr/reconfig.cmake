mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS release
    CC "-DLFS_NO_ASSERT"
)

mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1"
)

mcux_add_macro(
    CC "PSA_WANT_ALG_GCM\
        PSA_WANT_ALG_SHA_1"
)

# -------- adjust flash linker script for toolchains -----------
mcux_remove_armgcc_linker_script(
    TARGETS flash_debug flash_release
    LINKER ${SdkRootDirPath}/devices/MCX/MCXE/MCXE31B/gcc/MCXE31B_flash.ld
)
mcux_add_armgcc_linker_script(
            TARGETS flash_debug flash_release
            BASE_PATH ${SdkRootDirPath}
            LINKER ${board_root}/${board}/el2go_examples/el2go_csr/linker/gcc/MCXE31B_flash.ld
)

mcux_remove_iar_linker_script(
    TARGETS flash_debug flash_release
    LINKER ${SdkRootDirPath}/devices/MCX/MCXE/MCXE31B/iar/MCXE31B_flash.icf
)
mcux_add_iar_linker_script(
            TARGETS flash_debug flash_release
            BASE_PATH ${SdkRootDirPath}
            LINKER ${board_root}/${board}/el2go_examples/el2go_csr/linker/iar/MCXE31B_flash.icf
)

mcux_remove_mdk_linker_script(
    TARGETS flash_debug flash_release
    LINKER ${SdkRootDirPath}/devices/MCX/MCXE/MCXE31B/arm/MCXE31B_flash.scf
)
mcux_add_mdk_linker_script(
            TARGETS flash_debug flash_release
            BASE_PATH ${SdkRootDirPath}
            LINKER ${board_root}/${board}/el2go_examples/el2go_csr/linker/arm/MCXE31B_flash.scf
)

# -------- adjust heap and stack size -----------
# min 8kB needed, for swapping flash sector on pflash 
# with read-modify-write pattern
mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x2400"
)
mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x2400"
)
mcux_add_mdk_configuration(
  LD "--predefine=-D__heap_size__=0x2400\
     --predefine=-D__stack_size__=0x2400"
  )
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x2400\
      -Xlinker --defsym=__stack_size__=0x2400"
  )

# --------- add suppression flags -----------
mcux_add_iar_configuration(
    CC "--diag_suppress Pe188"
    CX "--diag_suppress Pe188"
)
