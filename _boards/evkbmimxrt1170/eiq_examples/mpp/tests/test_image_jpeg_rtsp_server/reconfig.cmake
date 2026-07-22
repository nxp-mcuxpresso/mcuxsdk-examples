# board+application specific cmake
mcux_add_macro(
    CC "-DRTOS_HEAP_SIZE=3000 \
    -DconfigGENERATE_RUN_TIME_STATS=1 \
    -DENABLE_ETHERNET_PHY \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DUSE_RTOS=1"
    CX "DconfigGENERATE_RUN_TIME_STATS=1"
)
