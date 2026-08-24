cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flexio_endat2_polling_transfer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/digital_encoder_examples/flexio_endat2/polling_transfer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/digital_encoder_examples/flexio_endat2/polling_transfer/flexio_endat2_polling_transfer.c
            examples/digital_encoder_examples/flexio_endat2/polling_transfer/flexio_endat2.c
            examples/digital_encoder_examples/flexio_endat2/polling_transfer/flexio_endat2.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/digital_encoder_examples/flexio_endat2/polling_transfer
             ${board_root}/${board}/digital_encoder_examples/flexio_endat2/polling_transfer/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/digital_encoder_examples/flexio_endat2/polling_transfer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
