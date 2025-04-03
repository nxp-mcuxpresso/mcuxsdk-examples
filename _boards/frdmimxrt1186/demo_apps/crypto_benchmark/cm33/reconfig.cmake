
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES components/crypto_benchmark
             components/crypto_benchmark/include
             examples/_boards/${board}/demo_apps/crypto_benchmark
)

mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=1\
       -DSCANF_FLOAT_ENABLE=0\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSCANF_ADVANCED_ENABLE=0"
)
