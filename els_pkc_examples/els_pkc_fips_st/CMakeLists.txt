cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(els_pkc_fips_st LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/els_pkc_examples/els_pkc_fips_st)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
            examples/els_pkc_examples/els_pkc_fips_st/src/els_pkc_fips_st.c
            examples/els_pkc_examples/els_pkc_fips_st/src/els_pkc_fips_cipher.c
            examples/els_pkc_examples/els_pkc_fips_st/inc/els_pkc_fips_cipher.h
            examples/els_pkc_examples/els_pkc_fips_st/src/els_pkc_fips_ecdsa.c
            examples/els_pkc_examples/els_pkc_fips_st/inc/els_pkc_fips_ecdsa.h
            examples/els_pkc_examples/els_pkc_fips_st/src/els_pkc_fips_rsa.c
            examples/els_pkc_examples/els_pkc_fips_st/inc/els_pkc_fips_rsa.h
            examples/els_pkc_examples/els_pkc_fips_st/src/els_pkc_fips_hash.c
            examples/els_pkc_examples/els_pkc_fips_st/inc/els_pkc_fips_hash.h
            examples/els_pkc_examples/els_pkc_fips_st/src/els_pkc_fips_mac.c
            examples/els_pkc_examples/els_pkc_fips_st/inc/els_pkc_fips_mac.h
            examples/els_pkc_examples/els_pkc_fips_st/src/els_pkc_fips_drbg.c
            examples/els_pkc_examples/els_pkc_fips_st/inc/els_pkc_fips_drbg.h
            examples/els_pkc_examples/els_pkc_fips_st/src/els_pkc_fips_key_gen.c
            examples/els_pkc_examples/els_pkc_fips_st/inc/els_pkc_fips_key_gen.h
            examples/els_pkc_examples/els_pkc_fips_st/src/els_pkc_fips_kdf.c
            examples/els_pkc_examples/els_pkc_fips_st/inc/els_pkc_fips_kdf.h
            examples/els_pkc_examples/els_pkc_fips_st/src/els_pkc_fips_ecdh.c
            examples/els_pkc_examples/els_pkc_fips_st/inc/els_pkc_fips_ecdh.h
            examples/els_pkc_examples/els_pkc_fips_st/src/els_pkc_fips_util.c
            examples/els_pkc_examples/els_pkc_fips_st/inc/els_pkc_fips_util.h
            examples/els_pkc_examples/els_pkc_fips_st/inc/els_pkc_fips_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/els_pkc_examples/els_pkc_fips_st/inc
             examples/els_pkc_examples/els_pkc_fips_st/inc/test_vectors
)

include(${SdkRootDirPath}/examples/_boards/${board}/els_pkc_examples/els_pkc_fips_st/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)