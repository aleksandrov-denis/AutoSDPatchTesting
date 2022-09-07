#include<stdio.h>
#include<stdlib.h>



int main(void){
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	// loop start for reading all the files in report dir
	
	// read individual file and store relevant info in json struct:
	//	kernel
	//	initrd
	//	userspace
	//	total_boot_time
	//	initrd_switch_root.service
	//	NetworkManager-wait-online.service
	//	NetworkManager.service
	//	systemd-logind.service
	//	modprobe@drm.service
	//	time to first camera frame
	

	// write contents of struct to csv file
	
	// loop end

	return 0;
}
