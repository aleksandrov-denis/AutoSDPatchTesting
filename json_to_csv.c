#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE_LENGTH 256

int main(void){
	FILE *results_file;
	FILE *file;
	FILE *csv;
	char path[MAX_LINE_LENGTH] = "clear_linux_patches_results/";
	int line_count;
	//char *line = NULL;
	//size_t len = 0;
	//ssize_t read;

	int status = system("ls clear_linux_patches_results > results.txt");
	if(status != 0){
		printf("Could not execute 'ls', exit code %d\n", status);
		return status;
	}


	csv = fopen("graph_data.csv", "a");
	if(!csv){
		perror("graph_data.csv");
		return EXIT_FAILURE;
	}

	results_file = fopen("results.txt", "r");
	if(results_file != NULL){
		fprintf(csv, "kernel,initrd,userspace,total_boot_time,initrd_switch_root.service,NetworkManager-wait-online.service,NetworkManager.service,modprobe@drm.service,systemd-logind.service,time_to_first_camera_frame\n"); 
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
			
			line_count = 0;
			while(fgets(line, sizeof line, file) != NULL){
				//printf("%s", line);
				// read individual file and store relevant info in json struct:
				// 	DATA OF INTEREST			LINE NUMBER
				//	kernel 					24
				//	initrd 					25
				//	userspace 				26
				//	total_boot_time 			
				//	initrd_switch_root.service 		29
				//	NetworkManager-wait-online.service 	30
				//	NetworkManager.service 			31 
				//	modprobe@drm.service 			32
				//	systemd-logind.service 			33
				//	time to first camera frame 		
				line_count++;
				
				switch(line_count){
					case 24:
						// write kernel to csv
						// store kernel
						printf("%s", line);
						break;
					case 25:
						// write initrd to csv
						// store initrd
						printf("%s", line);
						break;
					case 26:
						// write userpace to csv
						// store userspace
						// add kernel, initrd, and userspace
						// store total_boot_time
						printf("%s", line);
						break;
					case 29:
						// write to csv
						printf("%s", line);
						break;
					case 30:
						// write to csv
						printf("%s", line);
						break;
					case 31:
						// write to csv
                                                printf("%s", line);
						break;
					case 32:
						// write to csv
                                                printf("%s", line);
						break;
					case 33:
						// write to csv
                                                printf("%s\n", line);
						break;
				}

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

	if(fclose(csv)){
		perror("graph_data.csv");
		return EXIT_FAILURE;
	}

	if(fclose(results_file)){
			perror("results.txt");
			return EXIT_FAILURE;
	}
			
	return 0;
}
