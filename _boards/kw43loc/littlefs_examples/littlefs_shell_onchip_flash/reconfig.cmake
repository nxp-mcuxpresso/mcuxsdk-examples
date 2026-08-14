
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/littlefs_examples/littlefs_shell_onchip_flash/${core_id}
)



# Add additional configuration
mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1\
       -DSDK_DEBUGCONSOLE_UART"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS release
    CC "-DLFS_NO_ASSERT"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS sdram_release
    CC "-DLFS_NO_ASSERT"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS hyperram_release
    CC "-DLFS_NO_ASSERT"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS release
    CC "-DLFS_NO_ASSERT"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS sdram_release
    CC "-DLFS_NO_ASSERT"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS hyperram_release
    CC "-DLFS_NO_ASSERT"
)
