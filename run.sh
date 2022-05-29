#!/bin/bash

ARGS="-hda Image/x64BareBonesImage.qcow2 -m 1024 -display gtk,zoom-to-fit=on -cpu Nehalem"
#ARGS="-hda Image/x64BareBonesImage.qcow2 -m 512 -display gtk,zoom-to-fit=on -cpu Nehalem -icount shift=7,rr=record,rrfile=replay.bin"
#ARGS="-hda Image/x64BareBonesImage.qcow2 -m 512 -display gtk,zoom-to-fit=on -cpu Nehalem -icount shift=7,rr=replay,rrfile=replay.bin"


while [[ $# -gt 0 ]]; do
	case $1 in
		--debug|--gdb|gdb)
			ARGS="-s -S $ARGS" 
			shift
			;;
		--record|record)
			ARGS="$ARGS -icount shift=7,rr=record,rrfile=replay.bin" 
			shift
			;;
		--replay|replay)
			ARGS="$ARGS -icount shift=7,rr=replay,rrfile=replay.bin" 
			shift
			;;
		--rtc|rtc)
			ARGS="$ARGS -rtc base=localtime"
			shift
			;;
	esac
done

qemu-system-x86_64 $ARGS
