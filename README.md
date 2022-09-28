# Purpose
To test the effect of most kernel patches from https://github.com/clearlinux-pkgs/linux on the boot-time of the CentOS Automotive SIG's AutoSD [kernel](https://gitlab.com/CentOS/automotive/src/kernel/kernel-automotive-9).

## Structure
Each patch is applied on the pi remotely via ssh. The local x86_64 machine, in my case, is running Linux fedora 5.19.4 and the remote host (RPi4) is running AutoSD. You can find the image [here](https://autosd.sig.centos.org/AutoSD-9/nightly/non-sample-images/).

## Process
- main.sh calls expect_pswd.exp (the expect script is generated by autoexpect, makes it easier to deal with auto-password prompts when starting a new ssh session)
- expect_pswd.exp calls patcher_main.sh per patch
	- patcher_main.sh opens an ssh session to the remote host and feeds it patcher_ssh.sh
		- patcher_ssh.sh patches kernel source, builds and installs the kernel + modules
	- patcher_main.sh calls sut_boottest.py ten times
		- sut_boottest.py reboots remote host and gathers boot data via systemd-analyze into .json files
	- patcher_main.sh calls j2c.o to append averages and variances to average.csv and variance.csv
	- patcher_main.sh opens an ssh session to the remote host and feeds it patcher_rm_ssh.sh
		- patcher_rm_ssh.sh reverts previous patch from kernel source and deletes installed kernel + modules
- main.sh calls run.gnuplot to make two histograms based off of average.csv and variance.csv


1. A patch gets applied to the kernel source
2. Kernel source is built
3. Modules are installed
4. Compiled kernel is installed
5. New kernel is set as default boot kernel
6. Remote host gets rebooted and boot time is recorded via sut_boottest.py, which is written by John Harrigan at https://github.com/jharriga/BootTime. Test result file is written to pwd.
7. Step 6 is called 10 times
8. Last patch to the kernel source is reverted
9. Most recently installed kernel and all files relating to it, in /boot and /lib/modules, are deleted. Grub entry for most recent kernel is removed and default is set to the initially used kernel

Steps 1-9 are repeated for every patch.

NOTE: when the grub entries are modified, the script assumes that your initial kernel is at index=0 and will write the newly installed kernel to index=1. It is advised that you use caution and go through the code to make sure the dangerous operations are clear to you. If you start this script with more than one kernel installed to /boot on your remote host, the grub menu modification will behave unexpectedly and might lock you out of your device. Be very careful using this script on a remote host that has important data on it. Reference patcher_rm_ssh.sh for how the cleaning process works.

## Usage
In order to run the script safely, make sure to follow the prerequisite steps.

### Prereqs
1. patches.txt needs to be populated with the patches you want to be applied
2. The user, host, and password need to be configured properly in expect_pswd.exp, patcher_main.sh, and sut_boottest.py
3. The remote host should only have one kernel installed in /boot and the grub menu (see NOTE above, under the Process tab)
4. The remote host needs to have a kernel source repo and the path to it needs to be modified in patcher_ssh.sh and patcher_rm_ssh.sh (in this case the kernel source used is https://gitlab.com/CentOS/automotive/src/kernel/kernel-automotive-9)
5. The remote host needs to have a repo with the appropriate patches, modify the path to it in patcher_ssh.sh and patcher_rm_ssh.sh (in this case the patched used are from https://github.com/clearlinux-pkgs/linux)
6. Make sure that you have grubby installed on your remote host
7. The local machine needs to have gnuplot installed

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
You can find the histrograms for the averages in mavg.png and the variances in mvar.png

# To view them in the CLI, run:
$ gthumb <yourfile.png>
