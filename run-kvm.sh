#!/bin/bash

ARGS="-hda Image/x64BareBonesImage.qcow2 -m 1024 -display gtk,zoom-to-fit=on -rtc base=localtime -cpu Nehalem -enable-kvm"

if [ "$1" = "gdb" ]
then
    qemu-system-x86_64 -s -S $ARGS
else
    qemu-system-x86_64 $ARGS 
fi
