#
# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

set -e

# prerequisite: spsdk commands are available on the path.
if ! command -v nxpimage &> /dev/null; then
    echo "ERROR: nxpimage could not be found"
    echo "It is part of the spsdk tool suite"
    echo "https://github.com/nxp-mcuxpresso/spsdk"
    echo "Please ensure it is installed an available on the \$PATH."
    echo "If it was installed with pip, please check that the "
    echo "correct venv is activated."
    exit 1
fi

# designed and tested with spsdk version 2.0.1.

cd $( dirname -- "${BASH_SOURCE[0]}" )/spt_workspace

if [[ ! -f input/sdk20-app.bin ]]; then
    echo "ERROR: A compiled binary is expected at {$( pwd )/input/sdk20-app.bin"
    exit 1
fi

echo "Striping the boot header from the binary..."
nxpimage utils binary-image extract -b "input/sdk20-app.bin" -a 0x1000 -s 0 -o "output/sdk20-app.code.bin"
echo

# sign the code binary
echo "Signing the binary..."
nxpimage mbi export -c configs/mbi_config.yaml
echo

echo "Creating SB3 files for updating both images..."
nxpimage sb31 export -c configs/sb3_config_update_image0.yaml
nxpimage sb31 export -c configs/sb3_config_update_image1.yaml
echo

echo "Creating SB3 files for updating both images, including setup of Flash encryption..."
nxpimage sb31 export -c configs/sb3_config_update_image0_iped.yaml
nxpimage sb31 export -c configs/sb3_config_update_image1_iped.yaml
echo

cd -
