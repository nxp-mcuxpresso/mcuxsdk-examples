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

# -------- adjust linker script  for armgcc -----------
mcux_remove_armgcc_linker_script(
    TARGETS debug release
    LINKER ${SdkRootDirPath}/devices/MCX/MCXE/MCXE31B/gcc/MCXE31B_flash.ld
)
mcux_add_armgcc_linker_script(
            TARGETS debug release
            BASE_PATH ${SdkRootDirPath}
            LINKER ${board_root}/${board}/el2go_examples/el2go_csr/linker/armgcc/MCXE31B_flash.ld
)

