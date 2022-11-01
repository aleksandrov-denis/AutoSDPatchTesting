# Purpose
To test the effect of most kernel patches from https://github.com/clearlinux-pkgs/linux on the boot-time of the CentOS Automotive SIG's AutoSD [kernel](https://gitlab.com/CentOS/automotive/src/kernel/kernel-automotive-9).

## Structure
Each patch is applied on the pi remotely via ssh. The local x86_64 machine, in my case, is running Linux fedora 5.19.4 and the remote host (RPi4) is running AutoSD. You can find the image [here](https://autosd.sig.centos.org/AutoSD-9/nightly/non-sample-images/).

## Process
- main.sh sets up environment variables using configuration scripts in setup_env/
- main.sh calls patcher_main.sh per patch
	- patcher_main.sh opens an ssh session to the remote host and feeds it patcher_ssh.sh
		- patcher_ssh.sh patches kernel source, builds and installs the kernel + modules
	- patcher_main.sh calls sut_boottest.py ten times
		- sut_boottest.py reboots remote host and gathers boot data via systemd-analyze into .json files
	- patcher_main.sh calls j2c.o to append averages and variances to average.csv and variance.csv
	- patcher_main.sh opens an ssh session to the remote host and feeds it patcher_rm_ssh.sh
		- patcher_rm_ssh.sh reverts previous patch from kernel source and deletes installed kernel + modules
- main.sh calls run.gnuplot to make two histograms based off of average.csv and variance.csv

NOTE: This script assumes that when you compile a kernel (on your remote host) the folowing entries are created:
- /boot/vmlinuz*$KERNELVERSION*
- /boot/initramfs*$KERNELVERSION*
- /boot/System.map*$KERNELVERSION*
- /lib/modules/*$KERNELVERSION*

NOTE: If your remote host (system under test) is running Arch Linux, do not use this script. Arch Linux's .config file is not easily accessible and is not acocuted for in this script.

## Usage
In order to run the script safely, make sure to follow the prerequisite steps.

### Prereqs
1. patches.txt needs to be populated with the patches you want to be applied
2. Make sure that you have grubby installed on your remote host
3. The local machine needs to have gnuplot installed

### To Run
./main.sh

## Visualization
Specific data points' averages and variances are converted to csv format and plotted on a histogram.

### Data of Interest
Each patch gets time tested by running sut_boottest.py ten times, an average and variance is taken for each datapoint in the sample of ten trials, the average and variance points get recorded in .csv files and graphed on a histogram using gnuplot. The datapoints are generated from 'systemd-analyze' and 'systemd-analyze blame'. The points of interest are listed below.
1. kernel
2. initrd
3. userspace
4. total_boot_time
5. initrd_switch_root.service
6. NetworkManager-wait-online.service
7. NetworkManager.service
8. modprobe@drm.service
9. systemd-logind.service

### Graphs
You can find the histrograms for the averages in avg.png and the variances in var.png

#### To view them in the CLI, run:
$ gthumb <yourfile.png>
