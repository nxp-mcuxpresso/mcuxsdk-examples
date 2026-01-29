include(${CMAKE_CURRENT_LIST_DIR}/../../common/reconfig.cmake)

mcux_add_macro(
  CC
  "-DMBEDTLS_X509_CRT_PARSE_C\
        -DMBEDTLS_X509_USE_C\
        -DMBEDTLS_SSL_TLS_C\
        -DMBEDTLS_SSL_PROTO_TLS1_2\
        -DMBEDTLS_SSL_PROTO_TLS1_3\
        -DMBEDTLS_SSL_KEEP_PEER_CERTIFICATE\
        -DMBEDTLS_SSL_CLI_C")

mcux_add_iar_configuration(CC "--diag_suppress Pe177" CX
                           "--diag_suppress Pe177")
