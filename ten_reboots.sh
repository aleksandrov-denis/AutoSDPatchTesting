#!/bin/bash


i=1
while [ $i -le 10 ]
do
	./sut_boottest.py NOPATCH $i location_tester/
	i=$(( $i + 1 ))
done
