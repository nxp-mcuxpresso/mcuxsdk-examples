cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tflm_modelrunner_hifi4 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/eiq_examples/tflm_modelrunner_hifi4)
include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/eiq_examples/tflm_modelrunner_hifi4
             examples/eiq_examples/common
             ${board_root}/${board}/eiq_examples/tflm_modelrunner_hifi4/
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/tflm_modelrunner_hifi4/hifi4/main.cpp
            examples/eiq_examples/tflm_modelrunner_hifi4/hifi4/modelrunner.h
            examples/eiq_examples/tflm_modelrunner_hifi4/hifi4/modelrunner.cpp
            examples/eiq_examples/tflm_modelrunner_hifi4/hifi4/tf_benchmark.h
            examples/eiq_examples/tflm_modelrunner_hifi4/hifi4/tf_benchmark.cpp
            examples/eiq_examples/tflm_modelrunner_hifi4/hifi4/json_results.cpp
            ${board_root}/${board}/eiq_examples/tflm_modelrunner_hifi4/hifi4/flash_opts.h
            ${board_root}/${board}/eiq_examples/tflm_modelrunner_hifi4/hifi4/flash_opts.c
            examples/eiq_examples/common/timer_xtensa.c
            examples/eiq_examples/common/timer_xtensa.h
            examples/eiq_examples/common/timer.h

)
mcux_add_xtensa_configuration(
   CC "-mlongcalls"
   CX "-mlongcalls"
 )

include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_modelrunner_hifi4/hifi4/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/eiq_examples/${core_id}.cmake)
