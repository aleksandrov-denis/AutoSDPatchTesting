#!/bin/bash

# This script passes a patching and kernel build script to a ssh session
# Reboots the remote host and records the time for the seperate boot processes
# Passes a script to a ssh session to remove the previously patched kernel and all files involved in booting it

user="guest"
host="209.6.24.23"
#host="192.168.0.139"

# SOURCE FOR RUNNING MANY COMMANDS IN SSH
# https://www.howtogeek.com/devops/how-to-run-a-local-shell-script-on-a-remote-ssh-server/

# apply patch
ssh $user@$host "bash -s - $patch" < patcher_ssh.sh
 
trial=1
# get ten trials for kenrel boot time per patch
while [ $trial -le 10 ]
do
	# reboot and track kernel boot time (make sure ip is configured properly)
	# output the files to buffer_dir to then take avg
	./sut_boottest.py $patch $trial temp_location/
	trial=$(( $trial + 1 ))
done


# read all files in buffer_dir, take their averages and variances, write to two .csv files
./j2c.o average.csv variance.csv $patch_num

# remove bloat
ssh $user@$host "bash -s - $patch" < patcher_rm_ssh.sh
