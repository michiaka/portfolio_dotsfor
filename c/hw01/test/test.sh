#!/bin/bash

for f in {0000..0005};do 
	./../train.exe < ${f}_in.txt > /tmp/test.out
	if ! diff ${f}_out.txt /tmp/test.out; then 
	echo "Mismatch ${f}"
	exit 1
	fi 
	done

echo "ALL OK"
