#!/bin/bash


sshpass -p $pswd ssh $user@$host "bash -s - $kernel $patches $config $index" < test_ssh.sh

export ret=$?

case $ret in
	255)
		echo "SSH failed, something is wrong with the entered ssh credentials, test exits with code $ret"
		exit $ret
		;;
	1)
		echo "Bad path to kernel source in remote host, test exits with code $ret"
		exit $ret
		;;
	2)
		echo "Bad path to patch source in remote host, test exits with code $ret"
		exit $ret
		;;
	3)
		echo "Bad path to .config file in remote host, test exits with code $ret"
		exit $ret
		;;
	4)
		echo "The backup kernel index: $index is not correct, test exits with code $ret"
		exit $ret
		;;
esac



if [ ! -e $patches_txt ]
then
	echo "Bad path to text file with patches on local machine, test exits with code 1"
	exit 1
fi

if [ ! -e $temp_loc ]
then
	echo "Directory did not exist, made $temp_loc"
	mkdir $temp_loc
fi

if [ ! -e $final_loc ]
then
	echo "Directory did not exist, made $final_loc"
	mkdir $final_loc
fi

if [ ! -e $avg_csv ]
then
	echo "File did not exist, made $avg_csv"
	touch $avg_csv
fi

if [ ! -e $var_csv ]
then
	echo "File did not exist, made $var_csv"
	touch $var_csv
fi

if [ ! -e $avg_png ]
then
	echo "File did not exist, made $avg_png"
	touch $avg_png
fi

if [ ! -e $var_png ]
then
	echo "File did not exist, made $var_png"
	touch $var_png
fi


exit 0
