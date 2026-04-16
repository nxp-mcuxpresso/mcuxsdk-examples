
mcux_add_configuration(
	CC "-DSDK_I2C_BASED_COMPONENT_USED=1 -DBOARD_USE_ADP5585=1 -DBOARD_USE_PCAL6408=1"
)

# Camera firmware embedding control.
# Keep the switch in CMake (board reconfig) instead of hardcoding it in headers.
mcux_add_configuration(
        CC "-DCAMERA_NEED_LOAD_FM=1 -DAPP_CAMERA_FW_EMBEDDED=1"
)
set(APP_CAMERA_FW_EMBEDDED 1 CACHE BOOL "AP1302 firmware embedded" FORCE)

# -----------------------------------------------------------------------------
# AP1302 firmware embedding (board-level)
# -----------------------------------------------------------------------------
# When the board enables APP_CAMERA_FW_EMBEDDED, embed AP1302 firmware into the
# image and provide the linker symbols:
#   __ap1302_fw_start / __ap1302_fw_end
#
# Override firmware binary from command line:
#   west build ... -DAP1302_FW_BIN=/path/to/custom.bin
#
if(APP_CAMERA_FW_EMBEDDED)
  set(_AP1302_FW_BIN_DEFAULT "${SdkRootDirPath}/components/video/camera/device/ap1302/ap1302_60fps_ar0144_27M_2Lane_awb_tuning.bin")
  set(AP1302_FW_BIN "${_AP1302_FW_BIN_DEFAULT}" CACHE FILEPATH "AP1302 firmware binary to embed")

  message(STATUS "AP1302 firmware embedding enabled")
  message(STATUS "AP1302_FW_BIN=${AP1302_FW_BIN}")

  if(CONFIG_TOOLCHAIN STREQUAL "armgcc")
    # Generate a linkable object file from the binary (bin -> .o).
    set(_ap1302_fw_bin_copy "${CMAKE_CURRENT_BINARY_DIR}/ap1302_fw.bin")
    set(_ap1302_fw_obj      "${CMAKE_CURRENT_BINARY_DIR}/ap1302_fw.o")

    mcux_add_custom_command(
      TOOLCHAINS armgcc
      BUILD_EVENT PRE_COMPILE
      BYPRODUCTS ${_ap1302_fw_obj}
      BUILD_COMMAND ${CMAKE_COMMAND}
        -DINPUT_BIN=${AP1302_FW_BIN}
        -DBIN_COPY=${_ap1302_fw_bin_copy}
        -DOUTPUT_OBJ=${_ap1302_fw_obj}
        -DOBJCOPY=${CMAKE_OBJCOPY}
        -P ${SdkRootDirPath}/${board_root}/frdmimx95/driver_examples/isi/dwc_mipi_csi2/ap1302_embed_objcopy.cmake
    )

    mcux_add_source(BASE_PATH ${CMAKE_CURRENT_BINARY_DIR} SOURCES ap1302_fw.o)

  elseif(CONFIG_TOOLCHAIN STREQUAL "iar")
    # Generate an IAR assembly file (DC8...) that embeds the firmware into RW data.
    set(_ap1302_fw_iar_s "${CMAKE_CURRENT_BINARY_DIR}/ap1302_fw.s")

    mcux_add_custom_command(
      TOOLCHAINS iar
      BUILD_EVENT PRE_COMPILE
      BYPRODUCTS ${_ap1302_fw_iar_s}
      BUILD_COMMAND ${CMAKE_COMMAND}
        -DINPUT_BIN=${AP1302_FW_BIN}
        -DOUTPUT_S=${_ap1302_fw_iar_s}
        -P ${SdkRootDirPath}/${board_root}/frdmimx95/driver_examples/isi/dwc_mipi_csi2/ap1302_bin2iar_s.cmake
    )

    mcux_add_source(BASE_PATH ${CMAKE_CURRENT_BINARY_DIR} SOURCES ap1302_fw.s)

  endif()
endif()

mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/i.MX/i.MX952/MIMX9529/gcc/MIMX9529_cm7_ram.ld
)

mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/imx952evk/driver_examples/isi/dwc_mipi_csi2/linker/MIMX95294_cm7_isi_ram.ld
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/i.MX/i.MX952/MIMX9529/iar/MIMX9529_cm7_ram.icf
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/imx952evk/driver_examples/isi/dwc_mipi_csi2/linker/MIMX95294_cm7_isi_ram.icf
)

# include ISI example file
include(${SdkRootDirPath}/${board_root}/imx952evk/driver_examples/isi/reconfig.cmake)
