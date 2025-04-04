#!/bin/sh
#set -euo pipefail

# Uncomment line by line if you want that project to be skipped from build
declare -a SKIP_LIST=(
#           "coordinator_bm"
#           "coordinator_bm/r23"
#           "coordinator_freertos"
#           "coordinator_freertos/r23"
#           "router_bm"
#           "router_bm/r23"
#           "router_freertos"
#           "router_freertos/r23"
#           "ed_rx_off_bm"
#           "ed_rx_off_bm/r23"
#           "ed_rx_off_freertos"
#           "ed_rx_off_freertos/r23"
#           "ed_rx_on_bm"
#           "ed_rx_on_bm/r23"
#           "ed_rx_on_freertos"
#           "ed_rx_on_freertos/r23"
#           "coprocessor_bm"
          )
LOGLEVEL="error" # use debug for verbose
NUM_JOBS=20 # num of cores

EX_DIR=${PWD}
PROJECTS=`find ${EX_DIR}/ -name CMakeLists.txt`
LOG_FILE=build.log

if [ $# != 2 ]
then
    echo "Usage $0 <board> <ARM GCC path>"
    echo ""
    echo "Example: sh $ build_all.sh k32w148evk /c/NXP/arm-gnu-toolchain-13.3.rel1-mingw-w64-i686-arm-none-eabi"
    exit 1
fi

BOARD=$1
export ARMGCC_DIR=$2

# cleanup
rm -rf build_*
rm -rf build.log

if [[ "${BOARD}" == "mcxw71evk"  ||  \
      "${BOARD}" == "mcxw72evk"  ||  \
      "${BOARD}" == "frdmmcxw72" ||  \
      "${BOARD}" == "frdmmcxw71"     \
   ]]
then
	SKIP_LIST+=(
		"coordinator_ble_wu_freertos"
		"router_ble_wu_freertos"
	)
fi

if [[ "${BOARD}" == "mcxw72evk"  || \
      "${BOARD}" == "frdmmcxw72" || \
      "${BOARD}" == "frdmrw612"  || \
      "${BOARD}" == "rdrw612bga"    \
   ]]
then
	# MCXW72/RW612 doesn't support bm or coprocesor prjs
	SKIP_LIST+=(
		"coprocessor_bm"
		"router_bm"
		"coordinator_bm"
		"ed_rx_off_bm"
		"ed_rx_on_bm"
	)
fi

for project in `dirname ${PROJECTS}`
do
        prj_name=`echo ${project} | sed s,${EX_DIR}\/,, | sed s,\/,_,`
        if [[ "${SKIP_LIST[@]}" =~ "${prj_name}" ]]
        then
            echo "Building $prj_name (skip)..."
            SUCCESS_BUILDS+=("${prj_name} SKIPPED")
        else
            echo "Building $prj_name..."
            west build -b ${BOARD} --build-dir=build_${prj_name} ${project} --log-level=${LOGLEVEL} -p always -o=-j${NUM_JOBS} -Dcore_id=cm33_core0 --config debug | tee -a build.log

            if [ ${PIPESTATUS[0]} -eq 0 ]
            then
                SUCCESS_BUILDS+=("${prj_name} OK")
            else
                FAILED_BUILDS+=("${prj_name} KO")
            fi
            echo "done"
        fi
done


echo "BUILD REPORT:"

if [[ ${#SUCCESS_BUILDS[@]} -ne 0 ]]
then
    echo "    SUCCESS BUILDS: "
    for i in `seq 0 ${#SUCCESS_BUILDS[@]}`
    do
        echo "        ${SUCCESS_BUILDS[$i]}"
    done
fi

if [[ ${#FAILED_BUILDS[@]} -ne 0 ]]
then
    echo "    FAILED BUILDS: "
    for i in `seq 0 ${#FAILED_BUILDS[@]}`
    do
        echo "        ${FAILED_BUILDS[$i]}"
    done

    exit 2
fi

