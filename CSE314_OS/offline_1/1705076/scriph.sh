#!/bin/bash

working_dir=~
ignore_file=

if (( $# == 1 )); then
    ignore_file=$1

elif (( $# == 2 )); then
    working_dir=$1
    ignore_file=$2
    
else
    echo 'usage: script.sh <working-dir> <ignore-file>'
    echo 
    echo 'if no <working-dir> provided, home is used'
    exit 1

fi 

echo $ignore_file
echo $working_dir

if [ ! -d $working_dir ]; then
    echo 'invalid working directory'
    exit 1
fi

if [ ! -f $ignore_file ]; then
    echo 'invalid file'
    exit 1
fi
