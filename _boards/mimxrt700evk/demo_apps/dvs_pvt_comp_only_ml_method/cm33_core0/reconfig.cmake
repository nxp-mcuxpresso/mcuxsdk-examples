mcux_add_configuration(
    CC "-DPRINTF_FLOAT_ENABLE=1"
)

mcux_add_macro(
    AS "-DDSP_IMAGE_COPY_TO_RAM=1"
    CC "-DDSP_IMAGE_COPY_TO_RAM=1"
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

mcux_add_iar_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "--config_def=__stack_size__=0x1000\
        --config_def=__heap_size__=0x1800"
)
mcux_add_mdk_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "--predefine=\"-D__stack_size__=0x1000\"\
        --predefine=\"-D__heap_size__=0x1800\"\
        --keep=*(*core1_code)"
)
mcux_add_armgcc_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "-Xlinker --defsym=__stack_size__=0x1000\
        -Xlinker --defsym=__heap_size__=0x1800"
)

mcux_remove_mdk_configuration(
  CC "-fshort-wchar"
  CX "-fshort-wchar"
  )

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}/clock_config.c
            ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}/clock_config.h
            ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}/dvs_pvt_comp_only_core0.c
            ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/power_demo_config.h
            ${board_root}/${board}/lpm.h
            ${board_root}/${board}/lpm.c
            ${board_root}/${board}/demo_apps/dsp_support.c
            ${board_root}/${board}/demo_apps/dsp_support.h
            ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/dsp_config.h
            ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/hifi4/binary/*.bin
            ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}/model_mobilenet_ops_npu.cpp
            ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}/pvts_model_float_v2.h
            ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}/model.h
            ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}/model.cpp
            ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}/pvts_ml.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    EXCLUDE TRUE
    SOURCES ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/main_dsp.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}
             ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/
             ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/hifi4/binary/
             ${board_root}/${board}/demo_apps/
)

mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../dvs_pvt_comp_only_ml_method_secondary/iar/core1_image.bin,_core1_image,__core1_image,4\
        --keep=_core1_image\
        --image_input=${SdkRootDirPath}/${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/hifi4/binary/dsp_literal_release.bin,__dsp_literal_bin,__dsp_literal_section,4\
        --keep=__dsp_literal_bin\
        --image_input=${SdkRootDirPath}/${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/hifi4/binary/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
        --keep=__dsp_text_bin\
        --image_input=${SdkRootDirPath}/${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/hifi4/binary/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
        --keep=__dsp_data_bin"
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../dvs_pvt_comp_only_ml_method_secondary/${CONFIG_TOOLCHAIN}/
)

mcux_remove_iar_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.icf
  )
mcux_remove_iar_linker_script(
  TARGETS flash_debug flash_release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
  )
mcux_remove_mdk_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.scf
  )
mcux_remove_mdk_linker_script(
  TARGETS flash_debug flash_release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
  )
mcux_remove_armgcc_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
  )
mcux_remove_armgcc_linker_script(
  TARGETS flash_debug flash_release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
  )

mcux_add_armgcc_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}/gcc/MIMXRT798Sxxxx_cm33_core0_ram.ld
  )
mcux_add_armgcc_linker_script(
  TARGETS flash_debug flash_release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld
  )
mcux_add_mdk_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}/arm/MIMXRT798Sxxxx_cm33_core0_ram.scf
  )
mcux_add_mdk_linker_script(
  TARGETS flash_debug flash_release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}/arm/MIMXRT798Sxxxx_cm33_core0_flash.scf
  )
mcux_add_iar_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}/iar/MIMXRT798Sxxxx_cm33_core0_ram.icf
  )
mcux_add_iar_linker_script(
  TARGETS flash_debug flash_release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${board_root}/${board}/demo_apps/dvs_pvt_comp_only_ml_method/${core_id}/iar/MIMXRT798Sxxxx_cm33_core0_flash.icf
  )