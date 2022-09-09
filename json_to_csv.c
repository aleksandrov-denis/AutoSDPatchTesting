#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE_LENGTH 256

// The function takes the current line in some .json file and writes the numerical data
// to the specified .csv file
int get_time(char *aline, FILE *file){
	char data[MAX_LINE_LENGTH];
	int breakpoint = strlen(aline);
	int data_index = 0;
	for(int i = 17; i < strlen(aline); i++){
		if(aline[i] != '"' && aline[i] != ' ' && aline[i] != ',' && aline[i] != '\n'){
			if(aline[i] == ':')
				breakpoint = i;
			if(i > breakpoint){
				data[data_index] = aline[i];
				data_index++;
			}
		}
	}
	fprintf(file, data);
	fprintf(file, ",");
	return 0;
}

// Reads all of the .json files in the results directory and calls get_time() on
// the lines of interest (defined below)
int main(void){
	FILE *results_file;
	FILE *file;
	FILE *csv;
	char path[MAX_LINE_LENGTH] = "clear_linux_patches_results/";
	int line_count;

	int status = system("ls clear_linux_patches_results > results.txt");
	if(status != 0){
		printf("Could not execute 'ls', exit code %d\n", status);
		return status;
	}


	csv = fopen("graph_data.csv", "w");
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
				// This is a table specifying at which lines each data point is positioned in each .json file
				// However, this is not fully accurate for some files starting from line 29
				//
				// Note: I am working on sut_boottest to make the line numbers consistent for these data points
				//
				// 
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
						get_time(line, csv);
						break;
					case 25:
						get_time(line, csv);
						break;
					case 26:
						get_time(line, csv);
						break;
					case 29:
						get_time(line, csv);
						break;
					case 30:
						get_time(line, csv);
						break;
					case 31:
						get_time(line, csv);
						break;
					case 32:
						get_time(line, csv);
						break;
					case 33:
						get_time(line, csv);
						fprintf(csv, "\n");
						break;
				}

			}

			if(fclose(file)){
				perror(path);
				return EXIT_FAILURE;
			}

		}
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
