cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(els_pkc_benchmark LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/els_pkc_examples/els_pkc_benchmark)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES 
            examples/els_pkc_examples/els_pkc_benchmark/src/els_pkc_benchmark.c
            examples/els_pkc_examples/els_pkc_benchmark/src/els_pkc_bm_symmetric.c
            examples/els_pkc_examples/els_pkc_benchmark/inc/els_pkc_bm_symmetric.h
            examples/els_pkc_examples/els_pkc_benchmark/src/els_pkc_bm_asymmetric.c
            examples/els_pkc_examples/els_pkc_benchmark/inc/els_pkc_bm_asymmetric.h
            examples/els_pkc_examples/els_pkc_benchmark/src/els_pkc_bm_hash.c
            examples/els_pkc_examples/els_pkc_benchmark/inc/els_pkc_bm_hash.h
            examples/els_pkc_examples/els_pkc_benchmark/src/els_pkc_bm_mac.c
            examples/els_pkc_examples/els_pkc_benchmark/inc/els_pkc_bm_mac.h
            examples/els_pkc_examples/els_pkc_benchmark/src/els_pkc_bm_drbg.c
            examples/els_pkc_examples/els_pkc_benchmark/inc/els_pkc_bm_drbg.h
            examples/els_pkc_examples/els_pkc_benchmark/src/els_pkc_bm_kdf.c
            examples/els_pkc_examples/els_pkc_benchmark/inc/els_pkc_bm_kdf.h
            examples/els_pkc_examples/els_pkc_benchmark/src/els_pkc_bm_dh.c
            examples/els_pkc_examples/els_pkc_benchmark/inc/els_pkc_bm_dh.h
            examples/els_pkc_examples/els_pkc_benchmark/src/els_pkc_bm_kwa.c
            examples/els_pkc_examples/els_pkc_benchmark/inc/els_pkc_bm_kwa.h
            examples/els_pkc_examples/els_pkc_benchmark/src/els_pkc_benchmark_utils.c
            examples/els_pkc_examples/els_pkc_benchmark/inc/els_pkc_benchmark_utils.h
            examples/els_pkc_examples/els_pkc_benchmark/src/els_pkc_bm_key_gen.c
            examples/els_pkc_examples/els_pkc_benchmark/inc/els_pkc_bm_key_gen.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/els_pkc_examples/els_pkc_benchmark/inc
)

include(${SdkRootDirPath}/examples/_boards/${board}/els_pkc_examples/els_pkc_benchmark/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)