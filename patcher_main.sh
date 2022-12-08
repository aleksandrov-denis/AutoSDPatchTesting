#!/bin/bash

# This script passes a patching and kernel build script to a ssh session
# Reboots the remote host and records the time for the seperate boot processes
# Passes a script to a ssh session to remove the previously patched kernel and all files involved in booting it

#export patch=$1
# SOURCE FOR RUNNING MANY COMMANDS IN SSH
# https://www.howtogeek.com/devops/how-to-run-a-local-shell-script-on-a-remote-ssh-server/

# apply patch
sshpass -p $pswd ssh $user@$host "bash -s - $patch $kernel $config $patches" < patcher_ssh.sh

if [ $? -ne 0 ]
then
	echo "Applying $patch or kernel compilation failed, patcher_main exits with code 1"
	exit 1
fi

trial=0
# get ten trials for kernel boot time per patch
while [ $trial -le 9 ]
do
	# reboot and track kernel boot time (make sure ip is configured properly)
	# output the files to buffer_dir to then take avg
	./sut_boottest.py $patch $trial

	if [ $? -ne 0 ]
	then
		echo "Failed testing SUT, patcher_main.sh exits with code 1"
		exit 1
	fi

	trial=$(( $trial + 1 ))
done


# read all files in buffer_dir, take their averages and variances, write to two .csv files
./j2c.o $patch_num

if [ $? -ne 0 ]
then
	echo "Failed gathering average and variance data, patcher_main.sh exits with code 1"
	exit 1
fi

# remove bloat
sshpass -p $pswd ssh $user@$host "bash -s - $patch $kernel $patches $index" < patcher_rm_ssh.sh
if [ $? -ne 0 ]
then
	echo "Failed cleaning the host, patcher_main exits with code 1"
	exit 1
fi

exit 0
