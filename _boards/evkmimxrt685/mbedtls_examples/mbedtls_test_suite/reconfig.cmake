
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/mbedtls/mbedtls_test_suite/test_aes.c
            examples/demo_apps/mbedtls/mbedtls_test_suite/test_des.c
            examples/demo_apps/mbedtls/mbedtls_test_suite/test_shax.c
            examples/demo_apps/mbedtls/mbedtls_test_suite/erpc_config.h
            middleware/multicore/example/multiprocessor_examples/erpc_common/erpc_error_handler.h
            middleware/multicore/example/multiprocessor_examples/erpc_common/erpc_error_handler.cpp
            examples/demo_apps/mbedtls/mbedtls_test_suite/service/mbedtls_test_suite.erpc
            examples/demo_apps/mbedtls/mbedtls_test_suite/service/erpc_crc16.h
            examples/demo_apps/mbedtls/mbedtls_test_suite/service/mbedtls_test_suite.h
            examples/demo_apps/mbedtls/mbedtls_test_suite/service/mbedtls_test_suite_server.h
            examples/demo_apps/mbedtls/mbedtls_test_suite/service/mbedtls_test_suite_server.cpp
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/mbedtls/mbedtls_test_suite
             middleware/multicore/example/multiprocessor_examples/erpc_common
             examples/demo_apps/mbedtls/mbedtls_test_suite/service
)

mcux_add_armgcc_configuration(
    CC "-Wno-pointer-sign\
       -Wno-implicit-function-declaration\
       -Wno-incompatible-pointer-types"
)

mcux_add_macro(
    CC "-DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\"\
       -DFREESCALE_KSDK_BM\
       -DMBEDTLS_FREESCALE_HASHCRYPT_SHA1\
       -DMBEDTLS_FREESCALE_HASHCRYPT_SHA256\
       -DPRINTF_FLOAT_ENABLE=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)
