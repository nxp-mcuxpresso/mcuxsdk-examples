# SPDX-License-Identifier: BSD-3-Clause
#
# Helper CMake script used by mcux_add_custom_command() to embed AP1302 firmware
# using arm-none-eabi-objcopy (bin -> .o).
#
# Required -D variables:
#   INPUT_BIN   : path to firmware .bin
#   BIN_COPY    : path to a stable-name copy (e.g. <build>/ap1302_fw.bin)
#   OUTPUT_OBJ  : path to output object (e.g. <build>/ap1302_fw.o)
#   OBJCOPY     : objcopy executable (absolute path recommended)

if(NOT DEFINED INPUT_BIN OR NOT DEFINED BIN_COPY OR NOT DEFINED OUTPUT_OBJ OR NOT DEFINED OBJCOPY)
  message(FATAL_ERROR "ap1302_embed_objcopy.cmake missing INPUT_BIN/BIN_COPY/OUTPUT_OBJ/OBJCOPY")
endif()

if(NOT EXISTS "${INPUT_BIN}")
  message(FATAL_ERROR "AP1302 firmware bin not found: ${INPUT_BIN}")
endif()

# Copy to a stable filename.
file(COPY_FILE "${INPUT_BIN}" "${BIN_COPY}" ONLY_IF_DIFFERENT)

get_filename_component(_bin_dir  "${BIN_COPY}" DIRECTORY)
get_filename_component(_bin_name "${BIN_COPY}" NAME)

# NOTE: For objcopy -I binary, the auto-generated symbol names depend on the
# input file name as passed on the command line. If we pass an absolute path,
# slashes become part of the symbol name (undesirable). Therefore, we run objcopy
# with WORKING_DIRECTORY set to the directory containing BIN_COPY and pass only
# the base filename (ap1302_fw.bin).

# 1) bin -> .o
execute_process(
  COMMAND "${OBJCOPY}" -I binary -O elf32-littlearm -B arm "${_bin_name}" "${OUTPUT_OBJ}"
  WORKING_DIRECTORY "${_bin_dir}"
  RESULT_VARIABLE _rc
)
if(NOT _rc EQUAL 0)
  message(FATAL_ERROR "objcopy failed (${_rc}) while converting AP1302 FW")
endif()

# 2) Rename symbols to what the driver/example expects.
# With the stable input filename ap1302_fw.bin, the default symbols are:
#   _binary_ap1302_fw_bin_start/_end/_size
execute_process(
  COMMAND "${OBJCOPY}"
          --redefine-sym _binary_ap1302_fw_bin_start=__ap1302_fw_start
          --redefine-sym _binary_ap1302_fw_bin_end=__ap1302_fw_end
          --redefine-sym _binary_ap1302_fw_bin_size=__ap1302_fw_size
          "${OUTPUT_OBJ}"
  WORKING_DIRECTORY "${_bin_dir}"
  RESULT_VARIABLE _rc2
)
if(NOT _rc2 EQUAL 0)
  message(FATAL_ERROR "objcopy redefine-sym failed (${_rc2})")
endif()
