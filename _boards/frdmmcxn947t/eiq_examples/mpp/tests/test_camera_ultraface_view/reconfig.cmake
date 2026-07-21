# board+application specific cmake

mcux_add_macro(
    CC "-DconfigGENERATE_RUN_TIME_STATS=1 -DRTOS_HEAP_SIZE=117"
)
