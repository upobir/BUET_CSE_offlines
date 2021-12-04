#!/bin/bash

## function to create directory using argumen, if already exists delete it first
make_directory() {
    if [ -d "$1" ]; then
        rm -r "$1"
    elif [ -f "$1" ]; then
        rm "$1"
    fi

    mkdir "$1"
}

## function to get extension from filename, make sure it is only filename not a path
get_extension() {
    local x="$1"

    extension=${x##*.} 
    if [ "$x" == "$extension" ];
    then
        extension=other
    fi
}

## working directory and ignore file name
working_dir=.
ignore_file=

## if count is 1, only ignore file given
if (( $# == 1 )); then
    ignore_file="$1"

## if 2 arguments given then both argument given
elif (( $# == 2 )); then
    working_dir="$1"
    ignore_file="$2"
    
## otherwise count is wrong    
else
    echo 'usage: script.sh <working-dir> <ignore-file>'
    echo 
    echo 'if no <working-dir> provided, home is used'
    exit 1

fi 

## if working directory is invalid exit
if [ ! -d "$working_dir" ]; then
    echo 'invalid working directory'
    exit 1
fi

## if ignroefile is not a file, then keep reading until given proper filename
while [ ! -f "$ignore_file" ]; do
    echo -n 'invalid file, enter valid file: '
    read ignore_file
done

## read all ignored extension to a array
ignored=()
while ISF= read -r line ||[ -n "$line" ]; do
    line=${line//$'\n'/}
    line=${line//$'\r'/}
    ignored+=("$line")
done < "$ignore_file"

## create array of non-ignored files in working directory 
files=()
ignore_count=0
shopt -s globstar
for x in "$working_dir"/**/*; do
    if [[ -f "$x" ]] ; then
        filename="${x##*/}"
        get_extension "$filename"
        flag=0
        for y in "${ignored[@]}"
        do
            if [[ "$extension" == "$y" ]]; then
                flag=1
            fi
        done
        if [[ "$flag" == 0 ]]; then
            files+=("$x")
        else
            ignore_count=$((ignore_count+1))
        fi
        
    fi
done

## create output directory
output_dir="$working_dir"/../output_dir
make_directory "$output_dir"

## copy all files to output directory
for x in "${files[@]}"
do 
    filename="${x##*/}"
    get_extension "$filename"
    mkdir "$output_dir"/$extension 2> /dev/null 
    if ! [[ -e "$output_dir"/$extension/"$filename" ]] ; then
        cp "$x" "$output_dir"/$extension/"$filename"
        echo "$x" >> "$output_dir"/$extension/desc_"$extension".txt
    fi
done

## create and fill up csv file
echo file_type,no_of_files > output.csv 

for x in $( ls "$output_dir" );
do
    count=$(ls -1 "$output_dir"/"$x" | wc -l)
    count=$((count-1))
    echo $x,$count >> output.csv
done

echo ignored,$ignore_count >> output.csv
