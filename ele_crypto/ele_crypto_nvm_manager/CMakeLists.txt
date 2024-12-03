cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ele_crypto_nvm_manager LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ele_crypto/ele_crypto_nvm_manager)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES ele_crypto_nvm.c
            sdmmc_nvm_manager.c
)

mcux_add_include(
    INCLUDES .
)

# Add sources / includes for the loadable ELE FW and sdmmc related files
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/ele_crypto/ele_crypto_nvm_manager/ele_fw.h
            examples/_boards/${board}/ele_crypto/ele_crypto_nvm_manager/ffconf.h
            examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/sdmmc_examples/sdcard_fatfs
)

# Modify stack and heap sizes for all IDEs
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x800\
             __heap_size__=0x600"
)

include(${SdkRootDirPath}/examples/_boards/${board}/ele_crypto/ele_crypto_nvm_manager/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
