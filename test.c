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

void eAbort(char* message){
	perror(message);
	exit(EXIT_FAILURE);
}

int openSPI(){
	int status_value = -1;
	spi_fd = open("/dev/spidev0.0", O_RDWR);
	
	if( spi_fd < 0 ){
		eAbort("Could not open SPI device");
	}
	
	status_value = ioctl(spi_fd, SPI_IOC_RD_MODE, &SPI_MODE);
	if(status_value < 0){
		eAbort("Could not set SPIMode (RD)...ioctl fail");
	}
	
	status_value = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &SPI_BITS_PER_WORD);
    if(status_value < 0){
		eAbort("Could not set SPI BITS PER WORD (RD)...ioctl fail");
    }
    
    status_value = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &SPI_SPEED);
    if(status_value < 0){
		eAbort("Could not set SPI speed (RD)...ioctl fail");
    }
	return(status_value);
}

int closeSPI(){
	int status_value = -1;
	status_value = close(spi_fd);
	if(status_value < 0){
		eAbort("Error - Could not close SPI device");
	}
	return(status_value);
}

int readSPI(unsigned char * rx_data){
	struct spi_ioc_transfer spi;
	int return_value = -1;
	
	spi.tx_buf = 0x00;
	spi.rx_buf = (unsigned long) rx_data;
	spi.len = 2;
	spi.delay_usecs = 0;
	spi.speed_hz = SPI_SPEED;
	spi.bits_per_word = SPI_BITS_PER_WORD;
	spi.cs_change = 0; //set high after transfer
	
	return_value = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
	
	return(return_value);
}

int main(void) {
	long t1 = (long) clock();
	openSPI();
	printf("SPI OPEN\n");
	unsigned char *rx_data = "";
	readSPI(rx_data);
	printf(rx_data);
	closeSPI();
	printf("SPI Closed\n");
	
	long t2 = (long) clock();
	printf("Duration %f", (double)(t2-t1) / CLOCKS_PER_SEC);
	exit(EXIT_SUCCESS);
	
}
