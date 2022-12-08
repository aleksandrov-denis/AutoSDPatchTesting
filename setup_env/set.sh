#!/bin/bash

cmd=$1

if [[ $cmd -lt 0 ]] || [[ $cmd -gt 1 ]] || [[ $# -ne 1 ]]
then
        echo "usage: . set.sh <cmd>"
        echo "cmd == 0 -> configure everything, no kernel command-line parameters"
        echo "cmd == 1 -> only configure kernel command-line parameters"
        return
fi

if [[ $cmd -eq 1 ]]
then
	declare -x kclp=$(grep 'Local Path to commands.txt:' config.txt | awk '{print $5}')
fi

declare -x host=$(grep 'Remote Host:' config.txt | awk '{print $3}')
declare -x user=$(grep 'Remote User:' config.txt | awk '{print $3}')
declare -x pswd=$(grep 'Remote Password:' config.txt | awk '{print $3}')
declare -x temp_loc=$(grep 'Local Temp .json Location Path:' config.txt | awk '{print $6}')
declare -x final_loc=$(grep 'Local Final .json Location Path:' config.txt | awk '{print $6}')
declare -x avg_csv=$(grep 'Local Average CSV Path:' config.txt | awk '{print $5}')
declare -x var_csv=$(grep 'Local Variance CSV Path:' config.txt | awk '{print $5}')
declare -x avg_png=$(grep 'Local Average PNG Path:' config.txt | awk '{print $5}')
declare -x var_png=$(grep 'Local Variance PNG Path:' config.txt | awk '{print $5}')

if [ $cmd -eq 1 ]
then
	return
fi

declare -x index=$(grep 'Remote Backup Kernel Index:' config.txt | awk '{print $5}')
declare -x kernel=$(grep 'Remote Kernel Path:' config.txt | awk '{print $4}')
declare -x patches=$(grep 'Remote Path to Patches:' config.txt | awk '{print $5}')
declare -x patches_txt=$(grep 'Local Path to patches.txt:' config.txt | awk '{print $5}')
declare -x config=$(grep 'Remote Config Path:' config.txt | awk '{print $4}')
