#!/bin/bash

export params=$1
export cmd=$2

cd /boot

if [ $cmd -eq 0 ]
then
	sudo grubby --info=/boot/vmlinuz-$(uname -r) | grep "args=" | sed $'s/args=\"//g' | sed $'s/\"//g' > ~/temp
	sudo grubby --update-kernel=/boot/vmlinuz-$(uname -r) --args="$params"

	if [ $? -ne 0 ]
	then
		echo "setting kernel command-line parameters failed, exiting with code 1"
		exit 1
	fi
else
	sudo grubby --update-kernel=/boot/vmlinuz-$(uname -r) --remove-args="$params"

	if [ $? -ne 0 ]
	then
		echo "removing kernel command-line parameters failed, exiting with code 1"
		exit 1
	fi

	sudo grubby --update-kernel=/boot/vmlinuz-$(uname -r) --args="$(cat ~/temp)"
	if [ $? -ne 0 ]
	then
		echo $(cat ~/temp)
		echo "not able to reset kernel arguments, exiting with code 1"
		exit 1
	fi

	sudo rm ~/temp

fi

exit 0
