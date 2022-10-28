#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE_LENGTH 256

// GET SOME USE OUT OF getenv() CHECK THIS OUT http://www0.cs.ucl.ac.uk/staff/ucacbbl/getenv/

// The function takes the current line in a .json file and parses the data and name on that line
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
	return 0;
}

// Reads all of the .json files in the results directory and calls get_time() on
// the lines of interest (defined below)
int main(int argc, char *argv[]){
	FILE *results_file;
	FILE *file;
	FILE *avg;
	FILE *var;
	//char path[MAX_LINE_LENGTH] = "temp_location/";
	char path[MAX_LINE_LENGTH] = "";
	strcpy(path, getenv("temp_loc"));
	int line_count;
	double averages[9] = {0,0,0,0,0,0,0,0,0};
	double all_data[10][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};

	int status = system("ls $temp_loc > results.txt");
	if(status != 0){
		printf("Could not execute 'ls', exit code %d\n", status);
		return status;
	}


	avg = fopen(argv[1], "a");
	if(!avg){
		perror(argv[1]);
		return EXIT_FAILURE;
	}
	var = fopen(argv[2], "a");
	if(!var){
		perror(argv[2]);
		return EXIT_FAILURE;
	}

	results_file = fopen("results.txt", "r");
	if(results_file != NULL){
		char results_line[MAX_LINE_LENGTH];

		int section = 0;
		while(fgets(results_line, sizeof results_line, results_file) != NULL){

			char data_values[9][100] = {"kernel\0", "initrd\0", "userspace\0", "total_boot_time\0", "initrd_switch_root.service\0", "NetworkManager-wait-online.service\0", "NetworkManager.service\0", "modprobe@drm.service\0", "systemd-logind.service\0"};
			char data_values_check[9][100] = {"kernel\0", "initrd\0", "userspace\0", "total_boot_time\0",  "initrd_switch_root.service\0", "NetworkManager-wait-online.service\0", "NetworkManager.service\0", "modprobe@drm.service\0", "systemd-logind.service\0"};

			results_line[strcspn(results_line, "\n")] = 0;
			strcat(path, results_line);
			file = fopen(path, "r");
			
			if(!file){
				perror(path);
				return EXIT_FAILURE;
			}

			strcpy(path, getenv("temp_loc"));
			//path = getenv("temp_loc");
			char line[MAX_LINE_LENGTH];
			
			line_count = 0;
			double total_boot_time = 0;
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
						strcpy(data_values[0], data);
						total_boot_time += atof(data);
					}
					else if(strcmp(name, data_values[1]) == 0){
						strcpy(data_values[1], data);
						total_boot_time += atof(data);
					}
					else if(strcmp(name, data_values[2]) == 0){

						strcpy(data_values[2], data);

						char out[50];
						total_boot_time += atof(data);
						snprintf(out, 50, "%.3f", total_boot_time);
						strcat(out, "\0");
						strcpy(data_values[3], out);
					}
					else if(strcmp(name, data_values[4]) == 0){
						strcpy(data_values[4], data);
					}
					else if(strcmp(name, data_values[5]) == 0){
						strcpy(data_values[5], data);
					}
					else if(strcmp(name, data_values[6]) == 0){
						strcpy(data_values[6], data);
					}
					else if(strcmp(name, data_values[7]) == 0){
						strcpy(data_values[7], data);
					}
					else if(strcmp(name, data_values[8]) == 0){
						strcpy(data_values[8], data);
					}
				}
			}

			for(int i = 0; i < 9; i++){
				// if no data was found, set it to zero
				if(strcmp(data_values_check[i], data_values[i]) == 0){
					strcpy(data_values[i], "0");
				}

				// add time to the corresponding data index in the array in which the average is calculated
				averages[i] += atof(data_values[i]);
				// record all of the values in 2d array in order to calculate sample variance
				all_data[section][i] = atof(data_values[i]);
			}
			section++;

			if(fclose(file)){
				perror(path);
				return EXIT_FAILURE;
			}

		}

		// DIVIDE EACH DATA POINT FROM DOUBLE ARRAY BY 10 TO GET AVG
		// AND WRITE TO CSV
		fprintf(avg, "%s,", argv[3]);
		for(int i = 0; i < 9; i++){
			fprintf(avg, "%.3f,", averages[i]/10);
		}
		fprintf(avg, "\n");


		// GET THE SAMPLE VARIANCE FOR EACH DATA POINT
		fprintf(var, "%s,", argv[3]);
		double variance[9] = {0,0,0,0,0,0,0,0,0};
		for(int i = 0; i < 10; i++){
			for(int j = 0; j < 9; j++){
				double temp = all_data[i][j] - averages[j]/10;
				variance[j] += temp * temp;
			}
		}

		for(int i = 0; i < 9; i++){
			variance[i] = variance[i]/(10 - 1);
			fprintf(var, "%f,", variance[i]);
		}
		fprintf(var, "\n");
	}
	else{
		perror("results.txt");
		return EXIT_FAILURE;
	}

	if(fclose(avg)){
		perror(argv[1]);
		return EXIT_FAILURE;
	}
	if(fclose(var)){
		perror(argv[2]);
		return EXIT_FAILURE;
	}

	if(fclose(results_file)){
			perror("results.txt");
			return EXIT_FAILURE;
	}
			
	return 0;
}
