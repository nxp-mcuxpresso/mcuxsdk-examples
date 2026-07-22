# board+application specific cmake

mcux_add_macro(
    CC "-DOPENH264 \
    -DENABLE_ETHERNET_PHY \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DUSE_RTOS=1"
)