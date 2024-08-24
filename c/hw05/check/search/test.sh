#!/bin/bash

for f in ./data/{00..02}_in.txt; do
    output=$(./searchcom.exe < "$f")
    
    echo "$f"
    echo "$output"

    echo "-------------------------------------------------"
done