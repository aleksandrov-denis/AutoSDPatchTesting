#!/bin/bash


echo "Enter remote host ip:"
read user_host; export host=$user_host; echo Remote Host: $host > config.txt

echo "Enter remote user:"
read user_user; export user=$user_user; echo Remote User: $user >> config.txt

echo "Enter remote host password:"
read user_password; export pswd=$user_password; echo Remote Password: $pswd >> config.txt

echo "Enter the index of the backup kernel on your remote host (Hint: run 'sudo grubby --info=ALL' in /boot to see available options):"
read user_index; export index=$user_index; echo Remote Backup Kernel Index: $index >> config.txt

echo "Enter the full path of test kernel source (on your remote system):"
read user_kernel; export kernel=$user_kernel; echo Remote Kernel Path: $kernel >> config.txt

echo "Enter the full path of the patch directory (on your remote system):"
read user_patches; export patches=$user_patches; echo Remote Path to Patches: $patches >> config.txt

echo "You need to have a .txt file with all of the patches you want by name (on your local system), in the following format..."
echo 0101-i8042-decrease-debug-message-level-to-info.patch
echo 0102-increase-the-ext4-default-commit-age.patch
echo 0104-pci-pme-wakeups.patch
echo etc.patch
echo "Enter the full path of the .txt file:"
read user_patches_txt; export patches_txt=$user_patches_txt; echo Local Path to patches.txt: $patches_txt >> config.txt

echo "Enter the full path of the .config file of the kernel your remote system is currently running:"
read user_config; export config=$user_config; echo Remote Config Path: $config >> config.txt

echo "Enter the full path of an empty directory (on your local system terminated by /), this is used for temporary storage in the script:"
read user_temp_loc; export temp_loc=$user_temp_loc; echo Local Temp .json Location Path: $temp_loc >> config.txt

echo "Enter another full path of an empty directory (on your local system terminated by /), this is where all of the .json files from sut_boottest.sh will be stored:"
read user_final_loc; export final_loc=$user_final_loc; echo Local Final .json Location Path: $final_loc >> config.txt

echo "Enter the full path of an empty .csv file for the average data (on your local system):"
read user_avg_csv; export avg_csv=$user_avg_csv; echo Local Average CSV Path: $avg_csv >> config.txt

echo "Enter the full path of an empty .csv file for the variance data (on your local system):"
read user_var_csv; export var_csv=$user_var_csv; echo Local Variance CSV Path: $var_csv >> config.txt

echo "Enter the full path of an empty .png file for the average graph data (on your local system):"
read user_avg_png; export avg_png=$user_avg_png; echo Local Average PNG Path: $avg_png >> config.txt

echo "Enter the full path of an empty .png file for the variance graph data (on your local system):"
read user_var_png; export var_png=$user_var_png; echo Local Variance PNG Path: $var_png >> config.txt
