#!/bin/bash

if [ $1 == "create" ]
then
	cd /home/Camera/
	sudo touch created.txt
fi

cd

ls /home/Camera/created.txt
export ret=$?
if [[ ret -eq 0 ]]
then
	echo "file was created"
	sudo rm /home/Camera/created.txt
fi
