#
# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
START_WD=$( pwd )

# prerequisite: jlink command is available on the path.
if ! command -v jlink &> /dev/null; then
    echo "ERROR: jlink executable could not be found"
    exit 1
fi

function print_usage() {
    echo "Usage:"
    echo "    flash_ota_file \$IMG_NR \$PATH"
    echo "        IMG_NR: The dual image boot image number to which to write the "
    echo "                file to. Can be 0 or 1."
    echo "        PATH: The path to the file which is written to flash. The path "
    echo "              can be relative to the directory of this script."
}

image_index=$1
image_path=$2

if [[ $image_index == 0 ]] then
    let "image_start =  0x08040000"
elif [[ $image_index == 1 ]] then
    let "image_start =  0x08440000"
else
    echo "Invalid image index."
    print_usage
    exit 1
fi

if [[ ! -f "$image_path" ]] then
    echo "File not found: [$image_path]."
    print_usage
    exit 1
fi

unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     machine=Linux;;
    Darwin*)    machine=Mac;;
    CYGWIN*)    machine=Cygwin;;
    MINGW*)     machine=MinGw;;
    MSYS_NT*)   machine=Git;;
    *)          machine="UNKNOWN:${unameOut}"
esac

jlink_command_path=commands.jlink

cd ${SCRIPT_DIR}

if [[ "$machine" == "MinGw" ]] then
    image_path_win=$( cygpath -w "${image_path}" )
    image_path="$image_path_win"
fi

echo                                                                                               > ${jlink_command_path}
echo 'halt'                                                                                        >> ${jlink_command_path}
echo 'loadfile "'"${image_path}"'" '$( printf "%08x" ${image_start} ) noreset                      >> ${jlink_command_path}
echo 'w4 0xe000ed0c 0x05fa0004'                                                                    >> ${jlink_command_path}
echo 'sleep 500'                                                                                   >> ${jlink_command_path}
echo 'exit'                                                                                        >> ${jlink_command_path}

jlink -nogui 1 -device RW610 -if SWD -jtagconf -1,-1 -speed 4000 -autoconnect 1 -CommandFile ${jlink_command_path}

