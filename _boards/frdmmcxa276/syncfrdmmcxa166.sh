#!/bin/sh

COPYRIGHT_FILE="./.copyright.txt"
YEAR=`date "+%Y"`
echo "/*" > $COPYRIGHT_FILE
echo " * Copyright $YEAR NXP" >> $COPYRIGHT_FILE
echo " *" >> $COPYRIGHT_FILE
echo " * SPDX-License-Identifier: BSD-3-Clause" >> $COPYRIGHT_FILE
echo " */" >> $COPYRIGHT_FILE

if [ $OSTYPE == "cygwin" ]; then
    CFG_tool="/cygdrive/c/nxp/MCUX_CFG_v16/bin/tools.exe"
else
    CFG_tool="/c/nxp/MCUX_CFG_v16/bin/tools.exe"
fi

examples=`find ../frdmmcxa166 -iname "app.h"`

for example in $examples
do
    src_dir=`dirname $example`
    dest_dir=`echo $src_dir | sed "s/\.\.\/frdmmcxa166/\./g"`

    if [ ! -d $dest_dir ]; then
        mkdir -p $dest_dir
    fi

    echo " updating $dest_dir"

    # app.h
    cp $src_dir/app.h $dest_dir/app.h
    # hardware_init.c
    cp $src_dir/hardware_init.c $dest_dir/hardware_init.c
    # board.readme
    cp $src_dir/board.readme $dest_dir/board.readme

    # pin_mux
    diff $src_dir/pin_mux.c "../frdmmcxa166/pin_mux/pin_mux.c" > /dev/null

    if [ $? != 0 ];then
        cp $src_dir/pin_mux.h $dest_dir/pin_mux.h
        cp $src_dir/pin_mux.c $dest_dir/pin_mux.c
        sed -i "s/A166/A276/g" $dest_dir/pin_mux.c
        $CFG_tool -HeadlessTool Pins -ImportC $dest_dir/pin_mux.c -ExportSrc $dest_dir -CustomCopyright $COPYRIGHT_FILE

        cp $dest_dir/board/pin_mux.h $dest_dir/pin_mux.h
        cp $dest_dir/board/pin_mux.c $dest_dir/pin_mux.c

        rm -rf $dest_dir/board

    else 
        cp ./pin_mux/pin_mux.h $dest_dir/pin_mux.h
        cp ./pin_mux/pin_mux.c $dest_dir/pin_mux.c
    fi

done

rm -rf $COPYRIGHT_FILE

files_with_a166=`grep -lr "A166" * --include "*.readme"`
sed -i "s/A166/A276/g" $files_with_a166

