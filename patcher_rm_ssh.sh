#!/bin/bash

export patch_file=$1
export kernel=$2
export patches=$3
export backup_index=$4

cd $kernel
export KERNELRELEASE=$(make kernelrelease)

cd /boot

rm_index=$(sudo grubby --default-index)

# Sets default to be the user-set backup kernel
sudo grubby --set-default=$backup_index
if [ $? -ne 0 ]
then
	echo "Not able to set backup kernel as default, patcher_rm_ssh.sh exits with code 1"
	echo "Default backup kernel index was: $backup_index"
	exit 1
fi

# Removes the previously patched kernel from grub menu
sudo grubby --remove-kernel=$rm_index
if [ $? -ne 0 ]
then
	echo "Removing recently compiled kernel failed, patcher_rm_ssh.sh exits with code 1"
	echo "Recently compiled kernel index was: $rm_index"
	exit 1
fi

# remove latest patch
cd $kernel

sudo patch -R -p1 < $patches/$patch_file
if [ $? -ne 0 ]
then
	echo "Failed to revert the last patch, patcher_rm_ssh.sh exits with code 1"
	echo "The patch that failed to be reverted was:"
	echo $patch_file
	exit 1
fi

# remove the last patched kernel, for space management
if [ -e /boot/vmlinuz*$KERNELRELEASE* ]
then
	sudo rm -fr /boot/vmlinuz*$KERNELRELEASE*
	if [ $? -ne 0 ]
	then
		echo "Failed to remove /boot/vmlinuz*$KERNELRELEASE*, patcher_rm_ssh.sh exits with code 1"
		exit 1
	fi
fi

if [ -e /boot/initramfs*$KERNELRELEASE* ]
then
	sudo rm -fr /boot/initramfs*$KERNELRELEASE*
	if [ $? -ne 0 ]
	then
		echo "Failed to remove /boot/initramfs*$KERNELRELEASE*, patcher_rm_ssh.sh exits with code 1"
		exit 1
	fi
fi

if [ -e /boot/System.map*$KERNELRELEASE* ]
then
	sudo rm -fr /boot/System.map*$KERNELRELEASE*
	if [ $? -ne 0 ]
	then
		echo "Failed to remove /boot/System.map*$KERNELRELEASE*, patcher_rm_ssh.sh exits with code 1"
		exit 1
	fi
fi

if [ -e /lib/modules/*$KERNELRELEASE* ]
then
	sudo rm -fr /lib/modules/*$KERNELRELEASE*
	if [ $? -ne 0 ]
	then
		echo "Failed to remove /lib/modules/*$KERNELRELEASE*, patcher_rm_ssh.sh exits with code 1"
		exit 1
	fi
fi

exit 0
