cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tflm_modelrunner LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/eiq_examples/tflm_modelrunner)
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/eiq_examples/tflm_modelrunner
             examples/eiq_examples/tflm_modelrunner/picohttpparser
             ${board_root}/${board}/eiq_examples/tflm_modelrunner/
)

mcux_project_remove_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/lwip/src/include/lwip/
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/tflm_modelrunner/main.cpp
            examples/eiq_examples/tflm_modelrunner/modelrunner.h
            examples/eiq_examples/tflm_modelrunner/modelrunner.cpp
            examples/eiq_examples/tflm_modelrunner/tf_benchmark.h
            examples/eiq_examples/tflm_modelrunner/tf_benchmark.cpp
            examples/eiq_examples/tflm_modelrunner/json_results.cpp
            examples/eiq_examples/tflm_modelrunner/http.h
            examples/eiq_examples/tflm_modelrunner/http.cpp
            examples/eiq_examples/tflm_modelrunner/http_handler.h
            examples/eiq_examples/tflm_modelrunner/http_handler.cpp
            examples/eiq_examples/tflm_modelrunner/FreeRTOSConfig.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
            examples/eiq_examples/tflm_modelrunner/lwipopts.h
            examples/eiq_examples/tflm_modelrunner/picohttpparser/picohttpparser.h
            examples/eiq_examples/tflm_modelrunner/picohttpparser/picohttpparser.c
            ${board_root}/${board}/eiq_examples/tflm_modelrunner/flash_opts.h
            ${board_root}/${board}/eiq_examples/tflm_modelrunner/flash_opts.c
	    examples/eiq_examples/tflm_modelrunner/scripts/cli.py
	    examples/eiq_examples/tflm_modelrunner/scripts/main.py
	    examples/eiq_examples/tflm_modelrunner/scripts/modelrunner.py
	    examples/eiq_examples/tflm_modelrunner/scripts/xexpect.py
	    examples/eiq_examples/tflm_modelrunner/scripts/requirements.txt   
)

mcux_add_mdk_configuration(
  CC "-std=gnu++17"
)

mcux_remove_armgcc_configuration(
  CX "--specs=nano.specs"
)

mcux_remove_armgcc_configuration(
  CC "--specs=nano.specs"
)

mcux_add_iar_configuration(
  CC "--diag_suppress=Pe236"
)

mcux_add_armgcc_configuration(
  CC "-Wno-aggressive-loop-optimizations"
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_modelrunner/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
