#!/bin/bash

export patch_file=$1
export kernel=$2
export patches=$3

cd $kernel
export KERNELVERSION=$(make kernelversion)$(cat localversion-rt)

cd /boot

# Sets default to be the old unmodified kernel
sudo grubby --set-default=0
export ret=$?
if [ $ret -ne 0 ]
then
	echo "Not able to set old default kernel, patcher_rm_ssh.sh exits with code 1"
	exit 1
fi

# Removes the previously patched kernel from grub menu
sudo grubby --remove-kernel=1

# remove latest patch
cd $kernel

sudo patch -R -p1 < $patches/$patch_file
if [ $? -ne 0 ]
then
	echo "Failed to revert the last patch, patcher_rm_ssh.sh exits with code 1"
	exit 1
fi

# remove the last patched kernel, for space management
sudo rm -fr /boot/vmlinuz*$KERNELVERSION* /boot/initramfs*$KERNELVERSION* /boot/System.map*$KERNELVERSION* /lib/modules/*$KERNELVERSION*
if [ $? -ne 0 ]
then
	echo "Failed to remove some kernel related files, patcher_rm_ssh.sh exits with code 1"
	exit 1
fi

exit 0
