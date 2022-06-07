#!/bin/sh

helpFiles=$(find ../* -name help.txt -not -path "**/.*/help.txt")

exec 3>"compiled.txt"

for helpFile in $helpFiles
do
    programName=$(sed -r 's|.*/(.*)/.*|\1|'<<<$helpFile)

    echo "$programName: $(cat $helpFile)" >&3
done

exec 3>&-

exec 3>"help.c"
echo '#include "help.h"' >&3
xxd -i compiled.txt >&3
exec 3>&-
