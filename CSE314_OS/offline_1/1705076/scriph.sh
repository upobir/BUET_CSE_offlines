#!/bin/bash

make_directory() {
    if [ -d "$1" ]; then
        rm -r "$1"
    elif [ -f "$1" ]; then
        rm "$1"
    fi

    mkdir "$1"
}

get_extension() {
    local x="$1"

    extension=${x##*.} 
    if [ "$x" == "$extension" ];
    then
        extension=other
    fi
}

working_dir=~
ignore_file=

if (( $# == 1 )); then
    ignore_file="$1"

elif (( $# == 2 )); then
    working_dir="$1"
    ignore_file="$2"
    
else
    echo 'usage: script.sh <working-dir> <ignore-file>'
    echo 
    echo 'if no <working-dir> provided, home is used'
    exit 1

fi 

if [ ! -d "$working_dir" ]; then
    echo 'invalid working directory'
    exit 1
fi

while [ ! -f "$ignore_file" ]; do
    echo -n 'invalid file, enter valid file: '
    read ignore_file
done

files=()
for x in $( find "$working_dir" -type f );
do
    files+=("$x")
done

output_dir="$working_dir"/../output_dir
make_directory "$output_dir"

for x in "${files[@]}"
do 
    get_extension $x
    mkdir "$output_dir"/$extension 2> /dev/null 
    filename=${x##*/}
    cp $x "$output_dir"/$extension/"$filename"
    echo $x >> "$output_dir"/$extension/desc_"$extension".txt
done


ignore_count=0
while read line || [ -n "$line" ]; do
    line=${line//$'\n'/}
    line=${line//$'\r'/}
    if [ -d "$output_dir"/"$line" ]; then
        count=$(ls -1 "$output_dir"/"$line" | wc -l)
        count=$((count-1))
        ignore_count=$((ignore_count+count))
    fi
    rm -r "$output_dir"/$line 2> /dev/null
done < "$ignore_file"

echo file_type,no_of_files > output.csv 

for x in $( ls "$output_dir" );
do
    count=$(ls -1 "$output_dir"/"$x" | wc -l)
    count=$((count-1))
    echo $x,$count >> output.csv
done

echo ignored,$ignore_count >> output.csv
