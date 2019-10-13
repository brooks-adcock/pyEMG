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

	FILE *f_data = fopen("/tmp/adc_data.log", "w");
	if (f_data == NULL)
	{
		fprintf(f_err, "Failed to open data file");
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
	//const int N_SAMPLES = 1000;
	//int samples[N_SAMPLES];
	//int write_index = 0;
	
	double t;
	for(int i=0; i < 1000; i++){
		t = (double) (clock()) / CLOCKS_PER_SEC;
		fprintf(f_data, "%f\n", t);
	}
	
	/* Close logs */
	fclose(f_data);
	fclose(f_err);
	
	exit(EXIT_SUCCESS);
}
