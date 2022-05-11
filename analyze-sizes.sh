#!/bin/bash

get_files() {
	echo Kernel/kernel.bin
	find Userland -name "*.bin"
}

get_sizes() {
	get_files | xargs -n 1 du -b | sort -nr
}

add_sizes() {
	op=`awk '{print$1}' | tr '\n' '+'`0
	bc<<<$op
}

sizes=$(get_sizes)

total_size=$(add_sizes<<<$sizes)

echo "Total size: $(awk '{print $1/1024}'<<<$total_size)KiB"
echo

awk '{printf "%03.2f%%\t- %.2fKib \t- %s\n", $1*100/'"$total_size"', $1/1024, $2}' <<< $sizes
