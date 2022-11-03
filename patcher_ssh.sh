#!/bin/bash

# This script patches the kernel-automotive-9 kernel source tree, builds/installs it, and sets it as the default boot kernel on the remote host

export patch_file=$1
export kernel=$2
export config=$3
export patches=$4

cd $kernel

# set up .config --- make sure that the config file exists in /boot
sudo cp $config .config
if [ $? -ne 0 ]
then
	sudo make defconfig
	if [ $? -ne 0]
	then
		echo "Not able to copy config file from $config or 'make defconfig', patcher_ssh.sh exits with code 1"
		exit 1
	fi
else
	sudo make olddefconfig
	if [ $? -ne 0 ]
	then
		echo "Not able to set .config file with olddefconfig, patcher_ssh.sh exits with code 1"
		exit 1
	fi
fi

sudo patch -p1 < $patches/$patch_file
if [ $? -ne 0 ]
then
	echo "Not able to patch kernel, patcher_ssh.sh exits with code 1"
	exit 1
fi

# build and install patched kernel
sudo make -j$(nproc) && sudo make modules_install -j$(nproc) && sudo make install

if [ $? -ne 0 ]
then
	echo "Something went wrong during the make process, patcher_ssh.sh exits with code 1"
	exit 1
fi

# set new kernel as default
sudo grubby --make-default

exit 0
