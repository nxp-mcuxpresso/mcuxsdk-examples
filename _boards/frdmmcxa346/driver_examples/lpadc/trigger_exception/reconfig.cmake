
mcux_add_mdk_configuration(
    TARGETS release
    CC "-O1"
)

mcux_remove_mdk_configuration(
    TARGETS release
    CC "-Oz"
)
