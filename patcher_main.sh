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
	./sut_boottest.py $1 $i
	i=$(( $i + 1 ))
done

# remove bloat
ssh $user@$host "bash -s - $1" < patcher_rm_ssh.sh
