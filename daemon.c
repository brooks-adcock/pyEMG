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


#include <sys/ioctl.h>			//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
//#include <string>
//#include <iostream> //ERROR
//#include <cstring>  //ERROR

const int N_SAMPLES = 1000;
unsigned char SPI_MODE = SPI_MODE_0;
unsigned char SPI_BITS_PER_WORD = 8;
unsigned int SPI_SPEED = 1000000;
unsigned int SPI_DEVICE = 0; //chip select 0
int spi_fd; //SPI File Descriptor
FILE *f_err;

void eAbort(char* message){
	fprintf(f_err, message);
	fprintf(f_err, "\n");
	exit(EXIT_FAILURE);
}

int openSPI(){
	int status_value = -1;
	spi_fd = open("/dev/spidev0.0", O_RDWR);
	
	if( spi_fd < 0 ){
		eAbort("Could not open SPI device");
	}
	
	status_value = ioctl(spi_fd, SPI_IOC_RD_MODE, &SPI_MODE)
	if(status_value < 0){
		eAbort("Could not set SPIMode (RD)...ioctl fail");
	}
	
	status_value = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    {
		pAbort("Could not set SPI bitsPerWord(RD)...ioctl fail");
    }
    
    status_value = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    {
		pAbort("Could not set SPI speed (RD)...ioctl fail");
    }
	return(status_value);
}

int closeSPI(){
	int status_value = -1;
	status_value = close(spi_fd);
	if(status_value < 0){
		pAbort("Error - Could not close SPI device");
	}
	return(status_value)
}

int readSPI(unsigned char * rx_data){
	struct spi_ioc_transfer spi;
	int i=0;
	int return_value = -1;
	
	spi.tx_buf =  NULL;
	spi.rx_buf = (unsigned long) rx_data;
	spi.len = 2;
	spi.delay_usecs = 0;
	spi.speed_hz = SPI_SPEED;
	spi.bits_per_word = SPI_BITS_PER_WORD;
	spi.cs_change = 0; //set high after transfer
	
	return_value = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
}

void writeSamples(int *index, long *samples, FILE* f_err){
	FILE *f_data = fopen("/tmp/adc_data.log", "w");
	fprintf(f_data, "[");
	
	if (f_data == NULL)
	{
		fprintf(f_err, "Failed to open data file");
		exit(EXIT_FAILURE);
	}  
	
	double t;
	for(int i=0;i<N_SAMPLES; i++){
		t = (double) (samples[(*index+i) % N_SAMPLES]) / CLOCKS_PER_SEC;
		fprintf(f_data, "{'t': %f, 'y':%d},\n", t, *index);
	}
	
	fseek(f_data, -2, SEEK_CUR); //overwrites the last comma so that file is json complaint
	fprintf(f_data, "\n]");
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
	f_err = fopen("/var/log/adc_err.log", "w");
	if (f_err == NULL)
	{
		// I guess we can't log this failure anywhere
		exit(EXIT_FAILURE);
	}    
			
	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
		eAbort("TEST: Failed to create new SID for the child process.");
		//fprintf(f_err, "Failed to create new SID for the child process.");
		//exit(EXIT_FAILURE);
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
	int write_index = 3;
	
	for(int i=0; i < N_SAMPLES; i++){
		samples[i] = 0; 
	}
	
	for(int i=0; i < N_SAMPLES; i++){
		samples[write_index] = (long) clock();
		writeSamples(&write_index, samples, f_err);
		write_index = (write_index+1) % N_SAMPLES;
	}
	
	/* Close logs */
	fclose(f_err);
	
	exit(EXIT_SUCCESS);
}
