mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/mbedtls3x_examples/psa_crypto_opaque_key_examples/mbedtls_user_config.h
)

mcux_add_macro(
    CC "-DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_user_config.h\\\""
)
