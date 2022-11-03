#!/bin/bash

export patch_file=$1
export kernel=$2
export patches=$3
export backup_index=$4

cd $kernel
export KERNELRELEASE=$(make kernelrelease))

cd /boot

rm_index=$(sudo grubby --default-index)

# Sets default to be the user-set backup kernel
sudo grubby --set-default=$backup_index
export ret=$?
if [ $ret -ne 0 ]
then
	echo "Not able to set backup kernel as default, patcher_rm_ssh.sh exits with code 1"
	exit 1
fi

# Removes the previously patched kernel from grub menu
sudo grubby --remove-kernel=$rm_index

# remove latest patch
cd $kernel

sudo patch -R -p1 < $patches/$patch_file
if [ $? -ne 0 ]
then
	echo "Failed to revert the last patch, patcher_rm_ssh.sh exits with code 1"
	exit 1
fi

# remove the last patched kernel, for space management
if [ -e /boot/vmlinuz*$KERNELRELEASE* ]
then
	sudo rm -fr /boot/vmlinuz*$KERNELRELEASE*
fi

if [ -e /boot/initramfs*$KERNELRELEASE* ]
then
	sudo rm -fr /boot/initramfs*$KERNELRELEASE*
fi

if [ -e /boot/System.map*$KERNELRELEASE* ]
then
	sudo rm -fr /boot/System.map*$KERNELRELEASE*
fi

if [ -e /lib/modules/*$KERNELRELEASE* ]
then
	sudo rm -fr /lib/modules/*$KERNELRELEASE*
fi

exit 0
