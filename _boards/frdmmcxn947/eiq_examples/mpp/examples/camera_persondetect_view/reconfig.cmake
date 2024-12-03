# board+application specific cmake

mcux_add_macro(
    CC "-DRTOS_HEAP_SIZE=110\
        -DconfigGENERATE_RUN_TIME_STATS=1"
)
