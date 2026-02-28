mcux_add_iar_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "--config_def=__stack_size__=0x1000\
        --config_def=__heap_size__=0x1000"
)
mcux_add_mdk_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "--predefine=\"-D__stack_size__=0x1000\"\
        --predefine=\"-D__heap_size__=0x1000\"\
        --keep=*(*core1_code)"
)
mcux_add_armgcc_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "-Xlinker --defsym=__stack_size__=0x1000\
        -Xlinker --defsym=__heap_size__=0x1000"
)

mcux_remove_armgcc_configuration(
    CC "-Werror"
    CX "-Werror"
)

mcux_remove_iar_configuration(
    CC "--warnings_are_errors"
    CX "--warnings_are_errors"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/eiq_examples/tflm_power_consumption/${core_id}/clock_config.c
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/${core_id}/clock_config.h
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/${core_id}/tflm_power_consumption_core0.c
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/power_demo_config.h
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/tflm_label_image.cpp
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/demo_config.h
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/demo_info.cpp
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/demo_info.h
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/labels.h
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/timer.c
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/timer.h
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/image/image_data.h
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/image/image.h
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/image/image_load.c
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/image/image_decode_raw.c
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/image/image_utils.h
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/model/model.cpp
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/model/model.h
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/model/model_data.h
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/model/model_data_cpu.h
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/model/get_top_n.cpp
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/model/get_top_n.h
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/model/output_postproc.cpp
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/model/output_postproc.h
            ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/model/model_mobilenet_ops_npu.cpp
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/eiq_examples/tflm_power_consumption/${core_id}
             ${board_root}/${board}/eiq_examples/tflm_power_consumption/
             ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/
             ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/image/
             ${board_root}/${board}/eiq_examples/tflm_power_consumption/tflm_label_image/model/
)

mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../tflm_power_consumption_secondary/iar/core1_image.bin,_core1_image,__core1_image,4\
        --keep=_core1_image"
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../tflm_power_consumption_secondary/${CONFIG_TOOLCHAIN}/
)

# Use demo specific linker
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core0_ram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT700/MIMXRT798S/arm/MIMXRT798Sxxxx_cm33_core0_ram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_ram.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/eiq_examples/tflm_power_consumption/${multicore_foldername}/linker/MIMXRT798Sxxxx_cm33_core0_ram.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/eiq_examples/tflm_power_consumption/${multicore_foldername}/linker/MIMXRT798Sxxxx_cm33_core0_ram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/eiq_examples/tflm_power_consumption/${multicore_foldername}/linker/MIMXRT798Sxxxx_cm33_core0_ram.ld
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core0_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/RT/RT700/MIMXRT798S/arm/MIMXRT798Sxxxx_cm33_core0_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/eiq_examples/tflm_power_consumption/${multicore_foldername}/linker/MIMXRT798Sxxxx_cm33_core0_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/eiq_examples/tflm_power_consumption/${multicore_foldername}/linker/MIMXRT798Sxxxx_cm33_core0_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/eiq_examples/tflm_power_consumption/${multicore_foldername}/linker/MIMXRT798Sxxxx_cm33_core0_flash.ld
)

mcux_add_iar_configuration(
  CC "--diag_suppress=Pe167\
  --diag_suppress=Pe260,Pe1031"
)
mcux_add_macro(
  CC "-DSDK_DEBUGCONSOLE_UART\
  -DARM_MATH_CM33\
  -DTENSORARENA_NONCACHE=1\
  -D__FPU_PRESENT=1"
  CX "-DSDK_DEBUGCONSOLE_UART\
  -DARM_MATH_CM33\
  -DTENSORARENA_NONCACHE=1\
  -D__FPU_PRESENT=1"
)

# Include TFLM middleware headers
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/eiq/tensorflow-lite
                    middleware/eiq/tensorflow-lite/third_party/cmsis_nn/Include
                    middleware/eiq/tensorflow-lite/third_party/cmsis_nn/Include/Internal
                    middleware/eiq/tensorflow-lite/third_party/flatbuffers/include
                    middleware/eiq/tensorflow-lite/third_party/gemmlowp/fixedpoint
                    middleware/eiq/tensorflow-lite/third_party/kissfft
                    middleware/eiq/tensorflow-lite/third_party/kissfft/tools
                    middleware/eiq/tensorflow-lite/third_party/ruy
)
