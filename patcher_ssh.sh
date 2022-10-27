#!/bin/bash

# This script patches the kernel-automotive-9 kernel source tree, builds/installs it, and sets it as the default boot kernel on the remote host

export patch_file=$1
export kernel=$2
export config=$3
export patches=$4

cd $kernel
export KERNELVERSION=$(make kernelversion)$(cat localversion-rt)

# set up .config --- make sure that the config file exists in /boot
sudo cp $config .config
if [ $? -ne 0 ]
then
	echo "Not able to copy config file from /boot/config-*, patcher_ssh.sh exits with code 1"
	exit 1
fi

sudo patch -p1 < $patches/$patch_file
if [ $? -ne 0 ]
then
	echo "Not able to patch kernel, patcher_ssh.sh exits with code 1"
	exit 1
fi

# build and install patched kernel
sudo make olddefconfig && sudo make -j$(nproc) && sudo make modules_install -j$(nproc) && sudo make install

if [ $? -ne 0 ]
then
	echo "Something went wrong during the make process, patcher_ssh.sh exits with code 1"
	exit 1
fi

# set default kernel
sudo grubby --set-default=1
# MAKE SURE THIS IS NOT HARDCODED

exit 0
