#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

const int N_SAMPLES = 1000;

void writeSamples(int *index, long *samples, FILE* f_err){
	FILE *f_data = fopen("/tmp/adc_data.log", "w");
	if (f_data == NULL)
	{
		fprintf(f_err, "Failed to open data file");
		exit(EXIT_FAILURE);
	}  
	
	double t;
	for(int i=0;i<N_SAMPLES; i++){
		t = (double) (samples[i]) / CLOCKS_PER_SEC;
		fprintf(f_data, "%f foo\n", t);
	}
	
	fclose(f_data);
}

int main(void) {
        
	/* Our process ID and Session ID */
	pid_t pid, sid;

	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
			exit(EXIT_FAILURE);
	}
	/* If we got a good PID, then
	   we can exit the parent process. */
	if (pid > 0) {
			exit(EXIT_SUCCESS);
	}

	/* Change the file mode mask */
	umask(0);
			
	/* Open any logs here */    
	FILE *f_err = fopen("/var/log/adc_err.log", "w");
	if (f_err == NULL)
	{
		// I guess we can't log this failure anywhere
		exit(EXIT_FAILURE);
	}    
			
	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
		fprintf(f_err, "Failed to create new SID for the child process.");
		exit(EXIT_FAILURE);
	}

	/* Change the current working directory */
	if ((chdir("/")) < 0) {
		fprintf(f_err, "Failed to change working directory.");
		exit(EXIT_FAILURE);
	}

	/* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	/* Daemon-specific initialization goes here */

	/* The Big Loop */
	//while (1) {
	   /* Do some task here ... */
	   //sleep(30); /* wait 30 seconds */
	//}
	long samples[N_SAMPLES];
	int write_index = 0;
	
	for(int i=0; i < N_SAMPLES; i++){
		samples[i] = (long) clock();
	}
	
	writeSamples(&write_index, samples, f_err);
	
	/* Close logs */
	fclose(f_err);
	
	exit(EXIT_SUCCESS);
}
