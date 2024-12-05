
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/mbedtls3x_examples/psa_crypto_opaque_key_examples/ffconf.h
            examples/mbedtls3x_examples/psa_crypto_opaque_key_examples/mcux_mbedtls_psa_crypto_opaque_config.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/mbedtls3x_examples/psa_crypto_opaque_key_examples
             middleware/mbedtls3x/port
)

mcux_add_macro(
    CC "-DTEST_KEY_LOCATION=PSA_CRYPTO_ELE_S4XX_LOCATION -DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_user_config.h\\\""
)

mcux_add_macro(
        CC  "PSA_CRYPTO_DRIVER_ELE_S4XX"
)
	


