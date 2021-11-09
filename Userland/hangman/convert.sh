#!/bin/sh

iconv -f utf8 -t ascii//TRANSLIT text.txt | tr a-z A-Z | tr -c 'A-z' '\n' | sort -u | awk '{ if (length($0) > 4) print }' | xclip -selection clipboard