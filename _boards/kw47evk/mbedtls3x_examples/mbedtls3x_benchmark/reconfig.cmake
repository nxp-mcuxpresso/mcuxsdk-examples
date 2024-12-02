
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/mbedtls3x_examples/mbedtls3x_benchmark/${core_id}
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)

mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
)
