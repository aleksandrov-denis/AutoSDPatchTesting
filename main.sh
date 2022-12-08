#!/bin/bash

# This script runs the patch testing process by passing patcher_main.sh through an expect script to deal with the password prompt

cmd=$1

if [[ $cmd -lt 0 ]] || [[ $cmd -gt 1 ]] || [[ $# -ne 1 ]]
then
	echo "usage: ./main.sh <cmd>"
	echo "cmd == 0 -> configure everything, no kernel command-line parameters"
	echo "cmd == 1 -> only configure kernel command-line parameters"
	exit
fi

# Environement configuration
cd setup_env/
while true
do
	if [ -f "config.txt" ]
	then
		cat config.txt
		while true
		do
			echo "Is this configuration correct? y/n"
			read answer
			if [[ $answer == "y" || $answer == "n" ]]
			then
				break
			fi
		done

		if [ $answer == "n" ]
		then
			. config.sh $cmd
		elif [ $answer == "y" ]
		then
			. set.sh $cmd
		fi
	else
		. config.sh $cmd
	fi

	./test_main.sh $cmd

	if [ $? -eq 0 ]
	then
		break
	fi
done

cd ..

# CONTROL
#i=0
#while [ $i -lt 10 ]
#do
#	./sut_boottest.py "control" $i
#	if [ $? -ne 0 ]
#	then
#		exit 1
#	fi
#	i=$(( $i +1 ))
#done
#./j2c.o 0

#if [ $? -ne 0 ]
#then
#	exit 1
#fi

#mkdir $final_loc/control
#mv $temp_loc/* $final_loc/control

if [ $cmd -eq 1 ]
then
	command_num=1
	while read command; do
		i=0
		ctrl=0
		sshpass -p $pswd ssh $user@$host "bash -s -- \"$command\" $ctrl" < send_cmds.sh
		
		if [ $? -ne 0 ]
		then
			exit 1
		fi

		while [ $i -lt 10 ]
		do
			./sut_boottest.py "command-line" $i

			if [ $? -ne 0 ]
			then
				exit 1
			fi

			i=$(( $i + 1 ))
		done

		./j2c.o $command_num

		if [ $? -ne 0 ]
		then
			exit 1
		fi

		ctrl=1
		sshpass -p $pswd ssh $user@$host "bash -s -- \"$command\" $ctrl" < send_cmds.sh

		if [ $? -ne 0 ]
		then
			exit 1
		fi

		mkdir $final_loc/cmd_$command_num
		mv $temp_loc/* $final_loc/cmd_$command_num
		command_num=$(( $command_num + 1 ))

	done < $kclp
else
	patch_num=1
	while read patch; do

		dirname=$(echo "${patch}" | awk '{print substr ($0, 0, 4)}')
		export patch=$patch
		export patch_num=$dirname

		./patcher_main.sh

		if [ $? -ne 0 ]
		then
			echo "Something went wrong in patcher_main, exit with code 1"
		
			cd setup_env/
			. unset.sh
			cd ..

			exit 1
		fi

		patch_num=$(( $patch_num + 1 ))

		mkdir $final_loc/$dirname
		mv $temp_loc/* $final_loc/$dirname
	done < $patches_txt
fi

if [ $cmd -eq 0 ]
then
	# plot the graphs for the averages and variances
	gnuplot -c run.gnuplot $avg_csv $avg_png "Average Time Per Patch With n=10" "Patches" "Time"
	gnuplot -c run.gnuplot $var_csv $var_png "Variance Per Patch With n=10" "Patches" "Variance"
else
	gnuplot -c run.gnuplot $avg_csv $avg_png "Average Time Per Command With n=10" "Commands" "Time"
	gnuplot -c run.gnuplot $var_csv $var_png "Variance Per Command With n=10" "Commands" "Variance"
fi

cd setup_env/
. unset.sh
cd ..
