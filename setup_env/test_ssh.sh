#!/bin/bash

export kernel=$1
export patches=$2
export config=$3
export index=$4

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

cd /boot
sudo grubby --info=$index

if [ $? -ne 0 ]
then
	exit 4
fi

exit 0
