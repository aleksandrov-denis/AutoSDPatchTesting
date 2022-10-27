#!/bin/bash

# This script runs the patch testing process by passing patcher_main.sh through an expect script to deal with the password prompt


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
			. config.sh
		elif [ $answer == "y" ]
		then
			. set.sh
		fi
	else
		. config.sh
	fi

	./test_main.sh

	if [ $? -eq 0 ]
	then
		break
	fi
done


cd ..
echo stop
read blam

# expext/autoexpect SOURCE: https://web.archive.org/web/20160404120518/http://www.linuxjournal.com/article/3065?page=0,0
patch_num=1
while read patch; do

	export patch=$patch
	export patch_num=$patch_num

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

	dirname=$(echo "${patch}" | awk '{print substr ($0, 0, 4)}')
	mkdir $final_loc/$dirname
	mv $temp_loc/* $final_loc/$dirname
done < $patches_txt

# plot the graphs for the averages and variances
gnuplot -c run.gnuplot $avg_csv $avg_png "Average Time Per Patch With n=10" "Patches" "Time"
gnuplot -c run.gnuplot $var_csv $var_png "Variance Per Patch With n=10" "Patches" "Variance"

cd setup_env/
. unset.sh
cd ..
