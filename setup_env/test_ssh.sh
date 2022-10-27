#!/bin/bash

export kernel=$1
export patches=$2
export config=$3

if [ ! -e $kernel ]
then
	exit 1
fi

if [ ! -e $patches ]
then
	exit 2
fi

if [ ! -e $config ]
then
	exit 3
fi

exit 0
