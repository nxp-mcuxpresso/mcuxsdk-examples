# SPDX-License-Identifier: BSD-3-Clause
#
# Convert AP1302 firmware .bin into an IAR assembler .s file using DC8.
# This avoids reliance on INCBIN (not supported by some iasmarm versions).
#
# Required -D variables:
#   INPUT_BIN : input firmware .bin
#   OUTPUT_S  : output .s file

if(NOT DEFINED INPUT_BIN OR NOT DEFINED OUTPUT_S)
  message(FATAL_ERROR "ap1302_bin2iar_s.cmake missing INPUT_BIN/OUTPUT_S")
endif()

if(NOT EXISTS "${INPUT_BIN}")
  message(FATAL_ERROR "AP1302 firmware bin not found: ${INPUT_BIN}")
endif()

file(READ "${INPUT_BIN}" _hex HEX)

# _hex is a continuous hex string (2 chars per byte).
string(LENGTH "${_hex}" _hex_len)
math(EXPR _byte_count "${_hex_len} / 2")

set(_out "; Auto-generated from AP1302 firmware binary. Do not edit.\n")
string(APPEND _out "; SPDX-License-Identifier: BSD-3-Clause\n\n")
string(APPEND _out "        SECTION `.data`:DATA:REORDER:NOROOT(2)\n")
string(APPEND _out "        PUBLIC  __ap1302_fw_start\n")
string(APPEND _out "        PUBLIC  __ap1302_fw_end\n\n")
string(APPEND _out "__ap1302_fw_start:\n")

set(_cols 16)
set(_i 0)
while(_i LESS _byte_count)
  set(_line "        DC8 ")
  set(_j 0)
  while(_j LESS _cols AND _i LESS _byte_count)
    math(EXPR _pos "${_i} * 2")
    string(SUBSTRING "${_hex}" ${_pos} 2 _b)

    # IAR hex literal: must start with digit, so prefix with 0.
    set(_lit "0${_b}H")

    if(_j GREATER 0)
      string(APPEND _line ", ")
    endif()
    string(APPEND _line "${_lit}")

    math(EXPR _i "${_i} + 1")
    math(EXPR _j "${_j} + 1")
  endwhile()
  string(APPEND _out "${_line}\n")
endwhile()

string(APPEND _out "\n__ap1302_fw_end:\n")
string(APPEND _out "\n        END\n")

file(WRITE "${OUTPUT_S}" "${_out}")
