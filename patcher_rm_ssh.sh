#!/bin/bash

export patch_file=$1
cd /home/Camera/PatchTesting/kernel-automotive-9
export KERNELVERSION=$(make kernelversion)$(cat localversion-rt)

cd /boot

# Sets default to be the old unmodified kernel
sudo grubby --set-default=0

# Removes the previously patched kernel from grub menu
sudo grubby --remove-kernel=1

# remove latest patch
cd /home/Camera/PatchTesting/kernel-automotive-9

# Reverses the patch which was applied last
sudo patch -R -p1 < ../linux/$patch_file

# remove the last patched kernel, for space management
sudo rm -fr /boot/vmlinuz*$KERNELVERSION* /boot/initramfs*$KERNELVERSION* /boot/System.map*$KERNELVERSION* /lib/modules/*$KERNELVERSION*
