# board+application specific cmake

mcux_add_macro(
    CC "-DconfigGENERATE_RUN_TIME_STATS=1 \
    -DHAL_TIMER_PRECISION_1_US"
)
