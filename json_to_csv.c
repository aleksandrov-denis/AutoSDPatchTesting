#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE_LENGTH 256

// The function takes the current line in some .json file and writes the numerical data
// to the specified .csv file
int get_time(char *aline, char *data, char *name){
	//char data[MAX_LINE_LENGTH];
	//char name[MAX_LINE_LENGTH];
	int breakpoint = strlen(aline);
	int name_index = 0;
	int data_index = 0;
	for(int i = 17; i < strlen(aline); i++){
		if(aline[i] != '"' && aline[i] != ' ' && aline[i] != ',' && aline[i] != '\n'){
			if(aline[i] == ':')
				breakpoint = i;
			if(i > breakpoint){
				data[data_index] = aline[i];
				data_index++;
			}
			else if(i < breakpoint){
				name[name_index] = aline[i];
				name_index++;
			}
		}
	}
	data[data_index] = '\0';
	name[name_index] = '\0';
	//fprintf(file, name);
	//fprintf(file, ": ");
	//fprintf(file, data);
	//fprintf(file, ",");
	return 0;
}

// Reads all of the .json files in the results directory and calls get_time() on
// the lines of interest (defined below)
int main(void){
	FILE *results_file;
	FILE *file;
	FILE *csv;
	char path[MAX_LINE_LENGTH] = "result_test/";
	int line_count;

	int status = system("ls result_test > results.txt");
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
		//fprintf(csv, "kernel,initrd,userspace,total_boot_time,initrd_switch_root.service,NetworkManager-wait-online.service,NetworkManager.service,modprobe@drm.service,systemd-logind.service,time_to_first_camera_frame\n"); 
		char results_line[MAX_LINE_LENGTH];

			
		while(fgets(results_line, sizeof results_line, results_file) != NULL){

			char data_values[8][100] = {"kernel\0", "initrd\0", "userspace\0", "initrd_switch_root.service\0", "NetworkManager-wait-online.service\0", "NetworkManager.service\0", "modprobe@drm.service\0", "systemd-logind.service\0"};
			char data_values_check[8][100] = {"kernel\0", "initrd\0", "userspace\0", "initrd_switch_root.service\0", "NetworkManager-wait-online.service\0", "NetworkManager.service\0", "modprobe@drm.service\0", "systemd-logind.service\0"};

			results_line[strcspn(results_line, "\n")] = 0;
			strcat(path, results_line);
			file = fopen(path, "r");
			
			if(!file){
				perror(path);
				return EXIT_FAILURE;
			}

			strcpy(path, "result_test/");
			char line[MAX_LINE_LENGTH];
			
			line_count = 0;
			while(fgets(line, sizeof line, file) != NULL){
				// This is a table specifying at which lines each data point is positioned in each .json file
				// However, this is not fully accurate for some files starting from line 29
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

				if(line_count >= 24 && line_count <= 51){
					char data[MAX_LINE_LENGTH];
					char name[MAX_LINE_LENGTH];
					get_time(line, data, name);
				
					if(strcmp(name, data_values[0]) == 0){
						//strcat(data_values[0], ": ");
						//strcat(data_values[0], data);
						strcpy(data_values[0], data);
					}
					else if(strcmp(name, data_values[1]) == 0){
						//strcat(data_values[1], ": ");
						//strcat(data_values[1], data);
						strcpy(data_values[1], data);
					}
					else if(strcmp(name, data_values[2]) == 0){
						//strcat(data_values[2], ": ");
						//strcat(data_values[2], data);
						strcpy(data_values[2], data);
					}
					else if(strcmp(name, data_values[3]) == 0){
						//strcat(data_values[3], ": ");
						//strcat(data_values[3], data);
						strcpy(data_values[3], data);
					}
					else if(strcmp(name, data_values[4]) == 0){
						//strcat(data_values[4], ": ");
						//strcat(data_values[4], data);
						strcpy(data_values[4], data);
					}
					else if(strcmp(name, data_values[5]) == 0){
						//strcat(data_values[5], ": ");
						//strcat(data_values[5], data);
						//printf("MISSING VALUE TEST\n");
						strcpy(data_values[5], data);
					}
					else if(strcmp(name, data_values[6]) == 0){
						//strcat(data_values[6], ": ");
						//strcat(data_values[6], data);
						strcpy(data_values[6], data);
					}
					else if(strcmp(name, data_values[7]) == 0){
						//strcat(data_values[7], ": ");
						//strcat(data_values[7], data);
						strcpy(data_values[7], data);
					}
				}
			}

			for(int i = 0; i < 8; i++){
				if(strcmp(data_values_check[i], data_values[i]) == 0){
					//printf("Testing blank spaces: %s\n", data_values[i]);
					strcpy(data_values[i], "0");
				}
				//printf("The %d index in real array is %s\n", i, data_values[i]);
				//printf("The %d index in TEST array is %s\n", i, data_values_check[i]);
			}

			for(int i = 0; i < 8; i++){
				fprintf(csv, data_values[i]);
				//printf("%s\n", data_values[i]);
				fprintf(csv, ",");
			}
			fprintf(csv, "\n");


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
