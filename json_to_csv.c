#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE_LENGTH 256

int main(void){
	FILE *results_file;
	FILE *file;
	char path[MAX_LINE_LENGTH] = "clear_linux_patches_results/";
	//char *line = NULL;
	//size_t len = 0;
	//ssize_t read;

	int status = system("ls clear_linux_patches_results > results.txt");
	if(status != 0){
		printf("Could not execute 'ls', exit code %d\n", status);
		return status;
	}


	results_file = fopen("results.txt", "r");
	if(results_file != NULL){

		char results_line[MAX_LINE_LENGTH];

		
		while(fgets(results_line, sizeof results_line, results_file) != NULL){
			results_line[strcspn(results_line, "\n")] = 0;
			strcat(path, results_line);
			file = fopen(path, "r");
			
			if(!file){
				perror(path);
				return EXIT_FAILURE;
			}

			strcpy(path, "clear_linux_patches_results/");
			char line[MAX_LINE_LENGTH];

			while(fgets(line, sizeof line, file) != NULL){
				printf("%s", line);
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
				
			}
			if(fclose(file)){
				perror(path);
				return EXIT_FAILURE;
			}

		}
		// write contents of struct to csv file
			
		// dir read loop end
	}
	else{
		perror("results.txt");
		return EXIT_FAILURE;
	}

	if(fclose(results_file)){
			perror("results.txt");
			return EXIT_FAILURE;
	}
			
	return 0;
}
