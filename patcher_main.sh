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
ssh $user@$host "bash -s - $1" < patcher_ssh.sh

i=1

while [ $i -le 10 ]
do
	# reboot and track kernel boot time (make sure ip is configured properly)
	# output the files to buffer_dir to then take avg
	./sut_boottest.py $1 $i location_tester/
	i=$(( $i + 1 ))
done


# read all files in buffer_dir, take their averages, write to a single .json in ..
./j2c.o average.csv variance.csv $2

# move record files from buffer_dir to storage dir
mv location_tester/* storage_results_dir/


# remove bloat
ssh $user@$host "bash -s - $1" < patcher_rm_ssh.sh
