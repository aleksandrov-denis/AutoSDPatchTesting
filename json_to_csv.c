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
	return 0;
}

// Reads all of the .json files in the results directory and calls get_time() on
// the lines of interest (defined below)
int main(int argc, char *argv[]){
	FILE *results_file;
	FILE *file;
	FILE *csv;
	FILE *var;
	char path[MAX_LINE_LENGTH] = "result_test/";
	int line_count;
	double averages[9] = {0,0,0,0,0,0,0,0,0};
	double all_data[10][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};

	int status = system("ls result_test > results.txt");
	if(status != 0){
		printf("Could not execute 'ls', exit code %d\n", status);
		return status;
	}


	csv = fopen(argv[1], "a");
	if(!csv){
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

			strcpy(path, "result_test/");
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
						//strcat(data_values[0], ": ");
						//strcat(data_values[0], data);
						strcpy(data_values[0], data);
						total_boot_time += atof(data);
					}
					else if(strcmp(name, data_values[1]) == 0){
						//strcat(data_values[1], ": ");
						//strcat(data_values[1], data);
						strcpy(data_values[1], data);
						total_boot_time += atof(data);
					}
					else if(strcmp(name, data_values[2]) == 0){
						//strcat(data_values[2], ": ");
						//strcat(data_values[2], data);

						strcpy(data_values[2], data);

						char out[50];
						total_boot_time += atof(data);
						snprintf(out, 50, "%.3f", total_boot_time);
						strcat(out, "\0");
						strcpy(data_values[3], out);
					}
					else if(strcmp(name, data_values[4]) == 0){
						//strcat(data_values[3], ": ");
						//strcat(data_values[3], data);
						strcpy(data_values[4], data);
					}
					else if(strcmp(name, data_values[5]) == 0){
						//strcat(data_values[4], ": ");
						//strcat(data_values[4], data);
						strcpy(data_values[5], data);
					}
					else if(strcmp(name, data_values[6]) == 0){
						//strcat(data_values[5], ": ");
						//strcat(data_values[5], data);
						//printf("MISSING VALUE TEST\n");
						strcpy(data_values[6], data);
					}
					else if(strcmp(name, data_values[7]) == 0){
						//strcat(data_values[6], ": ");
						//strcat(data_values[6], data);
						strcpy(data_values[7], data);
					}
					else if(strcmp(name, data_values[8]) == 0){
						//strcat(data_values[7], ": ");
						//strcat(data_values[7], data);
						strcpy(data_values[8], data);
					}
				}
			}

			for(int i = 0; i < 9; i++){
				if(strcmp(data_values_check[i], data_values[i]) == 0){
					//printf("Testing blank spaces: %s\n", data_values[i]);
					strcpy(data_values[i], "0");
				}

				//fprintf(csv, data_values[i]);
				//fprintf(csv, ",");

				// ADD TO A DOUBLE ARRAY WITH CORRESPONDING DATA VALUES
				averages[i] += atof(data_values[i]);
				//printf("section: %d\n", section);
				all_data[section][i] = atof(data_values[i]);
				//section++;
			}
			//printf("section: %d\n", section);
			section++;

			//fprintf(csv, "\n");


			if(fclose(file)){
				perror(path);
				return EXIT_FAILURE;
			}

		}
		// DIVIDE EACH DATA POINT FROM DOUBLE ARRAY BY 10 TO GET AVG
		// AND WRITE TO CSV
		for(int i = 0; i < 9; i++){
			fprintf(csv, "%.3f,", averages[i]/10);
		}
		fprintf(csv, "\n");


		// GET THE SAMPLE VARIANCE FOR EACH DATA POINT
		double variance[9] = {0,0,0,0,0,0,0,0,0};
		for(int i = 0; i < 10; i++){
			for(int j = 0; j < 9; j++){
				double temp = all_data[i][j] - averages[j]/10;
				//pow(temp, 2);
				//variance[j] += temp;
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
	//fprintf(csv, "\n");

	if(fclose(csv)){
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
