#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
	int program_counter = 0;
	int process_counter = 0;
	int max_processes;
	
	if (argc == 2) {
		max_processes = stoi(argv[1]);
	}
	else if (argc > 2) {
		printf("Format as follows:\t./mandelmovie num_processes\n");
		return 0;
	}
	else {max_processes = 27;}
	while (program_counter < 50) {	
		program_counter++;
		int rc = fork();
		if (rc < 0) {
			fprintf(stderr, "fork failed\n");
		}
		else if (rc == 0) {
			//printf("proc1: %d\n",process_counter);
			printf("Starting process %d...\n",program_counter);
			char *myargs[16];
			float s = 2 - float(program_counter)*(2.-.0001)/50.;
			string outfile = "mandel" + to_string(program_counter) + ".bmp";
			const char* s_str = to_string(s).c_str();
			const char* outfile_str = outfile.c_str();
			myargs[0] = strdup("./mandel");
			myargs[1] = strdup("-x");
			myargs[2] = strdup("-.354632");
			myargs[3] = strdup("-y");
			myargs[4] = strdup(".634567");
			myargs[5] = strdup("-s");
			myargs[6] = strdup(s_str);
			myargs[7] = strdup("-m");
			myargs[8] = strdup("2000");
			myargs[9] = strdup("-W");
			myargs[10] = strdup("1500");
			myargs[11] = strdup("-H");
			myargs[12] = strdup("1500");
			myargs[13] = strdup("-o");
			myargs[14] = strdup(outfile_str);
			myargs[15] = NULL;
					
			execvp(myargs[0], myargs);
		}
		else {
			process_counter ++;
			if (process_counter == max_processes) { 
				int wc = wait(NULL);
				printf("Process complete.\n");
				process_counter--;
			}
		}
	}
	return 0;
}
