# Add additional configuration
mcux_add_macro(
    CC "-DDEBUG_CONSOLE_ASSERT_DISABLE=1\
        -DPBUF_POOL_SIZE=4\
        -DLWIP_IPV6=0\
        -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE"
)
mcux_add_iar_configuration(
    TARGETS debug sdram_debug sdram_release
    CC "-Oh -Ssize"
)
mcux_add_macro(
    TOOLCHAINS iar mdk armgcc
    TARGETS sdram_debug sdram_release
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_mdk_configuration(
    TARGETS sdram_debug
    CC "-Oz"
)

# Remove additional configuration
mcux_remove_iar_configuration(
    TARGETS debug
    CC "-On"
)
mcux_remove_iar_configuration(
    TARGETS sdram_debug sdram_release
    CC "-Om"
)
mcux_remove_mdk_configuration(
    TARGETS sdram_debug
    CC "-O1"
)
