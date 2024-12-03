# Add additional configuration
mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
        -DDEBUG_CONSOLE_ASSERT_DISABLE=1\
        -DPBUF_POOL_SIZE=4"
)
